// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// MARJERY DILIG
// SUFIAN MOJADDEDI

#include <iostream>
#include "windows.h"
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <sstream>

using namespace std;

const int NUM_ALIENS = 5;

struct AClass {
	int x = 1;
	int y = 4;
	bool alive = true;
	int direction = 1;
	char looks = 'A';
};

struct PacObject {
	int x;
	int y;
	char looks = '^';
};

struct BulletObject {
	int x;
	int y;
	char looks = '!';
	bool flying = false;
};



// BOOl bullet(vector)

BOOL gotoxy(const WORD x, const WORD y) {
	//-------do not touch, do not use-------
	//-------it is already placed and called for you in main loop---------
	COORD xy;
	xy.X = x;
	xy.Y = y;
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}
//insert loadMap function from before
bool  loadMap(vector<string>& map) {
	bool opened = true;
	string fileName, s;
	ifstream mapFile;
	int foodCounter = 0;
	cout << "Please enter the name of the file: " << endl;
	getline(cin, fileName);
	mapFile.open(fileName, ios::in);
	if (!mapFile) {
		cout << "Open Failed." << endl;
		opened = false;
	}
	else {
		while (getline(mapFile, s))
		{
			map.push_back(s);
		}
	}
	mapFile.close();

	return opened;
}
//insert displayMap function from before
void displayMap(vector<string>& map) {
	for (string s : map) {
		cout << s << endl;
	}
}

void moveInvaders(vector<string>& map, vector<AClass>& invaders) {
	// get current x, y, and direction from appropriate alien

	int xpos = 0;
	int ypos = invaders.front().y;
	int dir = invaders.front().direction;
	if (dir > 0) {
		xpos = invaders.back().x;
	}
	else {
		xpos = invaders.front().x;
	}

	// keep moving + to the right
	// if #:  flip direction sign
	if (map.at(ypos).at(xpos + dir) == '#') {
		dir *= -1;
	}

	map.at(ypos).at(invaders.back().x) = ' ';
	map.at(ypos).at(invaders.front().x) = ' ';

	for (int i = 0; i < invaders.size(); i++) {
		// update x location and direction for each alien
		invaders.at(i).x += dir;
		invaders.at(i).direction = dir;
		// put that alien on the map
		map.at(ypos).at(invaders.at(i).x) = (invaders.at(i).alive) ? invaders.at(i).looks : ' ';
	}
}

void movePlayer(vector<string>& map, PacObject& Player, int location) {
	//check new location if not '#' move
	if (Player.x + location != '#') {
		map.at(Player.y).at(Player.x) = ' ';
		Player.x += location;
		map.at(Player.y).at(Player.x) = Player.looks;
	}

}


void moveBullet(vector<string>& map, BulletObject& bullet, vector<AClass>& aliens) {
	//only if bullet is slying

	if (bullet.flying) {
		map.at(bullet.y).at(bullet.x) = ' ';
		switch (map.at(bullet.y - 1).at(bullet.x)) {
		case ' ':
			bullet.y--;
			map.at(bullet.y).at(bullet.x) = bullet.looks;
			break;
		case 'A':
			//bullet dies (stops moving and disappears)
			for (AClass& a : aliens) {
				if (a.x == bullet.x) {
					a.alive = false;
				}
			}

		case 'x':
			//bullet dies

		case '#':
			//bullet dies

			bullet.flying = false;

		}
	}
}




void shoot(vector<string>& map, BulletObject& bullet, PacObject& player) {
	//if bullet is not flying
	if (!bullet.flying) {
		bullet.x = player.x;
		bullet.y = player.y - 1;
		bullet.flying = true;
	}
	// set bullet x and y to  player location (one line above the player)
	// set flying to true
}

int main()
{

	bool game_running = true;
	int time = 100;

	vector<AClass> aliens(NUM_ALIENS);
	PacObject Player;
	BulletObject Bullet;

	for (int i = 0; i < aliens.size(); i++) {
		aliens.at(i).x += i;
	}
	vector<string> map;
	bool loadOK = loadMap(map);
	if (loadOK) {

		// init Player's y to be 2 lines before the end of map vector
		Player.y = map.size() - 2;
		// init player's x to be middle of the map line
		Player.x = map.back().size() / 2;



		system("cls");
		do {
			gotoxy(0, 0);
			//cout << Player.y << "    " << Player.x << endl;
			moveBullet(map, Bullet, aliens);
			moveInvaders(map, aliens);
			displayMap(map);

			//system("pause>nul");
			this_thread::sleep_for(chrono::milliseconds(time)); //include needed headers
			if (GetAsyncKeyState(VK_DOWN)) {
				//slows down game
				time += 15;
			}
			if (GetAsyncKeyState(VK_UP)) {
				//speeds up game
				time -= 15;
			}
			if (GetAsyncKeyState(VK_RIGHT)) {
				//move right
				movePlayer(map, Player, 1);
			}
			if (GetAsyncKeyState(VK_LEFT)) {
				//move left
				movePlayer(map, Player, -1);
			}
			if (GetAsyncKeyState(VK_SPACE)) {
				shoot(map, Bullet, Player);
				//shoots bullet
			}
			if (GetAsyncKeyState(VK_ESCAPE)) {
				game_running = false;
			}
		} while (game_running || !aliens.empty());
	}
	system("cls");
	cout << "GAME OVER";

	return 0;

	//when aliens are empty (at 0) add a You Win section
}