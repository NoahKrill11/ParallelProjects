//
//  P1C.c
//
//  Created by Noah Krill on 9/24/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>



int ARRAY_SIZE = 1000;
void limits(int*, int*, int*, int*, int, int, MPI_Comm);
void allocate(unsigned long long**,int,int);
void populate(unsigned long long[], int, int, int, int, int, MPI_Comm);
void factorial(unsigned long long[], int, int, MPI_Comm);
unsigned long long Fact(int);

int main(void)
{
    unsigned long long* local_A;
    int first, second, my_rank, comm_sz, size, local_size;
    double start, finish, loc_elapsed, times;
    
    MPI_Comm comm;
    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);
    for (ARRAY_SIZE = 1000; ARRAY_SIZE <= 16000; ARRAY_SIZE*=2){
    if (my_rank == 0)
    {
        printf("\nThe size of the array is: %d\n", ARRAY_SIZE);
    }
    size = ARRAY_SIZE;
    srandom(0);
     //gets the numbers to be used for the array
      limits(&first, &second, &size, &local_size, my_rank, comm_sz, comm);
    //Allocates and populates the array full of numbers between first and second
      allocate(&local_A, size, local_size);
      populate(local_A, first, second, size, local_size, my_rank, comm);
        
      MPI_Barrier(comm);
      start = MPI_Wtime();

      factorial(local_A, local_size, size, comm);
      finish = MPI_Wtime();

      loc_elapsed = finish- start;
      MPI_Reduce(&loc_elapsed, &times, 1, MPI_DOUBLE, MPI_MAX, 0, comm);

      if (my_rank == 0)
      {
      printf("Time required = %e\n", times);
      }
      free(local_A);
      
  }
      MPI_Finalize();
      return(0);
}
//This function gets the upper and lower numbers that are required to be put in the array
void limits(int* m_p, int* n_p, int* size, int* local_size, int my_rank, int comm_sz, MPI_Comm comm)
{
    if (my_rank == 0)
    {
        printf("Enter the lower number\n");
          scanf("%d", m_p);
          printf("Enter the higher (m):\n");
          scanf("%d", n_p);
          while (*n_p <= *m_p)
          {
              printf("The higher number must be greater than the lower.\nEnter the higher number\n");
              scanf("%d", n_p);
          }
    }
    MPI_Bcast(m_p, 1, MPI_INT, 0, comm);
      MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
      
      *local_size = *size/comm_sz;
}
//This function allocates enough space for the arrays
void allocate(unsigned long long** local_Ar, int local_size,int size)
{
    *local_Ar = malloc(local_size * sizeof(unsigned long long));
}
// This function populates the arrays with the numbers between first and second
void populate(unsigned long long local_Ar[], int first, int second, int size, int local_size,
    int my_rank, MPI_Comm comm)
{
    unsigned long long* Ar = NULL;
    if (my_rank == 0)
    {
        Ar = malloc(size * sizeof(unsigned long long));
        
        for (int i = 0; i < size; i++)
        {
            Ar[i] = ( (unsigned long long) random() % (second - first) ) + first;
        }
        
        MPI_Scatter(Ar, local_size, MPI_UNSIGNED_LONG_LONG, local_Ar, local_size, MPI_UNSIGNED_LONG_LONG, 0, comm);
        free(Ar);
    }
    else
    {
        MPI_Scatter(Ar, local_size, MPI_UNSIGNED_LONG_LONG, local_Ar, local_size, MPI_UNSIGNED_LONG_LONG, 0, comm);
    }
}
//This function sets up the call to the factorial function with the use of mpi scatter
void factorial(unsigned long long local_Ar[], int local_size, int size, MPI_Comm comm)
{
    int i;
    unsigned long long* Ar;
    Ar = malloc(size*sizeof(unsigned long long));
    
    MPI_Allgather(local_Ar, local_size, MPI_UNSIGNED_LONG_LONG, Ar, local_size, MPI_UNSIGNED_LONG_LONG, comm);
    
    for(i = 0; i < local_size; i++)
    {
        local_Ar[i] = Fact(local_Ar[i]);
    }
}
//This function returns the factorials
unsigned long long Fact(int n)
{
    if (n==0)
        return 1;
    else
        return (n* Fact(n-1));
}
