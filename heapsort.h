#include <stdlib.h>
#include <cmath>

void siftDown( int *my_array, size_t i, size_t end );

void alt_siftDown( int *my_array, size_t i, size_t end );

void alt_siftDown2( int *my_array, size_t i, size_t end );

void heapify( int *my_array, size_t n );

void heapSort( int *my_array, size_t n );