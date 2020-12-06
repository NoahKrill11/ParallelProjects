//Noah Krill
//Parallel
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>
void Usage(char* prog_name);
void checkNumbers(int *, int *);

int main(int argc, char* argv[])
{
    if (argc != 3) Usage(argv[0]);
    int thread_count=0,size=0,num1=0, num2=0;
    double factor=0.0;
    long long unsigned sum=1;
    clock_t start, end;
    double cpu_time_used;
    thread_count = atoi(argv[1]);
    size=atoi(argv[2]);
    int ar[size];

    printf("%s\n", "Please enter the smaller number to fill the array ");
    scanf("%d",&num1);
    printf("%s\n", "Please enter the bigger number to fill the array ");
    scanf("%d",&num2);
    checkNumbers(&num1,&num2);

    for(int i=0; i<size;++i)
    {
         ar[i] = (rand() % (num2 - num1))+num1;
         
    }
    start = clock();
    #pragma omp parallel for num_threads(thread_count) \
    
        for(int j=0; j<size; ++j)
        {
        for(int i=1; i<=ar[j]; ++i)
        {
            sum*=i;
        }
        printf("Thread%d - fac(%d)=%lld\n",omp_get_thread_num(), ar[j] ,sum);
        sum=1;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used by %d number of threads is %f",thread_count,cpu_time_used );
    printf("%s\n", " ");
}
void checkNumbers(int *num1, int *num2)
{
    while(*num2<*num1)
    {
        printf("%s\n", "You entered the numbers in wrong please try again" );
        printf("%s\n", "Please enter the smaller number to fill the array ");
        scanf("%d",num1);
        printf("%s\n", "Please enter the bigger number to fill the array ");
        scanf("%d",num2);
    }       
}
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count> <n>\n", prog_name);  /* Change */
   fprintf(stderr, "   thread_count is the number of threads >= 1\n");  /* Change */
   fprintf(stderr, "   n is the number of items in the array\n");
   exit(0);
}  /* Usage */