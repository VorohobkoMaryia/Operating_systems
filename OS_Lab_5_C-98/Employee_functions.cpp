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

employee::employee(employee const& emp) : num(emp.num), hours(emp.hours)
{
	strcpy_s(name, _countof(name), emp.name);
}

void employee::input() // read from console
{
	printf("input number: ");
	scanf_s("%d", &num);
	printf("input name: ");
	scanf_s("%s", &name, name_len);
	printf("input hours: ");
	scanf_s("%lf", &hours);
}

void employee::output() // write to console
{
	printf_s("%d ", num);
	printf_s("%s ", &name);
	printf_s("%f\n", hours);
}

void employee::input_file_bin(FILE* file) // mode rb - binary read
{
	fread(this, sizeof(employee), 1, file);
}

void employee::output_file_bin(FILE* file) const // mode wb - binary write
{
	fwrite(this, sizeof(employee), 1, file);
}

void employee::input_file_txt(FILE* file)
{
	fscanf_s(file, "%d", &num);
	fscanf_s(file, "%s", &name, name_len);
	fscanf_s(file, "%lf", &hours);
}

void employee::output_file_txt(FILE* file) const
{
	fprintf_s(file, "%d ", num);
	fprintf_s(file, "%s ", &name);
	fprintf_s(file, "%f ", hours);
}

employee& employee::operator = (employee const& sourse)
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