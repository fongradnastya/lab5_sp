/*!
 * \file main.c
 * \brief Файл для запуска основной логики программы
 */

#include <stdio.h>
#include<stdlib.h>
#include"matrix.c"

int main(){
    int size = InputSize();
    int** matrix = (int**) malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++)
    {
        matrix[i] = (int*)malloc(size * sizeof(int));
    }
    int res = InputMatrix(matrix, size);
    if(res == 1){
        matrixProcessing(matrix, size);
    }
    for(int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}
