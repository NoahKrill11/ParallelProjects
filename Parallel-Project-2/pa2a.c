//Noah Krill
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct structureOfArguments
{
	int start;
	int elements;
	int *Ar;
};
//Function for adding elements into the array
int *placeElements(int numOfElements, int low, int high)
{
	int *Arr = malloc(sizeof(int)*numOfElements);
	for(int i=0; i < numOfElements;++i)
	{
		Arr[i] = rand() % high + low;
	}
	return Arr;
}
//the function that does the calculation for the factoral
int fact(int num)
{
  int result = 1;
  while (num> 0)
  {
      result *= num--;
  }
  return result;
}
//the function that recursivly calls the factorial function
void *calcfact(void *args)
{
	struct structureOfArguments * arguments = args;
	for(int i=0; i < arguments->elements; ++i)
	{
		arguments->Ar[arguments->start +i]= fact(arguments->Ar[arguments->start + i]);
 	 }
 	 pthread_exit(NULL);
  	 return (NULL);
}
int main()
{
	double timeTotaled=0, singleTime, speedup, efficiency;
	struct timespec begin, end;
	int low, high;
	srand(time(NULL));
	printf("Please enter the lower bound\n");
	scanf("%d", &low);
	printf("Please enter the upper bound\n");
	scanf("%d", &high);
	struct structureOfArguments args;
	
	for (int arrLength = 1000; arrLength < 16001; arrLength *= 2)
  {
    singleTime = 0;
    printf("\nArray Length: %d\n", arrLength);
    args.Ar = placeElements(arrLength, low, high);
    for (int threadCount = 1; threadCount < 17; threadCount *= 2)
    {
      printf("Thread Count: %d; ", threadCount);
      pthread_t threads[threadCount];
      args.elements = arrLength / threadCount;
      printf("Each thread will process %d element(s)\n", args.elements);
      for (int i = 0; i < threadCount; ++i)
      {
        args.start = args.elements * i;
        //timing all the threads
        clock_gettime(CLOCK_MONOTONIC, &begin);
        //creating threads
        int unneed = pthread_create(&threads[i], NULL, &calcfact, (void *)&args);
        clock_gettime(CLOCK_MONOTONIC, &end);
        timeTotaled+= (end.tv_sec - begin.tv_sec);
        timeTotaled+= (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
      }
      if (threadCount == 1)
        singleTime += timeTotaled;
      printf("Total time %d thread(s): %f\n", threadCount, timeTotaled);
      speedup = singleTime/ timeTotaled;
      printf("Speedup from 1 to %d threads: %f / %f = %f\n", threadCount, singleTime, timeTotaled, speedup);
      efficiency = speedup / threadCount;
      printf("Efficiency from 1 to %d threads: %f / %d = %f\n\n", threadCount, speedup, threadCount, efficiency);
      for (int i = 0; i < threadCount; ++i)
      {
        pthread_join(threads[i], NULL);
      }
    }
  }
  printf("\n");
  pthread_exit(NULL);
  return 0;
}