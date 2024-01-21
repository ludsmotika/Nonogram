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
* file with functions which are mainly used for printing on the console
*
*/


#pragma once
#include <iostream>

void printCharMatrix(const char** matrix, int boardSize) {
	if (!matrix)
	{
		return;
	}

	for (size_t i = 0; i < boardSize; i++)
	{
		if (!matrix[i])
		{
			return;
		}
	}

	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			std::cout << matrix[i][j] << '|';
		}
		std::cout << std::endl;
	}
}

void printNonogramTitle() {
	std::cout << " _   _   ___   _   _   ___    ____  ____      _     __  __ \n";
	std::cout << "| \\ | | / _ \\ | \\ | | / _ \\  / ___||  _ \\    / \\   |  \\/  |\n";
	std::cout << "|  \\| || | | ||  \\| || | | || |  _ | |_) |  / _ \\  | |\\/| |\n";
	std::cout << "| |\\  || |_| || |\\  || |_| || |_| ||  _ <  / ___ \\ | |  | |\n";
	std::cout << "|_| \\_| \\___/ |_| \\_| \\___/  \\____||_| \\_\\/_/   \\_\\|_|  |_|\n";
}

int getCountOfSymbolsToIndent(int** rowsInfo, int longestRowLength, int boardSize)
{
	if (!rowsInfo)
	{
		return -1;
	}

	for (size_t i = 0; i < boardSize; i++)
	{
		if (!rowsInfo[i])
		{
			return -1;
		}
	}

	int longestRowSymbols = 0;
	int currentRowSymbols = 0;

	for (size_t i = 0; i < boardSize; i++)
	{
		for (size_t j = 0; j < longestRowLength; j++)
		{
			if (rowsInfo[i][j] >= 10 && rowsInfo[i][j] <= 99)
			{
				currentRowSymbols += 3;
			}
			else if (rowsInfo[i][j] >= 1 && rowsInfo[i][j] <= 9)
			{
				currentRowSymbols += 2;
			}
			else if (rowsInfo[i][j] == 0)
			{
				currentRowSymbols += 2;
			}
		}

		if (currentRowSymbols > longestRowSymbols)
		{
			longestRowSymbols = currentRowSymbols;
		}
		currentRowSymbols = 0;
	}

	return longestRowSymbols;
}

void printWholeBoard(int** rowsInfo, int longestRowLength, int** colsInfo, int longestColLength, char** currentBoard, int boardSize)
{
	if (!rowsInfo || !colsInfo || !currentBoard)
		return;

	for (size_t i = 0; i < boardSize; i++)
	{
		if (!currentBoard[i])
			return;
	}
	for (size_t i = 0; i < longestRowLength; i++)
	{
		if (!rowsInfo[i])
			return;
	}
	for (size_t i = 0; i < longestColLength; i++)
	{
		if (!colsInfo[i])
			return;
	}

	int longestRowSymbols = getCountOfSymbolsToIndent(rowsInfo, longestRowLength, boardSize);

	for (int i = longestColLength - 1; i >= 0; i--)
	{
		for (size_t i = 0; i < longestRowSymbols; i++)
		{
			std::cout << " ";
		}
		std::cout << "|";
		for (size_t j = 0; j < boardSize; j++)
		{
			if (colsInfo[j][i] >= 1 && colsInfo[j][i] <= 9)
			{
				std::cout << " " << colsInfo[j][i] << "|";
			}
			else if (!colsInfo[j][i])
			{
				std::cout << "  " << "|";
			}
			else
			{
				std::cout << colsInfo[j][i] << "|";
			}
		}
		std::cout << std::endl;
	}

	int wholeBoardLength = (boardSize * 3) + longestRowSymbols + 1;

	for (size_t i = 0; i < wholeBoardLength; i++)
	{
		std::cout << "-";
	}
	std::cout << std::endl;


	for (size_t i = 0; i < boardSize; i++)
	{

		int indent = 0;
		for (int j = longestRowLength - 1; j >= 0; j--)
		{
			if (rowsInfo[i][j])
			{
				(rowsInfo[i][j] >= 10 && rowsInfo[i][j] <= 99 ? indent += 3 : indent += 2);
				std::cout << rowsInfo[i][j] << " ";
			}
			else
			{
				indent += 2;
				std::cout << "  ";
			}
		}

		for (size_t i = indent; i < longestRowSymbols; i++)
		{
			std::cout << " ";
		}

		std::cout << "|";

		for (size_t j = 0; j < boardSize; j++)
		{
			const char* symbolsToPrint = (currentBoard[i][j] == '0' ? "  " : (currentBoard[i][j] == '1' ? "XX" : "--"));
			std::cout << symbolsToPrint << "|";
		}
		std::cout << std::endl;
		for (size_t i = 0; i < wholeBoardLength; i++)
		{
			std::cout << "-";
		}
		std::cout << std::endl;
	}
}