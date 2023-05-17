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

Manager::Manager() : works(false), some_buffer(0) 
{
	pipe_name = new char[StandartSTRSize];
	event_name = new char[StandartSTRSize];
	command_line = new char[BigSTRSize];
}

Manager::~Manager()
{
	delete[] pipe_name;
	delete[] event_name;
	delete[] command_line;
	delete state;
	CloseHandle(client_PrInf.hThread);
	CloseHandle(client_PrInf.hProcess);
	CloseHandle(communication_pipe);
	CloseHandle(clientHasData);
}

bool Manager::create(int number, Server* st)
{
	works = true;
	serv = st;
	state = new StateBegin(this);

	clientNumber = number;
	LPSTR clientNum = new char[StandartSTRSize];
	itoa(clientNumber, clientNum, 10);

	strcpy(pipe_name, "\\\\.\\pipe\\communication_pipe_");
	strcat(pipe_name, clientNum);

	communication_pipe = CreateNamedPipeA(pipe_name,
		PIPE_ACCESS_DUPLEX, PIPE_WAIT,
		1, PipeBufferSize, PipeBufferSize, INFINITE, nullptr);

	strcpy(event_name, "answer_");
	strcat(event_name, clientNum);
	clientHasData = CreateEventA(nullptr, false, false, event_name);

	if (communication_pipe == INVALID_HANDLE_VALUE || clientHasData == INVALID_HANDLE_VALUE)
	{
		delete[] clientNum;
		return false;
	}

	char ClientAllocation[] = "Client.exe";

	strcpy(command_line, ClientAllocation); // argv[0]
	strcat(command_line, " ");
	strcat(command_line, clientNum); // argv[1]
	strcat(command_line, " ");
	strcat(command_line, pipe_name); // argv[2]
	strcat(command_line, " ");
	strcat(command_line, event_name); // argv[3]

	delete[] clientNum;

	ZeroMemory(&client_StartInf, sizeof(_STARTUPINFOW));
	client_StartInf.cb = sizeof(_STARTUPINFOW);

	if (!CreateProcessA(nullptr, command_line, nullptr, nullptr, FALSE,
		CREATE_NEW_CONSOLE, nullptr, nullptr, &client_StartInf, &client_PrInf))
	{
		return false;
	}

	if (ConnectNamedPipe(communication_pipe, nullptr))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Manager::is_working() const
{
	return works;
}

void Manager::Manage()
{
	state->action();
}

void StateBegin::action()
{
	if (WaitForSingleObject(owner->clientHasData, WaitForOneClientTime) != WAIT_TIMEOUT)
	{
		char todo;
		ReadFile(owner->communication_pipe, &todo, sizeof(char), &owner->some_buffer, nullptr);

		if (todo == read)
		{
			owner->state = new StateRead(owner);
			SetEvent(owner->clientHasData);
			owner->state->action();
			delete this;
		}
		else if(todo == modify)
		{
			owner->state = new StateModify(owner);
			SetEvent(owner->clientHasData);
			owner->state->action();
			delete this;
		}
		else
		{
			owner->state = new StateEnd(owner);
			SetEvent(owner->clientHasData);
			owner->state->action();
			delete this;
		}
	}
}

void StateRead::action()
{
	if (WaitForSingleObject(owner->clientHasData, WaitForOneClientTime) != WAIT_TIMEOUT)
	{
		int num;
		ReadFile(owner->communication_pipe, &num, sizeof(num), &owner->some_buffer, nullptr);

		employee buf;
		int index = owner->serv->find_by_number(num);

		if (index != -1)
		{
			owner->serv->readRecord(index, buf);
		}
		WriteFile(owner->communication_pipe, &buf, sizeof(buf), &owner->some_buffer, nullptr);

		owner->state = new StateBegin(owner);
		delete this;
	}
}

void StateModify::action()
{
	if (WaitForSingleObject(owner->clientHasData, WaitForOneClientTime) != WAIT_TIMEOUT)
	{
		int num;
		ReadFile(owner->communication_pipe, &num, sizeof(num), &owner->some_buffer, nullptr);

		int index = owner->serv->find_by_number(num);

		owner->state = new StateWaitForModifyAccess(owner, index);
		owner->state->action();
		delete this;
	}
}

void StateWaitForModifyAccess::action()
{
	if (buffer == -1)
	{
		employee buf;
		WriteFile(owner->communication_pipe, &buf, sizeof(buf), &owner->some_buffer, nullptr);
		owner->state = new StateBegin(owner);
		delete this;
	}
	else if (owner->serv->record_access[buffer] == -1)
	{
		owner->serv->record_access[buffer] = buffer;
		employee to_write;
		owner->serv->readRecord(buffer, to_write);

		WriteFile(owner->communication_pipe, &to_write, sizeof(to_write), &owner->some_buffer, nullptr);
		owner->state = new StateWrite(owner, buffer);
		owner->state->action();
		delete this;
	}
}

void StateWrite::action()
{
	if (WaitForSingleObject(owner->clientHasData, WaitForOneClientTime) != WAIT_TIMEOUT)
	{
		employee buf;
		ReadFile(owner->communication_pipe, &buf, sizeof(buf), &owner->some_buffer, nullptr);
		owner->serv->overrideRecord(buffer, buf);
		owner->serv->record_access[buffer] = -1;

		owner->state = new StateBegin(owner);
		delete this;
	}
}

void StateEnd::action()
{
	owner->works = false;
}