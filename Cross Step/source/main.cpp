#include "AIE.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <Windows.h>


int screenWidth = 800;
int screenHeight = 800;
float GetDeltaTime();
char gameTimeString[10];
float gameTime = 0;
float menuTime = 0;
char menuText1[20] = "Cross Step";
char menuText2[20] = "Press Start";
unsigned int leftCap;
unsigned int rightCap;
unsigned int topCap;
unsigned int bottomCap;
int player1HighScore = 0;
int player2HighScore = 0;
char player1HighScoreString[10];
char player2HighScoreString[10];
char healthText[10] = "Health";
char playerText[10] = "Player";
char timeText[10] = "Time";
char gameOverText[10] = "Game Over";
char playerScoreText[10] = "Score";
bool IsTimeToShow = true;
void MenuFlash()
{
	menuTime += GetDeltaTime();
	if (IsTimeToShow)
	{
		DrawString(menuText2, screenWidth*.4, screenHeight*.6);
	}
	if (menuTime <= 1.5)
	{
		IsTimeToShow = true;
	}
	if (menuTime >= 1.5)
	{
		IsTimeToShow = false;
	}
	if (menuTime >= 3)
	{
		menuTime = 0;
	}
	
}
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
	float backgroundY2;
	unsigned int stageSprite;
	unsigned int stageSprite2;
};
class Bullet
{
public:

	float bulletUpperBoundary;
	float bulletLowerBoundary;
	float bulletLeftBoundary;
	float bulletRightBoundary;
	float bulletX;
	float bulletY;
	float bulletWidth = screenWidth*.01;
	float bulletHeight = screenHeight*.02;
	float bulletCount = 0;
	
	int bulletSprite = -1;
	bool HitboxActive = true;
	void Move()
	{
		bulletY += (screenHeight*.2) * GetDeltaTime();
		std::cout << bulletX << "," << bulletY << "";
		if (bulletY > screenHeight)
		{
			bulletCount -= 1;
		}
		if (HitboxActive)
		{// bounds
			bulletUpperBoundary = bulletY + bulletHeight*.5;
			bulletLowerBoundary = bulletY - bulletHeight*.5;
			bulletRightBoundary = bulletX + bulletWidth*.5;
			bulletLeftBoundary = bulletX - bulletWidth*.5;
			// movement
			// debug position
		}
	}
	
	void Draw()
	{
		std::cout << "ID" << bulletSprite << "\n";

		if (bulletSprite != -1)
		{
			MoveSprite(bulletSprite, bulletX, bulletY);
			DrawSprite(bulletSprite);
		}
	}
	


};
struct Enemy
{
	float enemyX;
	float enemyY;
	float enemyWidth = screenWidth*.1;
	float enemyHeight = screenHeight*.1;
	int enemySprite;
	float enemyUpperBoundary;
	float enemyLowerBoundary;
	float enemyLeftBoundary;
	float enemyRightBoundary;
	float enemyHealth;
	float timeToSpawn;
	float enemyCount;	
	bool IsDead = false;
	bool IsDrawn = true;
	

	void Collided()
	{
		
		enemyHealth -= 1;
		std::cout << "enemy was hit";
		if (enemyHealth == 0)
		{
			player1HighScore += 1;
			IsDead = true;
			if (IsDead)
			{
				IsDrawn = false;
				//remove boundaries
			} 
		}
	}

	void Draw()
	{
		if (IsDrawn)
		{
			DrawSprite(enemySprite);
		}
			
	}
	
};
Enemy bandit1[10];
std::vector<Enemy> banditVector;


// Create and move 10 enemies
void Load()
{
	// Iterate over the array
	for (int i = 0; i < 10; i++)
	{
		bandit1[i].enemySprite = CreateSprite("./images/enemy1.png", bandit1[i].enemyWidth, bandit1[i].enemyHeight, true);
		
	}
}
struct Player
{
public:

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
	float timeWhenShot = 0;
	float firingDelay = .25;
	float firingKey;
	
	Bullet bulletDagger;
	std::vector<Bullet> myBullets;

