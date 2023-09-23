#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int value = matrix[i][j];
            if (value < 25) {
                printf("\033[1;31m"); 
            } else if (value < 50) {
                printf("\033[1;33m"); 
            } else if (value < 75) {
                printf("\033[1;32m"); 
            } else {
                printf("\033[1;34m"); 
            }
            printf("%d ", value);
            printf("\033[0m"); 
        }
        printf("\n");
    }
}

void spiralSort(int** matrix, int rows, int cols) {
    int totalElements = rows * cols;
    int* elements = (int*)malloc(totalElements * sizeof(int));

    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            elements[index++] = matrix[i][j];
        }
    }

    for (int i = 0; i < totalElements - 1; i++) {
        for (int j = 0; j < totalElements - i - 1; j++) {
            if (elements[j] > elements[j + 1]) {
                int temp = elements[j];
                elements[j] = elements[j + 1];
                elements[j + 1] = temp;
            }
        }
    }

    int top = 0, bottom = rows - 1, left = 0, right = cols - 1;
    int direction = 0;
    index = 0;

    while (top <= bottom && left <= right) {
        if (direction == 0) {
            for (int i = left; i <= right; i++) {
                matrix[top][i] = elements[index++];
            }
            top++;
        } else if (direction == 1) {
            for (int i = top; i <= bottom; i++) {
                matrix[i][right] = elements[index++];
            }
            right--;
        } else if (direction == 2) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] = elements[index++];
            }
            bottom--;
        } else if (direction == 3) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] = elements[index++];
            }
            left++;
        }
        direction = (direction + 1) % 4;
    }

    free(elements);
}

int main() {
    int N, M;

    printf("Enter the number of rows (N > 10): ");
    scanf("%d", &N);
    printf("Enter the number of columns (M > 10): ");
    scanf("%d", &M);

    if (N <= 10 || M <= 10) {
        printf("N and M must be greater than 10.\n");
        return 1;
    }

    int** matrix = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        matrix[i] = (int*)malloc(M * sizeof(int));
    }
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            matrix[i][j] = rand() % 100; 
        }
    }

    printf("Original Matrix:\n");
    printMatrix(matrix, N, M);

    spiralSort(matrix, N, M);

    printf("\nSorted Matrix in Ascending Spiral Order:\n");
    printMatrix(matrix, N, M);

    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
