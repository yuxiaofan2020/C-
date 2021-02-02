#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>
#include<stdbool.h>

int key = 72;//��ʾ���ƶ��ķ���72Ϊ���¡����������������
int changeFlag = 0;//�����ж����Ƿ�Ե���ʳ���һ������Ҫ���漰���Ƿ���������ƶ���Ч���Լ�����������Ч��
int score = 0;//��¼��ҵĵ÷�

#define SNAKESIZE 100 //�ߵ�����������
#define MAPWIDTH 118  //��ͼ���
#define MAPHEIGHT 29  //��ͼ����

typedef struct Player {
	char name[20];
	int score;
	struct Player *next;
}player;

struct {
	int x;
	int y;
}food;//ʳ����ֵص�

struct {
	int speed;//���ƶ����ٶ�
	int len;//�ߵĳ���
	int x[SNAKESIZE];//��������ÿһ��С������x������
	int y[SNAKESIZE];//��������ÿһ��С������y������
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
}//������Ըı���λ�õĺ���

void drawmap()
{
	for (int i = 0;i <= MAPWIDTH;i += 2)//ע����ں���ռ������λ��
	{
		gotoxy(i, 0);
		printf("��");
		gotoxy(i, MAPHEIGHT);
		printf("��");
	}
	for (int i = 1;i < MAPHEIGHT;i++)//ע���ʱӦ����i = 1
	{
		gotoxy(0, i);
		printf("��");
		gotoxy(MAPWIDTH, i);
		printf("��");
	}

	gotoxy(50, 13);
	printf(">1��ʼ��Ϸ\n");
	gotoxy(50, 14);
	printf(">2���а�\n");
	gotoxy(50, 15);
	printf(">3�˳���Ϸ\n");
	gotoxy(44, 16);
	printf("(����������1��2��3���в���)\n");
	return;
}
void createFood()
{
	if (snake.x[0] == food.x && snake.y[0] == food.y)//��ͷ����ʳ��
	{
		//��ͷ����ʳ�ＴΪҪ�Ե����ʳ���ˣ������Ҫ�ٴ�����һ��ʳ��
		while (1)
		{
			int flag = 1;
			srand((unsigned int)time(NULL));
			food.x = rand() % (MAPWIDTH - 4) + 2;
			food.y = rand() % (MAPHEIGHT - 2) + 1;

			//������ɵ�ʳ�ﲻ�����ߵ�������
			for (int i = 0; i < snake.len; i++)
			{
				if (snake.x[i] == food.x && snake.y[i] == food.y)
				{
					flag = 0;
					break;
				}
			}
			//������ɵ�ʳ�ﲻ�ܺ�����Ϊ������Ҳ����������������������
			if (flag && food.x % 2 == 0)
				break;
		}

		//����ʳ��
		gotoxy(food.x, food.y);
		printf("��");

		snake.len++;//�Ե�ʳ������ȼ�1
		score += 10;//ÿ��ʳ���10��
		snake.speed -= 5;//���ųԵ�ʳ��Խ��Խ�࣬�ٶȻ�Խ��Խ��
		changeFlag = 1;//����Ҫ����Ϊ�Ե���ʳ��Ͳ����ٲ�����β����һ�ڣ��Դ������������������Ч��
	}
	return;
}

