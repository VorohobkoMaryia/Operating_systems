#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <process.h>

/*
Написать программу для консольного процесса,
который состоит из трех потоков: main, min_max и average
*/

constexpr int MinMaxSleepTime = 7;
constexpr int AverageSleepTime = 12;

struct Data {
    int size;
    double* Arr;
    double min_elem;
    double max_elem;
    double average_elem;
};


DWORD WINAPI Min_Max(LPVOID data) {

    Data* Array = static_cast<Data*>(data);

    double min = Array->Arr[0];
    double max = Array->Arr[0];

    for (int i = 0; i < Array->size; i++) {
        if (Array->Arr[i] < min)
        {
            min = Array->Arr[i];
        }
        Sleep(MinMaxSleepTime);
        if (Array->Arr[i] > max)
        {
            max = Array->Arr[i];
        }
        Sleep(MinMaxSleepTime);
    }

    printf("Min Element: %d \n", static_cast<int>(min));
    printf("Max Element: %d \n", static_cast<int>(max));

    Array->min_elem = min;
    Array->max_elem = max;

    ExitThread(0);
}

DWORD WINAPI Average(LPVOID data) {

    Data* Array = static_cast<Data*>(data);

    double sum = 0;
    for (int i = 0; i < Array->size; ++i) {
        sum += Array->Arr[i];
        Sleep(AverageSleepTime);
    }

    double average = sum / static_cast<double>(Array->size);

    printf("Average value of Array: %.4f \n", average);

    Array->average_elem = average;

    ExitThread(0);
}

int main() {

    HANDLE hThread_MinMax;
    HANDLE hThread_Average;

    double* Array;
    int size;

    printf("Enter number of elements: ");
    scanf_s("%d", &size);
    Array = new double[size];

    printf("Enter elements: \n");
    for (int i = 0; i < size; i++)
    {
        printf("element %d: ", i);
        int elem;
        scanf_s("%d", &elem);
        Array[i] = static_cast<double>(elem);
    }

    Data data;
    data.Arr = Array;
    data.size = size;

    hThread_MinMax = CreateThread(nullptr, 0, Min_Max, &data, 0, nullptr);

    hThread_Average = CreateThread(nullptr, 0, Average, &data, 0, nullptr);
    WaitForSingleObject(hThread_Average, INFINITE);
    WaitForSingleObject(hThread_MinMax, INFINITE);
    

    printf("Array with all min/max elements to average: \n");

    int min_intVal = static_cast<int>(data.min_elem);
    int max_intVal = static_cast<int>(data.max_elem);

    for (int i = 0; i < size; i++) // Changes !All! min/max elements in Array
    {
        int elem_intVal = static_cast<int>(Array[i]);
        if (elem_intVal == min_intVal || elem_intVal == max_intVal)
        {
            Array[i] = data.average_elem;
            printf("{%.4f} ", Array[i]);
        }
        else
        {
            printf("%d ", static_cast<int>(Array[i]));
        }
    }
    CloseHandle(hThread_Average);
    CloseHandle(hThread_MinMax);

    delete[] data.Arr;

    return 0;

}