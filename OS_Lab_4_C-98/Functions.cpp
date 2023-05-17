#include "Header.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <charconv>

#pragma warning (disable:4996)

using namespace MyFunctions;

void MyFunctions::sendMessage(FILE* file, const char* message)
{
	int length = strlen(message) + 1;
	fwrite(&length, sizeof(length), 1, file);
	for (int i = 0; i < length; i++)
	{
		fwrite(&message[i], sizeof(char), 1, file);
	}
}

void MyFunctions::readMessage(FILE* file, char* readHere)
{
	int length = 0;
	fread(&length, sizeof(length), 1, file);
	char* destination = new char[length];
	for (int i = 0; i < length; i++)
	{
		fread(&destination[i], sizeof(char), 1, file);
	}
	strcpy(readHere, destination);
	delete[] destination;
}

bool MyFunctions::all_zero(const int* threads, int emount)
{
	for (size_t i = 0; i < emount; i++)
	{
		if (threads[i] == 1)
		{
			return false;
		}
	}
	return true;
} // Function checks if all threads are stopped. If yes -- returns true. else -- false

/*char* MyFunctions::wchar_to_char(const wchar_t* pwchar)
{
	// get the number of characters in the string.
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	// allocate a new block of memory size char (1 byte) instead of wide char (2 bytes)
	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		// convert to char (1 byte)
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}

wchar_t* MyFunctions::GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}*/