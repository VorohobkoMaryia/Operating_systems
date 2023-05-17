#include "../Employee.h"
#include "Server_class.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <charconv>
#include <tchar.h>

#pragma warning (disable:4996)

/*
написать программу для управления доступом параллельных процессов к файлу по именованным каналам.

remember, no STD...
*/

int main(int argc, char* argv[])
{
	LPSTR file_name = new char[StandartSTRSize];
	printf("input name of binary file (it will be created): ");
	scanf_s("%s", file_name, StandartSTRSize - 1);

	char mode;
	printf("\'f\' to input from file and \'c\' to input with console: ");
	std::cin >> mode; // scanf is helpless here

	LPSTR inputName = nullptr;
	int studentEmount = -1;
	if (mode == file)
	{
		inputName = new char[StandartSTRSize];
		printf("input name of txt file (to read from): ");
		scanf_s("%s", inputName, StandartSTRSize - 1);
	}
	else
	{
		printf("input number of records in .bin file: ");
		scanf_s("%d", &studentEmount);
	}

	Server main_server;
	main_server.CreateDataBase(file_name, studentEmount, mode, inputName);

	int clientEmount = 0;
	printf("input number of client processes: ");
	scanf_s("%d", &clientEmount);

	if (!main_server.CreateClients(clientEmount))
	{
		printf("something went wrong :( \n");
		return 0;
	}

	main_server.Work();

	main_server.OutputDataBase();

	return 0;
}