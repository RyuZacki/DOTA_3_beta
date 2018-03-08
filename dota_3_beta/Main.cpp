#include "iostream"
#include "string" 
#include "conio.h"
#include "time.h"
#include "windows.h"

using namespace std;

COORD position = { 0, 0 }; //ïîçèöèÿ x è y
HANDLE ñonsole = GetStdHandle(STD_OUTPUT_HANDLE);//êîíñîëü

const int FPS = 1000 / 60;

const int SPACE = 0;
const int WALL = 1;
const int PLAYER = 2;
const int RUNE = 3;
const int MINE = 4;

const int H = 16;
const int W = 16;

bool isStart = true;

struct point
{
	int x;
	int y;
};

class player
{
public:
	string name;
	double health;
	double mana;
	point cord;
	
	void movePlayer(int dx, int dy)
	{
		int newX = this->cord.x + dx;
		int newY = this->cord.y + dy;

		map[this->cord.y][this->cord.x] = SPACE;

		this->checkCollision(newX, newY);
	}

	void checkCollision(int newX, int newY)
	{
		switch (map[newY][newX])
		{
		case WALL: newX = this->cord.x; newY = this->cord.y; break;
		case RUNE: this->mana += 100; createObject(RUNE); break;
		case MINE: this->health -= 10; createObject(MINE); break;
		}

		this->cord.x = newX;
		this->cord.y = newY;
	}
};

player p;

int map[H][W] = 
{ 
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,0,1,0,0,0,0,1,0,1,0,0,0,0,0,1 },
	{ 1,0,1,1,1,1,0,1,0,1,0,1,1,1,3,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,1 },
	{ 1,0,0,0,0,1,0,0,0,0,1,0,1,1,0,1 },
	{ 1,0,1,0,0,1,0,1,0,3,1,0,0,0,0,1 },
	{ 1,0,1,0,1,1,1,1,0,1,1,0,1,1,1,1 },
	{ 1,0,1,0,0,0,0,1,4,1,0,0,0,1,0,1 },
	{ 1,0,1,1,1,1,0,1,0,1,1,0,1,1,0,1 },
	{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1 },
	{ 1,3,1,1,0,3,0,0,0,0,1,0,0,1,0,1 },
	{ 1,0,0,1,0,1,1,1,1,0,1,1,0,1,1,1 },
	{ 1,1,0,1,0,1,0,0,0,1,1,0,0,1,0,1 },
	{ 1,1,0,0,0,1,0,1,0,0,0,0,0,3,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};

string getObject(int x, int y)
{
	string render;

	int cell = map[x][y];

	if (cell == WALL)
		render = "[ ]";
	else if (cell == SPACE)
		render = "   ";
	else if (cell == PLAYER)
		render = " X ";
	else if (cell == RUNE)
		render = " $ ";
	else if (cell == MINE)
		render = " * ";	
		

	return render;
}

void renderMap()
{
	SetConsoleCursorPosition(ñonsole, position);

	cout << "Hello Dota 3 beta" << endl;
	cout << "Health: " << p.health << "=====" << endl;
	cout << "score: " << p.mana << endl;

	for (int i = 0; i < H; i++)
	{
		for (int c = 0; c < W; c++)
		{
			cout << getObject(i, c);
		}
		cout << endl;
	}
}

void createObject(int object)
{
	int xR;
	int yR;

	do
	{
		xR = rand() % W;
		yR = rand() % H;
	} while (map[yR][xR] != SPACE);

	map[yR][xR] = object;
}

void checkKey()
{
	if (_kbhit())
	{
		int key = _getch();
		switch (key)
		{
		case 72: p.movePlayer(0, -1); break;
		case 80: p.movePlayer(0, 1); break;
		case 75: p.movePlayer(-1, 0); break;
		case 77: p.movePlayer(1, 0); break;
		case 27: isStart = false; break;
		}
	}
}

void renderPlayer(player pr)
{
	map[pr.cord.y][pr.cord.x] = PLAYER;
}

void checkGameWin()
{
	if (p.mana >= 1500)
	{
		system("cls");
		cout << "You WIN!!" << endl;
		isStart = false;
	}
}

void checkGameOver()
{
	if (p.health <= 0)
	{
		system("cls");
		cout << "Game Over!" << endl;
		isStart = false;
	}
}

int main()
{
	p.name = "pudge";
	p.health = 100;
	p.mana = 0;
	p.cord.x = 1;
	p.cord.y = 1;

	while (isStart)       
	{
		renderMap(); 
		renderPlayer(p);
		checkKey();

		checkGameWin();
		checkGameOver();

		Sleep(FPS);
	}

	system("pause");

	return 1;
}