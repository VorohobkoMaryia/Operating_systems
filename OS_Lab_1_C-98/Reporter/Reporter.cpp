#include <Windows.h>
#include <iostream>
#include "../Employee.h"
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable:4996)

int main(int argc, char* argv[])
{
	FILE* binary_input;
	FILE* report_out;

	errno_t error;

	error = fopen_s(&binary_input, argv[1], "rb");
	if (error != 0)
	{
		printf("Error opening file");
	}
	fseek(binary_input, 0, SEEK_SET);

	error = fopen_s(&report_out, argv[2], "w");
	if (error != 0)
	{
		printf("Error opening file");
		return 0;
	}

	int emount = 0;
	employee inputter;

	fread(&emount, sizeof(int), 1, binary_input);
	employee* array = new employee[emount];

	for (size_t i = 0; i < emount; i++)
	{
		inputter.input_file(binary_input);
		array[i] = inputter;
	}

	qsort(array, emount, sizeof(employee), compare_employee);

	double PayForHour = atof(argv[3]);

	for (size_t i = 0; i < emount; i++)
	{
		array[i].output_file_txt(report_out);
		double salary = array[i].hours * PayForHour;
		fprintf_s(report_out, "%f", salary);
		fprintf_s(report_out, "\n");
	}

	fclose(binary_input);
	fclose(report_out);

	return 0;
}