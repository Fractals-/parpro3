#include "blocksort.h"

void insertionSort( int *my_array, size_t start, size_t end ){
  size_t i, j;
  int temp;
  if ( start == 0 ) { // In this special case ensure no out of bounds
    for ( i = 1; i < end; i++ ) {
      temp = my_array[i];
      j = i - 1;
      while ( j > 0 && my_array[j] > temp ) {
        my_array[j + 1] = my_array[j];
        j--;
      }
      if ( j == 0 && my_array[j] > temp ){
        my_array[j + 1] = my_array[j];
        j--;
      }
      my_array[j + 1] = temp;
    }
  }
  else {
    for ( i = start + 1; i < end; i++ ) {
      temp = my_array[i];
      j = i - 1;
      while ( j >= start && my_array[j] > temp ) {
        my_array[j + 1] = my_array[j];
        j--;
      }
      my_array[j + 1] = temp;
    }
  }
}

void reversed_insertionSort( int *my_array, size_t start, size_t end ){
  size_t i, j;
  int temp;
  for ( i = end - 2; i > start; i-- ) {
    temp = my_array[i];
    j = i + 1;
    while ( j < end && my_array[j] < temp ) {
      my_array[j - 1] = my_array[j];
      j++;
    }
    my_array[j - 1] = temp;
  }
  temp = my_array[i];
  j = i + 1;
  while ( j <= end && my_array[j] < temp ) {
    my_array[j - 1] = my_array[j];
    j++;
  }
  my_array[j - 1] = temp;
}

// *************************************************************************************

size_t floorPowerOfTwo( size_t x ){
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  x |= x >> 32;
  return x - (x >> 1);
}

void reverse( int *my_array, size_t start, size_t end){
  int temp;
  size_t mid = (end + start) / 2;
  for ( size_t i = 0; start + i < mid; i++ ) {
    temp = my_array[start + i];
    my_array[start + i] = my_array[end - i - 1];
    my_array[end - i - 1] = temp;
  }
}

void rotate( int *my_array, size_t start, size_t mid, size_t end){
  reverse(my_array, start, end);
  reverse(my_array, start, start + end - mid);
  reverse(my_array, start + end - mid, end);
}

void merge( int *my_array, size_t start, size_t mid, size_t end){
  // TODO: this
}

void blockSort( int *my_array, size_t n ){
  // TODO: try this
  size_t i, j, start, mid, end,
         power_of_two = floorPowerOfTwo(n);
  double scale = n / (double) power_of_two;
  fprintf(stdout, "%llu, %.8f\n", power_of_two, scale);

  for ( i = 0; i < power_of_two; i += 16 ) {
    start = i * scale;
    end = (i + 16) * scale;
    fprintf(stdout, "%llu, %llu\n", start, end);
    reversed_insertionSort(my_array, start, end);
  }

  for ( j = 16; j < power_of_two; j *= 2 ){
    for ( i = 0; i < power_of_two; i += j * 2){
      start = i * scale;
      mid = (i + j) * scale;
      end = (i + j * 2) * scale;

      fprintf(stdout, "%llu, %llu, %llu\n", start, mid, end);

      if ( my_array[end - 1] < my_array[start] )
        rotate(my_array, start, mid, end);
      else if ( my_array[mid - 1] > my_array[mid] )
        merge(my_array, start, mid, end);
    }
  }
}
