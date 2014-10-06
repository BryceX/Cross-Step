#include "AIE.h"
#include <iostream>

int screenWidth = 900;
int screenHeight = 900;
float GetDeltaTime();
char gameTimeString[10];
float gameTime = 0;
unsigned int leftCap;
unsigned int rightCap;
unsigned int topCap;
unsigned int bottomCap;
char healthHUD[10] = "Health";
char timeHUD[10] = "Time";
char gameOverText[10] = "Game Over";
void MovementCap(float objWidth, float objHeight)
{
	topCap = screenHeight - objHeight*.5;
	bottomCap = 0 + objHeight*.5;
	leftCap = screenWidth*.125 + objWidth*.5;
	rightCap = screenWidth*.875 - objWidth*.5;
}



enum GAMESTATE
{
	MAINMENU,
	GAMEPLAY,
	GAMEOVER,
};

struct Background
{
	float backgroundY;
	unsigned int stageSprite;
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
	float enemyHealth;

};
struct Player
{
	char playerHealthString[10];
	int playerHealth = 5;
	unsigned int playerSprite;
	unsigned int upKey;
	unsigned int downKey;
	unsigned int leftKey;
	unsigned int rightKey;
	float playerX;
	float playerY;
	float playerWidth = screenWidth*.05;
	float playerHeight = screenHeight*.05;	
	float playerUpperBoundary;
	float playerLowerBoundary;
	float playerLeftBoundary;
	float playerRightBoundary;
	float timeWhenDamaged = 0;
	char timeWhenDamagedString[10];
	float invulTime = 2;
	//bullet part of player since idk how to do complicated shiz
	unsigned int bulletSprite;
	float bulletCount = 0;
	float bulletX;
	float bulletY;
	float bulletWidth = screenWidth*.01;
	float bulletHeight = screenHeight*.02;
	float bulletUpperBoundary;
	float bulletLowerBoundary;
	float bulletLeftBoundary;
	float bulletRightBoundary;
	float timeWhenShot = 0;
	float firingDelay = .25;
	float firingKey;
	
	
	

	void Shoot()
	{
		if (IsKeyDown(firingKey))
		{
			if (bulletCount < 3)
			{
				if (timeWhenShot + firingDelay < gameTime)
				{
					MoveSprite(bulletSprite, playerX, playerUpperBoundary);
					DrawSprite(bulletSprite);
					bulletY += screenHeight*.2;
					timeWhenShot = gameTime;
					bulletCount += 1;
					if (bulletY = screenHeight)
						bulletCount -= 1;
					std::cout << "bulletsareworking";
				}
			}
		}
	}


	

	void Move()
	{
		if (IsKeyDown(upKey))
		{
			playerY += screenHeight*.35*GetDeltaTime();
			if (playerY > screenHeight - playerHeight*.5)
			{
				playerY = screenHeight - playerHeight*.5;
			}
		}
		if (IsKeyDown(downKey))
		{
			playerY -= screenHeight*.35*GetDeltaTime();
			if (playerY < 0 + playerHeight*.5)
			{
				playerY = 0 + playerHeight*.5;
			}
		}
		if (IsKeyDown(leftKey))
		{
			playerX -= screenWidth*.35*GetDeltaTime();
			if (playerX < screenWidth*.125 + playerWidth*.5)
			{
				playerX = screenWidth*.125 + playerWidth*.5;
			}
		}
		if (IsKeyDown(rightKey))
		{
			playerX += screenWidth*.35*GetDeltaTime();
			if (playerX > screenWidth*.875 - playerWidth*.5)
			{
				playerX = screenWidth*.875 - playerWidth*.5;
			}
		}
	}



	// Called when collision is detected
	void Collided()
	{
		if (timeWhenDamaged + invulTime < gameTime) //if it's been two seconds since the last collision starting from 0, the player takes damage once
		{
			timeWhenDamaged = gameTime;
			playerHealth -= 1; //because of the timer health goes down by one
			//need to make the sprite flash to indicate being hit
		}

	}
	

};





