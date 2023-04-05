#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"matrix.c"

// A function to get an array from a string of integer values, divided by space
int** get_array_from_string(char* str, int* size) {
    char* token = strtok(str, " ");
    int length = atoi(token);
    // Initialize an array pointer and a size variable
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
  // Return the array pointer
  return matrix;
}

// A function to print an array
void print_array(int* arr, int size) {
  printf("[");
  for (int i = 0; i < size; i++) {
    printf("%d", arr[i]);
    if (i < size - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

// A main function to test the code
int main() {
  // Define a test string
  char str[] = "3 10 20 30 40 50 60 70 80";

  // Get an array from the string
  int size;
  int** arr = get_array_from_string(str, &size);

  // Print the array
  PrintMatrix(arr, 3, 3);

  // Free the memory
  free(arr);

  return 0;
}