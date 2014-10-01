#include "AIE.h"
#include <iostream>

int screenWidth = 900;
int screenHeight = 900;
//float GetDeltaTime();
char playerHealthString[10];
int playerHealth = 3000;
float gameTime = 0;
char gameOverText[10] = "Game Over";
unsigned int leftCap;
unsigned int rightCap;
unsigned int topCap;
unsigned int bottomCap;
void MovementCap(float objWidth, float objHeight)
{
	topCap = screenHeight - objHeight*.5;
	bottomCap = 0 + objHeight*.5;
	leftCap = screenWidth*.125 + objWidth*.5;
	rightCap = screenWidth*.875 - objWidth*.5;
}
/*void ShootBullet(float objBullet)
{
if (IsKeyDown(32))
{
MoveSprite
DrawSprite

}
}*/


enum GAMESTATE
{
	MAINMENU,
	GAMEPLAY,
	GAMEOVER,
};

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
	unsigned int upKey;
	unsigned int downKey;
	unsigned int leftKey;
	unsigned int rightKey;
	float playerUpperBoundary;
	float playerLowerBoundary;
	float playerLeftBoundary;
	float playerRightBoundary;
	float timeWhenLastDamaged;
	float invulTime = 2;

	void Move()
	{

		if (IsKeyDown(upKey))
		{
			playerY += screenHeight*.5*GetDeltaTime();
			if (playerY > screenHeight - playerHeight*.5)
			{
				playerY = screenHeight - playerHeight*.5;
			}
		}
		if (IsKeyDown(downKey))
		{
			playerY -= screenHeight*.5*GetDeltaTime();
			if (playerY < 0 + playerHeight*.5)
			{
				playerY = 0 + playerHeight*.5;
			}
		}
		if (IsKeyDown(leftKey))
		{
			playerX -= screenWidth*.5*GetDeltaTime();
			if (playerX < screenWidth*.125 + playerWidth*.5)
			{
				playerX = screenWidth*.125 + playerWidth*.5;
			}
		}
		if (IsKeyDown(rightKey))
		{
			playerX += screenWidth*.5*GetDeltaTime();
			if (playerX > screenWidth*.875 - playerWidth*.5)
			{
				playerX = screenWidth*.875 - playerWidth*.5;
			}
		}
	}



	// Called when collision is detected
	void Collided()
	{
		// subtract health or shit
		playerHealth -= 1;
	}

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
	float bulletWidth = screenWidth*.01;
	float bulletHeight = screenHeight*.02;
	unsigned int bulletSprite;
	float bulletTestY;
};

