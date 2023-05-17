#include "../Employee.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

constexpr int StandartSTRSize = 21;
constexpr int SmallSleepTime = 40;
constexpr int ConnectTime = 250;
constexpr char read = 'r';
constexpr char modify = 'm';
constexpr char end = 'e';

class Client
{
	HANDLE pipe_to_server;
	HANDLE send_data;
	DWORD some_buffer;

public:
	Client();

	bool Create(LPCSTR _pipe, LPCSTR _answer);

	void Work();

	void ActionRead();

	void ActionModify();

	void ActionEnd();

	~Client();
};