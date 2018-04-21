/*
 * This program demonstrates various generic sorting algorithms in C.
 * Copyright (C) 2018 Joel Savitz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, or any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WIHTOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>
 *
 * Version 0.1
 *
 * Algorithms demonstrated in this version include:
 *  -bubble sort
 *  -insertion sort
 *  -selection sort
 *  -shell sort
 *  -heap sort
 *  -quick sort (broken at large arrays sizes?)
 *  -merge sort 
 *
 *  TODO:
 *    -diagnose and heal quicksort
 *    -add user interface
 *    -clean up the code
 *    -reorder the functions
 *    -add more documentation
 *    -refactor some ugly bits
 *
 *  Known bugs:
 *    -quicksort segfaults at large array sizes (a feature? Maybe I should file a patent)
 *    
 */


#include <stdio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void printArray( int * array, int size ) ;

void bubbleSort_int(int * items, int size )
{
  int i, j ;

  for ( i = 0 ; i < size - 1 ; i++ ) {
    for ( j = 0 ; j < size - 1 - i ; j++ ) {
      if ( items[j] > items[j + 1] )
      {
        items[j] ^= items[j + 1] ;
        items[j + 1] ^= items[j] ;
        items[j] ^= items[j + 1] ;
      }
    }
  }
}

void insertionSort_int( int * arr, int size )
{
  int i, j ;

  for ( i = 1 ; i < size ; i++ ) {
    //printf("isort i=%d\n", i ) ;
    for ( j = i ; j > 0 ; j-- ) {
      if ( arr[j] < arr[j - 1] )
      {
        arr[j] ^= arr[j - 1] ;
        arr[j - 1] ^= arr[j] ;
        arr[j] ^= arr[j - 1] ;
      }
    }
  }
}

int find_imin_int( void * items, int size )
{
  int i , imin ;
  int * array = (int*)items ;
  
  imin = 0 ;

  for ( i = 1 ; i < size ; i++ )
  {
    //printf("compare %d < %d ? [%d]\n", array[i], array[imin], array[i] < array[imin] ) ;
    if ( array[i] < array[imin] ) imin = i ;
  }

  printf("min is %d at i=%d\n", array[imin], imin ) ;
  return imin ;
}

void bubbleSort( void * items, int arrSize, size_t elSize,
    int (*compare)(void*, void*), void (*swap)(void*,void*) )
{
  int i, j ;

  for ( i = 0; i < arrSize - 1; i++ ) {
    for ( j = 0; j < arrSize - 1 - i; j++)
    {
      if( (*compare)(items+elSize*j,items+elSize*(j+1)) )
      {
        //printf("compare is true\n") ;
        (*swap)(items+elSize*j,items+elSize*(j+1)) ;
      }
    }
  }
}

void selectionSort( void* items, int arrSize, size_t elSize,
    int (*find_ifirst)(void*, int), void (*swap)(void*,void*) )
{
  int i ;

  for ( i = 0 ; i < arrSize - 1 ; i++ )
  {
    (*swap)(items+i*elSize,items+((*find_ifirst)(items+(i*elSize),arrSize-i)+i)*elSize) ;
  }
}

int compare_int_ascending( void * a, void * b )
{
  
  /*
  printf("compare %d > %d ? [%d] \n", *(int*)a, *(int*)b,
    *(int*)a > *(int*)b ) ;
  */
  
  return *(int*)a > *(int*)b ;
}

void swap_int( void * a, void * b )
{
  int * x = (int*)a ;
  int * y = (int*)b ;

  //printf("Swap %d and %d.\n", *x, *y) ;

  if ( *x == *y ) return ;

  *x ^= *y ;
  *y ^= *x ;
  *x ^= *y ;
}

void shift_int( void * selected )
{
  int * item = (int *) selected ;

  int temp = * item ;
  //printf("Shifting %d\n", temp) ;

  // Slide 'em over until you find something not larger than the selected item
  while ( *(item - 1) > temp )
  {
    *item = *(item - 1) ;
    item-- ;
  }

  // Finally, set the last shifted location to the selected item
  *item = temp ;
}

void insertionSort( void * items, int arrSize, size_t elSize, void (*shift)(void *),
    void (*swap)(void *, void *), int (*find_ifirst)(void *, int) )
{
  int i ;

  (*swap)(items, items + (*find_ifirst)(items,arrSize) * elSize) ;

  for ( i = 1; i < arrSize; i++ )
  {
    (*shift)(items + i * elSize ) ;
  }
}

