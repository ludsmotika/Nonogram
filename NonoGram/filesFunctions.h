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
* file with functions which i use for working with files
*
*/

#pragma once
#include <fstream>
#include "basicFunctions.h";

const int MaxRowInFileLength = 50;

int fileRowsCount(const char* filename)
{
	if (!filename)
		return -1;

	std::ifstream filein(filename);

	if (!filein.is_open())
	{
		return -1;
	}

	int counter = 0;

	char currentRow[50];

	while (filein.getline(currentRow, 50, '\n'))
	{
		counter++;
	}


	filein.clear();
	filein.close();

	return counter;
}

void fillRowsFromFileToCharArray(const char* filename, char** accounts, int rowsCount)
{
	if (!filename)
		return;

	std::ifstream filein(filename);

	if (!filein.is_open())
	{
		return;
	}

	for (size_t i = 0; i < rowsCount; i++)
	{
		char currentRow[MaxRowInFileLength];
		filein.getline(currentRow, MaxRowInFileLength, '\n');
		int currentRowLength = mystrlen(currentRow);

		accounts[i] = new char[currentRowLength + 1];
		mystrcpy(currentRow, accounts[i]);
	}

	filein.clear();
	filein.close();
}

void saveUsernameAndPasswordInFile(const char* filename,const char* username,const char* password)
{
	if (!filename || !username || !password)
		return;

	std::ofstream ofs(filename, std::ofstream::app);

	if (!ofs.is_open())
	{
		return;
	}

	ofs << username << std::endl;
	ofs << password << std::endl;

	ofs.clear();
	ofs.close();
}


void deleteRows(const char* filename, int startRow, int endRow) 
{
	if (!filename)
		return;

	std::ifstream inputFile(filename);

	if (!inputFile.is_open()) {
		std::cout << "Error opening file: " << filename << std::endl;
		return;
	}

	if (startRow < 0 || endRow < startRow) {
		std::cout << "Invalid startRow or endRow values." << std::endl;
		inputFile.close();
		return;
	}

	std::ofstream outputFile("temp.txt");

	int currentRow = 0;
	char line[MaxRowInFileLength];

	while (inputFile.getline(line, sizeof(line)))
	{
		if (currentRow < startRow || currentRow > endRow)
		{
			outputFile << line << std::endl;
		}
		++currentRow;
	}

	inputFile.close();
	outputFile.close();

	if (std::remove(filename) != 0)
	{
		std::cout << "Error removing file: " << filename << std::endl;
		return;
	}

	if (std::rename("temp.txt", filename) != 0)
	{
		std::cout << "Error renaming file." << std::endl;
		return;
	}

}

int boardSizeForLevel(int level)
{
	switch (level)
	{
	case 1:
		return 5;
		break;
	case 2:
	case 3:
		return 10;
		break;
	case 4:
	case 5:
		return 15;
		break;

	default:
		return 0;
		break;
	}
}

void saveGame(int level, int boardForLevel, const char* username,char** board, int boardSize)
{
	if (!board || !username)
		return;

	for (size_t i = 0; i < boardSize; i++)
	{
		if (!board[i])
			return;
	}

	char filename[] = "savedGames.txt";
	int savedGamesFileRows = fileRowsCount(filename);
	char** savedGames = new char* [savedGamesFileRows];

	fillRowsFromFileToCharArray(filename, savedGames, savedGamesFileRows);

	bool doesUserHaveASavedGame = false;
	int rowToStartDeleting = -1;
	int rowToStopDeleting = -1;

	for (size_t i = 0; i < savedGamesFileRows; i++)
	{
		if (!mystrcmp(savedGames[i], "save") && !mystrcmp(savedGames[i + 1], username))
		{
			rowToStartDeleting = i;
			int levelForSavedGameToDelete = convertStrToUnsigned(savedGames[i + 2]);
			int rowsOfSavedGame = boardSizeForLevel(levelForSavedGameToDelete);

			rowToStopDeleting = rowToStartDeleting + 3 + rowsOfSavedGame;
			break;
		}
	}

	if (rowToStartDeleting != -1)
	{
		deleteRows(filename, rowToStartDeleting, rowToStopDeleting);
	}


	std::ofstream ofs(filename, std::ofstream::app);
	if (!ofs.is_open())
	{
		return;
	}

	ofs << "save" << std::endl;
	ofs << username << std::endl;
	ofs << level << std::endl;
	ofs << boardForLevel << std::endl;
	for (size_t i = 0; i < boardSize; i++)
	{
		ofs << board[i] << std::endl;
	}

	ofs.clear();
	ofs.close();

	deleteMatrix(savedGames, savedGamesFileRows);
}

char** checkForSavedGameOfUser(const char* username, int& level, int& boardForLevel, bool& doesUserHaveASavedGame)
{
	if (!username)
		return nullptr;

	char filename[] = "savedGames.txt";
	int savedGamesFileRows = fileRowsCount(filename);
	char** savedGames = new char* [savedGamesFileRows];

	fillRowsFromFileToCharArray(filename, savedGames, savedGamesFileRows);

	for (size_t i = 0; i < savedGamesFileRows; i++)
	{
		if (!mystrcmp(savedGames[i], "save") && savedGames[i + 1] && !mystrcmp(savedGames[i + 1], username))
		{
			level = convertStrToUnsigned(savedGames[i + 2]);
			boardForLevel = convertStrToUnsigned(savedGames[i + 3]);
			int boardSize = boardSizeForLevel(level);

			char** board = new char* [boardSize];

			for (size_t j = 0; j < boardSize; j++)
			{
				board[j] = new char[boardSize + 1];
				mystrcpy(savedGames[j + i + 4], board[j]);
			}

			deleteMatrix(savedGames, savedGamesFileRows);
			return board;
		}
	}

	deleteMatrix(savedGames, savedGamesFileRows);
	return nullptr;
}


char** loadLevel(int levelToLoad, int boardForLevel)
{
	char filename[] = "gameLevels.txt";
	int gameLevelsFileRows = fileRowsCount(filename);
	char** gameLevels = new char* [gameLevelsFileRows];

	fillRowsFromFileToCharArray(filename, gameLevels, gameLevelsFileRows);

	int counter = 1;
	int rowToStartReadingFrom = -1;

	for (size_t i = 0; i < gameLevelsFileRows; i++)
	{
		if (!mystrcmp(gameLevels[i], "level") && (counter++ == ((levelToLoad - 1) * 2) + boardForLevel))
		{
			rowToStartReadingFrom = i + 1;
			break;
		}
	}

	int boardSize = boardSizeForLevel(levelToLoad);

	char** board = new char* [boardSize];
	for (size_t i = 0; i < boardSize; i++)
	{
		board[i] = new char[boardSize];
		mystrcpy(gameLevels[rowToStartReadingFrom + i], board[i]);
	}

	return board;
}