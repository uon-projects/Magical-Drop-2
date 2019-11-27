#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "ZeoFlow_SFML.h"

//Compiler Directives
using namespace std;
using namespace zeoFlow;

sf::RenderWindow window(sf::VideoMode(800, 500), "Magical Drop 2");
ZeoFlow_SFML zfSFML;
bool ballsStreak, gameLost;
int ballsInHandNo = 0, ballsInHandType = 0;

void generateGameGrid(int level, int gameGrid[40][40], int linesNo, int columnsNo)
{

	if (level == 1) {
		srand(time(NULL));
		for(int i=0; i<linesNo; i++) {
			for(int j=0; j<columnsNo; j++) {
				if (i<linesNo - 8) {
					int randNo;
					do {
						randNo = rand() % 6;
					} while (randNo == 0);
					gameGrid[i][j] = randNo;
				} else if (i<linesNo - 7) {
					int chance = rand() % 4;
					if (chance == 0) {
						gameGrid[i][j] = 0;
					} else {
						gameGrid[i][j] = rand() % 5 + 1;
					}
				} else if (i<linesNo - 6) {
					int chance = rand() % 3;
					if (gameGrid[i-1][j] == 0 || chance == 0) {
						gameGrid[i][j] = 0;
					} else {
						gameGrid[i][j] = rand() % 5 + 1;
					}
				} else {
					gameGrid[i][j] = 0;
				}
			}
		}
	}

}

void addRow(int level, int gameGrid[40][40], int linesNo, int columnsNo)
{

	if (level == 1) {
		for(int i=linesNo - 1; i>0; i--) {
			for(int j=0; j<columnsNo; j++) {
				gameGrid[i][j] = gameGrid[i - 1][j];
			}
		}
		srand(time(NULL));
		for(int i=0; i<columnsNo; i++) {
			int randNo;
			do {
				randNo = rand() % 6;
			} while (randNo == 0);
			gameGrid[0][i] = randNo;
		}
	}

}

void generateGameBallColors(sf::Color ballColors[10], int colors)
{
	
	srand(time(NULL));
	for(int i=0; i<colors; i++) {
		sf::Color generatedColor(rand() % 255, rand() % 255, rand() % 255);
		ballColors[i] = generatedColor;
	}

}

const int objectSize = 35;
int characterAnimation = 0;
sf::Sprite characterBoy[15];
void drawCharacter(int type, int characterX, int characterY, int ballsInHandNo, int ballsInHandType, sf::Color gameBallColors[10])
{
	characterBoy[characterAnimation].setPosition(objectSize*characterY + 120, 40*characterX);
	characterBoy[characterAnimation].setOrigin(30, 430);
	characterBoy[characterAnimation].setScale(0.2, 0.2);
	window.draw(characterBoy[characterAnimation]);
	characterAnimation++;
	if(characterAnimation == 7) characterAnimation = 0;

	int i;
	sf::Sprite inHandBallSprite(zfSFML.loadSpriteFromTexture("Assets/", "hand_1ball", "png"));
	sf::Sprite inHandBalls2Sprite(zfSFML.loadSpriteFromTexture("Assets/", "hand_2balls", "png"));
	sf::Sprite inHandBalls3Sprite(zfSFML.loadSpriteFromTexture("Assets/", "hand_3balls", "png"));
	sf::Sprite handPointerSprite(zfSFML.loadSpriteFromTexture("Assets/", "hand_pointer", "png"));

	if (ballsInHandNo == 0) {
		i = characterX - 1;
	} else {
		i = characterX - 2;
		handPointerSprite.setPosition(objectSize*characterY + 120, 40*(i + 1));	
		window.draw(handPointerSprite);
		if (ballsInHandNo == 1) {
			inHandBallSprite.setPosition(objectSize*characterY + 120, 40*(i + 1));
			inHandBallSprite.setColor(gameBallColors[ballsInHandType]);
			window.draw(inHandBallSprite);
		} else if (ballsInHandNo == 2) {
			inHandBalls2Sprite.setPosition(objectSize*characterY + 120, 40*(i + 1));
			inHandBalls2Sprite.setColor(gameBallColors[ballsInHandType]);
			window.draw(inHandBalls2Sprite);
		} else if (ballsInHandNo >= 3) {
			inHandBalls3Sprite.setPosition(objectSize*characterY + 120, 40*(i + 1));
			inHandBalls3Sprite.setColor(gameBallColors[ballsInHandType]);
			window.draw(inHandBalls3Sprite);
		}
	}
}

