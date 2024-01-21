/**
*
* Solution to course project # 9
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Daniel Stefanov
* @idnumber 0MI0600397
* @compiler VC
*
* main NonoGram.cpp file
*
*/


#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include "basicFunctions.h"
#include "filesFunctions.h"
#include "printingFunctions.h"
#include "userInputFunctions.h"
#include "mainGameFunctions.h"


int main()
{
	char username[MaxAccountNameLength];
	bool doesCurrentUserAlreadyHaveAnAccount = setup(username);

	int level = 1;
	int boardForLevel = generateRandomNumberBetweenOneAndTwo();
	bool doesUserHaveASavedGame = false;
	char** currentBoard = initBoard(level);
	char** answerBoard;

	if (doesCurrentUserAlreadyHaveAnAccount)
	{
		currentBoard = checkForSavedGameOfUser(username, level, boardForLevel, doesUserHaveASavedGame);

		if (!currentBoard)
		{
			currentBoard = initBoard(level);
		}
		else
		{
			char answer = savedGameOrContinueOnPreviousLevelsHandler(level);

			if (answer != 'c')
			{
				level = convertCharToDigit(answer);
				currentBoard = initBoard(level);
			}
		}
	}

	while (true)
	{
		answerBoard = loadLevel(level, boardForLevel);
		int boardSize = boardSizeForLevel(level);

		char gameAnswer = playNonogram(answerBoard, currentBoard, boardSize, level);
		// w == won, l == lose, s == saveGame and exit

		if (!handleGameAnswer(gameAnswer, currentBoard, boardSize, level, boardForLevel, username))
			return 0;

		if (level == 6)
		{
			std::cout << "You passed all the levels. Congratulations!!!";
			return 0;
		}
	}
}


