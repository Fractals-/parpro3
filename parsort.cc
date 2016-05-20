#include <stdlib.h>
#include <cstdio>
#include <cmath>
#include <ctime>
//#include "mpi.h"

#include "heapsort.h"
#include "blocksort.h"


#define N 51ULL//160000000000ULL
#define BASE_SEED1 0x1234abcd
#define BASE_SEED2 0x10203040
#define BASE_SEED3 0x40e8c724
#define BASE_SEED4 0x79cbba1d
#define BASE_SEED5 0xac7bd459

/* Code taken from the GLIBC manual.
 *
 * Subtract the ‘struct timespec’ values X and Y,
 * storing the result in RESULT.
 * Return 1 if the difference is negative, otherwise 0.
 */
static int
timespec_subtract (struct timespec *result,
                   struct timespec *x,
                   struct timespec *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_nsec < y->tv_nsec) {
    int nsec = (y->tv_nsec - x->tv_nsec) / 1000000000 + 1;
    y->tv_nsec -= 1000000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_nsec - y->tv_nsec > 1000000000) {
    int nsec = (x->tv_nsec - y->tv_nsec) / 1000000000;
    y->tv_nsec += 1000000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_nsec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_nsec = x->tv_nsec - y->tv_nsec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}


static int rank;
static int mpi_size;
size_t n;



// *************************************************************************************

void mergeSortComponents( int *my_array ){

  // TODO: implement mergeSort for merging processor sorted arrays
}

// *************************************************************************************


int main( int argc, char **argv ){
  ///////////////////////////////////////////////////////////////
  /*
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  */
  rank = 0;
  mpi_size = 1;
  ///////////////////////////////////////////////////////////////

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

  ////////////////////////////////////////////////////////////////////////////////
  // double start_time = MPI_Wtime();
  struct timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  ////////////////////////////////////////////////////////////////////////////////

  // Debug output
  fprintf(stdout, "--------------------\n");
  for (size_t i = 0; i < n; i++)
    fprintf(stderr, "%d\n", my_array[i]);
  fprintf(stdout, "--------------------\n");

  // TODO: sort array (of this processor) (heapsort or mergesort?)
  heapSort(my_array, n);
  //blockSort(my_array, n);

  // Debug output
  fprintf(stdout, "--------------------\n");
  for (size_t i = 0; i < n; i++)
    fprintf(stderr, "%d\n", my_array[i]);
  fprintf(stdout, "--------------------\n");

  // TODO: combine sorted arrays
  mergeSortComponents(my_array);

  // double elapsed_time = MPI_Wtime - start_time;

  // Output part of sorted array
  if ( rank == 0 ) {
    for (size_t i = 0; i < N; i += 10000)
      fprintf(stdout, "%d\n", my_array[i]);
    fprintf(stdout, "--------------------\n");
    // fprintf(stdout, "Execution time: %.2f\n", elapsed_time);
  }

  ////////////////////////////////////////////////////////////////////////////////
  struct timespec end_time, elapsed_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  timespec_subtract(&elapsed_time, &end_time, &start_time);
  double elapsed = (double)elapsed_time.tv_sec +
      (double)elapsed_time.tv_nsec / 1000000000.0;
  fprintf(stderr, "elapsed time: %.4f s\n", elapsed);
  ///////////////////////////////////////////////////////////////////////////////

  free(my_array);
  /*
  fprintf(stdout, "%d: Execution time: %.2f\n", rank, elapsed_time);
  MPI_Finalize();
  */
}