void drawPointers(int characterX, int characterY, int gameGrid[40][40])
{
	sf::Sprite pointerSprite(zfSFML.loadSpriteFromTexture("Assets/", "pointer", "png"));

	for(int i = characterX - 1; i>=0; i--) {
		if (gameGrid[i][characterY] == 0) {
			pointerSprite.setPosition(objectSize*characterY + 120, 40*i);
			window.draw(pointerSprite);
		}
	}
}

int getBallX(int gameGrid[40][40], int characterY, int gameLines)
{
	int i = gameLines;
	bool found = false;
	while(i>=1 && !found) {
		if(gameGrid[i - 1][characterY] != 0) {
			found = true;
		}
		i--;
	}
	return i;
}

int getSameBalls(int gameGrid[40][40], int ballY, int ballX)
{
	int i = ballX;
	int ballsNo = 1;
	int ballType = gameGrid[i][ballY];
	bool stop = false;
	while(i>=0 && !stop) {
		if(gameGrid[i - 1][ballY] == ballType) {
			ballsNo++;
			i--;
		} else {
			stop = true;
		}
	}
	return ballsNo;
}

void throwBalls(int &type, int &ballsNo, int gameGrid[40][40], int characterY, int gameLines)
{
	int i = gameLines - 1;
	bool found = false;
	while(i>=1 && !found) {
		if(gameGrid[i - 1][characterY] != 0) {
			found = true;
		} else {
			i--;
		}
	}
	if (found || i == 0) {
		int startLine = i;
		while(startLine < gameLines && ballsNo > 0 && type != 0) {
			gameGrid[startLine][characterY] = type;
			startLine++;
			ballsNo--;
			if (ballsNo == 0 || startLine == gameLines) {
				type = 0;
				ballsNo = 0;
			}
		}
		if(ballsStreak) {
			type = 0;
			ballsNo = 0;
		}
	}
}

void getBalls(int &type, int &ballsNo, int gameGrid[40][40], int characterY, int gameLines)
{
	int i = gameLines - 1;
	bool exit = false;
	while(i >= 0 && !exit) {
		if(gameGrid[i][characterY] != 0) {
			if (type == 0) {
				type = gameGrid[i][characterY];
				gameGrid[i][characterY] = 0;
				ballsNo++;
			} else {
				if(gameGrid[i][characterY] == type) {
					gameGrid[i][characterY] = 0;
					ballsNo++;
				} else {
					exit = true;
				}
			}
		}
		i--;
	}
}

bool checkGameLost(int gameGrid[40][40], int linesNo, int columnsNo)
{
	bool lost = false;
	int i = 0;
	while(!lost && i<columnsNo) {
		if (gameGrid[linesNo - 1][i] != 0) {
			lost = true;
		}
		i++;
	}
	return lost;

}

bool checkGameWon(int gameGrid[40][40], int linesNo, int columnsNo)
{
	bool won = false;
	int i = 0;
	while(!won && i<columnsNo) {
		if (gameGrid[0][i] == 0) {
			won = true;
		}
		i++;
	}
	return won;

}

void checkEmptySpaces(int gameGrid[40][40], int linesNo, int columnsNo)
{
	for(int i = 0; i<columnsNo; i++) {
		for(int j = linesNo - 1; j>=1; j--) {
			if(gameGrid[j][i] != 0 && gameGrid[j - 1][i] == 0) {
				gameGrid[j - 1][i] = gameGrid[j][i];
				gameGrid[j][i] = 0;
				j = linesNo - 1;
			}
		}
	}
}

void checkBallTop(int gameGrid[40][40], int linesNo, int columnsNo, int ballY, int ballX, int ballType);
void checkBallBottom(int gameGrid[40][40], int linesNo, int columnsNo, int ballY, int ballX, int ballType);
void checkBallLeft(int gameGrid[40][40], int linesNo, int columnsNo, int ballY, int ballX, int ballType);
void checkBallRight(int gameGrid[40][40], int linesNo, int columnsNo, int ballY, int ballX, int ballType);

