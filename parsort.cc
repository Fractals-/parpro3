/* parsort.cc
 * Assignment 3, Parallel Programming, LIACS
 *
 * Hanjo Boekhout - s1281348
 * 25-5-2016
 */

#include <stdlib.h>
#include <cstdio>
#include <cmath>
#include <climits>
#include "mpi.h"

#include "heapsort.h"

#define N 16000000000UL//200000UL//
#define BASE_SEED1 0x1234abcd
#define BASE_SEED2 0x10203040
#define BASE_SEED3 0x40e8c724
#define BASE_SEED4 0x79cbba1d
#define BASE_SEED5 0xac7bd459

const int comm_size = 12500;
const int output_interval = 1000000;

static int rank;
static int mpi_size;
size_t n;

// *************************************************************************************

/* Merge local sorted array with received array to the end of the local array
 * Parameters:
 *    my_array   - Pointer to the local array
 *    comm_array - Pointer to part of the 'source' array to merge with
 *    local      - Index indicating how much of the local array has already been merged
 *    filled     - Index indicating how much has been filled (sorted) already
 */
void mergeTwo( int *my_array, int *comm_array, size_t &local, size_t &filled ){
  int other = comm_size - 1;
  while ( other >= 0 && local != ULONG_MAX ) {
    if ( my_array[local] < comm_array[other] ) {
      my_array[filled] = comm_array[other];
      other--;
    }
    else {
      my_array[filled] = my_array[local];
      local--;
    }
    filled--;
  }
  while ( other >= 0 ) {
    my_array[filled] = comm_array[other];
    other--;
    filled--;
  }
}

// *************************************************************************************

/* Merge local sorted array with received array to the output
 * Parameters:
 *    my_array   - Pointer to the local array
 *    comm_array - Pointer to part of the 'source' array to merge with
 *    local      - Index indicating how much of the local array has already been merged
 *    filled     - Index indicating how much has been filled (sorted) already
 */
void mergeOutput( int *my_array, int *comm_array, size_t &local, size_t &filled ){
  int other = 0;
  while ( other < comm_size && local < n ) {
    if ( my_array[local] > comm_array[other] ) {
      if ( filled % output_interval == 0 )
        fprintf(stdout, "%lu, %d\n", filled, comm_array[other]);
      other++;
    }
    else {
      if ( filled % output_interval == 0 )
        fprintf(stdout, "%lu, %d\n", filled, my_array[local]);
      local++;
    }
    filled++;
  }
  while ( other < comm_size ) {
    if ( filled % output_interval == 0 )
      fprintf(stdout, "%lu, %d\n", filled, comm_array[other]);
    other++;
    filled++;
  }
}


// *************************************************************************************

/* Reduce the sorted arrays of all processors to one array through a 'tree structure'
 *  At each step:
 *    - send/recv 'comm_size' (sorted) integers, merge them with the local array
 *      to the end of the local array (backhalf was empty!)
 * Parameters:
 *    my_array - Pointer to the local sorted array
 */
void mergeSortProcessors( int *&my_array ){
  int step = 1; // Stores the current step size
  int nstep, mod_rank;
  MPI_Status status;

  size_t i, new_n, local, filled;
  int *comm_array = (int*) malloc(sizeof(int) * comm_size);

  // Perform stepwise reduction
  while ( step != mpi_size ) {
    nstep = 2 * step;
    mod_rank = rank % nstep;

    if ( mod_rank == 0 ) { // Receive
      if ( 2 * step != mpi_size ) { // If this is not the final merge
        new_n = 2 * n;
        local = n - 1;
        filled = new_n - 1;
        // Perform this in pieces as to have relatively small communication sizes
        for ( i = 1; i < n; i += comm_size ) {
          MPI_Recv(&comm_array[0], comm_size, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
          mergeTwo(my_array, comm_array, local, filled);
        }
        n = new_n;
      }
      else { // The final merge, thus output the result immediately
        local = 0;
        filled = 0;
        for ( i = 1; i < n; i += comm_size ) {
          MPI_Recv(&comm_array[0], comm_size, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
          mergeOutput(my_array, comm_array, local, filled);
        }
        // Output (if necessary) any remaining values
        while ( local < n ) {
          if ( filled % output_interval == 0 )
            fprintf(stdout, "%lu, %d\n", filled / output_interval, my_array[local]);
          local++;
          filled++;
        }
      }
    }
    else if ( mod_rank - step == 0 ) { // Send
      if ( 2 * step != mpi_size ) {
        // Send data in pieces as to have relatively small communication sizes
        for ( i = n - comm_size; i > 0; i -= comm_size )
          MPI_Send(&my_array[i], comm_size, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
        MPI_Send(&my_array[0], comm_size, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
      }
      else {
        for ( i = 0; i < n; i += comm_size )
          MPI_Send(&my_array[i], comm_size, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
      }
    }

    step = nstep;
  }
}

// *************************************************************************************


int main( int argc, char **argv ){
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  // Allocate the required memory space for each processor
  n = N / mpi_size;
  int step = 2;
  while ( step < mpi_size ) {
    if ( rank % step == 0 )
      n *= 2;
    step *= 2;
  }
  int *my_array = (int*) malloc(sizeof(int) * n);
  n = N / mpi_size;

  /* Initialize the random number generator for the given BASE_SEED
  * plus an offset for the MPI rank of the node, such that on every
  * node different numbers are generated.
  */
  if ( argc >= 2 ){
    switch ( atoi(argv[1]) ){
      case 1: 
        srand(BASE_SEED1 + rank);
        break;
      case 2:
        srand(BASE_SEED2 + rank);
        break;
      case 3:
        srand(BASE_SEED3 + rank);
        break;
      case 4:
        srand(BASE_SEED4 + rank);
        break;
      case 5:
        srand(BASE_SEED5 + rank);
        break;
      default:
        srand(BASE_SEED1 + rank);
        break;
    }
  }
  else
    srand(BASE_SEED1 + rank);

  /* Generate N pseudo-random integers in the interval [0, RAND_MAX] */
  for (size_t i = 0; i < n; i++)
    my_array[i] = rand();

  double start_time = MPI_Wtime();

  // Sort the array
  heapSort(my_array, n);

  if ( mpi_size != 1 ) // If we do not perform sequential execution
    mergeSortProcessors(my_array);
  else {
    for ( size_t i = 0; i < N; i += output_interval )
      fprintf(stdout, "%lu, %d\n", i, my_array[i]);
  }

  double elapsed_time = MPI_Wtime() - start_time;

  // Output runtime
  if ( rank == 0 ) {
    fprintf(stdout, "--------------------\n");
    fprintf(stdout, "Execution time: %f\n", elapsed_time);
  }

  free(my_array);
  MPI_Finalize();
  return 0;
}