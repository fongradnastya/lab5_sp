/*!
 * \file main.c
 * \brief Файл для запуска основной логики программы
 */

#include <stdio.h>
#include<stdlib.h>
#include"fileProc.h"
#include"main.h"


void printMenu()
{
    printf("_____________________MENU________________________\n");
    printf("1 - to add a new record\n");
    printf("2 - to modify a record\n");
    printf("3 - to delete a record\n");
    printf("4 - to show all the records\n");
    printf("5 - to read a record\n");
    printf("6 - to get a list of the most populous countries\n");
    printf("7 - to sort countries by their population\n");
    printf("8 - to exit\n");
    printf("_________________________________________________\n");
}

int main(){
    int end = 0;
    while(end == 0)
    {
        printMenu();
        int command = 0;
        int res = 0;
        while(res == 0)
        {
            printf("Your command: ");
            res = InputInt(&command);
        }
        if(command == 1)
        {
            addRecord();
        }
        else if(command == 2)
        {
            modifyRecord();
        }
        else if(command == 3)
        {
            deleteRecord();
        }
        else if(command == 4)
        {
            readAllRecords();
        }
        else if(command == 5)
        {
            readRecord();
        }
        else if(command == 6)
        {
            getTheMostPopulous();
        }
        else if(command == 7)
        {
            getHighPopulate();
        }
        else if(command == 8)
        {
            end = 1;
        }
    }
    return 0;
}
