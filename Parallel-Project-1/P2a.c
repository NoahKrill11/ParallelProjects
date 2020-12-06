
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Get_dims(int* m_p, int* n_p, FILE *);
void Read_matrix(char prompt[], double A[], int m, int n, FILE *);
void Read_vector(char prompt [], double x[], int n,  FILE *);
void Print_matrix(char title[], double A[], int m, int n, int my_rank, int comm_sz, MPI_Comm comm);
void Print_vector(char title[], double y[], int m,int my_rank, int comm_sz, MPI_Comm comm);
void Mat_vect_mult(double A[], double x[], double y[], int m, int n);

/*-------------------------------------------------------------------*/

int main(void) {
    int        comm_sz;
    int        my_rank;
    MPI_Comm   comm;
    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);
    double* A = NULL;
    double* x = NULL;
    double* y = NULL;
   

   FILE* myfile = fopen("mv-data.txt", "r");
   int m, n;

   Get_dims(&m, &n, myfile);
   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   if (A == NULL || x == NULL || y == NULL) {
      fprintf(stderr, "Can't allocate storage\n");
      exit(-1);
   }
   Read_matrix("A", A, m, n, myfile);
#  ifdef DEBUG
   Print_matrix("A", A, m, n ,my_rank, comm_sz, comm);
#  endif
   Read_vector("x", x, n, myfile);
#  ifdef DEBUG
   Print_vector("x", x, n, my_rank, comm_sz, comm);
#  endif

   Mat_vect_mult(A, x, y, m, n);

   Print_vector("y", y, m, my_rank, comm_sz, comm);

   free(A);
   free(x);
   free(y);
   MPI_Finalize();
   return 0;
}  /* main */


/*-------------------------------------------------------------------
 * Function:   Get_dims
 * Purpose:    Read the dimensions of the matrix from stdin
 * Out args:   m_p:  number of rows
 *             n_p:  number of cols
 *
 * Errors:     If one of the dimensions isn't positive, the program
 *             prints an error and quits
 */
void Get_dims(int*  m_p, int*  n_p, FILE* myFile)
{
   
    int i = 0;
    fscanf (myFile, "%d", &i);
    *m_p=i;
    ++i;
    fscanf (myFile, "%d", &i);
    *n_p=i;
   if (*m_p <= 0 || *n_p <= 0) {
      fprintf(stderr, "m and n must be positive\n");
      exit(-1);
   }
    
    
}  /* Get_dims */


void Read_matrix(char prompt[] , double  A[], int m, int n, FILE * myFile)
{
    
   int i, j;
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
    fscanf(myFile,"%lf", &A[i*n+j]);
    
}  /* Read_matrix */

/*-------------------------------------------------------------------
 * Function:   Read_matrix
 * Purpose:    Read a vector from stdin
 * In args:    prompt:  description of matrix
 *             n:       order of matrix
 * Out arg:    x:       the vector being read in
 */
void Read_vector(
                 char    prompt[]  /* in  */,
                 double  x[]       /* out */, 
                 int     n         /* in  */,
                 FILE * myFile       ) {
   int i;
   for (i = 0; i < n; i++)
      fscanf(myFile,"%lf", &x[i]);
}  /* Read_vector */


/*-------------------------------------------------------------------
 * Function:   Print_matrix
 * Purpose:    Print a matrix to stdout
 * In args:    title:  title for output
 *             A:      the matrix
 *             m:      number of rows
 *             n:      number of cols
 */
void Print_matrix(
                  char    title[]  /* in */,
                  double  A[]      /* in */, 
                  int     m        /* in */, 
                  int     n        /* in */,
                  int my_rank,
                  int comm_sz,
                  MPI_Comm comm) {
   int i, j;
    if(my_rank==0)
    {
   printf("\nThe matrix %s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%f ", A[i*n+j]);
      printf("\n");
   }
   }
}  /* Print_matrix */

/*-------------------------------------------------------------------
 * Function:   Print_vector
 * Purpose:    Print the contents of a vector to stdout
 * In args:    title:  title for output
 *             y:      the vector to be printed
 *             m:      the number of elements in the vector
 */
void Print_vector(
                  char    title[]  /* in */, 
                  double  y[]      /* in */, 
                  int     m        /* in */,
                  int my_rank, int comm_sz, MPI_Comm comm) {
   int i;
  if(my_rank==0)
  {
   printf("\nThe vector %s\n", title);
   for (i = 0; i < m; i++)
      printf("%f ", y[i]);
   printf("\n");
      
}  /* Print_vector */
}



/*-------------------------------------------------------------------
 * Function:   Mat_vect_mult
 * Purpose:    Multiply a matrix by a vector
 * In args:    A: the matrix
 *             x: the vector being multiplied by A
 *             m: the number of rows in A and components in y
 *             n: the number of columns in A components in x
 * Out args:   y: the product vector Ax
 */
void Mat_vect_mult(
                   double  A[]  /* in  */, 
                   double  x[]  /* in  */, 
                   double  y[]  /* out */,
                   int     m    /* in  */, 
                   int     n    /* in  */) {
   int i, j;

   for (i = 0; i < m; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
         y[i] += A[i*n+j]*x[j];
   }
}  /* Mat_vect_mult */
