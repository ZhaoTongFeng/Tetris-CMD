#pragma once
#include <time.h>
#include <conio.h>
#include <iostream>

using namespace std;




//锚点
//struct Point {
//	Point(int a, int b) :x(a), y(b) {}
//	int x, y;
//};
class Vector2 {
public:
	Vector2() { x = 0; y = 0; }
	Vector2(int a, int b) :x(a), y(b) {}
	int x, y;
};

class Point {
public:
	Point(int a, int b) :col(a), row(b) {}
	int col, row;
	int dir_x = 0, dir_y = 0;

	Vector2 pos, dir;
};

class Game {
public:
	~Game();
	bool Initialize();
	void Loop();
	void Shutdown();

private:

	//按键输入
	void ProcessInput();
	//更新游戏
	void UpdateGame();
	//输出图像
	void GenerateOutput();



	//设置锚点所占地图区域，用于碰撞检测
	void SetArea();
	//碰撞检测
	bool CheckArea(unsigned char targetBlock[4][4]);

	//生成方块
	void SpawnBlock();

	//旋转
	void Rotate();

	//根据blockIndex设置currentBlock
	void SetCurrentBlock();

	//消除行
	void RemoveRow();
	//固定方块
	void FixBlocks();


	//装载图形的矩阵宽度为4
	const int boxLength = 4;
	//当前方块在blocks中的下标
	int blockIndex;
	//方块的各种角度，总数11个
	const unsigned char blocks[13][4][4] = {
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,1,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{0,0,1,0},
			{1,1,1,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,0,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{0,1,0,0},
			{1,1,1,0}
		},
		{
			{0,0,0,0},
			{0,0,1,0},
			{0,1,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		}
	};

	//方块种类个数
	const int typeLength = 5;
	//当前形状下标
	int numIndex;
	//每种方块所占角度个数
	const unsigned char blockNum[5] = { 4,4,2,1,2 };
	//方块范围,0-3,4-7...
	const unsigned char sumNum[5] = { 0,4,8,10,11 };

	//当前方块
	unsigned char currentBlock[4][4];
	//方块所在的区域
	unsigned char area[4][4];


	//方块活动区域大小
	Vector2 activitySize = Vector2(10, 20);
	//整个画布大小
	Vector2 mapSize;
	//画布
	unsigned char** map;

	//围墙宽度
	const int wallWidth = 1;


	//锚点
	Point* p;

	//累计毫秒数
	clock_t tickCount, currentTick;

	//是否需要更新游戏
	bool update = true;
	//游戏是否结束
	bool isRunning = true;

	bool rotate;
};