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
* most important functions
*
*/


#pragma once
#include <iostream>
#include <fstream>
#include <istream>
#include <string>

int generateRandomNumberBetweenOneAndTwo()
{
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	return (std::rand() % 2) + 1;
}

char** initBoard(int level)
{
	int size = boardSizeForLevel(level);

	char** board = new char* [size];

	for (size_t i = 0; i < size; i++)
	{
		board[i] = new char[size + 1];

		for (size_t j = 0; j < size; j++)
		{
			board[i][j] = '0';
		}

		board[i][size] = '\0';
	}

	return board;
}

bool setup(char* username)
{
	printNonogramTitle();

	//login register
	char answer[MaxRowLengthInput];
	std::cout << std::endl << "Do you have an account \"Y/N\": ";
	std::cin.getline(answer, MaxRowLengthInput);

	while (mystrcmp(answer, "Y") && mystrcmp(answer, "N") && mystrcmp(answer, "y") && mystrcmp(answer, "n"))
	{
		std::cout << "Invalid answer! Try again: ";
		std::cin.getline(answer, MaxRowLengthInput);
	}

	char doesUserHaveAnAccount = (!mystrcmp(answer, "Y") || !mystrcmp(answer, "y")) ? true : false;

	//load all accounts from the file
	char filename[] = "accounts.txt";
	int accountsFileRows = fileRowsCount(filename);
	char** accounts = new char* [accountsFileRows];

	fillRowsFromFileToCharArray(filename, accounts, accountsFileRows);

	userInput(accounts, accountsFileRows, username, doesUserHaveAnAccount);

	deleteMatrix(accounts, accountsFileRows);

	return doesUserHaveAnAccount;
}

int findBiggestCountOfSequencesForRowsOrCols(char** answerBoard, int size, bool isForRows)
{
	if (!answerBoard)
	{
		return -1;
	}
	for (size_t i = 0; i < size; i++)
	{
		if (!answerBoard[i])
		{
			return -1;
		}
	}

	int biggestCountOfSequences = 0;

	bool isInSequence = false;
	int currentCountOfSequences = 0;
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (isForRows ? answerBoard[i][j] == '1' : answerBoard[j][i] == '1')
			{
				isInSequence = true;
			}
			else
			{
				if (isInSequence == true)
				{
					currentCountOfSequences++;
					isInSequence = false;
				}
			}
		}
		if (isInSequence == true)
		{
			currentCountOfSequences++;
			isInSequence = false;
		}

		if (currentCountOfSequences > biggestCountOfSequences)
		{
			biggestCountOfSequences = currentCountOfSequences;
		}

		currentCountOfSequences = 0;
	}

	return biggestCountOfSequences;
}

void loadDataForRowsOrCols(char** answerBoard, int** data, int longestPossibleRowInData, int size, bool isForRows)
{
	if (!answerBoard || !data)
	{
		return;
	}

	for (size_t i = 0; i < size; i++)
	{
		if (!answerBoard[i])
		{
			return;
		}
	}

	for (size_t i = 0; i < longestPossibleRowInData; i++)
	{
		if (!data[i])
		{
			return;
		}
	}

	int indexToSaveOn = longestPossibleRowInData - 1;
	int currentCountOfSequences = 0;
	bool wasPreviousAValidPosition = false;

	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (isForRows ? answerBoard[i][j] == '1' : answerBoard[j][i] == '1')
			{
				currentCountOfSequences++;
				wasPreviousAValidPosition = true;
			}
			else
			{
				if (wasPreviousAValidPosition)
				{
					data[i][indexToSaveOn] = currentCountOfSequences;
					indexToSaveOn--;
					currentCountOfSequences = 0;
					wasPreviousAValidPosition = false;
				}
			}
		}
		if (currentCountOfSequences)
		{
			data[i][indexToSaveOn] = currentCountOfSequences;
		}
		currentCountOfSequences = 0;
		indexToSaveOn = longestPossibleRowInData - 1;
		wasPreviousAValidPosition = false;
	}
}

int** getRowsOrColsInfo(char** answerBoard, int size, bool isForRows, int& longestRowLength)
{
	if (!answerBoard)
	{
		return nullptr;
	}
	for (size_t i = 0; i < size; i++)
	{
		if (!answerBoard[i])
		{
			return nullptr;
		}
	}

	longestRowLength = findBiggestCountOfSequencesForRowsOrCols(answerBoard, size, isForRows);

	int** data = new int* [size];

	for (size_t i = 0; i < size; i++)
	{
		data[i] = new int[longestRowLength];
		for (size_t j = 0; j < longestRowLength; j++)
		{
			data[i][j] = 0;
		}
	}

	loadDataForRowsOrCols(answerBoard, data, longestRowLength, size, isForRows);

	return data;
}



void autofillLineIfCurrentMoveFilledIt(int x, int y, char** currentBoard,char** answerBoard, int boardSize)
{
	if (!currentBoard)
		return;

	for (size_t i = 0; i < boardSize; i++)
	{
		if (!currentBoard[i])
			return;
	}

	bool isColFinished = true;
	bool isRowFinished = true;
	for (size_t i = 0; i < boardSize; i++)
	{
		if (answerBoard[i][y] == '1' && currentBoard[i][y] != '1')
			isColFinished = false;

		if (answerBoard[x][i] == '1' && currentBoard[x][i] != '1')
			isRowFinished = false;
	}

	if (isRowFinished)
	{
		for (size_t i = 0; i < boardSize; i++)
		{
			if (answerBoard[x][i] == '0')
				currentBoard[x][i] = '2';
		}
	}
	if (isColFinished)
	{
		for (size_t i = 0; i < boardSize; i++)
		{
			if (answerBoard[i][y] == '0')
				currentBoard[i][y] = '2';
		}
	}

}

