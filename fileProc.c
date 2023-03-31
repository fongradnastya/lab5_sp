/*!
 * \file fileProc.c
 * \brief Основные функции для работы с векторным чтением и записью
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include"input.h"
#include"struct.h"
#include"fileProc.h"

#define STR_LEN 100

void saveCountry(Country country, struct iovec * iov [])
{
    (*iov)[0].iov_base = country.name;
    (*iov)[0].iov_len = STR_LEN; 
    (*iov)[1].iov_base = &country.area; 
    (*iov)[1].iov_len = sizeof(long); 
    (*iov)[2].iov_base = &country.population;
    (*iov)[2].iov_len = sizeof(long);
}

long inputPopulation()
{
    long population;
    int res = 0;
    while(res != 1)
    {
        printf("Please, enter the country population: ");
        res = InputLong(&population);
        if(res == 0)
        {
            printf("This value should be digit!\n");
        }
        else if(res == -1)
        {
            printf("This value should be positive!\n");
        }
    }
    return population;
}

long inputCountryArea()
{
    long area;
    int res = 0;
    while(res != 1)
    {
        printf("Please, enter the country area: ");
        res = InputLong(&area);
        if(res == 0)
        {
            printf("This value should be digit!\n");
        }
        else if(res == -1)
        {
            printf("This value should be positive!\n");
        }
    }
    return area;
}

Country getCountry()
{
    Country newCountry;
    printf("Please, enter the country name: ");
    scanf("%s", newCountry.name);
    getchar();
    newCountry.area = inputCountryArea();
    newCountry.population = inputPopulation();
    return newCountry;
}

void printChangeMenu()
{
    printf("___________________CHANGE_MENU___________________\n");
    printf("1 - to change a country name\n");
    printf("2 - to change a country area\n");
    printf("3 - to change a country population\n");
    printf("_________________________________________________\n");
}

void printCountry(Country country)
{
    printf("Country: %s - ", country.name);
    printf("population: %ld people, area: %ld units\n", country.population, country.area);
}

char* inputFileName()
{
    char* fileName;
    int res = 0;
    while(res != 1)
    {
        printf("Please, enter a file name: ");
        fileName = GetString();
        int len = strlen(fileName); // Получаем длину имени файла
        char *ext = fileName + len - 4; // Получаем расширение файла
        if (strcmp(ext, ".txt") == 0 || strcmp(ext, ".bin") == 0)
        {
            res = 1;
        }
        else{
            printf("Wrong file extension. Pleade, try again\n");
        }
    }
    return fileName;
}

void addRecord()
{
    Country country = getCountry();
    struct iovec iov[3]; // Массив структур iovec для векторной записи

    iov[0].iov_base = country.name;
    iov[0].iov_len = STR_LEN; 
    iov[1].iov_base = &country.area; 
    iov[1].iov_len = sizeof(long); 
    iov[2].iov_base = &country.population;
    iov[2].iov_len = sizeof(long); 
    
    char* fileName = inputFileName();
    int fileDescriptor = open(fileName, O_WRONLY | O_APPEND | O_CREAT, 0666);

    if (writev(fileDescriptor, iov, 3) == -1)
    {
        // Выполняем векторную запись из массива структур iovec в файл и
        // проверяем успешность операции
        perror("File write error");
        exit(1);
    }
    close(fileDescriptor); // Закрываем файл
    printf("The record has been added successfully.\n");
}

long countRecords(char* fileName)
{
    long count;
    int fileDescriptor = open(fileName, O_RDONLY);
    int recordLen = STR_LEN + sizeof(long) * 2; 
    count = lseek(fileDescriptor, 0, SEEK_END) / recordLen;
    close(fileDescriptor);
    return count;
}

int inputCountryNumber(int counter)
{
    int number;
    printf("There are %d countries in the file\n", counter);
    int res = 0;
    while(res != 1)
    {
        printf("Please, enter a country number: ");
        res = InputInt(&number);
        if(number > counter)
        {
            printf("This number is too big. Please, try again\n");
            res = 0;
        }
        else if ( res == -1)
        {
            printf("This value should be positive\n");
        }
    }
    printf("%d\n", number);
    return number;
}

void modifyRecord()
{
    struct iovec iov[3]; // Массив структур iovec для векторной записи
    Country country;
    iov[0].iov_base = country.name;
    iov[0].iov_len = STR_LEN; 
    iov[1].iov_base = &country.area; 
    iov[1].iov_len = sizeof(long); 
    iov[2].iov_base = &country.population;
    iov[2].iov_len = sizeof(long); 
    char* fileName = inputFileName();
    int fileDescriptor = open(fileName, O_RDWR, 0666); 
    int counter = countRecords(fileName);
    if(counter == 0)
    {
        printf("This file is empty. Nothing to modify");
    }
    else
    {
        int number = inputCountryNumber(counter);
        int pos = (number - 1) * (STR_LEN + sizeof(long) * 2);
        if (lseek(fileDescriptor, pos, SEEK_SET) == -1)
        {
            // Перемещаем указатель позиции в файле на начало нужной записи
            perror("Seaking file error");
            exit(1);
        }
        if (readv(fileDescriptor, iov, 3) == -1)
        {
            // Выполняем векторное чтение из файла в массив структур iovec
            // и проверяем успешность операции
            perror("Reading file error");
            exit(1);
        }
        close(fileDescriptor); // Закрываем файл
        printChangeMenu();
        int command;
        int res = 0;
        while(res == 0)
        {
            printf("Your command: ");
            res = InputInt(&command);
        }
        if(command == 1){
            printf("Please, enter the country name: ");
            scanf("%s", country.name);
            getchar();
            iov[0].iov_base = country.name; 
            iov[0].iov_len = 100;
        }
        else if(command == 2){
            long area = inputCountryArea();
            iov[1].iov_base = &area; 
            iov[1].iov_len = sizeof(long);
        }
        else if(command == 3){
            long population = inputPopulation();
                iov[2].iov_base = &population;
                iov[2].iov_len = sizeof(long);
        }
        else{
            printf("Wrong command\n");
        }
        fileDescriptor = open(fileName, O_WRONLY); // Открываем файл для записи
        if (fileDescriptor == -1)
        {
            perror("File opening error");
            exit(1);
        }
        if (lseek(fileDescriptor, pos, SEEK_SET) == -1)
        {
            perror("File moving error");
            exit(1);
        }
        if (writev(fileDescriptor, iov, 3) == -1)
        {
            perror("File write error");
            exit(1);
        }
        close(fileDescriptor); // Закрываем файл
        printf("Country added successfully\n");
    }
}

void deleteRecord()
{
    int number;
    long count;
    char buffer[STR_LEN + sizeof(long) * 2]; // Буфер для хранения данных 
    struct iovec iov[1]; // Массив структур iovec для векторной чтения/записи
    char* fileName = inputFileName();
    int fileDescriptor = open(fileName, O_RDWR, 0666); 
    int counter = countRecords(fileName);
    if(counter == 0)
    {
        printf("This file is empty. Nothing to modify");
    }
    else
    {
        count = countRecords(fileName);
        number = inputCountryNumber(count);
        for(int i=number; i<count; i++)
        {
            int pos = STR_LEN + sizeof(long) * 2;
            // Цикл по всем последующим записям
            if (lseek(fileDescriptor, i * pos, SEEK_SET) == -1)
            {
                // Перемещаем указатель позиции в файле на начало текущей записи
                perror("File moving error");
                exit(1);
            }
            iov[0].iov_base = buffer; // Указатель на данные в буфере
            iov[0].iov_len = pos; // Размер данных в буфере в байтах
            if(readv(fileDescriptor,iov,1)==-1)
            {
                // Выполняем векторное чтение из файла в массив структур iovec
                perror("File reading error");
                exit(1);
            }
            if(lseek(fileDescriptor,(i - 1) * pos, SEEK_SET) == -1)
            {
                // Перемещаем указатель позиции в файле на начало предыдущей
                perror("File moving error");
                exit(1);
            }
            if(writev(fileDescriptor, iov, 1) == -1)
            {
                // Выполняем векторную запись из массива структур iovec в файл
                perror("Write file error");
                exit(1);
            }
        }
        if(ftruncate(fileDescriptor,(count-1)*(100+sizeof(long)*2))==-1)
        {
            // Уменьшаем размер файла на одну запись и проверяем успешность операции
            perror("Changing file size error");
            exit(1);
        }
        close(fileDescriptor); // Закрываем файл
        printf("Country successfully deleted\n");
    }
}

void readRecord()
{
    Country country;
    struct iovec iov[3];
    char* fileName = inputFileName();
    int fileDescriptor = open(fileName, O_RDONLY);
    if (fileDescriptor == -1)
    {
        printf("No file with such name\n");
    }
    else
    {
        int count = countRecords(fileName);
        int number = inputCountryNumber(count);
        int pos = (STR_LEN + sizeof(long) * 2) * (number - 1);
        if (lseek(fileDescriptor, pos, SEEK_SET) == -1)
        {
            perror("File move error");
            exit(1);
        }
        iov[0].iov_base = country.name;
        iov[0].iov_len = STR_LEN; 
        iov[1].iov_base = &country.area; 
        iov[1].iov_len = sizeof(long); 
        iov[2].iov_base = &country.population;
        iov[2].iov_len = sizeof(long); 
        if (readv(fileDescriptor, iov, 3) == -1)
        {
            perror("Reading file error");
            exit(1);
        }
        close(fileDescriptor);
        printCountry(country); 
    }
}

void readAllRecords()
{
    Country country;
    struct iovec iov[3]; 
    int count = 0; // счетчик записей
    char* fileName = inputFileName();
    int fileDescriptor = open(fileName, O_RDONLY);
    if (fileDescriptor == -1)
    {
        printf("No file with such name\n");
    }
    else
    {
        while (1)
        { 
            iov[0].iov_base = country.name;
            iov[0].iov_len = STR_LEN; 
            iov[1].iov_base = &country.area; 
            iov[1].iov_len = sizeof(long); 
            iov[2].iov_base = &country.population;
            iov[2].iov_len = sizeof(long); 
            ssize_t bytesRead = readv(fileDescriptor, iov, 3);
            if (bytesRead == -1)
            { // Проверяем успешность операции чтения
                perror("Reading file error");
                exit(1);
            }
            if (bytesRead == 0)
            { 
                break;
            }
            count++; // Увеличиваем счетчик записей
            printCountry(country);
        }
        close(fileDescriptor);
        if (count == 0)
        {
            printf("This file is emptry\n");
        }
    }
}

void getTheMostPopulous()
{
    Country country;
    struct iovec iov[3];
    long maxDensity = 0;
    char* fileName = inputFileName();
    int fileDescriptor = open(fileName, O_RDONLY);
    int tmpCount = countRecords(fileName);
    if (tmpCount == 0)
    {
        printf("Файл пустой.\n");
    }
    else{
        while (1)
        { // бесконечный цикл
            iov[0].iov_base = country.name;
            iov[0].iov_len = STR_LEN; 
            iov[1].iov_base = &country.area; 
            iov[1].iov_len = sizeof(long); 
            iov[2].iov_base = &country.population;
            iov[2].iov_len = sizeof(long); 
            ssize_t bytesRead = readv(fileDescriptor, iov, 3); // выполнить векторное чтение из файла
            if (bytesRead == -1)
            {
                perror("Ошибка чтения из файла");
                exit(1);
            }
            if (bytesRead == 0)
            {
                break;
            }
            long density = country.population / country.area;
            if (maxDensity < density || maxDensity == 0)
            { // Если соотношение лучше или еще не определено
                maxDensity = density ; // Обновляем лучшее соотношение
            }
        }
        close(fileDescriptor);
        printf("The highest density is %ld\n ", maxDensity);
        printf("Countries:\n");
        fileDescriptor = open(fileName, O_RDONLY);
        while (1)
        { // бесконечный цикл
            saveCountry(country, &iov);
            ssize_t bytesRead = readv(fileDescriptor, iov, 3); // выполнить векторное чтение из файла
            if (bytesRead == -1)
            { // проверить успешность операции чтения
                perror("File reading error");
                exit(1);
            }
            if (bytesRead == 0)
            { // проверить конец файла
                break;
            }
            long density = country.population / country.area;
            if (density == maxDensity)
            { // Если соотношение совпадает с лучшим
                printCountry(country);
                // Выводим название предприятия
            }
        }
        close(fileDescriptor); // закрыть файл
    }
}

void getHighPopulate()
{
    Country country;
    struct iovec iov[3];
    long maxDensity = 0;
    char* fileName = inputFileName();
    int fileDescriptor = open(fileName, O_RDONLY);
    int tmpCount = countRecords(fileName);
    if (tmpCount == 0)
    {
        printf("This file is empty\n");
    }
    else{
        long minPopulation = inputPopulation();
        while (1)
        { // бесконечный цикл
            iov[0].iov_base = country.name;
            iov[0].iov_len = STR_LEN; 
            iov[1].iov_base = &country.area; 
            iov[1].iov_len = sizeof(long); 
            iov[2].iov_base = &country.population;
            iov[2].iov_len = sizeof(long); 
            ssize_t bytesRead = readv(fileDescriptor, iov, 3); // выполнить векторное чтение из файла
            if (bytesRead == -1)
            {
                perror("File reading error");
                exit(1);
            }
            if (bytesRead == 0)
            {
                break;
            }
            if (country.population >= minPopulation)
            {
                printCountry(country);
            }
        }
        close(fileDescriptor);
    }

}
