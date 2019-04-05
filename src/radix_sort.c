
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

void generate_number(int *arr, int n)
{
    int seed = 13516112 + 13516025;
    srand(seed);
    for (long i = 0; i < n; i++)
    {
        arr[i] = (int)rand();
    }
}

void count_unsorted(int *unsorted, int n_element, int counter_size, int exp, int *result)
{
    // Store count of occurrences in count[]
    int number_end_with;

    for (int i = 0; i < n_element; i++)
    {
        number_end_with = (unsorted[i] / exp) % counter_size;
        result[number_end_with]++;
    }
}

void countSort(int *unsorted, int *sorted, int n_element, int bit_block_size, int exp)
{

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int num_elements_per_proc = (n_element + world_size - 1) / world_size;

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_length;
    MPI_Get_processor_name(processor_name, &name_length);

    int counter_size = (int)pow((double)2, (double)bit_block_size);
    int count[counter_size]; // array of array(int)

    // Initialize count array
    for (int i = 0; i < counter_size; i++)
    {
        count[i] = 0;
    }

    /** =============================================== **/

    int *sub_unsorted = (int *)malloc(sizeof(int) * num_elements_per_proc);
    int sub_count[counter_size];

    for (int i = 0; i < counter_size; i++)
    {
        sub_count[i] = 0;
    }

    MPI_Scatter(unsorted,
                num_elements_per_proc,
                MPI_INT,
                sub_unsorted,
                num_elements_per_proc,
                MPI_INT,
                0,
                MPI_COMM_WORLD);

    // for each node
    count_unsorted(sub_unsorted, num_elements_per_proc, counter_size, exp, sub_count);

    MPI_Reduce(&sub_count,
               count,
               counter_size,
               MPI_INT,
               MPI_SUM,
               0,
               MPI_COMM_WORLD);

    free(sub_unsorted);

    MPI_Barrier(MPI_COMM_WORLD);

    /** =============================================== **/

    if (world_rank != 0)
    {
        return;
    }

    // Cummulative count
    for (int i = 1; i < counter_size; i++)
    {
        count[i] += count[i - 1];
    }

    // Build the output array
    for (int i = n_element - 1; i >= 0; i--)
    {
        sorted[count[(unsorted[i] / exp) % counter_size] - 1] = unsorted[i];
        count[(unsorted[i] / exp) % counter_size]--;
    }
}

void swap(int **a, int **b)
{
    int *temp = *a;
    *a = *b;
    *b = temp;
}

void radix_solve(int *unsorted, int *sorted, int n_element, int BIT_BLOCK_SIZE)
{
    int exp = 1;

    // Do counting sort for every digit.
    for (int i = 0; i < (32 / BIT_BLOCK_SIZE); i++)
    {
        exp *= pow(2, BIT_BLOCK_SIZE);

        countSort(unsorted, sorted, n_element, BIT_BLOCK_SIZE, exp);
        swap(&unsorted, &sorted);
    }
}

double get_elapsed_time(struct timespec start, struct timespec end)
{

    long seconds = (end.tv_sec - start.tv_sec);
    long nanosecs = (end.tv_nsec - start.tv_nsec);
    double microsecs = seconds * 1.0e6 + nanosecs * 1.0e-3;

    return microsecs;
}

void display_output(int *sorted, int head_count)
{
    for (int i = 0; i < head_count; i++)
    {
        printf("%d\n", sorted[i]);
    }
}

int main(int argc, char *argv[])
{
    int BIT_BLOCK_SIZE = 8; // pakai 4 core
    int NUM_OF_ELEMENT;
    int *unsorted;
    int *sorted;

    double elapsed_time = 0;
    double total_elapsed_time = 0;

    struct timespec start, end;

    int repeat = 10;

    if (argc <= 1)
    {
        return -1;
    }

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    NUM_OF_ELEMENT = atoi(argv[1]);
    unsorted = malloc(sizeof(int) * NUM_OF_ELEMENT);
    sorted = malloc(sizeof(int) * NUM_OF_ELEMENT);

    if (world_rank == 0)
    {
        printf("Sorting for %d, block: %d \n", NUM_OF_ELEMENT, BIT_BLOCK_SIZE);
    }

    for (int i = 0; i < repeat; i++)
    {
        if (world_rank == 0)
        {
            generate_number(unsorted, NUM_OF_ELEMENT);
        }

        // ignore CLOCK_MONOTONIC warning
        clock_gettime(CLOCK_MONOTONIC, &start);
        radix_solve(unsorted, sorted, NUM_OF_ELEMENT, BIT_BLOCK_SIZE);
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (world_rank == 0)
        {
            elapsed_time = get_elapsed_time(start, end);
            total_elapsed_time += elapsed_time;
            printf("Time elapsed (microsec) iter %d : %lg\n", i, elapsed_time);
        }
    }
    // printf("\nTime average elapsed (microsec) : %lg\n", total_elapsed_time / repeat);

    if (world_rank == 0)
    {
        swap(&unsorted, &sorted);
        // display_output(sorted, NUM_OF_ELEMENT);
    }

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}