void quickSort(void * items, int arrSize, size_t elSize, int (*compare)(void *, void *), void (*swap)(void *, void *) )
{

  // Base case
  //printf("Arrsize = %d, items[0] = %d\n", arrSize , *(int*)items) ;
  if (arrSize < 2) return ;

  // choose pivot (items for now)
  void * pPivot = items ;//+ ( rand() % ( arrSize - 1 ) * elSize) ;

  // Initialize left and right pointers
  void * pLeft = items ;
  void * pRight = items + (arrSize - 1) * elSize ;

  while(pLeft < pRight)
  {
    //while(pLeft < pRight && ! (*compare)(pPivot,pRight)) {
    while(pLeft < pRight && (*compare)(pRight,pPivot)) {
      pRight -= elSize ;
    }
    //printf("switch sweep\n") ;
    //while(pLeft < pRight && (*compare)(pPivot,pLeft)) {
    while(pLeft < pRight && ! (*compare)(pLeft,pPivot)) {
      pLeft += elSize ;
    }
    if ( pLeft < pRight ) {
      (*swap)(pRight,pLeft) ;
    }
    else {
      (*swap)(pLeft,pPivot) ;    
    }
  }
  quickSort(items, (pLeft-items)/elSize, elSize, compare, swap) ;
  quickSort(pLeft + elSize , ((items+(arrSize*elSize))-(pLeft+elSize))/elSize, elSize, compare, swap) ;
}

void h_sort(void * items, int arrSize, size_t elSize, int (*compare)(void *, void *), void (*swap)(void *, void *), int h )
{
  int i , j ;

  for ( i = h ; i < arrSize; i++ ) {
    for ( j = i - h; j >= 0; j -= h ) {
      if ( (*compare)(items+j*elSize,items+(j+h)*elSize) ) {
        (*swap)(items+j*elSize,items+(j+h)*elSize) ;
      }
    }
  }
}

void shellSort(void * items, int arrSize, size_t elSize, int (*compare)(void *, void *), void (*swap)(void *, void *) )
{
  int hStart = 15 ; // Arbitary constant, can be better tuned
  int h = hStart ;
  while ( h > 0 )
  {
    h_sort(items, arrSize, elSize, compare, swap, h ) ;
    h /= 2 ;
  } 
}

void fix_down(void * items, int arrSize, size_t elSize, int (*compare)(void *, void *), void (*swap)(void *, void *), int i )
{
  if ( 2*i + 1 >= arrSize ) return ;
  else
  if ( 2*i + 2 == arrSize ) {
    if ( ! (*compare)(items + i*elSize, items + (2*i + 1)*elSize) )
    {
      (*swap)(items + i*elSize, items + (2*i + 1) * elSize) ;
      return ;
    }
  }
  else {
    if ( (*compare)(items + (2*i + 1) * elSize, items + (2*i + 2) * elSize ) )
    {
      if ( ! (*compare)(items + i*elSize, items + (2*i + 1) * elSize) ) {
        (*swap)(items + i*elSize, items + (2*i + 1) * elSize) ;
        fix_down(items, arrSize, elSize, compare, swap, 2*i + 1) ;
        return ;
      }
      else {
        return ;
      }
    }
    else {
      if ( ! (*compare)(items + i*elSize, items + (2*i + 2) * elSize ) ) {
        (*swap)(items + i*elSize, items + (2*i + 2) * elSize) ;
        fix_down(items, arrSize, elSize, compare, swap, 2*i + 2) ;
        return ;
      }
      else {
        return ;
      }
    }
  }
}

void heapify(void * items, int arrSize, size_t elSize, int (*compare)(void *, void *), void (*swap)(void *, void *) )
{
  int i ;

  for ( i = (arrSize - 2) / 2; i >= 0; i-- )
  {
    //printf ("fix down i=%d\n",i) ;
    fix_down( items, arrSize, elSize, compare, swap, i ) ;
  }
}

void heapSort(void * items, int arrSize, size_t elSize, int (*compare)(void *, void *), void (*swap)(void *, void *) )
{
  int i ;

  heapify(items, arrSize, elSize, compare, swap) ;
  //printf("POSTHEAPIFY PREREMOVE\n") ;
  //printArray(items, ARRSIZE) ;

  for (i = arrSize - 1 ; i > 0 ; i--)
  {
    //printf ("swap i's 0 and %d\n", i ) ;
    (*swap)(items,items + i * elSize) ;
    fix_down(items, i , elSize, compare, swap, 0 ) ;
  }
}

