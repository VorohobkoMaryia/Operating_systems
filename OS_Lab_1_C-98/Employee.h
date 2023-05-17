#pragma once
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR_FILE_OPEN -3

struct employee
{
	int num; // идентификационный номер сотрудника
	char name[10]; // имя сотрудника
	double hours; // количество отработанных часов

	employee();

	employee(int new_num, char new_name[], double new_hours);

	employee(employee& emp);

	~employee() {}

	void input();

	void output();

	void output_file(FILE* file);

	void input_file(FILE* file);

	void output_file_txt(FILE* file);

	void input_file_txt(FILE* file);

	employee& operator = (employee& sourse);
};

int compare_employee(const void* emp1, const void* emp2);