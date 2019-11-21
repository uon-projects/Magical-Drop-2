#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "ZeoFlow_SFML.h"

//Compiler Directives
using namespace std;
using namespace zeoFlow;

sf::RenderWindow window(sf::VideoMode(800, 500), "Magical Drop 2");
ZeoFlow_SFML zfSFML;
bool ballsStreak;

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

void drawCharacter(int type, int characterX, int characterY)
{
	sf::Sprite ballSprite(zfSFML.loadSpriteFromTexture("Assets/", "ball", "png"));
	ballSprite.setColor(sf::Color(247, 36, 36));
	ballSprite.setPosition(40*characterY + 220, 40*characterX);
	window.draw(ballSprite);
}

void drawPointers(int characterX, int characterY, int ballsInHandNo, int ballsInHandType, int gameGrid[40][40], sf::Color gameBallColors[10])
{
	sf::Sprite pointerSprite(zfSFML.loadSpriteFromTexture("Assets/", "pointer", "png"));
	sf::Sprite handPointerSprite(zfSFML.loadSpriteFromTexture("Assets/", "hand_pointer", "png"));
	sf::Sprite inHandBallSprite(zfSFML.loadSpriteFromTexture("Assets/", "hand_1ball", "png"));
	sf::Sprite inHandBalls2Sprite(zfSFML.loadSpriteFromTexture("Assets/", "hand_2balls", "png"));
	sf::Sprite inHandBalls3Sprite(zfSFML.loadSpriteFromTexture("Assets/", "hand_3balls", "png"));

	int i;
	if (ballsInHandNo == 0) {
		i = characterX - 1;
	} else {
		i = characterX - 2;
		handPointerSprite.setPosition(40*characterY + 220, 40*(i + 1));	
		window.draw(handPointerSprite);
		if (ballsInHandNo == 1) {
			inHandBallSprite.setPosition(40*characterY + 220, 40*(i + 1));
			inHandBallSprite.setColor(gameBallColors[ballsInHandType]);
			window.draw(inHandBallSprite);
		} else if (ballsInHandNo == 2) {
			inHandBalls2Sprite.setPosition(40*characterY + 220, 40*(i + 1));
			inHandBalls2Sprite.setColor(gameBallColors[ballsInHandType]);
			window.draw(inHandBalls2Sprite);
		} else if (ballsInHandNo >= 3) {
			inHandBalls3Sprite.setPosition(40*characterY + 220, 40*(i + 1));
			inHandBalls3Sprite.setColor(gameBallColors[ballsInHandType]);
			window.draw(inHandBalls3Sprite);
		}
	}
	for(i; i>=0; i--) {
		if (gameGrid[i][characterY] == 0) {
			pointerSprite.setColor(sf::Color(247, 36, 36));
			pointerSprite.setPosition(40*characterY + 220, 40*i);
			window.draw(pointerSprite);
		}
	}
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

void getSameBallsRangeColumn(int begin, int &end, int column, int gameGrid[40][40])
{
	bool exit = false;
	int i = begin;
	int type = gameGrid[begin][column];
	while(i>=0 && !exit) {
		if(gameGrid[i][column] == type) {
			i--;
		} else {
			exit = true;
		}
	}
	end = i;
}

void removeSameBallRow(int gameGrid[40][40], int line, int columnsNo, int linesNo, int characterY, int type);

void removeSameBallColumn(int gameGrid[40][40], int line, int columnsNo, int characterY, int type, int linesNo)
{
	bool topExit = false;
	int i = line - 1;
	while(i>=0 && !topExit) {
		if(gameGrid[i][columnsNo] == type) {
			gameGrid[i][columnsNo] = 0;
			removeSameBallRow(gameGrid, i, columnsNo, linesNo, characterY, type);
			i--;
		} else {
			topExit = true;
		}
	}
	bool bottomExit = false;
	i = line + 1;
	while(i<linesNo && !bottomExit) {
		if(gameGrid[i][columnsNo] == type) {
			gameGrid[i][columnsNo] = 0;
			removeSameBallRow(gameGrid, i, columnsNo, linesNo, characterY, type);
			i++;
		} else {
			bottomExit = true;
		}
	}
}

void removeSameBallRow(int gameGrid[40][40], int line, int columnsNo, int linesNo, int characterY, int type)
{
	bool leftExit = false;
	int i = characterY - 1;
	while(i>=0 && !leftExit) {
		if(gameGrid[line][i] == type) {
			gameGrid[line][i] = 0;
			removeSameBallColumn(gameGrid, line, i, characterY, type, linesNo);
			i--;
		} else {
			leftExit = true;
		}
	}
	bool rightExit = false;
	i = characterY + 1;
	while(i<columnsNo && !rightExit) {
		if(gameGrid[line][i] == type) {
			gameGrid[line][i] = 0;
			removeSameBallColumn(gameGrid, line, i, characterY, type, linesNo);
			i++;
		} else {
			rightExit = true;
		}
	}

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

void checkCanRemoveBalls(int gameGrid[40][40], int linesNo, int columnsNo, int characterY)
{

	int i = linesNo - 1;
	bool found = false;
	while(i>=1 && !found) {
		if(gameGrid[i - 1][characterY] != 0) {
			found = true;
		}
		i--;
	}
	int begin = i, end;
	int type = gameGrid[begin][characterY];
	getSameBallsRangeColumn(begin, end, characterY, gameGrid);
	int noBalls = begin - end;
	if (noBalls >= 3 || ballsStreak) {
		i = begin;
		for(i; i>end; i--) {
			gameGrid[i][characterY] = 0;
			removeSameBallRow(gameGrid, i, columnsNo, linesNo, characterY, type);
		}
	}
	checkEmptySpaces(gameGrid, linesNo, columnsNo);

}

int main()
{
	window.setFramerateLimit(60);

	sf::Sprite backgroundSprite(zfSFML.loadSpriteFromTexture("Assets/", "background", "png"));
	sf::Sprite ballSprite(zfSFML.loadSpriteFromTexture("Assets/", "ball", "png"));
	sf::Sprite zeoFlowSprite(zfSFML.loadSpriteFromTexture("Assets/", "zeoflow_logo", "png"));

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
	int ballsInHandNo = 0, ballsInHandType = 0;
	bool rowGenerated = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			{
                window.close();
			} else if(event.type == sf::Event::KeyPressed) {
				if (event.key.code == 71)
				{
					//left
					if (characterY > 0) {
						characterY--;
					} else {
						characterY = levelColumns - 1;
					}
					drawCharacter(0, characterX, characterY);
					drawPointers(characterX, characterY, ballsInHandNo, ballsInHandType, gameGrid, gameBallColors);
				}
				if (event.key.code == 72)
				{
					//right
					if (characterY < levelColumns - 1) {
						characterY++;
					} else {
						characterY = 0;
					}
					drawCharacter(0, characterX, characterY);
					drawPointers(characterX, characterY, ballsInHandNo, ballsInHandType, gameGrid, gameBallColors);
				}
				if (event.key.code == 73)
				{
					//up
					int noBalls = ballsInHandNo;
					ballsStreak = ballsInHandNo >=3;
					throwBalls(ballsInHandType, ballsInHandNo, gameGrid, characterY, levelLines);
					if (noBalls != 0) {
						checkCanRemoveBalls(gameGrid, levelLines, levelColumns, characterY);
					}
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
				levelColumns = 7;
				levelLines = 12;
				generateGameGrid(1, gameGrid, levelLines, levelColumns);
				generateGameBallColors(gameBallColors, 10);
				characterX = levelLines - 1;
				characterY = levelColumns/2;
				clock.restart();
			} else {
				int sec = (int) clock.getElapsedTime().asSeconds();
				if(sec % 10 == 5 && !rowGenerated) {
					addRow(1, gameGrid, levelLines, levelColumns);
					rowGenerated = true;
				} else if (sec % 10 == 9) {
					clock.restart();
					rowGenerated = false;
				}
				//draw game pieces
				for(int i=0; i<levelLines; i++) {
					for(int j=0; j<levelColumns; j++) {
						if(gameGrid[i][j] != 0) {
							//ballSprite.setColor(gameBallColors[gameGrid[i][j]]);
							ballSprite.setPosition(40*j + 220, 40*i);
							window.draw(ballSprite);
						}
					}
				}
				drawCharacter(0, characterX, characterY);
				drawPointers(characterX, characterY, ballsInHandNo, ballsInHandType, gameGrid, gameBallColors);
				if(checkGameLost(gameGrid, levelLines, levelColumns)) {
					cout<<"Game Lost";
				}/* else if(checkGameWon(gameGrid, levelLines, levelColumns) && gameGridGenerated) {
					cout<<"Game Won";
				}*/
			}
		} else {
			window.draw(backgroundSprite);
		}
		
        window.display();
	}

	getchar();

	return 0;
}