#include "heapsort.h"

// *************************************************************************************

void siftDown( int *my_array, size_t i, size_t end ){
  size_t left = 2 * i + 1,
        right = 2 * i + 2,
      largest = i;
  int temp;

  if ( left <= end && my_array[left] > my_array[largest] )
    largest = left;
  if ( right <= end && my_array[right] > my_array[largest] )
    largest = right;

  if ( largest != i ) {
    temp = my_array[i];
    my_array[i] = my_array[largest];
    my_array[largest] = temp;
    siftDown(my_array, largest, end);
  }
}

void alt_siftDown( int *my_array, size_t i, size_t end ){
  size_t child = 2 * i + 1,
         largest = i;
  int temp;

  while ( child <= end ) {
    if ( my_array[child] > my_array[largest] )
      largest = child;
    child++;
    if ( child <= end && my_array[child] > my_array[largest] )
      largest = child;

    if ( largest != i ) {
      temp = my_array[i];
      my_array[i] = my_array[largest];
      my_array[largest] = temp;
      i = largest;
      child = 2 * i + 1;
    }
    else
      return;
  }
}

void alt_siftDown2( int *my_array, size_t i, size_t end ){
  size_t left = 2 * i + 1,
         right = 2 * i + 2,
         largest = i;
  int temp;

  while ( left <= end ) {
    if ( my_array[left] > my_array[largest] )
      largest = left;
    if ( right <= end && my_array[right] > my_array[largest] )
      largest = right;

    if ( largest != i ) {
      temp = my_array[i];
      my_array[i] = my_array[largest];
      my_array[largest] = temp;
      i = largest;
      left = 2 * i + 1;
      right = 2 * i + 2;
    }
    else
      return;
  }
}

// *************************************************************************************

void heapify( int *my_array, size_t n ){
  for ( size_t i = floor((n - 1)/2); i > 0; i-- )
    alt_siftDown2(my_array, i, n - 1);
  alt_siftDown2(my_array, 0, n - 1);
}

// *************************************************************************************

void heapSort( int *my_array, size_t n ){
  heapify(my_array, n);

  size_t end = n - 1, temp;
  while ( end > 0 ) {
    temp = my_array[0];
    my_array[0] = my_array[end];
    my_array[end] = temp;
    // Restore heap
    end--;
    alt_siftDown2(my_array, 0, end);
  }
}