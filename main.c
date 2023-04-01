/*!
 * \file main.c
 * \brief Файл для запуска основной логики программы
 */

#include <stdio.h>
#include<stdlib.h>
#include"matrix.c"
#include"main.h"


int main(){
    int size = InputSize();
    int** matrix = (int**) malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++)
    {
        matrix[i] = (int*)malloc(size * sizeof(int));
    }
    int res = InputMatrix(matrix, size);
    if(res == 1){
        replaceMinString(matrix, size);
        PrintMatrix(matrix, size, size);
        int** newMatrix = (int**) malloc(size * sizeof(int*));
        for(int i = 0; i < size; i++)
        {
            newMatrix[i] = (int*) malloc((size - 1) * sizeof(int));
        }
        deleteMaxColumn(matrix, newMatrix, size);
        PrintMatrix(newMatrix, size, size - 1);
        for(int i = 0; i < size; i++)
        {
            free(newMatrix[i]);
        }
        free(newMatrix);
    }
    for(int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}
