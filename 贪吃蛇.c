#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>//���ڹ�궨λ
#include <conio.h>//�����ж��Ƿ��°���

//���õ�ͼ�߽�
#define W 60
#define H 20

//��ʼ����ͼ
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

//����̰���߽ṹ��
typedef struct _pixel	//�����ʳ����������ɣ�ÿ�����غ�x��y����
{
	int x;	//x<W
	int y;	//y<H
}PIX;

typedef struct snake
{
	PIX list[W * H];	//�����������飬���������ռ��������ͼ
	int size;	//ʵ�����ظ���
	PIX food;	//ʳ��Ҳ�����꣬����һ����������
	COORD coord;	//��λ��꣨��windows.h�ж��壩
	int dx;	//x���ƶ���
	int dy;	//y���ƶ���
	PIX tail;	//��¼ԭ��β���ƶ���Ҫ���
	int score;	//��¼����
}SNAKE;

//��ʼ��ʳ��
void init_food(PIX* food)
{
	srand(time(NULL));
	food->x = rand() % (W-1) + 1;
	food->y = rand() % (H-1) + 1;
}

//��ʼ��̰����
void init_snake(SNAKE* snake)
{
	//��ͷ����
	snake->list[0].x = W / 2;
	snake->list[0].y = H / 2;
	//��β����
	snake->list[1].x = W / 2 -1;
	snake->list[1].y = H / 2;
	//���ظ���
	snake->size = 2;
	//��ʼ��ʳ��
	init_food(&(snake->food));

	//�����ƶ�����Ĭ������
	snake->dx = 1;
	snake->dy = 0;
	
	//���ó�ʼ����
	snake->score = 0;
}

//�����ߵ��ƶ�����
void control_snake(SNAKE* snake)
{
	char  key = 0;//һ��Ҫ����ֵ���������ж�һ������ַ�

	while (_kbhit()) //�ж��Ƿ��°���,���²�����0 
	{
		key = _getch();
	}

	//�����Ѿ�����̧�𰴼���Ҳ���ܸ���û���°���
	switch (key)
	{
	case'w':
		snake->dy = -1;
		snake->dx = 0;
		break;//����
	case's':
		snake->dy = 1;
		snake->dx = 0;
		break;//����
	case'a':
		snake->dy = 0;
		snake->dx = -1;
		break;//����
	case'd':
		snake->dy = 0;
		snake->dx = 1;
		break;//����
	}
}

//�ƶ���
void move_snake(SNAKE* snake)
{
	//��¼��β
	snake->tail = snake->list[snake->size - 1];

	//�������أ����߱��ǰ��(�Ӻ���ǰ����Ҫ)
	for (int i = snake->size - 1; i > 0; i--)
	{
		snake->list[i] = snake->list[i - 1];
	}
	//��ͷ���أ���x��y�ƶ�������
	snake->list[0].x += snake->dx;
	snake->list[0].y += snake->dy;
}

//��ʾ̰���ߺ�ʳ��
void show_snake(SNAKE* snake)
{
	for (int i = 0; i < snake->size; i++)
	{
		//��궨λ����Ӧλ���ٴ�ӡ��ʾ��
		snake->coord.X = snake->list[i].x;
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);

		if (i == 0)	//��ʾ��ͷ
		{
			printf("@");
		}
		else  //��ʾ����
		{
			printf("*");
		}
	}

	//��궨λ����Ӧλ���ٴ�ӡʳ��
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("#");

	//���ԭ��β
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf(" ");
}

//�Ե�ʳ��䳤
void snake_eat_food(SNAKE* snake)
{
	if (snake->list[0].x == snake->food.x && snake->list[0].y == snake->food.y)
	{
		snake->size++;//��һ��ͬʱʵ���������������ע�⿴���ƶ����㷨
		init_food(&(snake->food));//��������ʳ�ԭ�������߹��ͱ������˲������
		snake->score += 10;
	}
}

//��Ϸ����
void game_over(SNAKE* snake)
{
	//��궨λ����Ӧλ��
	snake->coord.X = 40;
	snake->coord.Y = 25;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("��Ϸ�������ܷ�����%d", snake->score);
}

//�Ե�����
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

//������Ϸ
void start_game(SNAKE* snake)
{
	while (snake->list[0].x > 0 && snake->list[0].x < W && snake->list[0].y > 0 && snake->list[0].y < H)
	{
		//�ж��ƶ�����
		control_snake(snake);

		//�жϳ�ʳ��
		snake_eat_food(snake);

		//�ж��Ƿ�Ե�����
		snake_eat_body(snake);

		//�ı�������
		move_snake(snake);

		//���´�ӡ�����ʳ��
		show_snake(snake);
		
		//�������ƶ����ٶ�
		Sleep(50);
	}
	
	//ײǽ
	game_over(snake);
}

//���ؿ���̨���
void hide_cur()
{
	CONSOLE_CURSOR_INFO  cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

//������
int main(int argc, char* argv[])
{
	//���ع��
	hide_cur();

	//��ʼ����ͼ
	init_ui();

	//��ʼ�������ʳ��
	SNAKE* snake = (SNAKE*)malloc(sizeof(SNAKE));
	init_snake(snake);

	//��ʾ�����ʳ��
	show_snake(snake);

	//������Ϸ
	start_game(snake);

	free(snake);
	return 0;
}