#include <Windows.h>
#include <iostream>
#include "../Employee.h"
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <charconv>

#pragma warning (disable:4996)

/*
Написать консольную программу Main и две утилиты (служебные программы)
Creator и Reporter, которые выполняют следующие действия.
Утилита Creator создает бинарный файл, имя которого и количество записей в котором
получает через командную строку.

remember, no STD...
*/

char* wchar_to_char(const wchar_t* pwchar)
{
	// get the number of characters in the string.
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	// allocate a new block of memory size char (1 byte) instead of wide char (2 bytes)
	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		// convert to char (1 byte)
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}

int main(int argc, char* argv[])
{
	// Input for Creator

	int emount = 0;
	printf("input number of elements: ");
	scanf_s("%d", &emount);
	LPWSTR file_name = new wchar_t[40];
	printf("input name of binary file (it will be created): ");
	scanf_s("%ls", file_name, 41);
	
	// Creator work

	LPWSTR CreatorAllocation = new wchar_t[40];
	wcscpy(CreatorAllocation, L"Creator.exe");
	
	LPWSTR parameters = new wchar_t[40];
	parameters = wcscpy(parameters, file_name); //"{file_name}"
	parameters = wcscat(parameters, L" "); //"{file_name} "

	std::wstring s = std::to_wstring(emount);
	const wchar_t* nchar = s.c_str();
	parameters = wcscat(parameters, nchar); // "{file_name} {emount}"

	STARTUPINFO Creator_StartInf;
	PROCESS_INFORMATION Creator_PrInf;

	ZeroMemory(&Creator_StartInf, sizeof(STARTUPINFO));
	Creator_StartInf.cb = sizeof(STARTUPINFO);

	LPWSTR destination = new wchar_t[80];
	wcscpy(destination, CreatorAllocation);
	wcscat(destination, L" ");
	wcscat(destination, parameters);

	// создаем Creator
	if (!CreateProcess(nullptr, destination, nullptr, nullptr, FALSE,
		CREATE_NEW_CONSOLE, nullptr, nullptr, &Creator_StartInf, &Creator_PrInf))
	{
		_cputs("The Creator is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	_cputs("The Creator is runned.\n");
	// ждем завершения Creator
	WaitForSingleObject(Creator_PrInf.hProcess, INFINITE);
	// закрываем дескрипторы процесса Creator
	CloseHandle(Creator_PrInf.hThread);
	CloseHandle(Creator_PrInf.hProcess);

	_cputs("The Creator is Finished.\n");

	// Вывод бинарника на консоль

	errno_t error;
	FILE* file;

	error = fopen_s(&file, wchar_to_char(file_name), "rb");
	if (error != 0)
	{
		printf("Error opening file");
		return 0;
	}
	fseek(file, 0, SEEK_SET);

	employee inputter;

	fread(&emount, sizeof(int), 1, file);

	for (size_t i = 0; i < emount; i++)
	{
		inputter.input_file(file);
		inputter.output(); 
		printf("\n");
	}

	fclose(file);

	// Конец вывода бинарника

	// Input for Reporter

	LPWSTR report_name = new wchar_t[40];
	printf("input name of report file (it will be created): ");
	scanf_s("%ls", report_name, 31);

	double PayForHour = 0;
	printf("input pay for hour: ");
	scanf_s("%lf", &PayForHour);

	// Reporter work

	LPWSTR ReporterAllocation = new wchar_t[40];
	wcscpy (ReporterAllocation, L"Reporter.exe");

	LPWSTR rep_parameters = new wchar_t[40];
	wcscpy(rep_parameters, file_name);	//"{file_name}"
	wcscat(rep_parameters, L" ");		//"{file_name} "
	wcscat(rep_parameters, report_name);	//"{file_name} {report_name}"
	wcscat(rep_parameters, L" ");			//"{file_name} {report_name} "

	std::wstring s_rep = std::to_wstring(PayForHour);
	const wchar_t* p_char = s_rep.c_str();
	wcscat(rep_parameters, p_char); //"{file_name} {report_name} {PayForHour}"

	STARTUPINFO Reporter_StartInf;
	PROCESS_INFORMATION Reporter_PrInf;

	ZeroMemory(&Reporter_StartInf, sizeof(STARTUPINFO));
	Reporter_StartInf.cb = sizeof(STARTUPINFO);

	LPWSTR destination_rep = new wchar_t[80];
	wcscpy(destination_rep, ReporterAllocation);
	wcscat(destination_rep, L" ");
	wcscat(destination_rep, rep_parameters);

	// создаем Reporter
	if (!CreateProcess(NULL, destination_rep, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &Reporter_StartInf, &Reporter_PrInf))
	{
		_cputs("The Reporter is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	_cputs("The Reporter is runned.\n");
	// ждем завершения Reporte
	WaitForSingleObject(Reporter_PrInf.hProcess, INFINITE);
	// закрываем дескрипторы процесса Reporter
	CloseHandle(Reporter_PrInf.hThread);
	CloseHandle(Reporter_PrInf.hProcess);

	_cputs("The Reporter is Finished.\n");

	// вывод {report_name} txt файла

	FILE* report_in;

	error = fopen_s(&report_in, wchar_to_char(report_name), "r");
	if (error != 0)
	{
		printf("Error opening file");
		return 0;
	}
	fseek(report_in, 0, SEEK_SET);

	employee inputter_rep;

	for (size_t i = 0; i < emount; i++)
	{
		inputter_rep.input_file_txt(report_in);
		double salary = 0;
		fscanf_s(report_in, "%lf", &salary);
		
		inputter_rep.output();
		printf("%f", salary);
		printf("\n");
	}

	fclose(report_in);

	// конец вывода {report_name} txt файла

	return 0;
}