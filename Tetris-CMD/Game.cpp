#include "Game.h"
#define _LOOP_ACTIVITY_BEGIN for (int i = 0; i < boxLength; i++) {for (int j = 0; j < boxLength; j++) {
#define _LOOP_ACTIVITY_END }}
#define _LOOP_MAP_BEGIN for (int i = 0; i < mapSize.y; i++) {for (int j = 0; j < mapSize.x; j++) {
#define _LOOP_MAP_END }}

bool Game::Initialize()
{
	//初始化地图
	mapSize = Vector2(activitySize.x + 2 * (boxLength + wallWidth), activitySize.y + wallWidth + boxLength);
	map = new unsigned char* [mapSize.y];
	for (int i = 0; i < mapSize.y; i++) {
		map[i] = new unsigned char[mapSize.x];
		for (int j = 0; j < mapSize.x; j++) {
			map[i][j] = 0;
			if (j >= mapSize.x - boxLength - wallWidth || j <= boxLength + wallWidth - 1) {
				map[i][j] = 3;
			}
			if (i == mapSize.y - 1) {
				map[i][j] = 3;
			}
		}
	}
	tickCount = clock();
	SpawnBlock();
	return true;
}

void Game::Loop()
{
	while (isRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}


void Game::Shutdown()
{
	for (int i = 0; i < mapSize.y; i++) {
		delete[] map[i];
	}
	delete[] map;
}

void Game::ProcessInput()
{
	//处理输入
	//有键盘输入返回1，没有返回0
	p->dir_x = 0;
	p->dir_y = 0;

	if (_kbhit())
	{
		update = true;
		int key = _getch();
		if (key == 'a') {
			p->dir_x -= 1;
		}
		if (key == 'd') {
			p->dir_x += 1;
		}
		if (key == 's') {
			p->dir_y += 1;
		}
		if (key == 'w') {
			rotate = true;
		}
	}
}

void Game::UpdateGame()
{
	currentTick = clock();
	if (currentTick - tickCount > 1000) {
		update = true;
		p->dir_y = 1;
		tickCount = currentTick;
	}

	//更新游戏
	if (!update) { return; }

	//左右移动
	if (p->dir_x != 0) {
		p->col += p->dir_x;
		if (CheckArea(currentBlock)) {
			p->col -= p->dir_x;
		}
	}

	//向下移动
	if (p->dir_y != 0) {
		p->row += p->dir_y;
		//检查是否会发生碰撞，如果会发生碰撞就不执行移动操作，固定方块生成新方块
		if (CheckArea(currentBlock)) {
			p->row -= p->dir_y;
			FixBlocks();
		}
	}
	if (rotate) {
		Rotate();
		rotate = false;
	}
}

void Game::SpawnBlock()
{
	delete p;
	p = new Point((int)(mapSize.x - boxLength) / 2, boxLength - 1);
	srand((int)time(0));
	//随机抽取形状
	numIndex = rand() % typeLength;
	//设置当前形状下标
	blockIndex = sumNum[numIndex] + rand() % blockNum[numIndex];
	SetCurrentBlock();
	//地图中生成方块
	_LOOP_ACTIVITY_BEGIN;
	map[p->row + i][p->col + j] = currentBlock[boxLength - i - 1][j];
	_LOOP_ACTIVITY_END;
}

void Game::Rotate()
{
	//旋转之后的下标
	int temp = blockIndex;
	blockIndex = sumNum[numIndex] + (((blockIndex - sumNum[numIndex]) % blockNum[numIndex]) + 1) % blockNum[numIndex];
	unsigned char targetBlock[4][4];
	_LOOP_ACTIVITY_BEGIN;
	targetBlock[i][j] = blocks[blockIndex][i][j];
	_LOOP_ACTIVITY_END;
	if (CheckArea(targetBlock)) {
		blockIndex = temp;
	}
	else {
		SetCurrentBlock();
	}
}

void Game::SetCurrentBlock()
{
	_LOOP_ACTIVITY_BEGIN;
	currentBlock[i][j] = blocks[blockIndex][i][j];
	_LOOP_ACTIVITY_END;
}

void Game::RemoveRow()
{
	for (int i = 0; i < mapSize.y; i++) {
		int sum = 0;
		for (int j = 0; j < mapSize.x; j++) {
			if (map[i][j] == 2) {
				sum++;
			}
		}
		if (sum == activitySize.x) {
			for (int n = i; n > 0; n--) {
				for (int m = 0; m < mapSize.x; m++) {
					map[n][m] = map[n - 1][m];
				}
			}
		}
	}
}

void Game::FixBlocks()
{
	_LOOP_ACTIVITY_BEGIN;
	int x = p->row - boxLength + 1 + i;
	if (map[x][p->col + j] == 1) {
		//检查游戏是否结束
		if (x == 5) {
			isRunning = false;
			return;
		}
		map[x][p->col + j] = 2;
	}
	_LOOP_ACTIVITY_END;
	RemoveRow();
	SpawnBlock();
}

bool Game::CheckArea(unsigned char targetBlock[4][4])
{
	
	_LOOP_ACTIVITY_BEGIN;
	//设置锚点所占地图区域
	area[i][j] = map[p->row - boxLength + 1 + i][p->col + j];
	if ((area[i][j] == 3 || area[i][j] == 2) && targetBlock[i][j] == 1) {
		return true;
	}
	_LOOP_ACTIVITY_END;
	return false;
}

void Game::GenerateOutput() {
	if (!update) {
		return;
	}
	system("cls");

	//清除之前的移动方块
	_LOOP_MAP_BEGIN;
	if (map[i][j] == 1) {
		map[i][j] = 0;
	}
	_LOOP_MAP_END;

	//把现在的把移动方块画上去
	_LOOP_ACTIVITY_BEGIN;
	if (map[p->row - boxLength + 1 + i][p->col + j] == 0) {
		map[p->row - boxLength + 1 + i][p->col + j] = currentBlock[i][j];
	}
	_LOOP_ACTIVITY_END;

	//输出
	_LOOP_MAP_BEGIN;
	if (map[i][j] == 0) {
		printf("  ");
	}
	else if (map[i][j] == 1) {
		printf("△");
	}
	else if (map[i][j] == 2) {
		printf("□");
	}
	else if (map[i][j] == 3) {
		printf("☆");
	}
	if (j == mapSize.x - 1) {
		cout << endl;
	}
	_LOOP_MAP_END;

	//关闭更新
	update = false;
}