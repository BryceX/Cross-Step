#include "AIE.h"
#include <iostream>
int screenWidth = 900;
int screenHeight = 900;
float GetDeltaTime();

struct Background
{
	float backgroundYSpeed;
	unsigned int stageSprite;
};
struct Player
{
	float playerX;
	float playerY;
	float playerWidth = screenWidth*.05;
	float playerHeight = screenHeight*.05;
	unsigned int playerSprite;
};
struct Enemy
{
	float enemyX;
	float enemyY;
	float enemyWidth = screenWidth*.1;
	float enemyHeight = screenHeight*.1;
	unsigned int enemySprite;
};




int main( int argc, char* argv[] )
{	
    Initialise(900, 900, false, "Cross Step");
    SetBackgroundColour(SColour(0, 0, 0, 255));
	
	Background stage1;
	stage1.stageSprite = CreateSprite("./images/stage1background.png", screenWidth, screenHeight * 3, true);
	stage1.backgroundYSpeed = screenHeight*1.5;
	
	Player lyn;
	lyn.playerSprite = CreateSprite("./images/lyn.png", lyn.playerWidth, lyn.playerHeight, true);
	lyn.playerX = screenWidth*.5;
	lyn.playerY = screenHeight*.05;

	Enemy bandit1;
	bandit1.enemySprite = CreateSprite("./images/pongball.png", bandit1.enemyWidth, bandit1.enemyHeight, true);
	bandit1.enemyX = screenWidth*.5;
	bandit1.enemyY = screenHeight*1.25;


 

    //Game Loop
    do
    {
        
		MoveSprite(stage1.stageSprite, screenWidth*.5, stage1.backgroundYSpeed);
		DrawSprite(stage1.stageSprite);
		MoveSprite(lyn.playerSprite, lyn.playerX, lyn.playerY);
        DrawSprite(lyn.playerSprite);
		stage1.backgroundYSpeed -= screenHeight*.000015;

		
		MoveSprite(bandit1.enemySprite, bandit1.enemyX, bandit1.enemyY);
		DrawSprite(bandit1.enemySprite);
		bandit1.enemyY -= 00015*GetDeltaTime();


		if (IsKeyDown ('W'))
		{
			lyn.playerY += screenHeight*.5*GetDeltaTime();
			if (lyn.playerY>screenHeight*.975)
			{
				lyn.playerY = screenHeight*.975;
			}

		}
		if (IsKeyDown ('S'))
		{
			lyn.playerY -= screenHeight*.5*GetDeltaTime();
			if (lyn.playerY < screenHeight*.025)
			{
				lyn.playerY = screenHeight*.025;
			}
		}
		if (IsKeyDown('A'))
		{
			lyn.playerX -= screenWidth*.5*GetDeltaTime();
			if (lyn.playerX < screenWidth*.025)
			{
				lyn.playerX = screenWidth*.025;
			}
		}
		if (IsKeyDown('D'))
		{
			lyn.playerX += screenWidth*.5*GetDeltaTime();
			if (lyn.playerX > screenWidth*.975)
			{
				lyn.playerX = screenWidth*.975;
			}
		}
		

        ClearScreen();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
