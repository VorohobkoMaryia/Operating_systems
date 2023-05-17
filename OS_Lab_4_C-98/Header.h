#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <charconv>

namespace MyFunctions
{
	void sendMessage(FILE* file, const char* message);

	void readMessage(FILE* file, char* readHere);

	bool all_zero(const int* threads, int emount);

	/*char* wchar_to_char(const wchar_t* pwchar);

	wchar_t* GetWC(const char* c);*/
}
