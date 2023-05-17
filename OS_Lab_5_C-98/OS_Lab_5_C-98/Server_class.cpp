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

Server::Server() : recordEmount(0), clientEmount(0) {}

Server::~Server()
{
	fclose(database);
	delete[] client_managers;
	delete[] file_name;
	delete[] record_access;
}

void Server::InputDataBaseConsole(int _emount)
{
	recordEmount = _emount;
	record_access = new int[recordEmount];

	employee buffer;

	for (int i = 0; i < recordEmount; i++)
	{
		buffer.input();
		buffer.output_file_bin(database);

		record_access[i] = -1;
	}
}

void Server::InputDataBaseFile(LPCSTR filename)
{
	FILE* input;
	if (fopen_s(&input, filename, "r") == 0)
	{
		employee buffer;

		int _emount = 0;
		fscanf_s(input, "%d", &_emount);

		recordEmount = _emount;
		record_access = new int[recordEmount];

		for (int i = 0; i < recordEmount; i++)
		{
			buffer.input_file_txt(input);
			buffer.output_file_bin(database);

			record_access[i] = -1;
		}

		fclose(input);
	}
}

bool Server::CreateDataBase(const char* _filename, int _emount, char mode, LPCSTR input_filename)
{
	file_name = new char[StandartSTRSize];
	strcpy(file_name, _filename);

	if (fopen_s(&database, _filename, "w+b"))
	{
		return false;
	}
	else
	{
		if (mode == console)
		{
			InputDataBaseConsole(_emount);
		}
		else if (mode == file)
		{
			InputDataBaseFile(input_filename);
		}
		else
		{
			return false;
		}

		OutputDataBase();

		return true;
	}
}

bool Server::CreateClients(int _emount)
{
	clientEmount = _emount;
	client_managers = new Manager[clientEmount];

	for (int i = 0; i < clientEmount; i++)
	{
		if (!client_managers[i].create(i + 1, this))
		{
			return false;
		}
	}

	return true;
}

int Server::find_by_number(int number)
{
	fseek(database, 0, SEEK_SET);
	employee buf;
	for (int i = 0; i < recordEmount; i++)
	{
		buf.input_file_bin(database);
		if (buf.num == number)
		{
			return i;
		}
	}

	return -1;
}

void Server::readRecord(int recordNum, employee& result)
{
	fseek(database, recordNum * sizeof(employee), SEEK_SET);
	fread(&result, sizeof(employee), 1, database);
}

void Server::overrideRecord(int recordNum, employee const& newRecord)
{
	fseek(database, recordNum * sizeof(employee), SEEK_SET);
	fwrite(&newRecord, sizeof(employee), 1, database);
	std::cout << "record " << recordNum << " modified" << std::endl;
}

void Server::Work()
{
	bool someoneWorks;
	do
	{
		someoneWorks = false;
		for (int i = 0; i < clientEmount; i++)
		{
			if (client_managers[i].is_working())
			{
				client_managers[i].Manage();
				someoneWorks = true;
			}
		}		

		Sleep(ServerCycleTime);
	} while (someoneWorks);
}

void Server::OutputDataBase()
{
	fseek(database, 0, SEEK_SET);
	printf("Database now: \n");
	employee buf;
	for (int i = 0; i < recordEmount; i++)
	{
		buf.input_file_bin(database);
		buf.output();
	}
}