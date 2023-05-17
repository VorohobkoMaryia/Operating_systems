#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <cstdio>
#include <condition_variable>

constexpr int WaitForEndTime = 200;
constexpr int MaxSynchoTime = 500;
constexpr int SleepTime = 5;

bool all_zero(const int* threads, int emount)
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

struct Array_Size_Num {
    int size;
    int* Arr;
};

Array_Size_Num information;

CRITICAL_SECTION struct_work;
HANDLE* MarkerN_wait;
HANDLE* MarkerN_console_resume;
HANDLE* MarkerN_console_stop;

DWORD WINAPI Marker(LPVOID number) 
{
    EnterCriticalSection(&struct_work); // Work is started -- protect Inf{Arr, size} from other threads 
    //1^
    int num = *(int*)(number); // Thread number
    int index; // oblivious
    int marked_count; // oblivious
    int* marked_array = new int[information.size]; // array to mark indexes of marked elements

    for (size_t i = 0; i < information.size; i++)
    {
        marked_array[i] = 0; 
    } // filling array of marks with "false" for each element

    srand(num); // Random generation seed = 2
    marked_count = 0;
    while (true) //3
    {
        index = rand() % information.size; // random index in {Inf.Arr} = 3.1 & 3.2

        if (information.Arr[index] == 0)  //3.3
        {
            Sleep(SleepTime); // 3.3.1
            information.Arr[index] = num; // the operation = 3.3.2
            marked_count++; // how much is marked
            marked_array[index] = 1; // marking indexes to clear them later
            Sleep(SleepTime); //3.3.3
        }
        else // 3.4
        {
            printf("Thread number: %d, marked_count: %d, unmarked index: %d \n", num, marked_count, index); //3.4.1
            SetEvent(MarkerN_wait[num - 1]); // Work is ended temporarily 
            LeaveCriticalSection(&struct_work); // Work is ended temporarily -- allow other threads to work = 3.4.2

            WaitForSingleObject(MarkerN_console_stop[num - 1], INFINITE); //3.4.3

            if (WAIT_OBJECT_0 == WaitForSingleObject(MarkerN_console_resume[num - 1], WaitForEndTime))
            {
                EnterCriticalSection(&struct_work); // Work is started -- protect Inf{Arr, size} from other threads
            }     
            else
            {
                EnterCriticalSection(&struct_work);
                break;
            }   
        }
    }

    for (size_t i = 0; i < information.size; i++)
    {
        if (marked_array[i] == 1)
        {
            information.Arr[i] = 0; //4.1
        }
    } // filling all marked elements with 0 before leaving

    LeaveCriticalSection(&struct_work); // Work is ended permanently -- allow other threads to work //4.2

    ExitThread(0);
}

int main() {
    int synchronization_time = 0;
    int thread_emount = 0;
    printf("Enter array size: ");
    scanf_s("%d", &information.size);  //1
    printf("Enter marker thread emount: ");
    scanf_s("%d", &thread_emount); //3

    if (thread_emount != 0)
    {
        synchronization_time = MaxSynchoTime / thread_emount;
    }
    
    information.Arr = new int[information.size];
    for (int i = 0; i < information.size; ++i)
    {
        information.Arr[i] = 0; //2
    }

    HANDLE* hThread = new HANDLE[thread_emount];

    MarkerN_wait = new HANDLE[thread_emount];
    MarkerN_console_resume = new HANDLE[thread_emount];
    MarkerN_console_stop = new HANDLE[thread_emount];
    // Memory allocation for Events and Threads

    for (int i = 0; i < thread_emount; i++)//4
    {
        MarkerN_wait[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        MarkerN_console_resume[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        MarkerN_console_stop[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    } // Events initialization 

    InitializeCriticalSection(&struct_work); //5

    int* end_of_threads = new int[thread_emount];
    for (int i = 0; i < thread_emount; ++i)
    {
        end_of_threads[i] = 1; // proof of thread not closed (needed further)
    }

    int* numbers = new int[thread_emount]; // Unique memory places for Marker() arguments

    for (int i = 0; i < thread_emount; i++)
    {
        numbers[i] = i + 1;
        hThread[i] = CreateThread(nullptr, 0, Marker, &numbers[i], CREATE_SUSPENDED, nullptr);
        ResumeThread(hThread[i]);

        Sleep(synchronization_time); 
    } 

    while (!all_zero(end_of_threads, thread_emount)) // 6.1
    {
        for (int i = 0; i < thread_emount; i++)
        {
            if (end_of_threads[i] != 0)
                WaitForSingleObject(MarkerN_wait[i], INFINITE);
        }

        int break_num;
        printf("Enter num of Marker() to break: "); //6.3
        do
        {
            scanf_s("%d", &break_num);
            if (end_of_threads[break_num - 1] == 0)
            {
                printf("This one is stopped, try another: ");
            }
        } while (end_of_threads[break_num - 1] == 0);
        break_num--; // input of index of Thread to stop 
        
        SetEvent(MarkerN_console_stop[break_num]); //6.4
        WaitForSingleObject(hThread[break_num], INFINITE); //6.5

        printf("Array: "); //6.2
        for (int i = 0; i < information.size; ++i)
        {
            printf("%d ", information.Arr[i]);
        }
        printf("\n");

        end_of_threads[break_num] = 0; // Proof, that thread (number {break_num}) is stopped

        for (size_t i = 0; i < thread_emount; i++)
        {
            if (end_of_threads[i] != 0)
            {
                SetEvent(MarkerN_console_stop[i]);
                SetEvent(MarkerN_console_resume[i]); //6.7

                Sleep(synchronization_time);
            }
        } 

        CloseHandle(hThread[break_num]);
        CloseHandle(MarkerN_wait[break_num]);
        CloseHandle(MarkerN_console_stop[break_num]);
        CloseHandle(MarkerN_console_resume[break_num]);
    }

    DeleteCriticalSection(&struct_work); //7

    delete[] numbers;
    delete[] end_of_threads;
    delete[] hThread;
    delete[] MarkerN_console_resume;
    delete[] MarkerN_console_stop;
    delete[] MarkerN_wait;
    delete[] information.Arr; // Memory cleaning

    return 0;
}