void checkBallTop(int gameGrid[40][40], int linesNo, int columnsNo, int ballY, int ballX, int ballType)
{
	if(ballX > 0) {
		ballX -= 1;
		int cBallType = gameGrid[ballX][ballY];
		if(cBallType == ballType) {
			gameGrid[ballX][ballY] = ballType * -1;
			checkBallTop(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
			checkBallLeft(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
			checkBallRight(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
		}
	}
}

void checkBallBottom(int gameGrid[40][40], int linesNo, int columnsNo, int ballY, int ballX, int ballType)
{
	if(ballX < linesNo - 1) {
		ballX += 1;
		int cBallType = gameGrid[ballX][ballY];
		if(cBallType == ballType) {
			gameGrid[ballX][ballY] = ballType * -1;
			checkBallBottom(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
			checkBallLeft(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
			checkBallRight(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
		}
	}
}

void checkBallLeft(int gameGrid[40][40], int linesNo, int columnsNo, int ballY, int ballX, int ballType)
{
	if(ballY > 0) {
		ballY -= 1;
		int cBallType = gameGrid[ballX][ballY];
		if(cBallType == ballType) {
			gameGrid[ballX][ballY] = ballType * -1;
			checkBallBottom(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
			checkBallTop(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
			checkBallLeft(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
		}
	}
}

void checkBallRight(int gameGrid[40][40], int linesNo, int columnsNo, int ballY, int ballX, int ballType)
{
	if(ballY < columnsNo - 1) {
		ballY += 1;
		int cBallType = gameGrid[ballX][ballY];
		if(cBallType == ballType) {
			gameGrid[ballX][ballY] = ballType * -1;
			checkBallBottom(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
			checkBallTop(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
			checkBallRight(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
		}
	}
}

int lvlScore = 0;
void removeAllBalls(int gameGrid[40][40], int linesNo, int columnsNo, int ballType)
{
	int score = 0;
	for(int i = 0; i<linesNo; i++) {
		for(int j=0; j<columnsNo; j++) {
			if(gameGrid[i][j] * -1 == ballType) {
				gameGrid[i][j] = 0;
				score++;
			}
		}
	}
	lvlScore += score*40;
}

void markBalls(int gameGrid[40][40], int linesNo, int columnsNo, int ballY, int ballX, int ballsStreak)
{

	int ballType = gameGrid[ballX][ballY];
	if(getSameBalls(gameGrid, ballY, ballX) >= 3 || ballsStreak >= 3) {
		gameGrid[ballX][ballY] = ballType * -1;
		checkBallTop(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
		checkBallLeft(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
		checkBallRight(gameGrid, linesNo, columnsNo, ballY, ballX, ballType);
		removeAllBalls(gameGrid, linesNo, columnsNo, ballType);
		checkEmptySpaces(gameGrid, linesNo, columnsNo);
		cout<<"score: "<<lvlScore<<'\n';
	}

}

int main()
{
	window.setFramerateLimit(30);

	sf::Sprite backgroundSprite(zfSFML.loadSpriteFromTexture("Assets/", "background", "png"));
	sf::Sprite ballSprite(zfSFML.loadSpriteFromTexture("Assets/", "ball", "png"));
	sf::Sprite zeoFlowSprite(zfSFML.loadSpriteFromTexture("Assets/", "zeoflow_logo", "png"));
	sf::Sprite portal(zfSFML.loadSpriteFromTexture("Assets/", "portal", "png"));

	string boyCharacter = "idle_";
	for(int i=0; i<15; i++) {
		sf::Sprite characterIdle(zfSFML.loadSpriteFromTexture("Assets/", boyCharacter + to_string(i + 1), "png"));
		characterBoy[i] = characterIdle;
	}

	backgroundSprite = zfSFML.formatSpriteForBackground(backgroundSprite);

	int currentScreen = 0;
	sf::Clock clock;
	int clockState = 0;
	bool gameGridGenerated = false;
	int gameGrid[40][40];
	sf::Color gameBallColors[10];
	
	sf::RectangleShape splashScreenBg;
	splashScreenBg.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	sf::Color color(16, 16, 16);
	splashScreenBg.setFillColor(color);
	
	int characterX, characterY;
	int levelLines, levelColumns;
	bool rowGenerated = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			{
                window.close();
			} else if(event.type == sf::Event::KeyPressed && !gameLost) {
				if (event.key.code == 71)
				{
					//left
					if (characterY > 0) {
						characterY--;
					} else {
						characterY = levelColumns - 1;
					}
				}
				if (event.key.code == 72)
				{
					//right
					if (characterY < levelColumns - 1) {
						characterY++;
					} else {
						characterY = 0;
					}
				}
				if (event.key.code == 73)
				{
					//up
					int noBalls = ballsInHandNo;
					ballsStreak = ballsInHandNo >=3;
					throwBalls(ballsInHandType, ballsInHandNo, gameGrid, characterY, levelLines);
					int ballX = getBallX(gameGrid, characterY, levelLines);
					markBalls(gameGrid, levelLines, levelColumns, characterY, ballX, ballsStreak);
				}
				if (event.key.code == 74)
				{
					//down
					getBalls(ballsInHandType, ballsInHandNo, gameGrid, characterY, levelLines);
				}
			}
		}

        window.clear();

		if (clockState == 0)
		{
			clock.restart();
			clockState++;
		}
		if (currentScreen == 0)
		{
			float sec = clock.getElapsedTime().asSeconds();
			if (sec < 2.0)
			{
				window.draw(splashScreenBg);
				zeoFlowSprite.setScale(0.15, 0.15);
				zeoFlowSprite = zfSFML.spriteInMiddle(zeoFlowSprite, window.getSize().x, window.getSize().y);
				window.draw(zeoFlowSprite);
			} 
			else
			{
				gameGridGenerated = false;
				currentScreen = 1;
			}
		} else if(currentScreen == 1) {
			window.draw(backgroundSprite);
			if (!gameGridGenerated) {
				gameGridGenerated = true;
				levelColumns = 15;
				levelLines = 12;
				generateGameGrid(1, gameGrid, levelLines, levelColumns);
				generateGameBallColors(gameBallColors, 10);
				gameLost = false;
				characterX = levelLines - 1;
				characterY = levelColumns/2;
				clock.restart();
			} else {
				int sec = (int) clock.getElapsedTime().asSeconds();
				int rangeSec;
				if(lvlScore < 2500) {
					rangeSec = 9;
				} else if(lvlScore < 4500) {
					rangeSec = 8;
				} else if(lvlScore < 6500) {
					rangeSec = 7;
				} else if(lvlScore < 8500) {
					rangeSec = 6;
				} else {
					rangeSec = 5;
				}
				if(sec % rangeSec == 4 && !rowGenerated) {
					if(!gameLost) {
						addRow(1, gameGrid, levelLines, levelColumns);
					}
					rowGenerated = true;
				} else if (sec % rangeSec == rangeSec - 1) {
					clock.restart();
					rowGenerated = false;
				}
				sf::RectangleShape rectangle;
				rectangle.setSize(sf::Vector2f(objectSize * levelColumns + 3, objectSize * levelLines + 3));
				rectangle.setOutlineColor(sf::Color(43, 43, 43, 255));
				//rectangle.setFillColor(sf::Color(43, 43, 43, 70));
				rectangle.setFillColor(sf::Color(135, 43, 240, 70));
				rectangle.setOutlineThickness(10);
				rectangle.setPosition(121, 11);
				window.draw(rectangle);
				//draw game pieces
				for(int i=0; i<levelLines; i++) {
					for(int j=0; j<levelColumns; j++) {
						if(gameGrid[i][j] != 0) {
							int ballType = gameGrid[i][j];
							ballSprite.setColor(gameBallColors[ballType]);
							ballSprite.setPosition(objectSize*j + 120, objectSize*i + 10);
							window.draw(ballSprite);
						}
					}
				}
				drawPointers(characterX, characterY, gameGrid);
				drawCharacter(0, characterX, characterY, ballsInHandNo, ballsInHandType, gameBallColors);
				if(checkGameLost(gameGrid, levelLines, levelColumns) && !gameLost) {
					cout<<"Game Lost";
					gameLost = true;
				}/* else if(checkGameWon(gameGrid, levelLines, levelColumns) && gameGridGenerated) {
					cout<<"Game Won";
				}*/
				portal.setScale(0.2, 0.2);
				portal.setPosition(objectSize*-2 + 120, objectSize*(characterX - 1));
				window.draw(portal);
				portal.setScale(0.2, 0.2);
				portal.setPosition(objectSize*levelColumns + 133, objectSize*(characterX - 1));
				window.draw(portal);
			}
		} else {
			window.draw(backgroundSprite);
		}
        window.display();
	}

	getchar();

	return 0;
}