int main(int argc, char* argv[])
{
	Initialise(900, 900, false, "Cross Step");
	SetBackgroundColour(SColour(0, 0, 0, 255));


	Background stage1;
	stage1.stageSprite = CreateSprite("./images/stage1background.png", screenWidth*.75, screenHeight * 4, true);
	stage1.backgroundYSpeed = screenHeight*1.5;

	Player lyn;
	lyn.playerSprite = CreateSprite("./images/lyn.png", lyn.playerWidth, lyn.playerHeight, true);
	lyn.playerX = screenWidth*.5;
	lyn.playerY = screenHeight*.05;
	lyn.upKey = 'W';
	lyn.downKey = 'S';
	lyn.leftKey = 'A';
	lyn.rightKey = 'D';
	//set all my keys

	Enemy bandit1;
	bandit1.enemySprite = CreateSprite("./images/enemy1.png", bandit1.enemyWidth, bandit1.enemyHeight, true);
	bandit1.enemyX = screenWidth*.5;
	bandit1.enemyY = screenHeight*1.25;


	Bullet playerDagger;
	playerDagger.bulletSprite = CreateSprite("./images/dagger.png", playerDagger.bulletWidth, playerDagger.bulletHeight, true);
	playerDagger.bulletX = lyn.playerX;
	playerDagger.bulletY = lyn.playerY + (playerDagger.bulletHeight*.5) + playerDagger.bulletHeight;
	playerDagger.bulletTestY = 0;
	GAMESTATE currentState = MAINMENU;

	do
	{
		itoa(playerHealth, playerHealthString, 10);
		switch (currentState)
		{
		case MAINMENU:

			if (IsKeyDown('T'))
				currentState = GAMEPLAY;

			break;


		case GAMEPLAY:

			stage1.backgroundYSpeed -= screenHeight*.01*GetDeltaTime();
			bandit1.enemyY -= screenHeight*.2*GetDeltaTime();
			gameTime += GetDeltaTime();
			lyn.Move();
			DrawString(playerHealthString, screenWidth*.9, screenHeight*.5);
			MoveSprite(stage1.stageSprite, screenWidth*.5, stage1.backgroundYSpeed);
			DrawSprite(stage1.stageSprite);
			MoveSprite(lyn.playerSprite, lyn.playerX, lyn.playerY);
			DrawSprite(lyn.playerSprite);
			MoveSprite(bandit1.enemySprite, bandit1.enemyX, bandit1.enemyY);
			DrawSprite(bandit1.enemySprite);
			lyn.playerUpperBoundary = lyn.playerY + lyn.playerHeight*.5;
			lyn.playerLowerBoundary = lyn.playerY - lyn.playerHeight*.5;
			lyn.playerRightBoundary = lyn.playerX + lyn.playerWidth*.5;
			lyn.playerLeftBoundary = lyn.playerX - lyn.playerWidth*.5;
			bandit1.enemyUpperBoundary = bandit1.enemyY + bandit1.enemyHeight*.5;
			bandit1.enemyLowerBoundary = bandit1.enemyY - bandit1.enemyHeight*.5;
			bandit1.enemyRightBoundary = bandit1.enemyX + bandit1.enemyWidth*.5;
			bandit1.enemyLeftBoundary = bandit1.enemyX - bandit1.enemyWidth*.5;
			if (IsKeyDown(32))
				bandit1.enemyY = screenHeight*1.25;

			if (bool(IsKeyDown('F') == true));
			{
				MoveSprite(playerDagger.bulletSprite, playerDagger.bulletX, playerDagger.bulletY);
				DrawSprite(playerDagger.bulletSprite);
				playerDagger.bulletTestY += .01*GetDeltaTime();
			}

			// if the top part of the ship goes inside the enemy
			if (lyn.playerUpperBoundary > bandit1.enemyLowerBoundary)
			{//left side collision for player ship top side
				if (lyn.playerUpperBoundary < bandit1.enemyUpperBoundary)
				{
					if (lyn.playerLeftBoundary < bandit1.enemyRightBoundary)
					{
						//playerHealth -= 1;
						if (lyn.playerLeftBoundary > bandit1.enemyLeftBoundary)
						{
							lyn.Collided();
						}
					}
				}
			}
			else if (lyn.playerUpperBoundary > bandit1.enemyLowerBoundary)
			{//right side collision
				if (lyn.playerUpperBoundary < bandit1.enemyUpperBoundary)
				{
					if (lyn.playerRightBoundary > bandit1.enemyLeftBoundary)
					{
						if (lyn.playerRightBoundary < bandit1.enemyRightBoundary)
						{
						lyn.Collided();
						}
					}
				}
			}
			else if (lyn.playerLowerBoundary < bandit1.enemyUpperBoundary)
			{//bottom side left side collision
				if (lyn.playerLowerBoundary > bandit1.enemyLowerBoundary)
				{
					if (lyn.playerLeftBoundary < bandit1.enemyRightBoundary)
					{
						if (lyn.playerLeftBoundary > bandit1.enemyLeftBoundary)
						{
							lyn.Collided();
						}
					}
				}
			}
			else if (lyn.playerLowerBoundary < bandit1.enemyUpperBoundary)
			{
				if (lyn.playerLowerBoundary > bandit1.enemyLowerBoundary)
				{
					if (lyn.playerRightBoundary > bandit1.enemyLeftBoundary)
					{
						if (lyn.playerRightBoundary < bandit1.enemyRightBoundary)
						{
							lyn.Collided();
						}
					}
				}
			}
			if (playerHealth == 0)
			{
				currentState = GAMEOVER;
			}
			break;
		case GAMEOVER:		
			DrawString(gameOverText, screenWidth*.45, screenHeight*.75);
			if (IsKeyDown(32))
			{
				currentState = GAMEPLAY;
			}
			break;
		};
		ClearScreen();

	} while (!FrameworkUpdate());

	Shutdown();

	return 0;
}