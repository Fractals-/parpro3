#include <stdlib.h>
#include <cstdio>
#include <cmath>
#include <climits>
#include "mpi.h"

#include "heapsort.h"
#include "blocksort.h"


#define N 200000UL//160000000000ULL
#define BASE_SEED1 0x1234abcd
#define BASE_SEED2 0x10203040
#define BASE_SEED3 0x40e8c724
#define BASE_SEED4 0x79cbba1d
#define BASE_SEED5 0xac7bd459

static int rank;
static int mpi_size;
size_t n;


// *************************************************************************************

/* Reduce the sorted arrays of all processors to one array through a 'tree structure'
 *  At each step:
 *    - allocate the required additional space
 *    - Until done: send/recv 'comm_size' (sorted) integers, merge them with the local array
 *                  to the end of the local array (backhalf was empty!)
 * Parameters:
 *    my_array - Pointer to the local sorted array
 */
void mergeSortProcessors( int *&my_array ){
  int step = 1; // Stores the current step size
  int nstep, mod_rank, comm_size = 10000, other;
  MPI_Status status;

  size_t i, new_n, local, filled;
  int *comm_array = (int*) malloc(sizeof(int) * comm_size);

  // Perform stepwise reduction
  while ( step != mpi_size ) {
    nstep = 2 * step;
    mod_rank = rank % nstep;

    if ( mod_rank == 0 ) {
      // Iterators used for merging two sorted lists from different processors
      new_n = 2 * n;
      local = n - 1;
      filled = new_n - 1;
      // Allocate the required additional space
      my_array = (int*) realloc(my_array, sizeof(int) * new_n);

      // Perform this in pieces as to have relatively small communication sizes
      for ( i = 1; i < n; i += comm_size ) {
        MPI_Recv(&comm_array[0], comm_size, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
        other = comm_size - 1;
        // Merge local sorted array with received array to the end of the local array
        while ( other >= 0 && local != ULONG_MAX ) {
          // fprintf(stdout, "%lu, %lu, %d: %d, %d\n", filled, local, other, my_array[local], comm_array[other]);
          if ( my_array[local] < comm_array[other] ){
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
      n = new_n;
    }
    else if ( mod_rank - step == 0 ) {
      // Send data in pieces as to have relatively small communication sizes
      for ( i = n - comm_size; i > 0; i -= comm_size )
        MPI_Send(&my_array[i], comm_size, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
      MPI_Send(&my_array[0], comm_size, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
    }

    step = nstep;
  }
}

// *************************************************************************************


int main( int argc, char **argv ){
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  n = N / mpi_size;
  int *my_array = (int*) malloc(sizeof(int) * n);

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

  // TODO: sort array (of this processor) (heapsort or mergesort?)
  heapSort(my_array, n);
  //blockSort(my_array, n);

  mergeSortProcessors(my_array);

  double elapsed_time = MPI_Wtime() - start_time;

  // Output part of sorted array
  if ( rank == 0 ) {
    for ( size_t i = 0; i < N; i += 10000 )
      fprintf(stdout, "%lu, %d\n", i, my_array[i]);
    fprintf(stdout, "--------------------\n");
    fprintf(stdout, "Execution time: %.4f\n", elapsed_time);
  }

  fprintf(stderr, "%d: Execution time: %.2f\n", rank, elapsed_time);

  free(my_array);
  MPI_Finalize();
}