int main(int argc, char* argv[])
{
	Initialise(900, 900, false, "Cross Step");
	SetBackgroundColour(SColour(0, 0, 0, 255));


	Background stage1;
	stage1.stageSprite = CreateSprite("./images/stage1background.png", screenWidth*.75, screenHeight * 4, true);
	stage1.backgroundY = screenHeight*2;

	Player lyn;
	lyn.playerSprite = CreateSprite("./images/lyn.png", lyn.playerWidth, lyn.playerHeight, true);
	lyn.playerX = screenWidth*.5;
	lyn.playerY = screenHeight*.05;
	lyn.upKey = 'W';
	lyn.downKey = 'S';
	lyn.leftKey = 'A';
	lyn.rightKey = 'D';
	
	//set all my keys
	Player playerDagger;
	playerDagger.bulletSprite = CreateSprite("./images/dagger.png", playerDagger.bulletWidth, playerDagger.bulletHeight, true);
	playerDagger.bulletX = lyn.playerX;
	playerDagger.bulletY = lyn.playerY + (lyn.playerHeight*.5) + playerDagger.bulletHeight;
	playerDagger.firingKey = 'F';

	//second ship for coop if i have time to make
	Player legault;
	legault.playerSprite = CreateSprite("./images/legault.png", legault.playerWidth, legault.playerHeight, true);
	legault.playerX = screenWidth*.75;
	legault.playerY = screenHeight*.05;
	legault.upKey = 'I';
	legault.downKey = 'K';
	legault.leftKey = 'J';
	legault.rightKey = 'L';

	Enemy bandit1;
	bandit1.enemySprite = CreateSprite("./images/enemy1.png", bandit1.enemyWidth, bandit1.enemyHeight, true);
	bandit1.enemyX = screenWidth*.5;
	bandit1.enemyY = screenHeight*1.25;
	bandit1.enemyHealth = 3;


	


	GAMESTATE currentState = MAINMENU;

	do
	{
		itoa(lyn.playerHealth, lyn.playerHealthString, 10);
		itoa(gameTime, gameTimeString, 10);
		itoa(lyn.timeWhenDamaged, lyn.timeWhenDamagedString, 10);
		switch (currentState)
		{
		case MAINMENU:

			if (IsKeyDown('T'))
				currentState = GAMEPLAY;

			break;


		case GAMEPLAY:
			//Setting game time
			gameTime += GetDeltaTime();
			//Setting speed
			playerDagger.Shoot();
			stage1.backgroundY -= screenHeight*.05*GetDeltaTime();
			bandit1.enemyY -= screenHeight*.002*GetDeltaTime();
			//Calling move function for my ship
			lyn.Move();
			
			//Drawing sprites and their location
			DrawString(lyn.playerHealthString, screenWidth*.9, screenHeight*.5);
			DrawString(gameTimeString, screenWidth*.9, screenHeight*.8);
			DrawString(healthHUD, screenWidth*.9, screenHeight*.6);
			DrawString(timeHUD, screenWidth*.9, screenHeight*.9);
			/*DrawString(lyn.timeWhenDamagedString, screenWidth*.9, screenHeight*.9);*/  //delete this to view the time when last damaged
			MoveSprite(stage1.stageSprite, screenWidth*.5, stage1.backgroundY);
			DrawSprite(stage1.stageSprite);
			MoveSprite(lyn.playerSprite, lyn.playerX, lyn.playerY);
			DrawSprite(lyn.playerSprite);
			MoveSprite(bandit1.enemySprite, bandit1.enemyX, bandit1.enemyY);
			DrawSprite(bandit1.enemySprite);
			//Boundaries
			lyn.playerUpperBoundary = lyn.playerY + lyn.playerHeight*.5;
			lyn.playerLowerBoundary = lyn.playerY - lyn.playerHeight*.5;
			lyn.playerRightBoundary = lyn.playerX + lyn.playerWidth*.5;
			lyn.playerLeftBoundary = lyn.playerX - lyn.playerWidth*.5;
			playerDagger.bulletUpperBoundary = playerDagger.bulletY + playerDagger.bulletHeight*.5;
			playerDagger.bulletLowerBoundary = playerDagger.bulletY - playerDagger.bulletHeight*.5;
			playerDagger.bulletRightBoundary = playerDagger.bulletX + playerDagger.bulletWidth*.5;
			playerDagger.bulletLeftBoundary = playerDagger.bulletX - playerDagger.bulletWidth*.5;
			bandit1.enemyUpperBoundary = bandit1.enemyY + bandit1.enemyHeight*.5;
			bandit1.enemyLowerBoundary = bandit1.enemyY - bandit1.enemyHeight*.5;
			bandit1.enemyRightBoundary = bandit1.enemyX + bandit1.enemyWidth*.5;
			bandit1.enemyLeftBoundary = bandit1.enemyX - bandit1.enemyWidth*.5;
			
			
			//shit to speed up my game for testing
			if (IsKeyDown('I'))
			{
			}
			if (IsKeyDown(32))
				bandit1.enemyY = screenHeight*.5;

			if (lyn.playerUpperBoundary > bandit1.enemyLowerBoundary)
			{//right side top corner collision
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
			// if the top part of the ship goes inside the enemy
			if (lyn.playerUpperBoundary > bandit1.enemyLowerBoundary)
			{//collission detection for top left corner of shipsprite
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
			
			if (lyn.playerLowerBoundary < bandit1.enemyUpperBoundary)
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
			if (lyn.playerLowerBoundary < bandit1.enemyUpperBoundary)
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
			if (lyn.playerHealth == 0)
			{
				currentState = GAMEOVER;
			}
			if (gameTime > 60)
			{
				stage1.backgroundY = screenHeight*(-1);
				//DrawSprite(FirstBoss, screenWidth*.5, screenHeight*.9)
			}
			break;
		case GAMEOVER:		
			DrawString(gameOverText, screenWidth*.45, screenHeight*.75);
			if (IsKeyDown('T'))//resets all the shit
			{
				lyn.playerHealth = 5;
				stage1.backgroundY = screenHeight*1.5;
				gameTime = 0;
				lyn.timeWhenDamaged = 0;
				lyn.playerX = screenWidth*.5;
				lyn.playerY = screenHeight*.05;
				currentState = GAMEPLAY;
			}
			break;
		};
		ClearScreen();

	} while (!FrameworkUpdate());

	Shutdown();

	return 0;
}