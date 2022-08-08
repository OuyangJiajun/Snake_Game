#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>//用于光标定位
#include <conio.h>//用于判断是否按下按键

//设置地图边界
#define W 60
#define H 20

//初始化地图
void init_ui()
{
	for (int i = 0; i <= H; i++)
	{
		for (int j = 0; j <= W; j++)
		{
			if (i == 0 || j == 0 || i == H || j == W)
				printf("+");
			else
				printf(" ");
		}
		printf("\n");
	}

}

//设置贪吃蛇结构体
typedef struct _pixel	//蛇身或食物由像素组成，每个像素含x、y坐标
{
	int x;	//x<W
	int y;	//y<H
}PIX;

typedef struct snake
{
	PIX list[W * H];	//蛇身像素数组，最大是像素占满整个地图
	int size;	//实际像素个数
	PIX food;	//食物也是坐标，仅有一个无须数组
	COORD coord;	//定位光标（在windows.h中定义）
	int dx;	//x轴移动量
	int dy;	//y轴移动量
	PIX tail;	//记录原蛇尾，移动后要清除
	int score;	//记录分数
}SNAKE;

//初始化食物
void init_food(PIX* food)
{
	srand(time(NULL));
	food->x = rand() % (W-1) + 1;
	food->y = rand() % (H-1) + 1;
}

//初始化贪吃蛇
void init_snake(SNAKE* snake)
{
	//蛇头像素
	snake->list[0].x = W / 2;
	snake->list[0].y = H / 2;
	//蛇尾像素
	snake->list[1].x = W / 2 -1;
	snake->list[1].y = H / 2;
	//像素个数
	snake->size = 2;
	//初始化食物
	init_food(&(snake->food));

	//设置移动方向，默认向右
	snake->dx = 1;
	snake->dy = 0;
	
	//设置初始分数
	snake->score = 0;
}

//控制蛇的移动方向
void control_snake(SNAKE* snake)
{
	char  key = 0;//一定要赋初值，否则在判断一个随机字符

	while (_kbhit()) //判断是否按下按键,按下不等于0 
	{
		key = _getch();
	}

	//可能已经按完抬起按键，也可能根本没按下按键
	switch (key)
	{
	case'w':
		snake->dy = -1;
		snake->dx = 0;
		break;//向上
	case's':
		snake->dy = 1;
		snake->dx = 0;
		break;//向下
	case'a':
		snake->dy = 0;
		snake->dx = -1;
		break;//向左
	case'd':
		snake->dy = 0;
		snake->dx = 1;
		break;//向右
	}
}

//移动蛇
void move_snake(SNAKE* snake)
{
	//记录蛇尾
	snake->tail = snake->list[snake->size - 1];

	//蛇身像素，后者变成前者(从后往前很重要)
	for (int i = snake->size - 1; i > 0; i--)
	{
		snake->list[i] = snake->list[i - 1];
	}
	//蛇头像素，由x、y移动量决定
	snake->list[0].x += snake->dx;
	snake->list[0].y += snake->dy;
}

//显示贪吃蛇和食物
void show_snake(SNAKE* snake)
{
	for (int i = 0; i < snake->size; i++)
	{
		//光标定位到相应位置再打印显示蛇
		snake->coord.X = snake->list[i].x;
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);

		if (i == 0)	//显示蛇头
		{
			printf("@");
		}
		else  //显示蛇身
		{
			printf("*");
		}
	}

	//光标定位到相应位置再打印食物
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("#");

	//清除原蛇尾
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf(" ");
}

//吃到食物变长
void snake_eat_food(SNAKE* snake)
{
	if (snake->list[0].x == snake->food.x && snake->list[0].y == snake->food.y)
	{
		snake->size++;//这一步同时实现了蛇身的增长，注意看蛇移动的算法
		init_food(&(snake->food));//重新生成食物，原来的蛇走过就被覆盖了不用清除
		snake->score += 10;
	}
}

//游戏结束
void game_over(SNAKE* snake)
{
	//光标定位到相应位置
	snake->coord.X = 40;
	snake->coord.Y = 25;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("游戏结束，总分数：%d", snake->score);
}

//吃到蛇身
void snake_eat_body(SNAKE* snake)
{
	for (int i = 1; i < snake->size; i++)
	{
		if (snake->list[0].x == snake->list[i].x && snake->list[0].y == snake->list[i].y)
		{
			game_over(snake);
			exit(0);
		}
	}
	
}

//启动游戏
void start_game(SNAKE* snake)
{
	while (snake->list[0].x > 0 && snake->list[0].x < W && snake->list[0].y > 0 && snake->list[0].y < H)
	{
		//判断移动方向
		control_snake(snake);

		//判断吃食物
		snake_eat_food(snake);

		//判断是否吃到身体
		snake_eat_body(snake);

		//改变蛇坐标
		move_snake(snake);

		//重新打印蛇身和食物
		show_snake(snake);
		
		//决定蛇移动的速度
		Sleep(50);
	}
	
	//撞墙
	game_over(snake);
}

//隐藏控制台光标
void hide_cur()
{
	CONSOLE_CURSOR_INFO  cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

//主程序
int main(int argc, char* argv[])
{
	//隐藏光标
	hide_cur();

	//初始化地图
	init_ui();

	//初始化蛇身和食物
	SNAKE* snake = (SNAKE*)malloc(sizeof(SNAKE));
	init_snake(snake);

	//显示蛇身和食物
	show_snake(snake);

	//启动游戏
	start_game(snake);

	free(snake);
	return 0;
}