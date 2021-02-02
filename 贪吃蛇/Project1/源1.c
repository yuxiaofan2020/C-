#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>
#include<stdbool.h>

int key = 72;//表示蛇移动的方向，72为按下“↑”所代表的数字
int changeFlag = 0;//用来判断蛇是否吃掉了食物，这一步很重要，涉及到是否会有蛇身移动的效果以及蛇身增长的效果
int score = 0;//记录玩家的得分

#define SNAKESIZE 100 //蛇的身体最大节数
#define MAPWIDTH 118  //地图宽度
#define MAPHEIGHT 29  //地图长度

typedef struct Player {
	char name[20];
	int score;
	struct Player *next;
}player;

struct {
	int x;
	int y;
}food;//食物出现地点

struct {
	int speed;//蛇移动的速度
	int len;//蛇的长度
	int x[SNAKESIZE];//组成蛇身的每一个小方块中x的坐标
	int y[SNAKESIZE];//组成蛇身的每一个小方块中y的坐标
}snake;

void gotoxy(int x, int y);
void drawmap();
void createFood();
void keyDown();
bool snakeStatus();
void startGame();
int rank();
int readrank();  
void chose();


int main()
{
	system("color f4");
	system("mode con cols=120 lines=30");
	drawmap();
	chose();
	return 0;
}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}//定义可以改变光标位置的函数

void drawmap()
{
	for (int i = 0;i <= MAPWIDTH;i += 2)//注意■在横向占据两个位置
	{
		gotoxy(i, 0);
		printf("■");
		gotoxy(i, MAPHEIGHT);
		printf("■");
	}
	for (int i = 1;i < MAPHEIGHT;i++)//注意此时应定义i = 1
	{
		gotoxy(0, i);
		printf("■");
		gotoxy(MAPWIDTH, i);
		printf("■");
	}

	gotoxy(50, 13);
	printf(">1开始游戏\n");
	gotoxy(50, 14);
	printf(">2排行榜\n");
	gotoxy(50, 15);
	printf(">3退出游戏\n");
	gotoxy(44, 16);
	printf("(请输入数字1，2，3进行操作)\n");
	return;
}
void createFood()
{
	if (snake.x[0] == food.x && snake.y[0] == food.y)//蛇头碰到食物
	{
		//蛇头碰到食物即为要吃掉这个食物了，因此需要再次生成一个食物
		while (1)
		{
			int flag = 1;
			srand((unsigned int)time(NULL));
			food.x = rand() % (MAPWIDTH - 4) + 2;
			food.y = rand() % (MAPHEIGHT - 2) + 1;

			//随机生成的食物不能在蛇的身体上
			for (int i = 0; i < snake.len; i++)
			{
				if (snake.x[i] == food.x && snake.y[i] == food.y)
				{
					flag = 0;
					break;
				}
			}
			//随机生成的食物不能横坐标为奇数，也不能在蛇身，否则重新生成
			if (flag && food.x % 2 == 0)
				break;
		}

		//绘制食物
		gotoxy(food.x, food.y);
		printf("★");

		snake.len++;//吃到食物，蛇身长度加1
		score += 10;//每个食物得10分
		snake.speed -= 5;//随着吃的食物越来越多，速度会越来越快
		changeFlag = 1;//很重要，因为吃到了食物，就不用再擦除蛇尾的那一节，以此来造成蛇身体增长的效果
	}
	return;
}

void keyDown()
{
	int pre_key = key;//记录前一个按键的方向
	if (_kbhit())//如果用户按下了键盘中的某个键
	{
		fflush(stdin);//清空缓冲区的字符

		//getch()读取方向键的时候，会返回两次，第一次调用返回0或者224，第二次调用返回的才是实际值
		key = _getch();//第一次调用返回的不是实际值
		key = _getch();//第二次调用返回实际值
	}

	/*
	*蛇移动时候先擦去蛇尾的一节
	*changeFlag为0表明此时没有吃到食物，因此每走一步就要擦除掉蛇尾，以此营造一个移动的效果
	*为1表明吃到了食物，就不需要擦除蛇尾，以此营造一个蛇身增长的效果
	*/
	if (changeFlag == 0)
	{
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf("  ");//在蛇尾处输出空格即擦去蛇尾
	}

	//将蛇的每一节依次向前移动一节（蛇头除外）
	for (int i = snake.len - 1; i > 0; i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}

	//蛇当前移动的方向不能和前一次的方向相反，比如蛇往左走的时候不能直接按右键往右走
	//如果当前移动方向和前一次方向相反的话，把当前移动的方向改为前一次的方向
	if (pre_key == 72 && key == 80)
		key = 72;
	if (pre_key == 80 && key == 72)
		key = 80;
	if (pre_key == 75 && key == 77)
		key = 75;
	if (pre_key == 77 && key == 75)
		key = 77;

	/**
	*控制台按键所代表的数字
	*“↑”：72
	*“↓”：80
	*“←”：75
	*“→”：77
	*/

	//判断蛇头应该往哪个方向移动
	switch (key)
	{
	case 75:
		snake.x[0] -= 2;//往左
		break;
	case 77:
		snake.x[0] += 2;//往右
		break;
	case 72:
		snake.y[0]--;//往上
		break;
	case 80:
		snake.y[0]++;//往下
		break;
	}
	//打印出蛇头
	gotoxy(snake.x[0], snake.y[0]);
	printf("■");
	gotoxy(MAPWIDTH - 2, 0);
	//由于目前没有吃到食物，changFlag值为0
	changeFlag = 0;
	return;
}

