#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <limits>

using namespace std;

void mergeSort(int* arr, int l, int r);
void merge(int* arr, int l, int m, int r);
void printArray(int* array, int size);

int main(){
	int size = 10000000;
	int* arr = new int[size]; 

	srand(static_cast<unsigned int>(clock()));
	for(int i=0; i<size; i++){
		arr[i]=rand();
	}

	cout << "arr[0]=" << arr[0] << " before sort\n";
	cout << "arr[" << size-1 << "]=" << arr[size-1] << " before sort\n";
  
	double before_single_mergesort = omp_get_wtime();
    mergeSort(arr, 0, size - 1); 
	double after_single_mergesort = omp_get_wtime();

	cout << "arr[0]=" << arr[0] << " after sort\n";
	cout << "arr[" << size-1 << "]=" << arr[size-1] << " after sort\n";	

	cout << "Single threaded mergesort took " << after_single_mergesort - before_single_mergesort << "[s]\n";
  
    return 0; 
}

void mergeSort(int* arr, int l, int r) { 
    if (l < r) { 
        int m = l+(r-l)/2; 
  
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

void merge(int* arr, int l, int m, int r) { 
    int i, j, k; 
    int left_size = m - l + 1; 
    int right_size =  r - m; 
    int* L = new int[left_size];
	int* R = new int[right_size]; 
  
    for (i = 0; i < left_size; i++) 
        L[i] = arr[l + i];

    for (j = 0; j < right_size; j++) 
        R[j] = arr[m + 1+ j]; 
  
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < left_size && j < right_size) 
    { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    while (i < left_size) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 

    while (j < right_size) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 


void printArray(int* array, int size){
	for(int i=0; i<size; i++){
		cout << array[i] << " ";
	}

	cout << "\n";
}






