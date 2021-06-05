#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<stdlib.h>
#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
//using namespace std;
#define SNAKE_NUM 500  //宏定义蛇的最大节数

//枚举
enum DIR  //表示蛇的方向
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

//蛇的结构
struct Snake
{
	int size; //蛇的节数
	int dir = RIGHT;  //蛇的方向
	int speed; //移动速度
	POINT coor[SNAKE_NUM];//定义好的结构体坐标，可以直接拿来用
}snake;

//食物结构
struct Food
{
	int x;
	int y;
	int r;          //食物大小
	bool flag;      //食物是否被吃
	DWORD color;    //食物颜色
}food;

//数据的初始化
void Gameinit()
{
	//播放背景音乐
	
	/*mciSendString(TEXT("open.imusic.mp3 alias bgm"), 0, 0, 0);
	mciSendString(TEXT("play bgm repeat"), 0, 0, 0);*/
	//PlaySound(TEXT("‪D:\vs2021\snakegame2\7895.wav"), NULL, SND_FILENAME | SND_ASYNC);

	initgraph(640, 480/*,SHOWCONSOLE*/);  //初始化图形窗口,SHOWCONSOLE显示控制台,宽度为640，长度为480
	//设置随机数种子
	srand(GetTickCount());  //GetTickCount()代表系统开机到现在所经过的毫秒数
	//初始化蛇一开始有3节
	snake.size = 3;
	snake.speed = 10;
	snake.dir;
	for (int i = 0; i < snake.size; i++)
	{
		snake.coor[i].x = 40 - 10 * i;
		snake.coor[i].y = 10;
	}

	//初始化食物
	//设置种子需要头文件<stdlib.h>,一般把时间作为种子，因为时间是在不断变化的
	food.x = rand() % 640;    //随机函数rand,随机产生一个整数，如果没有设置随机数种子，每次产生的都是固定的数值
	food.y = rand() % 480;
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);   //初始化颜色
	food.r = rand() % 10 + 5;
	food.flag = true;
}

void GameDraw()
{
	//双缓冲绘图
	BeginBatchDraw();

	//改变颜色有两步
	setbkcolor(RGB(32, 84, 125));  //设置背景颜色
	cleardevice();  //清空绘图设备

	//绘制蛇
	setfillcolor(YELLOW);
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y, 5);//5指的是半径
	}

	//绘制食物
	if (food.flag)  //如果食物存在
	{
		solidcircle(food.x, food.y, food.r);
	}
	EndBatchDraw();
}

//移动蛇
void snakeMove()
{
	//让身体跟着头移
	for (int i = snake.size - 1; i > 0; i--)
	{
		snake.coor[i] = snake.coor[i - 1];
	}

	//移动是蛇的方向(坐标）发生改变
	switch (snake.dir)
	{
	case UP:
		snake.coor[0].y -= snake.speed;
		if (snake.coor[0].y + 5 <= 0)  //坐标y<=0时超出了上边界
		{
			snake.coor[0].y = 480;
		}
		break;
	case DOWN:
		snake.coor[0].y += snake.speed;
		if (snake.coor[0].y - 5 >= 480)
		{
			snake.coor[0].y = 0;
		}
		break;
	case LEFT:
		snake.coor[0].x -= snake.speed;
		if (snake.coor[0].x + 5 <= 0)
		{
			snake.coor[0].x = 640;
		}
		break;
	case RIGHT:
		snake.coor[0].x += snake.speed;
		if (snake.coor[0].x - 5 >= 640)
		{
			snake.coor[0].x = 0;
		}
		break;
	}

}

//通过按键改变蛇的移动方向
void keyControl()
{
	//判断有没有按键,如果有按键，就返回真
	if (_kbhit())
	{
		//72 80 75 77 上下左右键值
		switch (_getch())
		{
		case 'w':
		case 'W':
		case 72:
			//改变方向
			if (snake.dir != DOWN)
			{
				snake.dir = UP;
			}
			break;
		case 's':
		case 'S':
		case 80:
			if (snake.dir != UP)
			{
				snake.dir = DOWN;
			}
			break;
		case 'a':
		case 'A':
		case 75:
			if (snake.dir != RIGHT)
			{
				snake.dir = LEFT;
			}
			break;
		case 'd':
		case 'D':
		case 77:
			if (snake.dir != LEFT)
			{
				snake.dir = RIGHT;
			}
			break;
		case ' ':    //游戏暂停
			while (1)
			{
				if (_getch() == ' ')
					return;
			}
			break;
		}
	}
}

//判断有没有吃食物
void EatFood()
{
	if (food.flag && snake.coor[0].x >= food.x - food.r && snake.coor[0].x <= food.x + food.r
		&& snake.coor[0].y >= food.y - food.r && snake.coor[0].y <= food.y + food.r)
	{
		food.flag = false;
		snake.size++;
	}

	//如果食物消失，则重新生成一个食物
	if (!food.flag)
	{
		food.x = rand() % 640;    //随机函数rand,随机产生一个整数，如果没有设置随机数种子，每次产生的都是固定的数值
		food.y = rand() % 480;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);   //初始化颜色
		food.r = rand() % 10 + 5;
		food.flag = true;
	}
}

int main()
{
	Gameinit();


	while (1)  //防止闪退
	{
		GameDraw();
		snakeMove();
		keyControl();
		EatFood();
		Sleep(120);//防止太快，延120毫秒
	}

	return 0;
}