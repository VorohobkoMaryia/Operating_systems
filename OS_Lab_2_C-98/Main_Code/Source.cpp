#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include "Header.h"

void Laboratory_2::Min_Max(std::vector<double>& Array, std::pair<double, double>& minmax)
{
    double min = Array[0];
    double max = Array[0];

    for (int i = 0; i < Array.size(); i++) {
        if (Array[i] < min)
        {
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(7));
            min = Array[i];
        }
        if (Array[i] > max)
        {
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(7));
            max = Array[i];
        }
    }

    std::cout << "Min Element: " << static_cast<int>(min) << std::endl;
    std::cout << "Max Element: " << static_cast<int>(max) << std::endl;

    minmax.first = min;
    minmax.second = max;

    return;
}

void Laboratory_2::Average(std::vector<double>& Array, double& average)
{
    double Summ = 0;
    for (int i = 0; i < Array.size(); ++i) {
        Summ += Array[i];
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(12));
    }

    average = Summ / static_cast<double>(Array.size());
    std::cout << "Average value of Array: " << average << std::endl;

    return;
}

void Laboratory_2::Change_Array(std::vector<double>& Array, std::pair<double, double> const& minmax, double const& average)
{
    for (int i = 0; i < Array.size(); i++) // Changes !All! min/max elements in Array
    {
        if ((abs(Array[i] - minmax.first) <= DBL_EPSILON)
            || (abs(Array[i] - minmax.second) <= DBL_EPSILON))
        {
            Array[i] = average;
        }
    }
}