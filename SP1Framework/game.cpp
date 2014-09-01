// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

double elapsedTime;
double deltaTime;
bool keyPressed[K_COUNT];
COORD consoleSize;

vector<fishInfo> fishvector;
double canspawn=50.000;
double canmove=0.000;
double canmove2=0.000;
double canmove3=0.000;
double typetime=0.00;
double raremultiplier=0.00;
int score=0;
PlayerInfo player;
Health health;
const WORD colors[] = {0x00,0x07,0x06,0x04};


void init()
{
	// Set precision for floating point output
	std::cout << std::fixed << std::setprecision(3);

	SetConsoleTitle(L"SP1 Framework");

	// Sets the console size, this is the biggest so far.
	setConsoleSize(79, 28);

	// Get console width and height
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */     

	/* get the number of character cells in the current buffer */ 
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
	consoleSize.X = csbi.srWindow.Right + 1;
	consoleSize.Y = csbi.srWindow.Bottom + 1;

	// set character location
	player.charLocation.X = 34;
	player.charLocation.Y = 4;
	player.hp = 3;

	// set health printing location
	health.charLocation.X=45;
	health.charLocation.Y=0;
	elapsedTime = 0.0;
}

void shutdown()
{
	// Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void getInput()
{    
	keyPressed[K_UP] = isKeyPressed(VK_UP);
	keyPressed[K_DOWN] = isKeyPressed(VK_DOWN);
	keyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
	keyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
	keyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
}

void update(double dt)
{
	// get the delta time
	elapsedTime += dt;
	deltaTime = dt;

	//function run control
	canspawn += dt;
	canmove += dt;
	canmove2 +=dt;
	canmove3 +=dt;
	typetime +=dt;
	raremultiplier +=dt;

	// Updating the location of the character based on the key press
	if (keyPressed[K_LEFT] && player.charLocation.X > 17)
	{
		erasechar();
		Beep(1440, 30);
		player.charLocation.X-=17; 
	}

	if (keyPressed[K_RIGHT] && player.charLocation.X < 51)
	{
		erasechar();
		Beep(1440, 30);
		player.charLocation.X += 17; 
	}
	// quits the game if player hits the escape key
	if (keyPressed[K_ESCAPE])
		g_quitGame = true;    
}

void render()
{	
	rendercharacter();

	int prevhp=player.hp;
	const string piece="><>";
	const int maxfish=6;
	const double spawntimer=50.00;
	const double raretimer=70.00;
	bool position1=false;
	bool position2=false;
	bool position3=false;

	//create fish
	if(canspawn>=spawntimer && ::fishvector.size()<maxfish)
	{
		srand(time(NULL));
		canspawn=0.0;
		fishInfo fish;
		::fishvector.push_back(fish);
		int i=(::fishvector.size())-1;
		int place = 0;

		//rare fish spawn
		if(typetime >=raretimer)
		{
			typetime=0.0;
			int type = 0;
			if(raremultiplier>=50.00)
		{
			type=rand()%4+1;
		}

		else
		{
			type=rand()%3+1;
		}

		switch(type)
		{
		case 1: ::fishvector[r].type=1;
			break;
		case 2: ::fishvector[r].type=2;
			break;
		case 3: ::fishvector[r].type=3;
			break;
		case 4: ::fishvector[r].type=4;
			break;
		}
		}

		else
		{
			::fishvector[i].type=1;
		}

		//overlap spawn prevention
		for(int t=0;t<::fishvector.size();t++)
		{
			if(::fishvector[t].x==17 && ::fishvector[t].y==31)
			{
				position1=true;
			}

			else if(::fishvector[t].x==37 && ::fishvector[t].y==26)
			{
				position2=true;
			}

			else if(::fishvector[t].x==37 && ::fishvector[t].y==31)
			{
				position3=true;
			}
		}

		if(position1==true)
		{
			if(position2==true)
			{
				place=3;
			}

			else if(position3==true)
			{
				place=2;
			}
			else
			{
				place=rand()%3+2;
			}
		}

		else if(position2==true)
		{
			if(position1==true)
			{
				place=3;
			}

			else if(position3==true)
			{
				place=1;
			}
			else
			{
				place=rand()%3+2;
				if(place==2)
				{
					place=3;
				}
			}
		}

		else if(position3==true)
		{
			if(position1==true)
			{
				place=2;
			}

			else if(position2==true)
			{
				place=1;
			}
			else
			{
				place=rand()%2+1;
			}
		}

		else
		{
			place=rand()%3+1;
		}

		switch(place)
		{
		case 1: ::fishvector[i].position = 1;
			::fishvector[i].x = 17;
			::fishvector[i].y = 31;
			break;
		case 2: ::fishvector[i].position = 2;
			::fishvector[i].x = 37;
			::fishvector[i].y = 26;
			break;
		case 3: ::fishvector[i].position = 3;
			::fishvector[i].x = 37;
			::fishvector[i].y = 31;
			break;
		}
		gotoXY(::fishvector[i].x,::fishvector[i].y);
		colour(colors[::fishvector[i].type]);
		std::cout << piece;
		colour(colors[1]);
	}

	//delete the fish
	for(int p=0;p<=((::fishvector.size())-1);p++)
	{
		const string del="   ";

		//delete current fish location
		gotoXY(::fishvector[p].x,::fishvector[p].y);
		std::cout << del;
	}

	movefish();

	//print the fish
	for(int e=0;e<::fishvector.size();e++)
	{
		gotoXY(::fishvector[e].x,::fishvector[e].y);
		colour(colors[::fishvector[e].type]);
		std::cout << piece;
		colour(colors[1]);
	}

	//check location of the character for score
	for(int c=0;c<::fishvector.size();c++)
	{
		if(::fishvector[c].y==6)
		{
			if(player.charLocation.X==17 && ::fishvector[c].x==17 || player.charLocation.X==34 && ::fishvector[c].x==37 || player.charLocation.X==51 && ::fishvector[c].x==60)
			{
				switch(::fishvector[c].type)//score
				{
				case 1: score = 1 * multiplier;
					break;
				case 2: score = 5 * multiplier;
					break;
				case 3: score = 1 * multiplier;; 
					if(player.hp<6)
					{
						player.hp++;
					}
					break;
				case 4: multiplier=multiplier*2;
						score = 1 * multiplier;
					break;
				}	
			}

			else
			{
				player.hp--;
			}
			replacefish(c);
		}
	}

	// render time taken to calculate this frame
	gotoXY(70, 0);
	std::cout << 1.0 / deltaTime << "fps" << std::endl;

	gotoXY(0, 0);
	std::cout << elapsedTime << "secs" << std::endl;

	//player health
	gotoXY(health.charLocation.X-8,0);
	std::cout << "Health: ";

	//deleting hearts
	for(int k=0;k<prevhp;k++)
	{
		gotoXY(health.charLocation.X, health.charLocation.Y);
		std::cout << " ";
		health.charLocation.X++;
	}
	health.charLocation.X=45;

	//printing hearts
	for(int j=0; j<player.hp; j++)
	{
		gotoXY (health.charLocation.X, health.charLocation.Y);
		std::cout << (char)3;
		health.charLocation.X++;
	}
	health.charLocation.X=45;

	//score
	gotoXY(20,0);
	std::cout << "Score: " << score;

	gotoXY(5,5);
	std::cout << ::fishvector.size();
}

void replacefish(int r)
{
	srand(time(NULL));
	const string del="   ";
	const string piece="><>";
	int replace =0;

	bool position1=false;
	bool position2=false;
	bool position3=false;

	//delete the fish at the top
	gotoXY(::fishvector[r].x,::fishvector[r].y);
	std::cout << del;

	//overlap respawn prevention
	for(int t=0;t<::fishvector.size();t++)
	{
		if(::fishvector[t].x==17 && ::fishvector[t].y==31)
		{
			position1=true;
		}

		else if(::fishvector[t].x==37 && ::fishvector[t].y==26)
		{
			position2=true;
		}

		else if(::fishvector[t].x==37 && ::fishvector[t].y==31)
		{
			position3=true;
		}
	}

	if(position1==true)
	{
		if(position2==true)
		{
			replace=3;
		}

		else if(position3==true)
		{
			replace=2;
		}
		else
		{
			replace=rand()%3+2;
		}
	}

	else if(position2==true)
	{
		if(position1==true)
		{
			replace=3;
		}

		else if(position3==true)
		{
			replace=1;
		}
		else
		{
			replace=rand()%3+2;
			if(replace==2)
			{
				replace=3;
			}
		}
	}

	else if(position3==true)
	{
		if(position1==true)
		{
			replace=2;
		}

		else if(position2==true)
		{
			replace=1;
		}
		else
		{
			replace=rand()%2+1;
		}
	}

	else
	{
		replace=rand()%3+1;
	}

	switch(replace)		
	{
	case 1: ::fishvector[r].position = 1;
		::fishvector[r].x = 17;
		::fishvector[r].y = 31;
		break;
	case 2: ::fishvector[r].position = 2;
		::fishvector[r].x = 37;
		::fishvector[r].y = 26;	
		break;
	case 3: ::fishvector[r].position = 3;
		::fishvector[r].x = 37;
		::fishvector[r].y = 31;
		break;
	}		


	//rare respawn
	if(typetime >=13.00)
	{
		typetime=0.0;
		int type = 0;

		if(raremultiplier>=75.00)
		{
			type=rand()%4+1;
			raremultiplier=0.00;
		}

		else
		{
			type=rand()%3+1;
		}

		switch(type)
		{
		case 1: ::fishvector[r].type=1;
			break;
		case 2: ::fishvector[r].type=2;
			break;
		case 3: ::fishvector[r].type=3;
			break;
		case 4: ::fishvector[r].type=4;
			break;
		}
	}

	else
	{
		::fishvector[r].type=1;
	}

	//print at reset position
	gotoXY(::fishvector[r].x,::fishvector[r].y);
	colour(colors[::fishvector[r].type]);
	std::cout << piece;
	colour(colors[1]);
}
void rendercharacter()
{
	gotoXY(player.charLocation);

	if (player.charLocation.X == 17)
	{
		std::ifstream left;
		std::string asciileft;

		left.open ("ascii/left.txt");
		while (!left.eof())
		{
			gotoXY (player.charLocation);
			std::getline (left, asciileft);
			std::cout << asciileft;
			player.charLocation.Y += 1;
			if (player.charLocation.Y == 10)
			{
				player.charLocation.Y = 4;
			}
		}

	}

	else if (player.charLocation.X == 51)
	{
		std::ifstream right;
		std::string asciiright;

		right.open ("ascii/right.txt");
		while (!right.eof())
		{
			gotoXY (player.charLocation);
			std::getline (right, asciiright);
			std::cout << asciiright;
			player.charLocation.Y += 1;
			if (player.charLocation.Y == 10)
			{
				player.charLocation.Y = 4;
			}
		}
	}

	else
	{
		std::ifstream middle;
		std::string asciimid;

		middle.open ("ascii/middle.txt");
		while (!middle.eof())
		{ 
			gotoXY (player.charLocation);
			std::getline (middle, asciimid);
			std::cout << asciimid;
			player.charLocation.Y += 1;
			if (player.charLocation.Y == 10)
			{
				player.charLocation.Y = 4;
			}
		}
	}
}
void erasechar()
{
	gotoXY(player.charLocation);

	if(player.charLocation.X==34)
	{
		std::ifstream emptymiddle;
		std::string asciiemptymiddle;

		emptymiddle.open ("ascii/emptymiddle.txt");
		while (!emptymiddle.eof())
		{
			gotoXY(player.charLocation);
			std::getline (emptymiddle, asciiemptymiddle);
			std::cout << asciiemptymiddle;
			player.charLocation.Y += 1;
			if (player.charLocation.Y == 10)
			{
				player.charLocation.Y = 4;
			}
		}
	}

	else
	{
		std::ifstream emptyside;
		std::string asciiemptyside;

		emptyside.open ("ascii/emptyside.txt");
		while (!emptyside.eof())
		{
			gotoXY(player.charLocation);
			std::getline (emptyside, asciiemptyside);
			std::cout << asciiemptyside;
			player.charLocation.Y += 1;
			if (player.charLocation.Y == 10)
			{
				player.charLocation.Y = 4;
			}
		}
	}
}
void movefish()
{
	if(canmove>1.50)
	{
		canmove=0.00;
		for(int o=0;o<::fishvector.size();o++)
		{
			if(::fishvector[o].position==1)
			{
				::fishvector[o].y-=5;
			}
		}

	}
	if(canmove2>1.25)
	{
		canmove2=0.0;
		for(int y=0;y<fishvector.size();y++)
		{
			if(::fishvector[y].position==2)
			{
				::fishvector[y].y-=5;
			}
		}
	}
	if(canmove3>1.75)
	{
		canmove3=0.0;
		for(int z=0;z<fishvector.size();z++)
		{
			if(::fishvector[z].x==37 && ::fishvector[z].y==31)
			{
				::fishvector[z].x=60;
			}
			else if(::fishvector[z].position==3)
			{
				::fishvector[z].y-=5;
			}
		}
	}
}