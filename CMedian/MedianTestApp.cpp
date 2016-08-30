// MedianTestApp.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include "Median.h"

#define NUM_OF_PARTIALSORT_TEST_ITERATIONS			262144	// Number of test iterations we like to carry out.
#define MAX_SIZE_ARRAY4MEDIAN						2048	// For each test iteration, we randomly select the array size so that it lies
#define MIN_SIZE_ARRAY4MEDIAN						256		// between MAX and MIN_SIZE_ARRAY2SORT.
#define DATA_DUPLICATION_RATE						0.02	// The array entries used in the test workload are randomly generated, which
															// might already include duplicated data.  Here we want to make sure "roughly"
															// at least (uiArraySize*DATA_DUPLICATION_RATE) entries consist with
															// duplicated data.
#define DUPLICATED_DATA_INDEX_SIZE					((int) (MAX_SIZE_ARRAY4MEDIAN*DATA_DUPLICATION_RATE))
#define INITIAL_RAND_SEED							0

int main()
{
	unsigned int	i, j, uiArraySize, uiDuplicatedDataSize;
	unsigned int	uiNumMismatches = 0;
	unsigned int	uiDuplicatedDataIndex, uiDuplicatingDataIndex;
	float			fValues[MAX_SIZE_ARRAY4MEDIAN], fValuesCopy[MAX_SIZE_ARRAY4MEDIAN];
	float			fCorrectMedian, fEvaluatedMedian;

	// Initialize the random seed.
	std::srand(INITIAL_RAND_SEED);

	// Here we will implement the median evaluation with test-driven development
	// (https://en.wikipedia.org/wiki/Test-driven_development).  We first start coding the test, which
	// fails 100% of the time, then adding the minimum code in median() to make it pass 100%.
	for (i = 0; i < NUM_OF_PARTIALSORT_TEST_ITERATIONS; i++)
	{
		// Generating the array size.
		uiArraySize = MIN_SIZE_ARRAY4MEDIAN + (unsigned int)((MAX_SIZE_ARRAY4MEDIAN - MIN_SIZE_ARRAY4MEDIAN + 1)*
															 ((float) std::rand() / (RAND_MAX + 1)));

		// Filling the random array for which we want to find the median.
		for (j = 0; j < uiArraySize; j++)
			fValues[j] = (float) std::rand() / (RAND_MAX + 1);

		// Introducing duplicated data in the workload array according to DATA_DUPLICATION_RATE.  Note that we only guarantee
		// the number of entry pairs to be duplicated, and we do not have guarantees that a duplication will be replaced with
		// another subsequent duplication, nor whether two duplications will hit the same pair (and become effectively just
		// one duplication).  However, if DATA_DUPLICATION_RATE is small, we can rest assured that these scenarios would be
		// rare.
		uiDuplicatedDataSize = (int)(uiArraySize * DATA_DUPLICATION_RATE);
		for (j = 0; j < uiDuplicatedDataSize; j++)
		{
			// uiDuplicatedDataIndex corresponds to an index of data which we want to introduce a duplicated entry.
			uiDuplicatedDataIndex = (unsigned int)(((float)std::rand() / (RAND_MAX + 1)) * uiArraySize);
			uiDuplicatingDataIndex = (unsigned int)(((float)std::rand() / (RAND_MAX + 1)) * uiArraySize);
			assert((0 <= uiDuplicatedDataIndex) && (uiDuplicatedDataIndex < uiArraySize));
			assert((0 <= uiDuplicatingDataIndex) && (uiDuplicatingDataIndex < uiArraySize));
			fValues[uiDuplicatedDataIndex] = fValues[uiDuplicatingDataIndex];
		}

		// Finding the median based on the implemented function.
		fEvaluatedMedian = median(fValues, uiArraySize);

		// Copying fValues[] to fValuesCopy[] for possible debugging purposes.
		for (j = 0; j < uiArraySize; j++)
			fValuesCopy[j] = fValues[j];

		// Sorting fValuesCopy.
		std::sort(fValuesCopy, fValuesCopy + uiArraySize);

		// Verifying the median values based on my implemented function.
		if ((uiArraySize & 0x1))
		{
			// size is odd
			fCorrectMedian = fValuesCopy[(uiArraySize - 1) / 2];
		}
		else
		{
			// size is even
			fCorrectMedian = (fValuesCopy[(uiArraySize - 2) / 2] + fValuesCopy[uiArraySize / 2]) / 2.0f;
		}

		if (fCorrectMedian != fEvaluatedMedian)
		{
			std::cout << "Medians disagree at " << i << "th run!  Correct value " << fCorrectMedian
					  << " and evaluated value " << fEvaluatedMedian << std::endl;
			uiNumMismatches++;
		}
	}

	std::cout << "Among " << NUM_OF_PARTIALSORT_TEST_ITERATIONS << " runs, " << uiNumMismatches << " failed.  Failure rate "
			  << (float)uiNumMismatches / NUM_OF_PARTIALSORT_TEST_ITERATIONS << std::endl;

    return 0;
}

