#include "../Header.h"
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

constexpr int StandartSTRSize = 50;
constexpr int BigSTRSize = 150;
constexpr int MaxSynchoTime = 50;
constexpr int SmallSleepTime = 10;
constexpr int EndTime = 250;


/*
Написать программу для передачи сообщений между процессами через общий файл. 
Программа включает один процесс Receiver и несколько процессов Sender. 
Процессы Sender посылают сообщения процессу Receiver

remember, no STD...
*/

int main(int argc, char* argv[])
{
	errno_t error_file;
	FILE* file_bin;
	// Input for Creator

	LPSTR file_name = new char[StandartSTRSize];
	printf("input name of binary file (it will be created): ");
	scanf_s("%s", file_name, StandartSTRSize);

	error_file = fopen_s(&file_bin, file_name, "wb");
	if (error_file != 0)
	{
		printf("Error opening file");
		return 0;
	}
	fclose(file_bin); // just file creation (and check of input)

	int stringsEmount = 0;
	printf("input number of strings in .bin file: ");
	scanf_s("%d", &stringsEmount);

	int sendersEmount = 0;
	printf("input number of Sender processes: ");
	scanf_s("%d", &sendersEmount);

	HANDLE* senders_ready = new HANDLE[sendersEmount];
	HANDLE* continue_work = new HANDLE[sendersEmount];
	HANDLE* end_work = new HANDLE[sendersEmount];
	HANDLE string_counter;
	HANDLE senders_counter;

	LPSTR* names_senders_ready = new LPSTR[sendersEmount];
	LPSTR* names_continue_work = new LPSTR[sendersEmount];
	LPSTR* names_end_work = new LPSTR[sendersEmount];

	// Events initialization 
	for (int i = 0; i < sendersEmount; i++)
	{
		LPSTR senderNum = new char[StandartSTRSize];
		itoa(i + 1, senderNum, 10);

		names_senders_ready[i] = new char[StandartSTRSize];
		strcpy(names_senders_ready[i], "sender_ready_");
		strcat(names_senders_ready[i], senderNum);
		names_continue_work[i] = new char[StandartSTRSize];
		strcpy(names_continue_work[i], "continue_work_");
		strcat(names_continue_work[i], senderNum);
		names_end_work[i] = new char[StandartSTRSize];
		strcpy(names_end_work[i], "end_work_");
		strcat(names_end_work[i], senderNum);

		senders_ready[i] = CreateEventA(nullptr, FALSE, FALSE, names_senders_ready[i]);
		continue_work[i] = CreateEventA(nullptr, FALSE, FALSE, names_continue_work[i]);
		end_work[i] = CreateEventA(nullptr, FALSE, FALSE, names_end_work[i]);
	} 
	char name_str_counter[] = "string_counter";
	char name_sender_counter[] = "senders_counter";
	string_counter = CreateSemaphoreA(nullptr, 0, stringsEmount, name_str_counter);
	senders_counter = CreateSemaphoreA(nullptr, 0, sendersEmount, name_sender_counter);

	if (senders_counter == nullptr || string_counter == nullptr)
	{
		printf("Wrong HANDLEs of sync objects");
		// Чистка
		{
			delete[] names_end_work;
			delete[] names_continue_work;
			delete[] names_senders_ready;

			delete[] end_work;
			delete[] continue_work;
			delete[] senders_ready;
		}
		return 0;
	}
	for (int i = 0; i < sendersEmount; i++)
	{
		if (senders_ready[i] == nullptr || continue_work[i] == nullptr || end_work[i] == nullptr)
		{
			printf("Wrong HANDLEs of sync objects");
			// Чистка
			{
				delete[] names_end_work;
				delete[] names_continue_work;
				delete[] names_senders_ready;

				delete[] end_work;
				delete[] continue_work;
				delete[] senders_ready;
			}
			return 0;
		}
	}

	ReleaseSemaphore(string_counter, stringsEmount, nullptr);

	// Senders work
	LPSTR SenderAllocation = new char[StandartSTRSize];
	strcpy(SenderAllocation, "Sender.exe");

	LPSTR* data = new LPSTR[sendersEmount];
	for (int i = 0; i < sendersEmount; i++)
	{
		LPSTR senderNum = new char[StandartSTRSize];
		itoa(i + 1, senderNum, 10);

		data[i] = new char[BigSTRSize];
		strcpy(data[i], SenderAllocation); // argv[1]
		strcat(data[i], " ");
		strcat(data[i], file_name); // argv[1]
		strcat(data[i], " ");
		strcat(data[i], names_senders_ready[i]); // argv[2]
		strcat(data[i], " ");
		strcat(data[i], names_continue_work[i]); // argv[3]
		strcat(data[i], " ");
		strcat(data[i], names_end_work[i]); // argv[4]
		strcat(data[i], " ");
		strcat(data[i], name_str_counter); // argv[5]
		strcat(data[i], " ");
		strcat(data[i], name_sender_counter); // argv[6]
		strcat(data[i], " ");
		strcat(data[i], senderNum); // argv[7]
	}

	_STARTUPINFOA* senders_StartInf = new _STARTUPINFOA[sendersEmount];
	_PROCESS_INFORMATION* senders_PrInf = new _PROCESS_INFORMATION[sendersEmount];

	for(int i=0; i<sendersEmount; i++)
	{
		ZeroMemory(&senders_StartInf[i], sizeof(_STARTUPINFOW));
		senders_StartInf[i].cb = sizeof(_STARTUPINFOW);
	}	
	
	// создаем Senders
	for (int i = 0; i < sendersEmount; i++)
	{
		if (!CreateProcessA(nullptr, data[i], nullptr, nullptr, FALSE,
			CREATE_NEW_CONSOLE, nullptr, nullptr, &senders_StartInf[i], &senders_PrInf[i]))
		{
			printf("The Sender %d is not created. Process stopping...\n", (i + 1));
			// Чистка
			{
				delete[] names_end_work;
				delete[] names_continue_work;
				delete[] names_senders_ready;

				delete[] end_work;
				delete[] continue_work;
				delete[] senders_ready;

				delete[] data;

				delete[] senders_StartInf;
				delete[] senders_PrInf;
			}// Чистка
			return 0;
		}
		else
		{
			printf("The Sender %d is runned.\n", (i + 1));
		}
	}

	int action = 0;
	int* end_of_threads = new int[sendersEmount];
	for (int i = 0; i < sendersEmount; ++i)
	{
		end_of_threads[i] = 1; // proof of thread not closed (needed further)
	}

	while(!MyFunctions::all_zero(end_of_threads, sendersEmount))
	{
		for (int i = 0; i < sendersEmount; ++i)
		{
			if (end_of_threads[i] == 1)
			{
				WaitForSingleObject(senders_ready[i], INFINITE);
				if (WaitForSingleObject(end_work[i], SmallSleepTime) != WAIT_TIMEOUT)
				{
					end_of_threads[i] = 0;
					CloseHandle(senders_PrInf[i].hThread);
					CloseHandle(senders_PrInf[i].hProcess);
				}
			}
		}

		printf("<1> to read messages, <other> to end work : \n");
		scanf_s("%d", &action);

		if (action == 1)
		{
			fopen_s(&file_bin, file_name, "rb");

			char* readed_message = new char[StandartSTRSize];
			printf("Messages: \n");

			while(WaitForSingleObject(senders_counter, SmallSleepTime) != WAIT_TIMEOUT)
			{
				MyFunctions::readMessage(file_bin, readed_message);
				printf("%s\n", readed_message);
			}

			delete[] readed_message;
			fclose(file_bin);

			ReleaseSemaphore(string_counter, stringsEmount, nullptr);
			for (int i = 0; i < sendersEmount; i++)
			{
				SetEvent(continue_work[i]);
			}
		}

		if (action != 1)
		{
			printf("There are no more working senders, process stopping...");
			for (int i = 0; i < sendersEmount; i++)
			{
				if (end_of_threads[i] == 1)
				{
					SetEvent(continue_work[i]);
					Sleep(SmallSleepTime);
					SetEvent(end_work[i]);
					Sleep(SmallSleepTime);
					CloseHandle(senders_PrInf[i].hThread);
					CloseHandle(senders_PrInf[i].hProcess);
				}
			}			
			break;
		}

		fopen_s(&file_bin, file_name, "wb");
		fclose(file_bin);

		printf("Now waiting for messages\n");
		Sleep(EndTime);
	}
	// Чистка
	{
		delete[] names_end_work;
		delete[] names_continue_work;
		delete[] names_senders_ready;

		delete[] end_work;
		delete[] continue_work;
		delete[] senders_ready;

		delete[] data;

		delete[] senders_StartInf;
		delete[] senders_PrInf;
	}
	return 0;
}