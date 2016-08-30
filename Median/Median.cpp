// Median.cpp : Defines the entry point for the application.

#include <iostream>
#include <cassert>
#include "Median.h"


// MeanPartition() carries out a "partial" quick sort and returns a pivotIndex so that all entries in A[0:pivotIndex]
// are smaller than or equal to A[(pivotIndex + 1):(iASize - 1)].  This means the original quick sort can be now divided
// into two quick sub-sorts.  A[] could be reordered as PartialSortPartition() returns.
//
// Here we would like to use Hoare partition scheme (https://en.wikipedia.org/wiki/Quicksort) for the partition.
int		MeanPartition(float A[], int iASize)
{
	int		i, j;
	float	fTemp, fPivot;

	// pivot: = A[lo]
	// i : = lo – 1
	// j : = hi + 1
	fPivot = A[0];
	i = -1;
	j = iASize;

	// loop forever
	while (1)
	{
		// do
		// i : = i + 1
		// while A[i] < pivot
		do {} while (A[++i] < fPivot);
		// Now i points to the smallest index where A[i] >= fPivot.

		// do
		// j : = j – 1
		// while A[j] > pivot
		do {} while (A[--j] > fPivot);
		// Now j points to the smallest index where A[j] <= fPivot.

		// if i >= j then
		//     return j
		if (i >= j)
			return j;

		// swap A[i] with A[j]
		fTemp = A[i];
		A[i] = A[j];
		A[j] = fTemp;
	}
}


// find_kth_element() will sort f[] in ascending order of size fsize, but the sorting is not complete -- it stops when the kth
// element of the sorted array is found and kept in the kth position.  This function is allowed to rearrange f[] (through
// partial sorting.
void	find_kth_element(float f[], unsigned int ifsize, int k)
{
	// We do not need to do anything if ifsize is 1.
	if (ifsize == 2)
	{
		// f[] consists of f[0] and f[1] only.  Swapping them if they are in descending order.
		if (f[0] > f[1])
		{
			float	fTemp = f[0];
			f[0] = f[1];
			f[1] = fTemp;
		}
	}
	else if (ifsize > 2)
	{
		int		iPivotIndex = MeanPartition(f, ifsize);

		if (k <= iPivotIndex)
		{
			// The kth entry is in the first part of f[].  We will search for the kth entry from f[0] to f[iPivotIndex].
			// Note that this includes the case when k == iPivotIndex.
			find_kth_element(f, (iPivotIndex + 1), k);
		}
		else if (k > iPivotIndex)
		{
			// The kth entry is in the second part of f[].  We will search for the (k - iPivotIndex - 1)th entry
			// from f[iPivotIndex + 1] to f[ifSize - 1].  Note that the entry position does not change, but the
			// reference of the array changed.
			find_kth_element(&(f[iPivotIndex + 1]), (ifsize - iPivotIndex - 1), (k - iPivotIndex - 1));
		}
	}

	return;
}


// Given a float array f[] of unsigned int size fsize, median() returns the median of f[] where
// (1) if fsize is odd, the median value will be f[(fsize - 1)/2];
// (2) if fsize is even, the median value will be (f[(fsize - 2/2] + f[fsize/2])/2.
//
// Please refer to median (https://en.wikipedia.org/wiki/Median) for more details.
float	median(const float f[], unsigned int fsize)
{
	// To find the median of f[], we essentially do the followings.  We have two auxiliary functions:
	// (1) kthentry_array(): it returns the kth entry of a sorted array.
	// (2) kthkp1thentries_array(): it returns the kth and (k+1)th entries of a sorted array.
	//
	// The two functions are recursive and they can call each other alternatively.  To find the median, we essentially make use of
	// Quick Sort, though we are only interested in sorting the subarray involving the entries we need for median.
	unsigned int		i;
	float	fkth, fkp1th, fRetValue = 0.0f;
	float*	pfCopy = new float[fsize];

	// Making a copy of f[] into pfCopy[] if new succeeded.
	if (pfCopy == 0x0)
	{
		std::cout << "Insufficient memory for allocation in median()." << std::endl;
		return 0.0f;
	}
	else
	{
		for (i = 0; i < fsize; i++)
			pfCopy[i] = f[i];
	}

	if (fsize % 2 == 0)
	{
		// fsize is even.  We return the mean of the (fsize - 2)/2 entry and fsize/2 entry.
		find_kth_element(pfCopy, fsize, (fsize - 2)/2);
		fkth = pfCopy[(fsize - 2)/2];
		find_kth_element(pfCopy, fsize, fsize/2);
		fkp1th = pfCopy[fsize/2];
		fRetValue = ((fkth + fkp1th)/2.0f);
	}
	else
	{
		// fsize is odd.  We simply return the (fsize - 1)/2 entry of a sorted f[].
		find_kth_element(pfCopy, fsize, (fsize - 1)/2);
		fRetValue = pfCopy[(fsize - 1)/2];
	}

	// Cleanup before returning
	delete[] pfCopy;

	return fRetValue;
}