	bool IsDrawn = true;
	void Collided()
	{
		if (timeWhenDamaged + invulTime < gameTime) //if it's been two seconds since the last collision starting from 0, the player takes damage once
		{
			timeWhenDamaged = gameTime;
			playerHealth -= 1; //because of the timer health goes down by one
			//need to make the sprite flash to indicate being hit
		}
	}
	void Shoot()
	{
		if (IsDrawn)
		{
			if (IsKeyDown(firingKey))
			{
				if (bulletDagger.bulletCount < 3) //if less than 3 bullets on screen then it shoots
				{
					if (timeWhenShot + firingDelay < gameTime)
					{
						// change the position of the bullet
						{
							bulletDagger.bulletSprite = CreateSprite("./images/dagger.png", bulletDagger.bulletWidth, bulletDagger.bulletHeight, true);
							bulletDagger.bulletY = playerY + (playerHeight*.5) + bulletDagger.bulletHeight;
							bulletDagger.bulletX = playerX;
							timeWhenShot = gameTime;
							bulletDagger.bulletCount += 1;
						}
					}
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
};



// Called when collision is detected









int main(int argc, char* argv[])
{
	Initialise(800, 800, false, "Cross Step");
	SetBackgroundColour(SColour(0, 0, 0, 255));
	Background titleScreen;
	titleScreen.stageSprite = CreateSprite("./images/mainmenu.png", screenWidth*.75, screenHeight*1, true);
	titleScreen.stageSprite2 = CreateSprite("./images/mainmenu.png", screenWidth*.75, screenHeight*1, true);
	titleScreen.backgroundY = screenHeight*.5;
	titleScreen.backgroundY2 = screenHeight*1.5;
	Background stage1;
	stage1.stageSprite = CreateSprite("./images/stage1background.png", screenWidth*.75, screenHeight * 4, true);
	stage1.backgroundY = screenHeight*2;
	Player lyn;
	lyn.playerSprite = CreateSprite("./images/lyn.png", lyn.playerWidth, lyn.playerHeight, true);
	lyn.playerX = screenWidth*.5;
	lyn.playerY = screenHeight*.05;
	lyn.firingKey = 'F';
	lyn.upKey = 'W';
	lyn.downKey = 'S';
	lyn.leftKey = 'A';
	lyn.rightKey = 'D';
	Load();
	//set all my keys
	

	//second ship for coop if i have time to make
	/*Player legault;
	legault.playerSprite = CreateSprite("./images/legault.png", legault.playerWidth, legault.playerHeight, true);
	legault.playerX = screenWidth*.75;
	legault.playerY = screenHeight*.05;
	legault.upKey = 'I';
	legault.downKey = 'K';
	legault.leftKey = 'J';
	legault.rightKey = 'L';*/

	

	
	//bandit1[i].enemyHealth = 1;
	//bandit1[i].timeToSpawn = 15;
	//bandit1[i].enemyCount = 0;
	
	
	


	


	GAMESTATE currentState = MAINMENU;

	do
	{
		std::fstream file;
		file.open("./highscore.txt", std::ios_base::out);
		file << player1HighScoreString << std::endl;
		//highscores save to text doc incomplete
		itoa(player1HighScore, player1HighScoreString, 10);
		itoa(player2HighScore, player2HighScoreString, 10);
		itoa(lyn.playerHealth, lyn.playerHealthString, 10);
		itoa(gameTime, gameTimeString, 10);
		itoa(lyn.timeWhenDamaged, lyn.timeWhenDamagedString, 10);
		switch (currentState)
		{
		case MAINMENU:
			MoveSprite(titleScreen.stageSprite, screenWidth*.5, titleScreen.backgroundY);
			DrawSprite(titleScreen.stageSprite);
			MoveSprite(titleScreen.stageSprite2, screenWidth*.5, titleScreen.backgroundY2);
			DrawSprite(titleScreen.stageSprite2);
			titleScreen.backgroundY -= GetDeltaTime()*screenHeight*.2;
			titleScreen.backgroundY2 -= GetDeltaTime()*screenHeight*.2;
			if (titleScreen.backgroundY <= screenHeight*-.5)
			{
				titleScreen.backgroundY = screenHeight*1.5;
			}
			if (titleScreen.backgroundY2 <= screenHeight*-.5)
			{
				titleScreen.backgroundY2 = screenHeight*1.5;
			}


			MenuFlash();
			DrawString(menuText1, screenWidth*.4, screenHeight*.8);
			std::cout << menuTime << "\n";

			if (IsKeyDown('T'))
				currentState = GAMEPLAY;
			break;


		case GAMEPLAY:
			//Setting game time
			gameTime += GetDeltaTime();
			//Setting speed
			
			MoveSprite(stage1.stageSprite, screenWidth*.5, stage1.backgroundY);
			DrawSprite(stage1.stageSprite);
			MoveSprite(lyn.playerSprite, lyn.playerX, lyn.playerY);
			DrawSprite(lyn.playerSprite);
			stage1.backgroundY -= screenHeight*.05*GetDeltaTime();
			
			//Calling move function for my ship
			lyn.Move();
			lyn.Shoot();
			lyn.bulletDagger.Move();
			lyn.bulletDagger.Draw();
			for (int i = 0; i < 10; i++)
			{
				MoveSprite(bandit1[i].enemySprite, bandit1[i].enemyX, bandit1[i].enemyY);
				bandit1[i].Draw();
				bandit1[i].enemyY -= screenHeight*.02*GetDeltaTime();
				bandit1[i].enemyX = screenWidth*.5;
				bandit1[i].enemyY = screenHeight*.9;
				bandit1[i].enemyHealth = 1;
			}
			
			//Drawing sprites and their location
			DrawString(lyn.playerHealthString, screenWidth*.9, screenHeight*.5);
			DrawString(gameTimeString, screenWidth*.9, screenHeight*.8);
			DrawString(healthText, screenWidth*.9, screenHeight*.55);
			DrawString(playerText, screenWidth*.9, screenHeight*.6);
			DrawString(timeText, screenWidth*.9, screenHeight*.9);
			DrawString(player1HighScoreString, screenWidth*.025, screenHeight*.8);
			DrawString(playerScoreText, screenWidth*.025, screenHeight*.9);

			/*DrawString(lyn.timeWhenDamagedString, screenWidth*.9, screenHeight*.9);*/  //delete this to view the time when last damaged





			/*if (bandit1.timeToSpawn = 0)
			{
			while (bandit1.enemyCount < 5)
			{
			MoveSprite(bandit1.enemySprite, bandit1.enemyX, bandit1.enemyY);
			DrawSprite(bandit1.enemySprite);
			bandit1.timeToSpawn += 15;
			}

			}*/




			//Boundaries
			lyn.playerUpperBoundary = lyn.playerY + lyn.playerHeight*.5;
			lyn.playerLowerBoundary = lyn.playerY - lyn.playerHeight*.5;
			lyn.playerRightBoundary = lyn.playerX + lyn.playerWidth*.5;
			lyn.playerLeftBoundary = lyn.playerX - lyn.playerWidth*.5;

			for (int i = 0; i < 10; i++)
			{
				bandit1[i].enemyUpperBoundary = bandit1[i].enemyY + bandit1[i].enemyHeight*.5;
				bandit1[i].enemyLowerBoundary = bandit1[i].enemyY - bandit1[i].enemyHeight*.5;
				bandit1[i].enemyRightBoundary = bandit1[i].enemyX + bandit1[i].enemyWidth*.5;
				bandit1[i].enemyLeftBoundary = bandit1[i].enemyX - bandit1[i].enemyWidth*.5;


				//shit to speed up my game for testing



				if (lyn.playerUpperBoundary > bandit1[i].enemyLowerBoundary)
				{//right side top corner collision
					if (lyn.playerUpperBoundary < bandit1[i].enemyUpperBoundary)
					{
						if (lyn.playerRightBoundary > bandit1[i].enemyLeftBoundary)
						{
							if (lyn.playerRightBoundary < bandit1[i].enemyRightBoundary)
							{
								lyn.Collided();
							}
						}
					}
				}
				// if the top part of the ship goes inside the enemy
				if (lyn.playerUpperBoundary > bandit1[i].enemyLowerBoundary)
				{//collission detection for top left corner of shipsprite
					if (lyn.playerUpperBoundary < bandit1[i].enemyUpperBoundary)
					{
						if (lyn.playerLeftBoundary < bandit1[i].enemyRightBoundary)
						{
							//playerHealth -= 1;
							if (lyn.playerLeftBoundary > bandit1[i].enemyLeftBoundary)
							{
								lyn.Collided();
							}
						}
					}
				}

				if (lyn.playerLowerBoundary < bandit1[i].enemyUpperBoundary)
				{//bottom side left side collision
					if (lyn.playerLowerBoundary > bandit1[i].enemyLowerBoundary)
					{
						if (lyn.playerLeftBoundary < bandit1[i].enemyRightBoundary)
						{
							if (lyn.playerLeftBoundary > bandit1[i].enemyLeftBoundary)
							{
								lyn.Collided();
							}
						}
					}
				}
				if (lyn.playerLowerBoundary < bandit1[i].enemyUpperBoundary)
				{
					if (lyn.playerLowerBoundary > bandit1[i].enemyLowerBoundary)
					{
						if (lyn.playerRightBoundary > bandit1[i].enemyLeftBoundary)
						{
							if (lyn.playerRightBoundary < bandit1[i].enemyRightBoundary)
							{
								lyn.Collided();
							}
						}
					}
				}
				//top left side collision for bullet into enemy (I reversed the order unintentionally! SORRY!)

				if (bandit1[i].enemyLowerBoundary < lyn.bulletDagger.bulletUpperBoundary)
				{
					if (bandit1[i].enemyUpperBoundary > lyn.bulletDagger.bulletUpperBoundary)
					{
						if (bandit1[i].enemyRightBoundary > lyn.bulletDagger.bulletLeftBoundary)
						{
							if (bandit1[i].enemyLeftBoundary < lyn.bulletDagger.bulletLeftBoundary)
							{
								bandit1[i].Collided();
							}
						}
					}
				}
				//top right side collision for bullet into enemy
				if (bandit1[i].enemyLowerBoundary < lyn.bulletDagger.bulletUpperBoundary)
				{
					if (bandit1[i].enemyUpperBoundary > lyn.bulletDagger.bulletUpperBoundary)
					{
						if (bandit1[i].enemyRightBoundary > lyn.bulletDagger.bulletRightBoundary)
						{
							if (bandit1[i].enemyLeftBoundary < lyn.bulletDagger.bulletRightBoundary)
							{
								bandit1[i].Collided();
							}
						}
					}
				}
				if (bandit1[i].enemyLowerBoundary < lyn.bulletDagger.bulletLowerBoundary)
				{
					if (bandit1[i].enemyUpperBoundary > lyn.bulletDagger.bulletLowerBoundary)
					{
						if (bandit1[i].enemyRightBoundary > lyn.bulletDagger.bulletLeftBoundary)
						{
							if (bandit1[i].enemyLeftBoundary < lyn.bulletDagger.bulletLeftBoundary)
							{
								bandit1[i].Collided();
							}
						}
					}
				}
				if (bandit1[i].enemyLowerBoundary < lyn.bulletDagger.bulletLowerBoundary)
				{
					if (bandit1[i].enemyUpperBoundary > lyn.bulletDagger.bulletLowerBoundary)
					{
						if (bandit1[i].enemyRightBoundary > lyn.bulletDagger.bulletRightBoundary)
						{
							if (bandit1[i].enemyLeftBoundary < lyn.bulletDagger.bulletRightBoundary)
							{
								bandit1[i].Collided();
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
				lyn.bulletDagger.bulletCount = 0;
				currentState = GAMEPLAY;
			}
			break;
		};
		ClearScreen();

	}
	while (!FrameworkUpdate());
	
	Shutdown();

	return 0;
}