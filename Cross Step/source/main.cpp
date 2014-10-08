#include "AIE.h"
#include <iostream>
#include <vector>
#include <fstream>


int screenWidth = 900;
int screenHeight = 900;
float GetDeltaTime();
char gameTimeString[10];
float gameTime = 0;
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
//bullet part of player since idk how to do complicated shiz
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
	
	void Move()
	{
		// bounds
		bulletUpperBoundary = bulletY + bulletHeight*.5;
		bulletLowerBoundary = bulletY - bulletHeight*.5;
		bulletRightBoundary = bulletX + bulletWidth*.5;
		bulletLeftBoundary = bulletX - bulletWidth*.5;
		
		// movement
		bulletY += (screenHeight*.2) * GetDeltaTime();
		
		// debug position
		std::cout << bulletX << "," << bulletY << "";
		
		
		if (bulletY > screenHeight)
		{
			bulletCount -= 1;
			std::cout << "bulletsaredespawning";
			
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
	unsigned int enemySprite;
	typedef std::vector<unsigned int> Row;  //turning it into a vector or something
	typedef std::vector<Row> Grid;

	float enemyUpperBoundary;
	float enemyLowerBoundary;
	float enemyLeftBoundary;
	float enemyRightBoundary;
	float enemyHealth;
	float timeToSpawn;
	float enemyCount;

	void Collided()
	{
		enemyHealth -= 1;
	std::cout << "enemy was hit";
	player1HighScore += 1;
		//if enemy health = 0
	}

};
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
		if (IsKeyDown(firingKey))
		{
			if (bulletDagger.bulletCount < 3) //if less than 3 bullets on screen then it shoots
			{
				if (timeWhenShot + firingDelay < gameTime)
				{
					// change the position of the bullet
					bulletDagger.bulletSprite = CreateSprite("./images/dagger.png", bulletDagger.bulletWidth, bulletDagger.bulletHeight, true);
					bulletDagger.bulletY = playerY + (playerHeight*.5) + bulletDagger.bulletHeight;
					bulletDagger.bulletX = playerX;
					timeWhenShot = gameTime;
					bulletDagger.bulletCount += 1;
					std::cout << "bullets are spawning";
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
	Initialise(900, 900, false, "Cross Step");
	SetBackgroundColour(SColour(0, 0, 0, 255));


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
	//set all my keys
	

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
	bandit1.enemyY = screenHeight*.9;
	bandit1.enemyHealth = 1;
	bandit1.timeToSpawn = 15;
	bandit1.enemyCount = 0;
	


	


	GAMESTATE currentState = MAINMENU;

	do
	{
		std::fstream file;
		file.open("./highscore.txt", std::ios_base::out);
		file << player1HighScoreString << player2HighScoreString << std::endl;
		//highscores save to text doc incomplete

		itoa(player1HighScore, player1HighScoreString, 10);
		itoa(player2HighScore, player2HighScoreString, 10);
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
			
			MoveSprite(stage1.stageSprite, screenWidth*.5, stage1.backgroundY);
			DrawSprite(stage1.stageSprite);
			MoveSprite(lyn.playerSprite, lyn.playerX, lyn.playerY);
			DrawSprite(lyn.playerSprite);
			stage1.backgroundY -= screenHeight*.05*GetDeltaTime();
			bandit1.enemyY -= screenHeight*.02*GetDeltaTime();
			//Calling move function for my ship
			lyn.Move();
			lyn.Shoot();
			lyn.bulletDagger.Move();
			lyn.bulletDagger.Draw();
			//Drawing sprites and their location
			DrawString(lyn.playerHealthString, screenWidth*.9, screenHeight*.5);
			DrawString(gameTimeString, screenWidth*.9, screenHeight*.8);
			DrawString(healthText, screenWidth*.9, screenHeight*.55);
			DrawString(playerText, screenWidth*.9, screenHeight*.6);
			DrawString(timeText, screenWidth*.9, screenHeight*.9);
			DrawString(player1HighScoreString, screenWidth*.025, screenHeight*.8);
			DrawString(playerScoreText, screenWidth*.025, screenHeight*.9);
			
			/*DrawString(lyn.timeWhenDamagedString, screenWidth*.9, screenHeight*.9);*/  //delete this to view the time when last damaged
			
			


			bandit1.timeToSpawn -= GetDeltaTime();
			/*if (bandit1.timeToSpawn = 0)
			{
				while (bandit1.enemyCount < 5)
				{
					MoveSprite(bandit1.enemySprite, bandit1.enemyX, bandit1.enemyY);
					DrawSprite(bandit1.enemySprite);
					bandit1.timeToSpawn += 15;
				}
				
			}*/
			MoveSprite(bandit1.enemySprite, bandit1.enemyX, bandit1.enemyY);
			DrawSprite(bandit1.enemySprite);
			//Boundaries
			lyn.playerUpperBoundary = lyn.playerY + lyn.playerHeight*.5;
			lyn.playerLowerBoundary = lyn.playerY - lyn.playerHeight*.5;
			lyn.playerRightBoundary = lyn.playerX + lyn.playerWidth*.5;
			lyn.playerLeftBoundary = lyn.playerX - lyn.playerWidth*.5;
			
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
			//top left side collision for bullet into enemy (I reversed the order unintentionally! SORRY!)
			if (bandit1.enemyLowerBoundary < lyn.bulletDagger.bulletUpperBoundary)
			{
				if (bandit1.enemyUpperBoundary > lyn.bulletDagger.bulletUpperBoundary)
				{
					if (bandit1.enemyRightBoundary > lyn.bulletDagger.bulletLeftBoundary)
					{
						if (bandit1.enemyLeftBoundary < lyn.bulletDagger.bulletLeftBoundary)
						{
							bandit1.Collided();
						}
					}
				}
			}
			//top right side collision for bullet into enemy
			if (bandit1.enemyLowerBoundary < lyn.bulletDagger.bulletUpperBoundary)
			{
				if (bandit1.enemyUpperBoundary > lyn.bulletDagger.bulletUpperBoundary)
				{
					if (bandit1.enemyRightBoundary > lyn.bulletDagger.bulletRightBoundary)
					{
						if (bandit1.enemyLeftBoundary < lyn.bulletDagger.bulletRightBoundary)
						{
							bandit1.Collided();
						}
					}
				}
			}
			if (bandit1.enemyLowerBoundary < lyn.bulletDagger.bulletLowerBoundary)
			{
				if (bandit1.enemyUpperBoundary > lyn.bulletDagger.bulletLowerBoundary)
				{
					if (bandit1.enemyRightBoundary > lyn.bulletDagger.bulletLeftBoundary)
					{
						if (bandit1.enemyLeftBoundary < lyn.bulletDagger.bulletLeftBoundary)
						{
							bandit1.Collided();
						}
					}
				}
			}
			if (bandit1.enemyLowerBoundary < lyn.bulletDagger.bulletLowerBoundary)
			{
				if (bandit1.enemyUpperBoundary > lyn.bulletDagger.bulletLowerBoundary)
				{
					if (bandit1.enemyRightBoundary > lyn.bulletDagger.bulletRightBoundary)
					{
						if (bandit1.enemyLeftBoundary < lyn.bulletDagger.bulletRightBoundary)
						{
							bandit1.Collided();
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

	} while (!FrameworkUpdate());

	Shutdown();

	return 0;
}