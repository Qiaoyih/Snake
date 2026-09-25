#include <locale.h>
#include "Snake.h"

//游戏测试逻辑
void test()
{
	//创建贪吃蛇
	Snake snake = { 0 };
	//游戏初始化
	//1.打印环境界面和功能介绍
	//2.创建地图
	//3.创建蛇
	//4.创建食物
	GameStart(&snake);

	//运行游戏
	GameRun(&snake);


	//结束游戏
	GameEnd(&snake);
	
}


int main()
{
	//适配本地环境
	setlocale(LC_ALL, "");
	//为生成坐标取随机数
	srand((unsigned int)time(NULL));

	int ch = 0;
	do
	{
		test();
		SetPos(24, 15);
		printf("再来一局吗？(Y/N):");
		ch = getchar();
		getchar(); 
	} while (ch == 'Y' || ch == 'y');
	SetPos(0, 27);
	return 0;
}