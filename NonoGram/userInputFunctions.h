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
* function for managing the input
*
*/

#pragma once

#include <iostream>
#include "basicFunctions.h"
#include "filesFunctions.h"

const int MaxAccountNameLength = 50;
const int MaxAccountPasswordLength = 50;
const int MaxRowLengthInput = 50;

bool isValidInputForLogin(int indexOfUsername, const char* password, const char* passwordToCompareWith)
{
	if (!password || !passwordToCompareWith)
		return false;

	if (indexOfUsername == -1)
	{
		std::cout << "Invalid username!" << std::endl;
		return false;
	}

	if (mystrcmp(password, passwordToCompareWith))
	{
		std::cout << "Invalid password!" << std::endl;
		return false;
	}

	return true;
}

bool isValidInputForRegister(int indexOfUsername,const char* username, const char* password)
{
	if (!password || !username)
		return false;

	if (indexOfUsername != -1)
	{
		std::cout << "This username is already in use!" << std::endl;
		return false;
	}

	if (mystrlen(username) < 5)
	{
		std::cout << "Please enter a username which is at least 5 characters long!" << std::endl;
		return false;
	}

	if (mystrlen(password) < 8)
	{
		std::cout << "Please enter a password which is at least 8 characters long!" << std::endl;
		return false;
	}

	return true;
}


bool isInputValid(char** accounts, int rows, const char* username, const char* password, bool doesUserHaveAnAccount)
{
	//in order for the input to be valid the username must be in the accounts and the next element by index next to the username must be the password 

	if (!username || !password || !accounts)
		return false;

	for (size_t i = 0; i < rows; i += 2)
	{
		if (!accounts[i])
			return false;
	}

	int indexOfUsername = -1;
	for (size_t i = 0; i < rows; i += 2)
	{
		if (!mystrcmp(username, accounts[i]))
		{
			indexOfUsername = i;
			break;
		}
	}

	if (doesUserHaveAnAccount)
	{
		return isValidInputForLogin(indexOfUsername, password, accounts[indexOfUsername + 1]);
	}
	else
	{
		return isValidInputForRegister(indexOfUsername, username, password);
	}
}

void userInput(char** accounts, int accountsFileRows, char* loggedInUsername, bool doesUserHaveAnAccount)
{
	if (!accounts)
		return;

	for (size_t i = 0; i < accountsFileRows; i += 2)
	{
		if (!accounts[i])
			return;
	}

	std::cout << (doesUserHaveAnAccount ? "Login" : "Register") << std::endl;

	//get the input of the user
	std::cout << "Username: ";
	char username[MaxAccountNameLength];
	std::cin.getline(username, MaxAccountNameLength);

	std::cout << "Password: ";
	char password[MaxAccountPasswordLength];
	std::cin.getline(password, MaxAccountPasswordLength);

	//check if the input contains in the accounts

	while (!isInputValid(accounts, accountsFileRows, username, password, doesUserHaveAnAccount))
	{
		std::cout << "Username: ";
		std::cin.getline(username, MaxAccountNameLength);

		std::cout << "Password: ";
		std::cin.getline(password, MaxAccountPasswordLength);
	}

	if (!doesUserHaveAnAccount)
	{
		//save the new account username and password
		char filename[] = "accounts.txt";
		saveUsernameAndPasswordInFile(filename, username, password);
	}

	mystrcpy(username, loggedInUsername);
}

bool areValidIndexes(int x, int y,char** currentBoard, int boardSize)
{
	if (!currentBoard)
		return false;

	for (size_t i = 0; i < boardSize; i++)
	{
		if (!currentBoard[i])
			return false;
	}

	if (x < 0 || x >= boardSize || y < 0 || y >= boardSize)
	{
		std::cout << "You have entered an invalid indexes! Please try again." << std::endl;
		return false;
	}

	if (currentBoard[x][y] != '0')
	{
		std::cout << "You have already filled this position! Enter new coordinates." << std::endl;
		return false;
	}

	return true;
}



void getInput(int& x, int& y, bool& toFill,char** currentBoard, int boardSize)
{
	if (!currentBoard)
		return;

	for (size_t i = 0; i < boardSize; i++)
	{
		if (!currentBoard[i])
			return;
	}

	std::cout << "Enter x:";
	char xInput[MaxRowLengthInput];
	std::cin.getline(xInput, MaxRowLengthInput);
	x = convertStrToSigned(xInput) - 1;

	std::cout << "Enter y:";
	char yInput[MaxRowLengthInput];
	std::cin.getline(yInput, MaxRowLengthInput);
	y = convertStrToSigned(yInput) - 1;

	std::cout << "Mark as (fill = '1') or (empty = '0'):";
	char mark[MaxRowLengthInput];
	std::cin.getline(mark, MaxRowLengthInput);
	int numToCast = convertStrToSigned(mark);


	while (!areValidIndexes(x, y, currentBoard, boardSize) || (numToCast != 0 && numToCast != 1))
	{
		std::cout << "Enter x:";
		std::cin.getline(xInput, MaxRowLengthInput);
		x = convertStrToSigned(xInput) - 1;
		std::cout << "Enter y:";
		std::cin.getline(yInput, MaxRowLengthInput);
		y = convertStrToSigned(yInput) - 1;
		std::cout << "Mark as (fill = '1') or (empty = '0'):";
		std::cin.getline(mark, MaxRowLengthInput);
		numToCast = convertStrToSigned(mark);
	}

	toFill = numToCast;
}

char getYesOrNoInput(const char* message)
{
	if (!message)
		return '\0';

	char answer[MaxRowLengthInput];
	std::cout << message << " Yes='y' No='n': ";
	std::cin.getline(answer, MaxRowLengthInput);

	while (mystrcmp(answer, "Y") && mystrcmp(answer, "N") && mystrcmp(answer, "y") && mystrcmp(answer, "n"))
	{
		if (mystrcmp(answer, ""))
		{
			std::cout << "Invalid answer! Try again: ";
		}
		std::cin.getline(answer, MaxRowLengthInput);
	}

	return (!mystrcmp(answer, "Y") || !mystrcmp(answer, "y")) ? 'y' : 'n';
}