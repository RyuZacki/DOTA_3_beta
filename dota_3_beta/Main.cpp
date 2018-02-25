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

const int H = 16;
const int W = 16;

bool isStart = true;

string name = "pudge";
double mana = 0;
int x = 1;
int y = 1;

int map[H][W] = 
{ 
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,0,1,0,0,0,0,1,0,1,0,0,0,0,0,1 },
	{ 1,0,1,0,1,1,1,1,0,1,0,1,1,1,3,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,1 },
	{ 1,0,0,0,0,1,0,0,0,0,1,0,1,1,0,1 },
	{ 1,0,1,0,0,1,0,1,0,3,1,0,0,0,0,1 },
	{ 1,0,1,0,1,1,1,1,0,1,1,0,1,1,1,1 },
	{ 1,0,1,0,0,0,0,1,0,1,0,0,0,0,0,1 },
	{ 1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,3,1,1,1,3,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1 },
	{ 1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1 },
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

	return render;
}

void renderMap()
{
	SetConsoleCursorPosition(ñonsole, position);

	cout << "Hello Dota 3 beta" << endl;
	cout << "score: " << mana << endl;

	for (int i = 0; i < H; i++)
	{
		for (int c = 0; c < W; c++)
		{
			cout << getObject(i, c);
		}
		cout << endl;
	}
}

void createRune()
{
	int xR;
	int yR;

	do
	{
		xR = rand() % W;
		yR = rand() % H;
	} while (map[yR][xR] != SPACE);

	map[yR][xR] = RUNE;
}

void checkCollision(int newX, int newY)
{
	switch (map[newY][newX])
	{
	case WALL: newX = x; newY = y; break;
	case RUNE: mana += 10; createRune(); break;
	}

	x = newX;
	y = newY;
}

void movePlayer(int dx, int dy)
{
	int newX = x + dx;
	int newY = y + dy;

	map[y][x] = SPACE;

	checkCollision(newX, newY);
}

void checkKey()
{
	if (_kbhit())
	{
		int key = _getch();
		switch (key)
		{
		case 72: movePlayer(0, -1); break;
		case 80: movePlayer(0, 1); break;
		case 75: movePlayer(-1, 0); break;
		case 77: movePlayer(1, 0); break;
		case 27: isStart = false; break;
		}
	}
}

void renderPlayer()
{
	map[y][x] = PLAYER;
}

int main()
{
	while (isStart)       
	{
		renderMap(); 
		renderPlayer();
		checkKey();

		Sleep(FPS);
	}

	return 1;
}