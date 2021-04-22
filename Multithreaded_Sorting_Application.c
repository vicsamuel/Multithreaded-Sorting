#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>

// Number of barriers
#define BARRIERS 2

// Elements in total
int MAX;

// Number of threads
int T_MAX;

// Array of size MAX
int *A;

sem_t sem[BARRIERS];
pthread_mutex_t mut[BARRIERS];
int count[BARRIERS] = {0};

// Thread parameters
struct param
{
    int p_num;
    int p_low;
    int p_high;
};

// Barrier function using semaphores
void barrierWait(int bid, int sem_count)
{

    pthread_mutex_lock(&mut[sem_count]);
    count[sem_count] += 1;
    pthread_mutex_unlock(&mut[sem_count]);

    if (count[sem_count] == bid)
    {
        sem_post(&sem[sem_count]);
    }
    sem_wait(&sem[sem_count]);
    pthread_mutex_lock(&mut[sem_count]);
    count[sem_count] -= 1;
    pthread_mutex_unlock(&mut[sem_count]);
    if (count[sem_count] != 0)
    {
        sem_post(&sem[sem_count]);
    }
}

// Merge function for merging two parts
void mergeArray(int low, int mid, int high)
{

    // l_size is size of left part
    int l_size = mid - low + 1;
    // r_size is size of right part
    int r_size = high - mid;

    // Allocate array space
    int *left = malloc(l_size * sizeof(int));
    int *right = malloc(r_size * sizeof(int));

    int i, j, k;

    // Store values of the left array
    for (i = 0; i < l_size; i++)
    {
        left[i] = A[i + low];
    }

    // Store values of the right array
    for (i = 0; i < r_size; i++)
    {
        right[i] = A[i + mid + 1];
    }
    
    k = low;
    i = j = 0;

    // Merge both parts in ascending order
    while (i < l_size && j < r_size)
    {
        if (left[i] <= right[j])
        {
            A[k++] = left[i++];
        }
        else
        {
            A[k++] = right[j++];
        }
    }

    // Insert final left values
    while (i < l_size)
    {
        A[k++] = left[i++];
    }

    // Insert final right values
    while (j < r_size)
    {
        A[k++] = right[j++];
    }

    free(left);
    free(right);
}

// Simple merge sort function
void mergeSort(int low, int high)
{
    // Find middle of array
    int mid = low + (high - low) / 2;

    if (low < high)
    {
        // Calls first half
        mergeSort(low, mid);

        // Calls second half
        mergeSort(mid + 1, high);

        // Merge both halves
        mergeArray(low, mid, high);
    }
}

// Function for Multi-Threading
void *threadFunc(void *arg)
{
    struct param *param = arg;
    int low;
    int high;
    int n = 0;

    // Set the low and the high
    low = param->p_low;
    high = param->p_high;

    // Find the middle
    int mid = low + (high - low) / 2;

    if (low < high)
    {
        mergeSort(low, mid);
        mergeSort(mid + 1, high);
        mergeArray(low, mid, high);
    }

    // Barrier to wait for the threads to complete
    barrierWait(BARRIERS, n);
    n = (n + 1) % BARRIERS;

    return 0;
}

// Main function
int main(int argc, char **argv)
{

    // Create parameter structure
    struct param *param;

    // File pointer
    FILE *fp;
    // Character variable to write to file
    char ch;

    // Checks for correct amount of arguments
    if (argc != 4)
    {
        printf("Please only enter 4 arguments in the format: ./filename N M out.txt\n");
        return 0;
    }
    // Converts strings to integers
    else
    {
        T_MAX = atoi(argv[1]);
        MAX = atoi(argv[2]);
        //BARRIERS = T_MAX - 1;
    }

    // Opens file to write results to
    fp = fopen(argv[3], "w");
    if (fp == NULL)
    {
        printf("Error opening file.");
        exit(1);
    }

    // Allocate memory for array
    A = malloc(sizeof(int) * MAX);

    // Put random values in the array
    printf("Unsorted Array: ");
    fprintf(fp, "Unsorted Array: ");
    for (int i = 0; i < MAX; i++)
    {
        A[i] = rand() % 100;
        printf("%d ", A[i]);
        fprintf(fp, "%d ", A[i]);
    }
    printf("\n");

    pthread_mutex_init(&mut[BARRIERS], NULL);
    pthread_t threads[T_MAX];
    struct param p_array[T_MAX];

    int len = MAX / T_MAX;

    printf("\nThread Count: %d \n", T_MAX);
    printf("Max Number of Elements: %d \n", MAX);
    printf("Length of each Sub-Array: %d\n", len);

    int low = 0;

    for (int i = 0; i < T_MAX; i++, low += len)
    {
        param = &p_array[i];
        param->p_num = i;

        param->p_low = low;
        param->p_high = low + len - 1;
        if (i == (T_MAX - 1))
        {
            param->p_high = MAX - 1;
        }
    }
    printf("\n");

    // Create the threads
    for (int i = 0; i < T_MAX; i++)
    {
        param = &p_array[i];
        pthread_create(&threads[i], NULL, threadFunc, param);
    }

    // Join all the threads
    for (int i = 0; i < T_MAX; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the Sub-Arrays for each thread
    for (int i = 0; i < T_MAX; i++)
    {
        param = &p_array[i];
        printf("Sub-Array #%d: ", param->p_num + 1);
        for (int j = param->p_low; j <= param->p_high; ++j)
        {
            printf("%d ", A[j]);
        }

        printf("\n");
    }

    // Merging the final parts
    struct param *p_merge = &p_array[0];
    for (int i = 1; i < T_MAX; i++)
    {
        struct param *param = &p_array[i];
        mergeArray(p_merge->p_low, param->p_low - 1, param->p_high);
    }

    // Display final sorted array
    printf("\nSorted array: ");
    fprintf(fp, "\n\nSorted array: ");
    for (int i = 0; i < MAX; i++)
    {
        printf(" %d", A[i]);
        fprintf(fp, "%d ", A[i]);
    }
    printf("\n");

    //Destroy semaphores and mutexes
    for (int i = 0; i < BARRIERS; i++)
    {
        sem_destroy(&sem[i]);
    }

    pthread_mutex_destroy(&mut[BARRIERS]);

    fclose(fp);
    return 0;
}