void keyDown()
{
	int pre_key = key;//��¼ǰһ�������ķ���
	if (_kbhit())//����û������˼����е�ĳ����
	{
		fflush(stdin);//��ջ��������ַ�

		//getch()��ȡ�������ʱ�򣬻᷵�����Σ���һ�ε��÷���0����224���ڶ��ε��÷��صĲ���ʵ��ֵ
		key = _getch();//��һ�ε��÷��صĲ���ʵ��ֵ
		key = _getch();//�ڶ��ε��÷���ʵ��ֵ
	}

	/*
	*���ƶ�ʱ���Ȳ�ȥ��β��һ��
	*changeFlagΪ0������ʱû�гԵ�ʳ����ÿ��һ����Ҫ��������β���Դ�Ӫ��һ���ƶ���Ч��
	*Ϊ1�����Ե���ʳ��Ͳ���Ҫ������β���Դ�Ӫ��һ������������Ч��
	*/
	if (changeFlag == 0)
	{
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf("  ");//����β������ո񼴲�ȥ��β
	}

	//���ߵ�ÿһ��������ǰ�ƶ�һ�ڣ���ͷ���⣩
	for (int i = snake.len - 1; i > 0; i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}

	//�ߵ�ǰ�ƶ��ķ����ܺ�ǰһ�εķ����෴�������������ߵ�ʱ����ֱ�Ӱ��Ҽ�������
	//�����ǰ�ƶ������ǰһ�η����෴�Ļ����ѵ�ǰ�ƶ��ķ����Ϊǰһ�εķ���
	if (pre_key == 72 && key == 80)
		key = 72;
	if (pre_key == 80 && key == 72)
		key = 80;
	if (pre_key == 75 && key == 77)
		key = 75;
	if (pre_key == 77 && key == 75)
		key = 77;

	/**
	*����̨���������������
	*��������72
	*��������80
	*��������75
	*��������77
	*/

	//�ж���ͷӦ�����ĸ������ƶ�
	switch (key)
	{
	case 75:
		snake.x[0] -= 2;//����
		break;
	case 77:
		snake.x[0] += 2;//����
		break;
	case 72:
		snake.y[0]--;//����
		break;
	case 80:
		snake.y[0]++;//����
		break;
	}
	//��ӡ����ͷ
	gotoxy(snake.x[0], snake.y[0]);
	printf("��");
	gotoxy(MAPWIDTH - 2, 0);
	//����Ŀǰû�гԵ�ʳ�changFlagֵΪ0
	changeFlag = 0;
	return;
}

void startGame()
{
	system("cls");
	system("1000");
	system("mode con cols=120 lines=30");
	for (int i = 0;i <= MAPWIDTH;i += 2)//ע����ں���ռ������λ��
	{
		gotoxy(i, 0);
		printf("��");
		gotoxy(i, MAPHEIGHT);
		printf("��");
	}
	for (int i = 1;i < MAPHEIGHT;i++)//ע���ʱӦ����i = 1
	{
		gotoxy(0, i);
		printf("��");
		gotoxy(MAPWIDTH, i);
		printf("��");
	}

	while (1)
	{
		srand((unsigned int)time(NULL));
		food.x = rand() % (MAPWIDTH - 4) + 2;
		food.y = rand() % (MAPHEIGHT - 2) + 1;
		//���ɵ�ʳ����������ż����ͳ���ʱ��ͷ�����������żһ�£���Ϊһ���ַ�ռ�����ֽ�λ�ã�����һ��
		//�ᵼ�³�ʳ���ʱ��ֻ�Ե�һ��
		if (food.x % 2 == 0)
			break;
	}
	//������Ƶ�ʳ������괦��ӡʳ��
	gotoxy(food.x, food.y);
	printf("��");

	snake.len = 3;
	snake.speed = 200;

	//����Ļ�м�������ͷ
	snake.x[0] = MAPWIDTH / 2 + 1;//x����Ϊż��
	snake.y[0] = MAPHEIGHT / 2;
	//��ӡ��ͷ
	gotoxy(snake.x[0], snake.y[0]);
	printf("��");

	//���ɳ��Ե�����
	for (int i = 1; i < snake.len; i++)
	{
		//����Ĵ�ӡ�������겻�䣬������Ϊ��һ�����������ֵ+2
		snake.x[i] = snake.x[i - 1] + 2;
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		printf("��");
	}
	gotoxy(MAPWIDTH - 2, 0);//������ƶ����Ϸ����������

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
	printf("������Ϸ�÷�Ϊ��%d\n", score);
	rank();//д�����������
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
	//��ͷ�������±߽磬��Ϸ����
	if (snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)
		return false;
	//��ͷ�������ұ߽磬��Ϸ����
	if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH)
		return false;
	//��ͷ����������Ϸ����
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
	printf("�������������֣�");
	scanf("%s", &tmp.name);
	tmp.score = score;
	fp = fopen("3.txt", "a");
	if (fp == NULL)
	{
		printf("�ļ��𻵣�");
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
		printf("�ļ��𻵣�");
		exit(1);
	}
	for (i = 0;i < n - 1;i++)//����ð�ݷ��������� 
	{
		k = i;
		for (j = i + 1;j < n;j++)
			if (tmp[j].score > tmp[k].score)//����Ƚϴ�С  
				k = j;
		temp = tmp[k];
		tmp[k] = tmp[i];
		tmp[i] = temp;//����������ʱ�������ݽ���
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



