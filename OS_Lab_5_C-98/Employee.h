#pragma once
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

constexpr int name_len = 10;

struct employee
{
	int num;
	char name[name_len];
	double hours;

	employee();

	employee(int new_num, char new_name[], double new_hours);

	employee(employee const& emp);

	~employee() = default;

	void input();

	void output();

	void input_file_bin(FILE* file);

	void output_file_bin(FILE* file) const;

	void input_file_txt(FILE* file);

	void output_file_txt(FILE* file) const;

	employee& operator = (employee const& sourse);
};