void startGame()
{
	system("cls");
	system("1000");
	system("mode con cols=120 lines=30");
	for (int i = 0;i <= MAPWIDTH;i += 2)//注意■在横向占据两个位置
	{
		gotoxy(i, 0);
		printf("■");
		gotoxy(i, MAPHEIGHT);
		printf("■");
	}
	for (int i = 1;i < MAPHEIGHT;i++)//注意此时应定义i = 1
	{
		gotoxy(0, i);
		printf("■");
		gotoxy(MAPWIDTH, i);
		printf("■");
	}

	while (1)
	{
		srand((unsigned int)time(NULL));
		food.x = rand() % (MAPWIDTH - 4) + 2;
		food.y = rand() % (MAPHEIGHT - 2) + 1;
		//生成的食物横坐标的奇偶必须和初试时蛇头所在坐标的奇偶一致，因为一个字符占两个字节位置，若不一致
		//会导致吃食物的时候只吃到一半
		if (food.x % 2 == 0)
			break;
	}
	//将光标移到食物的坐标处打印食物
	gotoxy(food.x, food.y);
	printf("★");

	snake.len = 3;
	snake.speed = 200;

	//在屏幕中间生成蛇头
	snake.x[0] = MAPWIDTH / 2 + 1;//x坐标为偶数
	snake.y[0] = MAPHEIGHT / 2;
	//打印蛇头
	gotoxy(snake.x[0], snake.y[0]);
	printf("■");

	//生成初试的蛇身
	for (int i = 1; i < snake.len; i++)
	{
		//蛇身的打印，纵坐标不变，横坐标为上一节蛇身的坐标值+2
		snake.x[i] = snake.x[i - 1] + 2;
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		printf("■");
	}
	gotoxy(MAPWIDTH - 2, 0);//将光标移动到上方，避免干扰

	while (1)
	{
		keyDown();
		if (!snakeStatus())
			break;
		createFood();
		Sleep(snake.speed);
	}
	gotoxy(MAPWIDTH / 2, MAPHEIGHT / 2);
	printf("Game Over!\n");
	gotoxy(MAPWIDTH / 2, MAPHEIGHT / 2+2);
	printf("本次游戏得分为：%d\n", score);
	rank();//写入分数并排序
	getchar();
	Sleep(5000);
	return;
}

void chose()
{
	while (1)
	{
		switch (getch())
		{
		case '1':
			startGame();
			break;
		case '2':
			readrank();
			break;
		case '3':
			exit(0);
			return;
		}
	}
}

bool snakeStatus()
{
	//蛇头碰到上下边界，游戏结束
	if (snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)
		return false;
	//蛇头碰到左右边界，游戏结束
	if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH)
		return false;
	//蛇头碰到蛇身，游戏结束
	for (int i = 1; i < snake.len; i++)
	{
		if (snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
			return false;
	}
	return true;
}


int rank()
{
	player tmp;
	FILE *fp;
	gotoxy(MAPWIDTH / 2, MAPHEIGHT / 2+1);
	printf("请输入您的名字：");
	scanf("%s", &tmp.name);
	tmp.score = score;
	fp = fopen("3.txt", "a");
	if (fp == NULL)
	{
		printf("文件损坏！");
		exit(1);
	}
	fprintf(fp, "%s %d", tmp.name, tmp.score);
	fclose(fp);
	return 0;
}

int readrank()
{
	system("cls");
	player tmp[10];
	player temp;
	int n = 0, j = 0, i = 0, k = 0;
	FILE *fp;
	fp = fopen("3.txt", "rb");
	if (fp == NULL)
	{
		printf("文件损坏！");
		exit(1);
	}
	for (i = 0;i < n - 1;i++)//采用冒泡法进行排序 
	{
		k = i;
		for (j = i + 1;j < n;j++)
			if (tmp[j].score > tmp[k].score)//排序比较大小  
				k = j;
		temp = tmp[k];
		tmp[k] = tmp[i];
		tmp[i] = temp;//当符合条件时进行数据交换
	}
	i = 0;
	
	while (i <= 10 && fscanf(fp, "%s%d", tmp[i].name, &tmp[i].score) != EOF)
	{
		gotoxy(50, 1+i);
		printf("%3d%15s%10d\n", i + 1, tmp[i].name, tmp[i].score);
		i++;
	}
	fclose(fp);
	return 0;
}



