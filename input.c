/*!
 * \file input.c
 * \brief Основные функции для считывания данных из консоли
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"input.h"

char* GetString()
{
    int len = 0;
    int capacity = 1;
    char *s = (char*) malloc(sizeof(char)); // динамическая пустая строка
    char c = getchar(); // символ для чтения данных
    while (c != '\n') 
    {
        s[(len)++] = c; // заносим в строку новый символ
        if (len >= capacity) 
        {
            capacity *= 2; // увеличиваем ёмкость строки в два раза
            s = (char*) realloc(s, capacity * sizeof(char)); // создаём новую строку с увеличенной ёмкостью
        }
        c = getchar(); // считываем следующий символ
    }
    s[len] = '\0';
    return s;
}

int InputInt(int* val)
{
    
    char* str = GetString();
    for(int i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]) && !(i == 0 && str[0] == '-')) return 0;
    }
    *val = atoi(str);
    if(*val > 0){
        return 1;
    }
    return -1;
}

int InputLong(long* val)
{
    char* str = GetString();
    for(int i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]) && !(i == 0 && str[0] == '-')) return 0;
    }
    *val = atoll(str);
    return 1;
}

