#include <stdio.h>
#include <vector>		//使用vector容器
#include <graphics.h>	//使用easyX图形库
#include <time.h>  //使用了time函数
#include <tchar.h>  //将int类型变为字符串类型打印
using namespace std;

/*游戏参数*/
//枚举类型:蛇头方向
enum Direction{UP, DOWN, LEFT, RIGHT};
//网格坐标常量
const int GRID_SIZE = 20;
const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 20;

//结构体
struct coordinate
{
	int x;
	int y;
};
//Snake类
class Snake
{
private:
	//vector容器声明
	vector<coordinate> snakebody;
	//蛇头方向声明
	Direction direction;
	//随机数生成
	int randnumX = rand() % GRID_WIDTH;
	int randnumY = rand() % GRID_HEIGHT;
public:
	Snake()  //构造初始化蛇的函数
	{
		//蛇头初始网格坐标变量
		int headX = GRID_WIDTH / 2;
		int headY = GRID_HEIGHT / 2;
		//vector容器定义
		snakebody.push_back({ headX, headY });
		snakebody.push_back({ headX + 1, headY });
		snakebody.push_back({ headX + 2, headY });
		snakebody.push_back({ headX + 3, headY });
		//初始化蛇头方向
		direction = LEFT;
	}
	void directionChange(Direction newdirection)  //防止朝相反方向移动蛇头的函数
	{
		if (!(	direction == UP		&&	newdirection == DOWN	||
				direction == DOWN	&&	newdirection == UP		||
				direction == LEFT	&&	newdirection == RIGHT	||
				direction == RIGHT	&&	newdirection == LEFT))
			direction = newdirection;
	}
	/*蛇的移动*/
	void snakeMove()  //蛇的移动的函数
	{

		int i = snakebody.size() - 1;  //蛇尾网格坐标
		//蛇尾覆盖
		setfillcolor(BLACK);
		fillrectangle
		(
			 snakebody[i].x * GRID_SIZE,       snakebody[i].y * GRID_SIZE,
			(snakebody[i].x + 1) * GRID_SIZE, (snakebody[i].y + 1) * GRID_SIZE
		);
		//蛇身移动
		for (; i > 0; i--)
		{
			snakebody[i] = snakebody[i - 1];
		}
		//蛇头移动
		switch (direction)
		{
		case UP:	snakebody[0].y--; break;
		case DOWN:	snakebody[0].y++; break;
		case LEFT:	snakebody[0].x--; break;
		case RIGHT:	snakebody[0].x++; break;
		}
		//实现穿墙
		if (snakebody[0].x < 0) snakebody[0].x = GRID_WIDTH - 1;
		if (snakebody[0].x >= GRID_WIDTH) snakebody[0].x = 0;
		if (snakebody[0].y < 0) snakebody[0].y = GRID_HEIGHT - 1;
		if (snakebody[0].y >= GRID_HEIGHT) snakebody[0].y = 0;
	}
	/*蛇的绘制*/
	void drawSnake()  //蛇的绘制函数
	{
		//蛇身的绘制
		setfillcolor(LIGHTGREEN);
		for (int i = 1; i < snakebody.size(); i++)
		{
			fillrectangle
			(
				snakebody[i].x * GRID_SIZE, snakebody[i].y * GRID_SIZE,
				(snakebody[i].x + 1) * GRID_SIZE, (snakebody[i].y + 1) * GRID_SIZE
			);
		}
		//蛇头的绘制
		setfillcolor(GREEN);
		fillrectangle
		(
			 snakebody[0].x * GRID_SIZE, snakebody[0].y * GRID_SIZE,
			(snakebody[0].x + 1) * GRID_SIZE, (snakebody[0].y + 1) * GRID_SIZE
		);
	}
	/*生成果子*/
	void generateFood()
	{
		
		//不和蛇身重合检查
		for (int i = 0; i < snakebody.size(); i++)
		{
			if (randnumX == snakebody[i].x && randnumY == snakebody[i].y)
			{
				randnumX = rand() % GRID_WIDTH;
				randnumY = rand() % GRID_HEIGHT;
			}
		}
		//绘制果子
		setfillcolor(MAGENTA);
		fillcircle
		(
			(randnumX * GRID_SIZE + (randnumX + 1) * GRID_SIZE)/2,
			(randnumY * GRID_SIZE + (randnumY + 1) * GRID_SIZE)/2,
			GRID_SIZE / 2
		);
	}
	/*蛇吃果子后*/
	void afterEatFood()
	{
		if (snakebody[0].x == randnumX && snakebody[0].y == randnumY)
		{	
			//增长蛇身
			coordinate newsnakebody = snakebody.back();
			snakebody.push_back(newsnakebody);
			//再生成果子
			for (int i = 0; i < snakebody.size(); i++)  //不和蛇身重合检查
			{
				if (randnumX == snakebody[i].x && randnumY == snakebody[i].y)
				{
					randnumX = rand() % GRID_WIDTH;
					randnumY = rand() % GRID_HEIGHT;
				}
			}
			setfillcolor(MAGENTA);  //绘制果子
			fillcircle
			(
				(randnumX * GRID_SIZE + (randnumX + 1) * GRID_SIZE) / 2,
				(randnumY * GRID_SIZE + (randnumY + 1) * GRID_SIZE) / 2,
				GRID_SIZE / 2
			);
		}
	}
	/*蛇吃自己*/
	void eatItself()
	{
		for (int i = 1; i < snakebody.size(); i++)
		{
			if (snakebody[0].x == snakebody[i].x && snakebody[0].y == snakebody[i].y)
			{
				//游戏结束

				cleardevice();
				settextstyle(39, 39, _T("宋体"));
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
	//初始化窗口
	initgraph(GRID_WIDTH * GRID_SIZE , GRID_HEIGHT * GRID_SIZE);
	/*初始化网格*/
	//初始化网格_横线
	int num_crosswise;
	for (num_crosswise = 0; num_crosswise <= GRID_HEIGHT; num_crosswise++)
	{
		line(0, GRID_SIZE * num_crosswise, GRID_WIDTH * GRID_SIZE, GRID_SIZE * num_crosswise);
	}
	//初始化网格_竖线
	int num_vertical;
	for (num_vertical = 0; num_vertical <= GRID_WIDTH; num_vertical++)
	{
		line(GRID_SIZE * num_vertical, 0, GRID_SIZE * num_vertical, GRID_HEIGHT * GRID_SIZE);
	}
	//时间种子初始化
	srand((unsigned int)time(NULL));
	//创建蛇对象
	Snake snake;
	//果子初始化
	snake.generateFood();
	//游戏主循环
	while (true)
	{
		//键盘控制
			 if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000))
			snake.directionChange(UP);
		else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000))
			snake.directionChange(DOWN);
		else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000))
			snake.directionChange(LEFT);
		else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000))
			snake.directionChange(RIGHT);
		//蛇的绘制
			 snake.drawSnake();
		//蛇的移动
			 snake.snakeMove();
		//蛇吃果子判定
			 snake.afterEatFood();
		//蛇吃自己判定
			 snake.eatItself();
		//蛇的移动速度调控
			 Sleep(100);

	}
		
}