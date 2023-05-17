#include "Employee.h"
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

employee::employee() : num(0), hours(0)
{
	strcpy_s(name, _countof(name), "");
}

employee::employee(int new_num, char new_name[], double new_hours) : num(0), hours(0)
{
	num = new_num;
	strcpy_s(name, _countof(name), new_name);
	hours = new_hours;
}

employee::employee(employee& emp) : num(emp.num), hours(emp.hours)
{
	strcpy_s(name, _countof(name), emp.name);
}

void employee::input() // read from console
{
	char name_scan[10];
	printf("input number: ");
	scanf_s("%d", &num);
	printf("input name: ");
	scanf_s("%s", &name_scan, 10);
	strcpy_s(name, _countof(name), name_scan);
	printf("input hours: ");
	scanf_s("%lf", &hours);
}

void employee::output() // write to console
{
	printf_s("%d", num);
	printf_s(" ");
	printf_s("%s", &name);
	printf_s(" ");
	printf_s("%f", hours);
	printf_s(" ");
}

void employee::input_file(FILE* file) // mode rb - binary write, mode r - text write
{
	fread(&num, sizeof(int), 1, file);

	int length = 0;
	fread(&length, sizeof(int), 1, file);
	for (int i = 0; i < length; i++)
	{
		fread(&name[i], sizeof(char), 1, file);
	}
	fread(&hours, sizeof(double), 1, file);
}

void employee::output_file(FILE* file) // mode wb - binary write, mode w - text write
{
	fwrite(&num, sizeof(int), 1, file);

	int length = strlen(name) + 1;
	fwrite(&length, sizeof(int), 1, file);
	for(int i = 0; i<length; i++)
	{
		fwrite(&name[i], sizeof(char), 1, file);
	}
	fwrite(&hours, sizeof(double), 1, file);
}

void employee::output_file_txt(FILE* file)
{
	fprintf_s(file, "%d", num);
	fprintf_s(file, " ");
	fprintf_s(file, "%s", &name);
	fprintf_s(file, "\n");
	fprintf_s(file, "%f", hours);
	fprintf_s(file, " ");
}

void employee::input_file_txt(FILE* file)
{
	fscanf_s(file, "%d", &num);
	fscanf_s(file, "%s", &name, 11);
	fscanf_s(file, "%lf", &hours);
}

employee& employee::operator = (employee& sourse)
{
	if (this == &sourse)
	{
		return *this;
	}
	else
	{
		this->num = sourse.num;
		this->hours = sourse.hours;
		strcpy_s(name, _countof(name), sourse.name);
		return *this;
	}
}

//comparator for qsort
int compare_employee(const void* emp1, const void* emp2)
{
	return (((const employee*)emp1)->num) - (((const employee*)emp2)->num);
}