bool doesUserWonTheGame(char** answerBoard,char** currentBoard, int boardSize)
{
	if (!answerBoard || !currentBoard)
		return false;

	for (size_t i = 0; i < boardSize; i++)
	{
		if (!answerBoard[i] || !currentBoard[i])
			return false;
	}

	for (size_t i = 0; i < boardSize; i++)
	{
		for (size_t j = 0; j < boardSize; j++)
		{
			if (answerBoard[i][j] == '1' && currentBoard[i][j] != '1')
				return false;
		}
	}

	return true;
}



char playNonogram(char** answerBoard, char** currentBoard, int boardSize, int level)
{
	if (!answerBoard || !currentBoard)
		return 'e';

	for (size_t i = 0; i < boardSize; i++)
	{
		if (!answerBoard[i] || !currentBoard[i])
			return 'e';
	}

	int longestRowLength = 0;
	int** rowsInfo = getRowsOrColsInfo(answerBoard, boardSize, true, longestRowLength);
	int longestColLength = 0;
	int** colsInfo = getRowsOrColsInfo(answerBoard, boardSize, false, longestColLength);

	int wrongAnswers = 0;

	while (true)
	{
		printWholeBoard(rowsInfo, longestRowLength, colsInfo, longestColLength, currentBoard, boardSize);
		std::cout << "You have only " << level - wrongAnswers << " lives left!" << std::endl;

		char saveGameMessage[] = "Do you want to save and exit the game?";
		char toSaveGame = getYesOrNoInput(saveGameMessage);

		if (toSaveGame == 'y')
			return 's';

		int x = 0;
		int y = 0;
		bool toFill = false;
		getInput(x, y, toFill, currentBoard, boardSize);

		if ((answerBoard[x][y] == '1' && toFill) || (answerBoard[x][y] == '0' && !toFill))
		{
			currentBoard[x][y] = (toFill ? '1' : '2');
			autofillLineIfCurrentMoveFilledIt(x, y, currentBoard, answerBoard, boardSize);
			if (doesUserWonTheGame(answerBoard, currentBoard, boardSize))
			{
				printWholeBoard(rowsInfo, longestRowLength, colsInfo, longestColLength, currentBoard, boardSize);
				std::cout << "You have successfully passed the level!";
				return 'w';
			}
		}
		else
		{
			std::cout << "You made a mistake. This position should not be " << (toFill ? "checked" : "empty") << "." << std::endl;
			if (++wrongAnswers == level)
			{
				std::cout << "You have lost! ";
				return 'l';
			}
		}
	}
}



bool handleAnswerAfterGameEnded(char answer, char**& currentBoard, int boardSize, int& level, int& boardForLevel,const char* username, bool doesUserWon)
{
	if (!username)
		return false;

	if (answer == 'n' || answer == 'N')
	{
		currentBoard = initBoard(level);
		saveGame(level, boardForLevel, username, currentBoard, boardSize);

		return false;
	}
	else if (answer == 'y' || answer == 'Y')
	{
		if (doesUserWon)
		{
			level++;
			boardForLevel = generateRandomNumberBetweenOneAndTwo();
		}
		currentBoard = initBoard(level);
	}

	return true;
}

bool handleGameAnswer(char gameAnswer, char**& currentBoard, int boardSize, int& level, int& boardForLevel,const char* username)
{
	if (!username)
		return false;

	if (gameAnswer == 'w')
	{
		char message[] = "Do you want to continue to the next level?";
		char answer = getYesOrNoInput(message);

		if (!handleAnswerAfterGameEnded(answer, currentBoard, boardSize, level, boardForLevel, username, true))
		{
			return false;
		}
	}
	else if (gameAnswer == 'l')
	{
		char message[] = "Try again?";
		char answer = getYesOrNoInput(message);

		if (!handleAnswerAfterGameEnded(answer, currentBoard, boardSize, level, boardForLevel, username, false))
		{
			return false;
		}
	}
	else if (gameAnswer == 's')
	{
		saveGame(level, boardForLevel, username, currentBoard, boardSize);
		return false;
	}
	else if (gameAnswer == 'e')
	{
		return false;
	}

	return true;
}

bool continueGameAnswerIsValid(char answer, int level)
{
	if (answer != 'c' && convertCharToDigit(answer) > level)
	{
		return false;
	}
	return true;
}

char savedGameOrContinueOnPreviousLevelsHandler(int& level)
{
	std::cout << "You have a saved game! You can continue the game or start on level which you have already played on." << std::endl;
	std::cout << "Your saved game is on level " << level << ". Choose 'c' for continue or enter the number of the level which you want to start play on. And remember you can play on levels before the one you reached:";

	char answer;
	std::cin >> answer;

	while (!continueGameAnswerIsValid(answer, level))
	{
		char answer;
		std::cin >> answer;
	}

	return answer;
}