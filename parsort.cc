#include <stdlib.h>
//#include "mpi.h"


#define N 200000UL
#define BASE_SEED 0x1234abcd



static int rank;
static int mpi_size;

static double start_time;


void heapify(){
  // TODO: implement heapify
}

void heapSort(){
  heapify();

  // TODO: implement heapsort
}

void mergeSort(){

  // TODO: implement mergeSort for merging processor sorted arrays
}


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

  int *my_array = malloc(sizeof(int) * N);

  /* Initialize the random number generator for the given BASE_SEED
  * plus an offset for the MPI rank of the node, such that on every
  * node different numbers are generated.
  */
  srand(BASE_SEED+rank);

  /* Generate N pseudo-random integers in the interval [0, RAND_MAX] */
  for (std::size_t i = 0; i < N; i++)
    my_array[i] = rand();

  // start_time = MPI_Wtime();

  // TODO: sort array (of this processor)

  // TODO: combine sorted arrays

  free(my_array);
  /*
  fprintf(stdout, "%d: Execution time: %.2f\n", rank, MPI_Wtime - start_time);
  MPI_Finalize();
  */
}