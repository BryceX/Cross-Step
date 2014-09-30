#include "AIE.h"
#include <iostream>
int screenWidth = 900;
int screenHeight = 900;
float GetDeltaTime();
float playerHealth;
char playerHealthString[10];

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
	float playerHealth;
	char playerHealthString[10];
	float playerUpperBoundary;
	float playerLowerBoundary;
	float playerLeftBoundary;
	float playerRightBoundary;
};
struct Enemy
{
	float enemyX;
	float enemyY;
	float enemyWidth = screenWidth*.1;
	float enemyHeight = screenHeight*.1;
	unsigned int enemySprite;
	float enemyUpperBoundary;
	float enemyLowerBoundary;
	float enemyLeftBoundary;
	float enemyRightBoundary;
};

struct Bullet
{
	float bulletX;
	float bulletY;
	float bulletWidth = screenWidth*.001;
	float bulletHeight = screenHeight*.001;
	unsigned int bulletSprite;
};



int main( int argc, char* argv[] )
{	
    Initialise(900, 900, false, "Cross Step");
    SetBackgroundColour(SColour(0, 0, 0, 255));
	

	Background stage1;
	stage1.stageSprite = CreateSprite("./images/stage1background.png", screenWidth*.75, screenHeight * 3, true);
	stage1.backgroundYSpeed = screenHeight*1.5;
	
	Player lyn;
	lyn.playerSprite = CreateSprite("./images/lyn.png", lyn.playerWidth, lyn.playerHeight, true);
	lyn.playerX = screenWidth*.5;
	lyn.playerY = screenHeight*.05;
	lyn.playerHealth = 3;
	lyn.playerUpperBoundary = lyn.playerY + lyn.playerHeight*.5;
	lyn.playerLowerBoundary = lyn.playerY - lyn.playerHeight*.5;
	lyn.playerRightBoundary = lyn.playerX + lyn.playerWidth*.25;
	lyn.playerLeftBoundary = lyn.playerX - lyn.playerWidth*.25;

	Enemy bandit1;
	bandit1.enemySprite = CreateSprite("./images/pongball.png", bandit1.enemyWidth, bandit1.enemyHeight, true);
	bandit1.enemyX = screenWidth*.5;
	bandit1.enemyY = screenHeight*1.25;
	bandit1.enemyUpperBoundary = bandit1.enemyY + bandit1.enemyHeight*.5;
	bandit1.enemyLowerBoundary = bandit1.enemyY - bandit1.enemyHeight*.5;
	bandit1.enemyRightBoundary = bandit1.enemyX + bandit1.enemyWidth*.5;
	bandit1.enemyLeftBoundary = bandit1.enemyX - bandit1.enemyWidth*.5;

	Bullet playerDagger;
	playerDagger.bulletSprite = CreateSprite("./images/pongball.png", playerDagger.bulletWidth, playerDagger.bulletHeight, true);
	playerDagger.bulletX = lyn.playerX;
	playerDagger.bulletY = lyn.playerY + (playerDagger.bulletHeight*.5) + playerDagger.bulletHeight;


 

    
    do
    {
		itoa(playerHealth, playerHealthString, 10);
		MoveSprite(stage1.stageSprite, screenWidth*.5, stage1.backgroundYSpeed);
		DrawSprite(stage1.stageSprite);
		MoveSprite(lyn.playerSprite, lyn.playerX, lyn.playerY);
        DrawSprite(lyn.playerSprite);
		DrawString(playerHealthString, screenWidth*.9, screenHeight*.5);
		stage1.backgroundYSpeed -= screenHeight*.01*GetDeltaTime();

		
		MoveSprite(bandit1.enemySprite, bandit1.enemyX, bandit1.enemyY);
		DrawSprite(bandit1.enemySprite);
		bandit1.enemyY -= screenHeight*.2*GetDeltaTime();


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
		if (IsKeyDown('F'))
		{
			MoveSprite(playerDagger.bulletSprite, playerDagger.bulletX, playerDagger.bulletY);
			DrawSprite(playerDagger.bulletSprite);
		}

		if (lyn.playerUpperBoundary > bandit1.enemyLowerBoundary)
		{
			if (lyn.playerLeftBoundary < bandit1.enemyRightBoundary)
			{
				if (lyn.playerRightBoundary > bandit1.enemyLeftBoundary)
					lyn.playerHealth -= 1;
			}
		}
		if (lyn.playerHealth == 1)
		{
			std::cout << "GAME OVER";
		}

        ClearScreen();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
