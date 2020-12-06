//Noah Krill
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h> 

unsigned long long sum;
int number;
pthread_mutex_t lock; 
sem_t mutex;
pthread_rwlock_t rwlock;
int busy=0;
struct structureOfArguments
{
	int start;
	int elements;
	int *Ar;
};
//the function used for putting all the elements in the array
int *placeElements(int numOfElements, int low, int high)
{
	int *Arr = malloc(sizeof(int)*numOfElements);
	for(int i=0; i < numOfElements;++i)
	{
		Arr[i] = rand() % high + low;
	}
	return Arr;
}
//this function now uses 4 different methods of locking to not allow other threads
//to mess with the sum as its being calculated
int fact(int num)
{
  
  unsigned long long result = 1;
  while (num> 0)
  {
      result *= num--;
  }
  if(number==1)
  {
  	 sum+=result;
  }
  else if(number==2)
  {
  	while(busy==1)
  	{

  	}
  	busy=1;
  	sum+=result;
  	busy=0;
  	
  }
  else if(number==3)
  {
  	pthread_mutex_lock(&lock); 
  	sum+=result;
	pthread_mutex_unlock(&lock); 
  }
  else if (number==4)
  {
  	sem_wait(&mutex);
  	sum+=result;
  	sem_post(&mutex);
  }
  else if(number==5)
  {
  	pthread_rwlock_wrlock(&rwlock);
  	sum+=result;
  	pthread_rwlock_unlock(&rwlock);

  }
  else
  {

  }
  return result;
}
//calculates the factorial
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
int main(int argc, char *argv[])
{
	sem_init(&mutex, 0, 1); 
	pthread_rwlock_init(&rwlock,NULL);

	double timeTotaled=0, singleTime, speedup, efficiency;
	struct timespec begin, end;
	int low, high;
	number = atoi(argv[1]);
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
  if(number>0 && number<6)
  {
   printf("The value of sum is %llu",sum );
}
  printf("\n");
  pthread_exit(NULL);
  return 0;
}