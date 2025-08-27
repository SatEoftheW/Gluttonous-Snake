#include <stdio.h>
#include <vector>		//ʹ��vector����
#include <graphics.h>	//ʹ��easyXͼ�ο�
#include <time.h>  //ʹ����time����
#include <tchar.h>  //��int���ͱ�Ϊ�ַ������ʹ�ӡ
using namespace std;

/*��Ϸ����*/
//ö������:��ͷ����
enum Direction{UP, DOWN, LEFT, RIGHT};
//�������곣��
const int GRID_SIZE = 20;
const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 20;

//�ṹ��
struct coordinate
{
	int x;
	int y;
};
//Snake��
class Snake
{
private:
	//vector��������
	vector<coordinate> snakebody;
	//��ͷ��������
	Direction direction;
	//���������
	int randnumX = rand() % GRID_WIDTH;
	int randnumY = rand() % GRID_HEIGHT;
public:
	Snake()  //�����ʼ���ߵĺ���
	{
		//��ͷ��ʼ�����������
		int headX = GRID_WIDTH / 2;
		int headY = GRID_HEIGHT / 2;
		//vector��������
		snakebody.push_back({ headX, headY });
		snakebody.push_back({ headX + 1, headY });
		snakebody.push_back({ headX + 2, headY });
		snakebody.push_back({ headX + 3, headY });
		//��ʼ����ͷ����
		direction = LEFT;
	}
	void directionChange(Direction newdirection)  //��ֹ���෴�����ƶ���ͷ�ĺ���
	{
		if (!(	direction == UP		&&	newdirection == DOWN	||
				direction == DOWN	&&	newdirection == UP		||
				direction == LEFT	&&	newdirection == RIGHT	||
				direction == RIGHT	&&	newdirection == LEFT))
			direction = newdirection;
	}
	/*�ߵ��ƶ�*/
	void snakeMove()  //�ߵ��ƶ��ĺ���
	{

		int i = snakebody.size() - 1;  //��β��������
		//��β����
		setfillcolor(BLACK);
		fillrectangle
		(
			 snakebody[i].x * GRID_SIZE,       snakebody[i].y * GRID_SIZE,
			(snakebody[i].x + 1) * GRID_SIZE, (snakebody[i].y + 1) * GRID_SIZE
		);
		//�����ƶ�
		for (; i > 0; i--)
		{
			snakebody[i] = snakebody[i - 1];
		}
		//��ͷ�ƶ�
		switch (direction)
		{
		case UP:	snakebody[0].y--; break;
		case DOWN:	snakebody[0].y++; break;
		case LEFT:	snakebody[0].x--; break;
		case RIGHT:	snakebody[0].x++; break;
		}
		//ʵ�ִ�ǽ
		if (snakebody[0].x < 0) snakebody[0].x = GRID_WIDTH - 1;
		if (snakebody[0].x >= GRID_WIDTH) snakebody[0].x = 0;
		if (snakebody[0].y < 0) snakebody[0].y = GRID_HEIGHT - 1;
		if (snakebody[0].y >= GRID_HEIGHT) snakebody[0].y = 0;
	}
	/*�ߵĻ���*/
	void drawSnake()  //�ߵĻ��ƺ���
	{
		//����Ļ���
		setfillcolor(LIGHTGREEN);
		for (int i = 1; i < snakebody.size(); i++)
		{
			fillrectangle
			(
				snakebody[i].x * GRID_SIZE, snakebody[i].y * GRID_SIZE,
				(snakebody[i].x + 1) * GRID_SIZE, (snakebody[i].y + 1) * GRID_SIZE
			);
		}
		//��ͷ�Ļ���
		setfillcolor(GREEN);
		fillrectangle
		(
			 snakebody[0].x * GRID_SIZE, snakebody[0].y * GRID_SIZE,
			(snakebody[0].x + 1) * GRID_SIZE, (snakebody[0].y + 1) * GRID_SIZE
		);
	}
	/*���ɹ���*/
	void generateFood()
	{
		
		//���������غϼ��
		for (int i = 0; i < snakebody.size(); i++)
		{
			if (randnumX == snakebody[i].x && randnumY == snakebody[i].y)
			{
				randnumX = rand() % GRID_WIDTH;
				randnumY = rand() % GRID_HEIGHT;
			}
		}
		//���ƹ���
		setfillcolor(MAGENTA);
		fillcircle
		(
			(randnumX * GRID_SIZE + (randnumX + 1) * GRID_SIZE)/2,
			(randnumY * GRID_SIZE + (randnumY + 1) * GRID_SIZE)/2,
			GRID_SIZE / 2
		);
	}
	/*�߳Թ��Ӻ�*/
	void afterEatFood()
	{
		if (snakebody[0].x == randnumX && snakebody[0].y == randnumY)
		{	
			//��������
			coordinate newsnakebody = snakebody.back();
			snakebody.push_back(newsnakebody);
			//�����ɹ���
			for (int i = 0; i < snakebody.size(); i++)  //���������غϼ��
			{
				if (randnumX == snakebody[i].x && randnumY == snakebody[i].y)
				{
					randnumX = rand() % GRID_WIDTH;
					randnumY = rand() % GRID_HEIGHT;
				}
			}
			setfillcolor(MAGENTA);  //���ƹ���
			fillcircle
			(
				(randnumX * GRID_SIZE + (randnumX + 1) * GRID_SIZE) / 2,
				(randnumY * GRID_SIZE + (randnumY + 1) * GRID_SIZE) / 2,
				GRID_SIZE / 2
			);
		}
	}
	/*�߳��Լ�*/
	void eatItself()
	{
		for (int i = 1; i < snakebody.size(); i++)
		{
			if (snakebody[0].x == snakebody[i].x && snakebody[0].y == snakebody[i].y)
			{
				//��Ϸ����

				cleardevice();
				settextstyle(39, 39, _T("����"));
				outtextxy((GRID_WIDTH / 4) * GRID_SIZE, (GRID_HEIGHT / 4) * GRID_SIZE, _T("GAMEOVER"));
				int score = snakebody.size() - 4;
				TCHAR str[20];
				_stprintf_s(str, _T("%d"), score);
				outtextxy(200, 300, _T("Score:"));
				outtextxy(400, 300 , str);

				getchar();
			}
		}
	}
};


