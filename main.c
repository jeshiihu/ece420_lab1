/* File:     
 *     main.c 
 *
 * Purpose:  
 *     Computes a shared-memory parallel program for matrix multiplication 
 *	   using pthreads
 *
 * Input:
 *     n: order of matrix
 *     A, B the matrices to be multiplied
 *
 * Output:
 *     C: the product matrix
 *
 * Compile:  gcc -g -Wall -o main main.c -lpthread -lm
 * Usage:
 *     main <thread_count>
 *
 * Notes:  
 *     1.  Local storage for A, B, C is dynamically allocated.
 *     2.  Number of threads (thread_count) should evenly divide both 
 *         n.  The program doesn't check for this.
 *     3.  We use a 2-dimensional array for A and B and compute subscripts
 *         using the formulae P[x][y] where x = floor(k/p) and y = k%sqrt(p)
 *     4.  Distribution of A, B, and C is logical:  all three are 
 *         globally shared.
 *
 * IPP:    Section 4.3 (pp. 159 and ff.).  Also Section 4.10 (pp. 191 and 
 *         ff.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

/* Global variables */
int     thread_count;
int     n;
double* A;
double* B;
double* C;

/* Serial functions */
void Usage(char* prog_name);
void Read_matrix(char* prompt, double A[], int n);
void Print_matrix(char* title, double A[], int n);

/* Parallel function */
void *pth_mat_mult(void* rank);

int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles;

   if (argc != 2) Usage(argv[0]);
   thread_count = atoi(argv[1]);
   thread_handles = malloc(thread_count*sizeof(pthread_t));

   printf("Enter n\n");
   scanf("%d%d", &n);

   A = malloc(n*n*sizeof(double));
   B = malloc(n*n*sizeof(double));
   C = malloc(n*n*sizeof(double));

   Read_matrix("Enter the matrix A", A, n);
   Print_matrix("We read", A, n);

   Read_matrix("Enter the matrix B", B, n);
   Print_matrix("We read", B, n);

   for (thread = 0; thread < thread_count; thread++)
	  pthread_create(&thread_handles[thread], NULL,
		 pth_mat_mult, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
	  pthread_join(thread_handles[thread], NULL);

	Print_matrix("The product matrix is: ", C, n);

	free(A);
	free(x);
	free(y);

   return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    Read_matrix
 * Purpose:     Read in the matrix
 * In args:     prompt, n
 * Out arg:     A/B
 */
void Read_matrix(char* prompt, double A[], int n) {
   int i, j;

   printf("%s\n", prompt);
   for (i = 0; i < n; i++) 
	  for (j = 0; j < n; j++)
		 scanf("%lf", &A[i*n+j]);
}  /* Read_matrix */


/*------------------------------------------------------------------
 * Function:    Print_matrix
 * Purpose:     Print the matrix
 * In args:     title, A, n
 */
void Print_matrix( char* title, double A[], int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < n; i++) {
	  for (j = 0; j < n; j++)
		 printf("%4.1f ", A[i*n+j]);
	  printf("\n");
   }
}  /* Print_matrix */


/*------------------------------------------------------------------
 * Function:       pth_mat_mult
 * Purpose:        Multiply an nxn matrix A by an nxn matrix B
 * In arg:         rank
 * Global in vars: A, B, n, thread_count
 * Global out var: C
 */
void *Pth_mat_mult(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int x, y;

   x = floor(my_rank/sqrt(thread_count));
   y = my_rank % sqrt(thread_count);

   int local_n = n/sqrt(thread_count); 
   int my_first_row = my_rank*local_n;
   int my_last_row = (my_rank+1)*local_n - 1;

   for (i = my_first_row; i <= my_last_row; i++) {
	  for (j = 0; j < n; j++)
		  C[x*n+y] += A[i*n+j]*B[j];
   }

   return NULL;
}  /* Pth_mat_vect */














