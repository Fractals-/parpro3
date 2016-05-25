/* heapsort.cc
 * Assignment 3, Parallel Programming, LIACS
 *
 * Hanjo Boekhout - s1281348
 * 25-5-2016
 */

#include "heapsort.h"

// *************************************************************************************

void siftDown( int *my_array, size_t i, size_t end ){
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
      // Swap parent and largest child
      temp = my_array[i];
      my_array[i] = my_array[largest];
      my_array[largest] = temp;
      // Determine parent and children
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
    siftDown(my_array, i, n - 1);
  siftDown(my_array, 0, n - 1);
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
    siftDown(my_array, 0, end);
  }
}