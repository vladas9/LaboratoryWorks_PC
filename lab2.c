#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct timeval start_time, end_time;
int n = 50; 


int getRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
    
}

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        swap(&arr[i], &arr[min_index]);
    }
    
}

// Insertion Sort
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}


int partition(int arr[], int low, int high) {
    int pivot = arr[(low + high) / 2]; 
    int i = low - 1;
    int j = high + 1;

    while (1) {
        do {
            i++;
        } while (arr[i] < pivot);

        do {
            j--;
        } while (arr[j] > pivot);

        if (i >= j) {
            return j; // Return the pivot index
        }

        // Swap arr[i] and arr[j]
        swap(&arr[i], &arr[j]);
    }
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quicksort(arr, low, pivotIndex);
        quicksort(arr, pivotIndex + 1, high);
    }
}

int main() {
    int arr[n];
    int copy_arr[n];
    int line_count = 0;
    printf("Lenght of array: %i\n", n);
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        arr[i] = getRandom(1, 1000);
    }
    for(int i = 0; i<n; i++){
        copy_arr[i] = arr[i];
    }

    printf("\033[1;32mInitial Unsorted List:\033[1;0m\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
        if (line_count > 11){
            printf("\n");
            line_count = 0;
        }else{
            line_count++;
        }
    }
    printf("\n\n");

    gettimeofday(&start_time, NULL);

    bubbleSort(copy_arr,n);

    gettimeofday(&end_time, NULL);
    double elapsed_time_bubble = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("\033[1;32mSorted List with Bubble Sort\033[1;0m \033[1;31m(Time: %.6f seconds):\033[1;0m\n", elapsed_time_bubble);
    line_count = 0;
    for (int i = 0; i < n; i++) {
        printf("%d ", copy_arr[i]);
        if (line_count > 11){
            printf("\n");
            line_count = 0;
        }else{
            line_count++;
        }
    }
    printf("\n\n");

    for(int i = 0; i<n; i++){
        copy_arr[i] = arr[i];
    }

    gettimeofday(&start_time, NULL);

    selectionSort(copy_arr,n);

    gettimeofday(&end_time, NULL);
    elapsed_time_bubble = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("\033[1;32mSorted List with Section Sort\033[1;0m \033[1;31m(Time: %.6f seconds):\033[1;0m\n", elapsed_time_bubble);
    line_count = 0;
    for (int i = 0; i < n; i++) {
        printf("%d ", copy_arr[i]); 
        if (line_count > 11){
            printf("\n");
            line_count = 0;
        }else{
            line_count++;
        }
    }
    printf("\n\n");

    for(int i = 0; i<n; i++){
        copy_arr[i] = arr[i];
    }

    gettimeofday(&start_time, NULL);

    insertionSort(copy_arr,n);

    gettimeofday(&end_time, NULL);
    elapsed_time_bubble = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("\033[1;32mSorted List with Insertion Sort\033[1;0m \033[1;31m(Time: %.6f seconds):\033[1;0m\n", elapsed_time_bubble);
    line_count = 0;
    for (int i = 0; i < n; i++) {
        printf("%d ", copy_arr[i]);
        if (line_count > 11){
            printf("\n");
            line_count = 0;
        }else{
            line_count++;
        }
    }
    printf("\n\n");

    for(int i = 0; i<n; i++){
        copy_arr[i] = arr[i];
    }
    gettimeofday(&start_time, NULL);

    quicksort(copy_arr, 0, n-1);

    gettimeofday(&end_time, NULL);
    elapsed_time_bubble = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("\033[1;32mSorted List with Quick Sort\033[1;0m \033[1;31m(Time: %.6f seconds):\033[1;0m\n", elapsed_time_bubble);
    line_count = 0;
    for (int i = 0; i < n; i++) {
        printf("%d ", copy_arr[i]);
        if (line_count > 11){
            printf("\n");
            line_count = 0;
        }else{
            line_count++;
        }
    }
    printf("\n\n");
}
