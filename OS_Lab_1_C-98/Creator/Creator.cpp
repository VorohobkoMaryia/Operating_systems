#include <Windows.h>
#include <iostream>
#include "../Employee.h"
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable:4996)

int main(int argc, char* argv[])
{
    employee inputter;
	FILE* file;

	errno_t error;

	error = fopen_s(&file, argv[1], "wb");
	if (error != 0)
	{
		printf("Error opening file");
		return 0;
	}

	int emount = atoi(argv[2]);

	fwrite(&emount, sizeof(int), 1, file);

	for (size_t i = 0; i < emount; i++) 
	{
		inputter.input();  // console read
		inputter.output_file(file);  // write in file (binary)
	}

	fclose(file);

	return 0;
}