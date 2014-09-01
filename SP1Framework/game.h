#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include <vector>
#include <string>
#include <ctime>

using std::vector;
extern StopWatch g_timer;
extern bool g_quitGame;
using std::string;

enum Keys
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_COUNT
};

struct fishInfo
{
	int position;
	int x;
	int y;
	int type;
};

struct PlayerInfo
{
	COORD charLocation;
	int hp;
};

struct Health
{
	COORD charLocation;
};

void init();                // initialize your variables, allocate memory, etc
void getInput();            // get input from player
void update(double dt);     // update the game and the state of the game
void render();				// renders the current state of the game to the console
void shutdown();            // do clean up, free memory
void replacefish(int);
void rendercharacter();
void erasechar();
void movefish();

#endif // _GAME_H