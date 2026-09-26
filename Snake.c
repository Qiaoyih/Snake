#include "Snake.h"

#define WALL L'□' //表示墙的宽字符
#define BODY L'●' //表示蛇身节点的宽字符
#define FOOD L'★' //表示食物的宽字符
#define POS_X 24  //蛇头初始x坐标
#define POS_Y 5   //蛇头初始y坐标

#define KEY_PRESS(VK)  (GetAsyncKeyState(VK)&0X01)? 1:0   //检测按键之前是否按下过

//定位光标位置
void SetPos(short x, short y)
{
	HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x,y };
	SetConsoleCursorPosition(houtput, pos);
}

void WelcomeToGame()
{
	SetPos(40, 14);
	printf("欢迎来到贪吃蛇小游戏");
	SetPos(42, 24);
	system("pause");
	system("cls");
	SetPos(25, 12);
	printf("用 ↑.↓.←.→ 分别控制蛇的移动， F1为加速，F2为减速");
	SetPos(25, 13);
	printf("加速能获得更高的分数");
	SetPos(42, 24);
	system("pause");
	system("cls");
}


void CreatMap()
{
	int i = 0;
	for (i = 0;i < 29;i++)
	{
		wprintf(L"%lc",WALL);	
	}
	SetPos(0, 26);
	for (i = 0;i < 29;i++)
	{
		wprintf(L"%lc", WALL);
	}
	for (i = 1;i < 26;i++)
	{
		SetPos(0, i);
		wprintf(L"%lc", WALL);
	}
	for (i = 1;i < 26;i++)
	{
		SetPos(56, i);
		wprintf(L"%lc", WALL);
	}
	
}


void InitSnake(pSnake ps)
{
	pSnakeNode cur = NULL;
	int i = 0;
	for (i = 0;i < 5;i++)
	{
		//创建蛇身节点
		cur = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (cur == NULL)
		{
			perror("InitSnake()::malloc()");
			return;
		}
		cur->next = NULL;
		cur->x = POS_X + i * 2;
		cur->y = POS_Y;
		//头插法
		if (ps->_phead == NULL)
		{
			ps->_phead = cur;
		}
		else
		{
			cur->next = ps->_phead;
			ps->_phead = cur;
		}
	}
	//打印蛇的身体
	cur = ps->_phead;
	while (cur)
	{
		SetPos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}
	//初始化蛇数据
	ps->_dir = RIGHT;
	ps->_food_weight = 10;
	ps->_score = 0;
	ps->_sleep_time = 200;
	ps->_status = OK;
	
}


//随机生成食物坐标，不能与蛇身重合，x必须是2的倍数，必须在地图内部
void CreatFood(pSnake ps)
{
	//生成食物坐标
	int x;
	int y;
again:
	do
	{
		x = rand() % 53 + 2;
		y = rand() % 25 + 1;
	} while (x % 2 != 0);
	pSnakeNode cur = ps->_phead;
	while (cur)
	{
		if (x == cur->x && y == cur->y)
		{
			goto again;
		}
		cur = cur->next;
	}
	//创建食物节点
	ps->_pFood = (pSnakeNode)malloc(sizeof(SnakeNode));
	if (ps->_pFood == NULL)
	{
		perror("CreatFood()::malloc()");
		return;
	}
	ps->_pFood->x = x;
	ps->_pFood->y = y;
	ps->_pFood->next = NULL;
	SetPos(x, y);
	wprintf(L"%lc", FOOD);

}