void merge(void * arr1, int arr1Size, void * arr2, int arr2Size, void * arrDest, int arrDestSize, size_t elSize, int (*compare)(void *, void *), void (*copy)(void *, void *) )
{
  void * arr1_c, * arr2_c ;

  // Allocate space for the subarrays
  arr1_c = malloc(arr1Size * elSize ) ;
  arr2_c = malloc(arr2Size * elSize ) ;

  // Copy the subarrays to working allocated space to avoid overwriting in destination array
  int i , j;
  for (i = 0; i < arr1Size; i++)
  {
    (*copy)(arr1 + i * elSize, arr1_c + i * elSize) ;
  }

  for (i = 0; i < arr2Size; i++)
  {
    (*copy)(arr2 + i * elSize, arr2_c + i * elSize) ;
  }

  // Keep merging until you have reached the ends of both arr1 and arr2
  i = j = 0 ;
  while ( i < arr1Size || j < arr2Size )
  {
    
    // Case: both i and j reference indices within the bounds of their alloc'd space
    if ( i < arr1Size && j < arr2Size )
    {
      if ( !(*compare)(arr1_c + i * elSize, arr2_c + j * elSize ) )
      {
        (*copy)(arr1_c + i * elSize, arrDest + (i+j) * elSize)  ;
        i++ ;
      }
      else
      {
        (*copy)(arr2_c + j * elSize, arrDest + (i+j) * elSize)  ;
        j++ ;
      }
    }

    // Case: i has reached the end of arr1
    else
    if ( i >= arr1Size )
    {
      // We know that the only elements left to copy are in j's array, arr2
      (*copy)(arr2_c + j * elSize, arrDest + (i+j) * elSize)  ;
      j++ ;
    }
    // Case: j has reached the end of arr2
    else
    if ( j >= arr2Size )
    {
      // We know that the only elements left to copy are in i's array, arr1
      (*copy)(arr1_c + i * elSize, arrDest + (i+j) * elSize)  ;
      i++ ;
    }
    // Case: Something went wrong. Send out smoke signals to user
    else
    {
      printf("Control flow got lost in merge()...\n") ;
    }
  }

  // Free up those copied working subarrays
  free(arr1_c) ;
  free(arr2_c) ;
}

void mergeSort(void * items, int arrSize, size_t elSize , int (*compare)(void *, void *), void (*copy)(void *, void *))
{
  if (arrSize < 2) return ;

  int iDivider = arrSize / 2 ;

  int size1 = iDivider ;
  int size2 = arrSize - iDivider ;

  mergeSort(items, size1, elSize, compare , copy) ;
  mergeSort(items + size1 * elSize , size2, elSize, compare , copy) ;

  merge(items, size1, items + size1 * elSize, size2, items, arrSize, elSize, compare, copy) ;

}

void copy_int(void * from, void * to)
{
  *(int *) to = *(int *) from ;
}

void printArray( int * array, int size )
{
  int i ;
  printf ( "Array of size %d:\n", size ) ;
  for ( i = 0 ; i < size ; i ++ )
  {
    printf("[%d] = %d\n", i, array[i] ) ;
  }
}

int main( int argc, char ** argv )
{
  int * items ;
  int max = 100000000 ;
  unsigned long long arrSize = 100000000 ;

  printf("Creating an array of %llu random numbers between 1 and %d...\n", arrSize, max) ;
  items = (int*)malloc(sizeof(int)*arrSize ) ;
  if (!items)
  {
    printf("Array allocation failed! Killing self...\n") ;
  }
 
  srand(time(0)) ;
  for ( int i = 0 ; i < arrSize; i ++ )
  {
    items[i] = rand() % max + 1 ;
  }
  
  /*
  items[0] = 3 ;
  items[1] = 2 ;
  items[2] = 1 ;
  */
  

  //printArray(items, arrSize) ;

  printf("Begin sorting...\n");

  // Generic sorts ahoy

  //bubbleSort(items, arrSize, sizeof(int), &compare_int_ascending, &swap_int) ;
  //selectionSort(items, arrSize, sizeof(int), &find_imin_int, &swap_int) ;
  //insertionSort(items, arrSize, sizeof(int), &shift_int, &swap_int, &find_imin_int) ; 
  //shellSort(items, arrSize, sizeof(int), &compare_int_ascending, &swap_int) ;
  //heapSort(items, arrSize, sizeof(int), &compare_int_ascending, &swap_int) ;
  //quickSort(items, arrSize, sizeof(int), &compare_int_ascending, &swap_int) ;
  mergeSort(items, arrSize, sizeof(int), &compare_int_ascending, &copy_int) ;

  printf("Done!\n");

  //printArray(items, arrSize) ;

  free(items) ;

  return 0 ;
}
