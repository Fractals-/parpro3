#include <stdlib.h>
#include <cmath>

/* Sifts the value at node 'i' down the heap, but no further than 'end'
 * Parameters:
 *    i   - The node whose value is to be sifted down
 *    end - The furthest the value may be sifted down
 */
void siftDown( int *my_array, size_t i, size_t end );

/* Transform a given array to a heap
 * Parameters:
 *    my_array - Pointer to the array to transform
 *    n        - The size of the array
 */
void heapify( int *my_array, size_t n );

/* Sorts a given array using Heapsort
 * Parameters:
 *    my_array - Pointer to the array to be sorted
 *    n        - The size of the array
 */
void heapSort( int *my_array, size_t n );