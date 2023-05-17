#include "../Employee.h"
#include "Client_class.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

Client::Client(): some_buffer(0) {}

Client::~Client() 
{ 
	CloseHandle(pipe_to_server); 
	CloseHandle(send_data); 
}

bool Client::Create(LPCSTR _pipe, LPCSTR _answer)
{
	if (!WaitNamedPipeA(_pipe, ConnectTime))
	{
		return false;
	}

	pipe_to_server = CreateFileA(_pipe, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
		OPEN_EXISTING, NULL, nullptr);

	send_data = OpenEventA(EVENT_ALL_ACCESS, EVENT_MODIFY_STATE, _answer);

	if (pipe_to_server == INVALID_HANDLE_VALUE || send_data == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	return true;
}

void Client::Work()
{
	char action = '0';

	do
	{
		printf("\'r\' to read record, \'m\' to modify record, <other> to end work: ");
		std::cin >> action;

		if (action == read)
		{
			ActionRead();
		}
		else if (action == modify)
		{
			ActionModify();
		}
	} while (action == read || action == modify);

	ActionEnd();
}

void Client::ActionRead()
{
	int number;

	printf("Enter emloyee's number to read record: ");
	scanf_s("%d", &number);
	WriteFile(pipe_to_server, &read, sizeof(read), &some_buffer, nullptr);
	WriteFile(pipe_to_server, &number, sizeof(number), &some_buffer, nullptr);

	SetEvent(send_data);
	printf("Request sended\n"); // to delete

	employee buf;
	ReadFile(pipe_to_server, &buf, sizeof(buf), &some_buffer, nullptr);

	if (buf.num != 0)
	{
		printf("Here is the record: \n");
		buf.output();
	}
	else
	{
		printf("What's a pity, no such record!\n");
	}

	char c;
	printf("Enter any char to complete operation ");
	std::cin >> c;
}

void Client::ActionModify()
{
	int number;

	printf("Enter emloyee's number to modify record: ");
	scanf_s("%d", &number);

	WriteFile(pipe_to_server, &modify, sizeof(modify), &some_buffer, nullptr);
	WriteFile(pipe_to_server, &number, sizeof(number), &some_buffer, nullptr);
	SetEvent(send_data);

	printf("Waiting for server responce... (try to complete other clients) \n");

	employee buf;
	ReadFile(pipe_to_server, &buf, sizeof(buf), &some_buffer, nullptr);

	if (buf.num != 0)
	{
		printf("Here is the record: \n");
		buf.output();

		printf("Enter a record to replace: \n");
		buf.input();

		WriteFile(pipe_to_server, &buf, sizeof(buf), &some_buffer, nullptr);
		SetEvent(send_data);
	}
	else
	{
		printf("What's a pity, no such record!\n");
	}

	char c;
	printf("Enter any char to complete operation ");
	std::cin >> c;
}

void Client::ActionEnd()
{
	char c;
	printf("Enter any char to exit program ");
	std::cin >> c;

	WriteFile(pipe_to_server, &end, sizeof(end), &some_buffer, nullptr);
	SetEvent(send_data);
}