#pragma once
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//类型声明

//蛇的方向
enum DIRECTION
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
};

//蛇的状态
//正常，撞墙，撞到自己，正常退出
enum GAME_STATUS
{
	OK,
	KILL_BY_WALL,
	KILL_BY_SELF,
	EXIT
};
//蛇身的节点类型
typedef struct SnakeNode
{
	int x;
	int y;
	//指向下一个节点的指针
	struct SnakeNode* next;
}SnakeNode,*pSnakeNode;

//贪吃蛇
typedef struct Snake
{
	pSnakeNode _phead;//蛇头
	pSnakeNode _pFood;//食物
	enum DIRECTION _dir;//方向
	enum GAME_STATUS _status;//游戏状态
	int _food_weight;//食物分数
	int _score;      //总分数
	int _sleep_time;//休眠时间，越短蛇移动速度越快
}Snake, * pSnake;

void SetPos(short x, short y);

void GameStart(pSnake ps);

void WelcomeToGame();

void CreatMap();

void InitSnake(pSnake ps);

void CreatFood(pSnake ps);

void GameRun(pSnake ps);

void GameEnd(pSnake ps);




