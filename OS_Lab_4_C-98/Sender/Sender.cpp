#include "../Header.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

constexpr int StandartSTRSize = 21;
constexpr int SmallSleepTime = 10;

#pragma warning (disable:4996)

int main(int argc, char* argv[])
{
    FILE* file_bin;
    errno_t error_file;
    const char* file_name = argv[1];

    LPCSTR name_senders_ready = argv[2];
    LPCSTR name_continue_work = argv[3];
    LPCSTR name_end_work = argv[4];
    LPCSTR string_counter = argv[5];
    LPCSTR senders_counter = argv[6];

    printf("I'm Sender number %s, type below to sent messages\n", argv[7]);

    HANDLE this_ready = OpenEventA(EVENT_ALL_ACCESS, EVENT_MODIFY_STATE, name_senders_ready);
    HANDLE continue_work = OpenEventA(EVENT_ALL_ACCESS, EVENT_MODIFY_STATE, name_continue_work);
    HANDLE end_work = OpenEventA(EVENT_ALL_ACCESS, EVENT_MODIFY_STATE, name_end_work);
    HANDLE semaphore_string = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, SEMAPHORE_MODIFY_STATE, string_counter);
    HANDLE semaphore_senders = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, SEMAPHORE_MODIFY_STATE, senders_counter);

    if (this_ready == nullptr || semaphore_string == nullptr || semaphore_senders == nullptr || continue_work == nullptr || end_work == nullptr)
    {
        printf("Wrong HANDLEs of sync objects");
        return 0;
    }

    int action = 0;

    do
    {
        printf("<1> to input message, <other> to end work : \n");
        scanf_s("%d", &action);

        if (action == 1)
        {
            error_file = fopen_s(&file_bin, file_name, "ab");
            if (error_file != 0)
            {
                printf("Error opening file");
                return 0;
            }
            auto message = new char[StandartSTRSize];
            printf("Enter the message (maximum 20 symbols)\n");
            scanf_s("%s", message, StandartSTRSize);

            if (WaitForSingleObject(semaphore_string, SmallSleepTime) == WAIT_TIMEOUT)
            {
                delete[] message;
                printf("The file is full\n");
                SetEvent(this_ready);
                fclose(file_bin);
                WaitForSingleObject(continue_work, INFINITE);
                if (WaitForSingleObject(end_work, SmallSleepTime) == WAIT_TIMEOUT)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            else
            {
                ReleaseSemaphore(semaphore_senders, 1, nullptr);
                MyFunctions::sendMessage(file_bin, message);
                delete[] message;
                fclose(file_bin);
                continue;
            }
        }
        else if (action != 1)
        {
            printf("Process stopping...\n");

            SetEvent(this_ready);
            SetEvent(end_work);
            break;
        }
    } while (true);

    return 0;
}
