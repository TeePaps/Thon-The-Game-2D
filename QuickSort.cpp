#include "QuickSort.h"

void QuickSort (vector<MyTuple>& myVector, int low, int high)
{
	int mid;

	if (low<high) {
		mid = partition(myVector,low,high);
		//     A[i]<=A[mid] for low<=i<mid
		//     A[mid] < A[j] for mid<j<=high

		QuickSort(myVector,low,mid-1);
		// A[low]...A[mid-1] is sorted
		QuickSort(myVector,mid+1,high);
		// A[mid+1]...A[high] is sorted
	}
	//A[low]...A[high] is sorted
}

int partition(vector<MyTuple>& myVector, int low, int high) {
	int mid;
	int pivotValue;
	int i,j;

	// choose pivot and move it to beginning of array segment
	mid = (low+high)/2;
	swap(myVector[low], myVector[mid]);
	pivotValue = get<1>(myVector[low]);

	// initialize i & j to index first & last elements to partition
	i = low+1;
	j=high;

	//move i & j towards each other, swapping elements when necessary
	while (i<=j) {

		// starting at i, find the first value greater than pivot Value
		while(i<=high && get<1>(myVector[i]) >= pivotValue)
			i++;
		//starting at j, find the next value less than or equal to pivot Value
		while(j>=low+1 && pivotValue > get<1>(myVector[j]))
			j--;

		//if we found two values then swap
		if (i<j)
			// myVector[i]>pivotValue  && myVector[j]<=pivotValue
			swap(myVector[i], myVector[j]);
	}

	//swap pivot into position
	swap(myVector[low], myVector[j]);

	//return the index of the pivot
	return j;
}