#include "iostream"
#include "string" 
#include "conio.h"
#include "time.h"
#include "windows.h"
#include "locale.h"
#include "fstream"

using namespace std;

COORD position = { 0, 0 }; //позиция x и y
HANDLE сonsole = GetStdHandle(STD_OUTPUT_HANDLE);//консоль

const int FPS = 1000 / 60;

const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;
const int KEY_ENTER = 13;

const int SPACE = 0;
const int WALL = 1;
const int PLAYER = 2;
const int RUNE = 3;
const int MINE = 4;

const int H = 16;
const int W = 16;

const int COUNT_PARAMS = 4;
string params[COUNT_PARAMS];
int cursor = 0;

int level = 1;
string lvlName;

bool isStart = true;
bool isMenu = true;

int map[H][W];

void startGame();
void createObject(int object);
void exitGame();
void startGame();

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
	string body;
	
	player()
	{
		this->cord.x = 1;
		this->cord.y = 1;
		this->mana = 0;
	}

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

const int COUNT_PLAYERS = 3;
int numPlayer = 0;

player players[COUNT_PLAYERS];
player p;

void renderPlayer(player pr)
{
	map[pr.cord.y][pr.cord.x] = PLAYER;
}

void initPlayer()
{
	player p1;

	p1.name = "pudge";
	p1.health = 200;;
	p1.body = " @ ";

	player p2;

	p2.name = "invoker";
	p2.health = 100;
	p2.mana = 100;
	p2.body = " I ";

	player p3;

	p3.name = "Slark";
	p3.health = 150;
	p3.body = " ! ";

	players[0] = p1;
	players[1] = p2;
	players[2] = p3;

	p = players[0];
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

string getObject(int x, int y)
{
	string render;

	int cell = map[x][y];

	if (cell == WALL)
		render = "[ ]";
	else if (cell == SPACE)
		render = "   ";
	else if (cell == PLAYER)
		render = p.body;
	else if (cell == RUNE)
		render = " $ ";
	else if (cell == MINE)
		render = " * ";			

	return render;
}

void renderMap()
{
	SetConsoleCursorPosition(сonsole, position);

	cout << "Hello Dota 3 beta" << endl;
	cout << "Health: " << p.health << "     " << endl;
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

void loadMap()
{
	ifstream in("levels/lvl" + to_string(level) + ".txt");

	int j = 0;
	string str;

	while (getline(in, str))
	{
		for (int i = 0; i < W; i++)
		{
			char c = str[i];
			map[j][i] = atoi(&c);
		}
		j++;
	}
}

void checkKeyPlayer()
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

void initMenu()
{
	params[0] = "Начать игру";
	params[1] = "Персонаж = " + players[numPlayer].name;
	params[2] = "Карта = " + to_string(level);
	params[3] = "Выйти";
}

void renderMenu()
{
	system("cls");
	cout << "   === МЕНЮ ===  " << endl;

	params[cursor] = " -> " + params[cursor];

	for (int i = 0; i < COUNT_PARAMS; i++)
		cout << params[i] << endl;
}

void processKeyMenu(int k)
{
	if (k == KEY_ENTER)
	{
		if (cursor == 0)
			startGame();
		else if (cursor == 3)
			exitGame();
	}
	else if (k == KEY_LEFT)
	{
		if (cursor == 1)
		{
			numPlayer--;
			if (numPlayer < 0)
				numPlayer++;
		}
		if (cursor == 2)
			level--;
	}
	else if (k == KEY_RIGHT)
	{
		if (cursor == 1)
		{
			numPlayer++;
			if (numPlayer >= COUNT_PLAYERS)
				numPlayer--;
		}
		if (cursor == 2)
			level++;
	}
	else if (k == KEY_UP)
	{
		cursor--;
		if (cursor < 0)
			cursor++;
	}
	else if (k == KEY_DOWN)
	{
		cursor++;
		if (cursor >= COUNT_PARAMS)
			cursor--;
	}
}

void checkKeyMenu()
{
	if (_kbhit())
		processKeyMenu(_getch());
}

void exitGame()
{
	isStart = false;
	isMenu = false;
}

void checkGameWin()
{
	if (p.mana >= 1500)
	{
		system("cls");
		cout << "You WIN!!" << endl;
		exitGame();
	}
}

void checkGameOver()
{
	if (p.health <= 0)
	{
		system("cls");
		cout << "Game Over!" << endl;
		exitGame();
	}
}

void startGame()
{
	loadMap();

	p = players[numPlayer];

	while (isStart)
	{
		renderMap();
		renderPlayer(p);
		checkKeyPlayer();

		checkGameWin();
		checkGameOver();

		Sleep(FPS);
	}
}

void runMenu()
{
	while (isMenu)
	{
		initMenu();
		checkKeyMenu();
		renderMenu();

		Sleep(FPS);
	}
}

int main()
{
	setlocale(0, "RUS");
	
	initPlayer();
	runMenu();

	system("pause");

	return 1;
}