void GameStart(pSnake ps)
{
	//0.设置窗口大小，隐藏光标
	system("mode con cols=100 lines=30");
	system("title 贪吃蛇");
	HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(houtput, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(houtput, &CursorInfo);


	
	//1.打印环境界面和功能介绍
	WelcomeToGame();
	
	//2.创建地图
	CreatMap();

	//3.创建蛇
	InitSnake(ps);

	//4.创建食物
	CreatFood(ps);


}


void PrintHelpInfo()
{
	SetPos(64, 17);
	printf("不能穿墙，不能咬到自己");
	SetPos(64, 18);
	printf("用 ↑.↓.←.→ 分别控制蛇的移动");
	SetPos(64, 19);
	printf("F1为加速，F2为减速");
	SetPos(64, 20);
	printf("ESC：退出游戏. SPACE：暂停游戏.");
}

//暂停
void Pause(pSnake ps)
{
	while (1)
	{
		Sleep(200);
		if (KEY_PRESS(VK_SPACE))
		{
			break;
		}
		if (KEY_PRESS(VK_ESCAPE))
		{
			ps->_status = EXIT;
			break;
		}
	}
}


int NextIsFood(pSnake ps,pSnakeNode psn)
{
	return (psn->x == ps->_pFood->x) && (psn->y == ps->_pFood->y);
}


void EatFood(pSnake ps, pSnakeNode psn)
{
	psn->next = ps->_phead;
	ps->_phead = psn;
	pSnakeNode cur = ps->_phead;
	while (cur)
	{
		SetPos(cur->x, cur->y);
		wprintf(L"%c", BODY);
		cur = cur->next;
	}

	ps->_score += ps->_food_weight;
	free(ps->_pFood);
	CreatFood(ps);

}


void NoFood(pSnake ps, pSnakeNode psn)
{
	psn->next = ps->_phead;
	ps->_phead = psn;
	pSnakeNode cur = ps->_phead;
	while (cur->next->next)
	{
		SetPos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}
	SetPos(cur->next->x, cur->next->y);
	printf("  ");
	free(cur->next);
	cur->next = NULL;
}


void KillByWall(pSnake ps)
{
	if (ps->_phead->x == 0 || ps->_phead->x == 56 || ps->_phead->y == 0 || ps->_phead->y == 26)
	{
		ps->_status = KILL_BY_WALL;
	}
}


void KillBySelf(pSnake ps)
{
	pSnakeNode cur = ps->_phead->next;
	while (cur)
	{
		if ((cur->x == ps->_phead->x) && (cur->y == ps->_phead->y))
		{
			ps->_status = KILL_BY_SELF;
		}
		cur = cur->next;
	}
}


void SnakeMove(pSnake ps)
{
	//设置下个节点的坐标
	pSnakeNode pNextNode = (pSnakeNode)malloc(sizeof(SnakeNode));
	if (pNextNode == NULL)
	{
		perror("SnakeNode()::malloc()");
		return;
	}
	switch (ps->_dir)
	{
		case UP:
		{
			pNextNode->x = ps->_phead->x;
			pNextNode->y = ps->_phead->y-1;		
		}
		break;
		case DOWN:
		{
			pNextNode->x = ps->_phead->x;
			pNextNode->y = ps->_phead->y + 1;
		}
		break;
		case LEFT:
		{
			pNextNode->x = ps->_phead->x - 2;
			pNextNode->y = ps->_phead->y;
		}
		break;
		case RIGHT:
		{
			pNextNode->x = ps->_phead->x + 2;
			pNextNode->y = ps->_phead->y;
		}
		break;
	}
	//判断下个节点是否是食物
	if (NextIsFood(ps,pNextNode))
	{
		EatFood(ps, pNextNode);
	}
	else
	{
		NoFood(ps, pNextNode);
	}
	//检测蛇是否撞墙
	KillByWall(ps);

	//检测蛇是否撞到自己
	KillBySelf(ps);
}



void GameRun(pSnake ps)
{
	//打印帮助信息
	PrintHelpInfo();
	//按键检测
	do
	{
		SetPos(64, 12);
		printf("得分：%d 每个食物得分：%d分", ps->_score, ps->_food_weight);
		if ((KEY_PRESS(VK_UP)) && (ps->_dir != DOWN))
		{
			ps->_dir = UP;
		}
		else if ((KEY_PRESS(VK_DOWN)) && (ps->_dir != UP))
		{
			ps->_dir = DOWN;
		}
		else if ((KEY_PRESS(VK_LEFT)) && (ps->_dir != RIGHT))
		{
			ps->_dir = LEFT;
		}
		else if ((KEY_PRESS(VK_RIGHT)) && (ps->_dir != LEFT))
		{
			ps->_dir = RIGHT;
		}
		else if (KEY_PRESS(VK_F1))
		{
			if (ps->_sleep_time >= 80)
			{
				ps->_sleep_time -= 30;
				ps->_food_weight += 2;
			}
		}
		else if (KEY_PRESS(VK_F2))
		{
			if (ps->_sleep_time < 320)
			{
				ps->_sleep_time += 30;
				ps->_food_weight -= 2;
			}
		}
		else if (KEY_PRESS(VK_SPACE))
		{
			Pause(ps);
		}
		else if (KEY_PRESS(VK_ESCAPE))
		{
			ps->_status = EXIT;
			break;
		}
		
		//蛇每次一定之间要休眠的时间，时间短，蛇移动速度就快
		SnakeMove(ps);
		Sleep(ps->_sleep_time);
	} while (ps->_status == OK);	

}


void GameEnd(pSnake ps)
{
	SetPos(24, 12);
	switch (ps->_status)
	{
	case EXIT:
		printf("您主动退出游戏\n");
		break;
	case KILL_BY_WALL:
		printf("您撞墙了，游戏结束\n");
		break;
	case KILL_BY_SELF:
		printf("您撞到自己了，游戏结束\n");
		break;
	}
	//释放蛇身链表
	pSnakeNode cur = ps->_phead;
	while (cur)
	{
		pSnakeNode del = cur;
		cur = cur->next;
		free(del);
	}
}