#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

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
    if(*val > 0)
    {
        return 1;
    }
    return -1;
}

void PrintMatrix(int** matrix, int sizeX, int sizeY)
{
    printf("Matrix:\n");
    for(int i = 0; i < sizeX; i++)
    {
        for(int j = 0; j < sizeY; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}


int InputMatrix(int** matrix, int size)
{
    printf("Please enter a %d:%d matrix\n", size, size);
    for(int i = 0; i < size; i++)
    {
        printf("%d string: ", i + 1);
        int num;
        for(int j = 0; j < size; j++)
        {
            int res = scanf("%d", &num);
            if(res == 1)
            {
                matrix[i][j] = num;
            }
            else return 0;
        }
    }
    return 1;
}

int InputSize()
{
    int size = 0;
    while(size < 1)
    {
        printf("Please, enter matrix size: ");
        int res = InputInt(&size);
        if(res == 0) printf("Incorrect number format. Please, try again.\n");
        else if(size < 1) printf("Size should be positive\n");
    }
    return size;
}

int replaceMinString(int** matrix, int size)
{
    int minX = 0;
    int minY = 0;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(matrix[i][j] < matrix[minX][minY])
            {
                minX = i;
                minY = j;
            }
        }
    }
    for(int i = 0; i < size; i++)
    {
        matrix[minX][i] = 0;
    }
    return minX;
}

int deleteMaxColumn(int** matrix, int** newMatrix, int size)
{
    int maxX = 0;
    int maxY = 0;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(matrix[i][j] > matrix[maxX][maxY])
            {
                maxX = i;
                maxY = j;
            }
        }
    }
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < (size - 1); j++)
        {
            if(j >= maxY)
            {
                newMatrix[i][j] = matrix[i][j + 1];
                
            }
            else{
                newMatrix[i][j] = matrix[i][j];
            }
        }
    }
    return maxY;
}

void matrixProcessing(int** matrix, int size)
{
    printf("----------------------------------\n");
    PrintMatrix(matrix, size, size);
    int minRow = replaceMinString(matrix, size);
    printf("Replased row id: %d\n", minRow);
    PrintMatrix(matrix, size, size);
  
    int** newMatrix = (int**) malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++)
    {
       newMatrix[i] = (int*) malloc((size - 1) * sizeof(int));
    }
    int maxColumn = deleteMaxColumn(matrix, newMatrix, size);
    printf("Deleted column id: %d\n", maxColumn);
    PrintMatrix(newMatrix, size, size - 1);
    for(int i = 0; i < size; i++)
    {
      free(newMatrix[i]);
    }
    free(newMatrix);
    printf("----------------------------------\n");
}

int** getMatrixFromString(char* str) 
{
    char* token = strtok(str, " ");
    int length = atoi(token);
    // Initialize a matrix
    int** matrix = (int**) malloc(length * sizeof(int*));
    for(int i = 0; i < length; i++)
    {
        matrix[i] = (int*)malloc(length * sizeof(int));
    }   
    // Loop through the string and split it by space
    for(int r = 0; r < length; r++)
    {
        for(int c = 0; c < length; c++)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                int num = atoi(token);
                matrix[r][c] = num;
            }
        }
    }
    matrixProcessing(matrix, length);
    for(int i = 0; i < length; i++)
    {
      free(matrix[i]);
    }
    free(matrix);
    return matrix;
}