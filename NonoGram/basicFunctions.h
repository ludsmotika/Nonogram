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
* file with stl functions written by me
*
*/

#pragma once

int convertCharToDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	return -1;
}

unsigned convertStrToUnsigned(const char* str)
{
	if (!str)
		return 0;

	unsigned result = 0;
	while (*str)
	{
		int digit = convertCharToDigit(*str);
		if (digit == -1)
			return 0;
		(result *= 10) += digit;
		str++;
	}
	return result;
}

int convertStrToSigned(const char* str)
{
	if (!str)
		return 0;

	if (*str == '-')
		return -1 * convertStrToUnsigned(str + 1);
	else
		return convertStrToUnsigned(str);
}

int mystrcmp(const char* first, const char* second)
{
	if (!first || !second)
	{
		return 0; //some error value
	}

	while ((*first) && (*second) && ((*first) == (*second)))
	{
		first++;
		second++;
	}

	return (*first - *second);

}

void mystrcpy(const char* source, char* dest)
{
	if (!source || !dest)
		return;
	while (*source)
	{
		*dest = *source;
		dest++;
		source++;
	}
	*dest = '\0';
}

int mystrlen(const char* ptr)
{
	if (!ptr)
	{
		return 0;
	}

	int count = 0;

	while (*ptr)
	{
		count++;
		ptr++;
	}

	return count;
}

void deleteMatrix(char** matrix, int rows)
{
	for (int i = 0; i < rows; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}