#include "../Employee.h"
#include "Client_class.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable:4996)

int main(int argc, char* argv[])
{
    Client client;

    printf("I am client number %s \n", argv[1]);

    if (!client.Create(argv[2], argv[3]))
    {
        printf("connection went wrong :( \n");

        char c;
        printf("Enter any char to exit program");
        scanf_s("%c", &c);

        return 0;
    } 

    client.Work();

    return 0;
}