int main(void)
{
	//��ʼ������
	initgraph(GRID_WIDTH * GRID_SIZE , GRID_HEIGHT * GRID_SIZE);
	/*��ʼ������*/
	//��ʼ������_����
	int num_crosswise;
	for (num_crosswise = 0; num_crosswise <= GRID_HEIGHT; num_crosswise++)
	{
		line(0, GRID_SIZE * num_crosswise, GRID_WIDTH * GRID_SIZE, GRID_SIZE * num_crosswise);
	}
	//��ʼ������_����
	int num_vertical;
	for (num_vertical = 0; num_vertical <= GRID_WIDTH; num_vertical++)
	{
		line(GRID_SIZE * num_vertical, 0, GRID_SIZE * num_vertical, GRID_HEIGHT * GRID_SIZE);
	}
	//ʱ�����ӳ�ʼ��
	srand((unsigned int)time(NULL));
	//�����߶���
	Snake snake;
	//���ӳ�ʼ��
	snake.generateFood();
	//��Ϸ��ѭ��
	while (true)
	{
		//���̿���
			 if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000))
			snake.directionChange(UP);
		else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000))
			snake.directionChange(DOWN);
		else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000))
			snake.directionChange(LEFT);
		else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000))
			snake.directionChange(RIGHT);
		//�ߵĻ���
			 snake.drawSnake();
		//�ߵ��ƶ�
			 snake.snakeMove();
		//�߳Թ����ж�
			 snake.afterEatFood();
		//�߳��Լ��ж�
			 snake.eatItself();
		//�ߵ��ƶ��ٶȵ���
			 Sleep(100);

	}
		
}