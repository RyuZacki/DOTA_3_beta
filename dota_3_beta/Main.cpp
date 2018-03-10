#include <SFML/Graphics.hpp>
#include "iostream"
#include "string" 
#include "conio.h"
#include "time.h"
#include "windows.h"

using namespace sf;
using namespace std;

COORD position = { 0, 0 }; //позиция x и y
HANDLE сonsole = GetStdHandle(STD_OUTPUT_HANDLE);//консоль

const int PX = 32;

Texture player_t;
Sprite player_s;

Texture mine_t;
Sprite mine_s;

Texture rune_t;
Sprite rune_s;

Texture wall_t;
Sprite wall_s;

const int FPS = 1000 / 60;

const int SPACE = 0;
const int WALL = 1;
const int PLAYER = 2;
const int RUNE = 3;
const int MINE = 4;

const int H = 16;
const int W = 16;

bool isStart = true;

string name = "pudge";
double mana = 0;
int x = 1;
int y = 1;

int mappp[H][W] = 
{ 
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,0,1,0,0,0,0,1,0,1,0,0,4,0,0,1 },
	{ 1,0,1,0,1,1,1,1,0,1,0,1,1,1,3,1 },
	{ 1,0,0,0,0,0,0,4,0,0,0,0,0,1,0,1 },
	{ 1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,1 },
	{ 1,0,0,0,0,1,0,0,0,0,1,0,1,1,0,1 },
	{ 1,0,1,0,0,1,0,1,0,3,1,0,0,0,0,1 },
	{ 1,0,1,0,1,1,1,1,0,1,1,0,1,1,1,1 },
	{ 1,0,1,0,0,0,4,1,0,1,0,0,0,0,0,1 },
	{ 1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,3,1,1,1,3,4,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1 },
	{ 1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,4,0,0,0,0,0,3,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};

string getObject(int x, int y)
{
	string render;

	int cell = mappp[x][y];

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

void renderMapConsole()
{
	SetConsoleCursorPosition(сonsole, position);

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

void createObj(int n)
{
	int xR;
	int yR;

	do
	{
		xR = rand() % W;
		yR = rand() % H;
	} while (mappp[yR][xR] != SPACE);

	mappp[yR][xR] = n;
}

void checkCollision(int newX, int newY)
{
	switch (mappp[newY][newX])
	{
	case WALL: newX = x; newY = y; break;
	case RUNE: mana += 10; createObj(RUNE); break;
	case MINE: mana -= 10; createObj(MINE); break;
	}

	x = newX;
	y = newY;
}

void movePlayer(int dx, int dy)
{
	int newX = x + dx;
	int newY = y + dy;

	mappp[y][x] = SPACE;

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
	mappp[y][x] = PLAYER;
}

void runConsole()
{
	while (isStart)
	{
		renderMapConsole();
		renderPlayer();
		checkKey();
		
		Sleep(FPS);
	}
}

void loadTextures()
{
	player_t.loadFromFile("res/player.png");
	player_s.setTexture(player_t);

	rune_t.loadFromFile("res/rune.png");
	rune_s.setTexture(rune_t);

	wall_t.loadFromFile("res/wall.png");
	wall_s.setTexture(wall_t);

	mine_t.loadFromFile("res/mine.png");
	mine_s.setTexture(mine_t);
}

void checkControl(Event &event)
{
	if (event.key.code == Keyboard::W)
		movePlayer(0, -1);
	else if (event.key.code == Keyboard::A)
		movePlayer(-1, 0);
	else if (event.key.code == Keyboard::S)
		movePlayer(0, 1);
	else if (event.key.code == Keyboard::D)
		movePlayer(1, 0);
}

void renderMap(RenderWindow &window)
{
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			int object = mappp[i][j];
			Sprite tmp;

			if (object == WALL)
				tmp = wall_s;
			else if (object == PLAYER)
				tmp = player_s;
			else if (object == RUNE)
				tmp = rune_s;
			else if (object == MINE)
				tmp = mine_s;

			tmp.setPosition(j * PX, i * PX + 32);
			window.draw(tmp);
		}
	}
}

void runGraphics()
{
	RenderWindow window(VideoMode(512, 544), "DOTA_3");

	Font font;//шрифт 
	font.loadFromFile("res/CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
	
	Text text("", font, 32);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setFillColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setPosition(0, 0);//ставим позицию

	loadTextures();

	while (window.isOpen())
	{
		renderPlayer();

		string manaText = to_string(mana).substr(0, 4);
		text.setString("MANA = " + manaText);
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				checkControl(event);
		}

		window.clear();

		renderMap(window);

		window.draw(text);
		window.display();
	}
}

int main()
{
	//runConsole();
	runGraphics();

	system("pause");
	return 1;
}