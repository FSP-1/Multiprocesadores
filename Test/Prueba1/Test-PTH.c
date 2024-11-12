#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <pthread.h>
#include <omp.h>

#define PRINT 0
#define DEBUG 1
//To avoid repeating struct drand48_data.
typedef struct drand48_data StDrand48Data;

//I'll use the same structure to init and to sum the vector.
typedef struct th_args
{
 int 		Rows;
 int		rows_start;
 int 		n_sub_rows;
 int **pMatrixA;
 int **pMatrixB;
 int **pMatrix;
 int **pResultMatrix;
 int Cols;
 pthread_mutex_t * pth_Sum_mutex;	//To update *pSum in exclusive way.
 int 		ThreadId;
} Type_Th_Args;

void HowtoShareVector(int ThisTh, int NTh, int NRows, int *start, int *n)
{
 int n_sub_rows= NRows/NTh;			//How many sub rows.
 int n_larger_threads= NRows%NTh;	//Threads with more rows
 int row_start;						//In which row a thread starts.

 if (ThisTh < n_larger_threads)
    { 
     n_sub_rows = n_sub_rows + 1;
     row_start = 0 + ThisTh * n_sub_rows;
    }  
 else 
    if (n_sub_rows > 0)
       row_start = n_larger_threads + ThisTh * n_sub_rows;
  else 
     { // this process has only zero elements
      row_start = -1;
      n_sub_rows  =  0;
     }

 *n		= n_sub_rows;
 *start	= row_start;
}
/*---------------------------------------------------------------------------*/
void ParametersError()
{
    puts("Options are:");
    puts("\t[ -h To show this help ]");
    puts("\t  -r <n rows>");
    puts("\t  -c <n columns>");
     puts("\t  -nt <n trheads>			");
    exit(0);
}

/*---------------------------------------------------------------------------*/
void InitMatrixInt(int **pMatrix, const int Rows, const int Cols)
{
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            pMatrix[i][j] = lrand48() % 10;  // Llenar con valores aleatorios entre 0 y 9
        }
    }
}

/*---------------------------------------------------------------------------*/
/*typedef struct th_args
{
 int 		Rows;
 int		rows_start;
 int 		n_sub_rows;
 int **pMatrixA;
 int **pMatrixB;
 int **pMatrix;
 int **pResultMatrix
 int Cols;
 pthread_mutex_t * pth_Sum_mutex;	//To update *pSum in exclusive way.
 int 		ThreadId;
} Type_Th_Args;
*/
void * SumMatricesInt(void * pTh_Args)
{
    Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args; 

 int		rows_start;
 int 		 		rows_end;
 int **pMatrixA;
 int **pMatrixB;
 int **pResultMatrix;
 int		Cols;
 pMatrixA	= pMyData->pMatrixA;
  pMatrixB	= pMyData->pMatrixB;
   pResultMatrix	= pMyData->pResultMatrix;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
   Cols	= pMyData->Cols;
 #if (PRINT==1)
 printf("I am thread %d. Doing rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif

    for (int i = rows_start; i < rows_end; i++) {
        for (int j = 0; j < Cols; j++) {
            pResultMatrix[i][j] = pMatrixA[i][j] + pMatrixB[i][j];
        }
    }
     pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
void PrintMatrixInt(FILE *out, int **pMatrix, const int Rows, const int Cols)
{
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            fprintf(out, "%d ", pMatrix[i][j]);
        }
        fprintf(out, "\n");
    }
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    int Rows, Cols;
    int **pMatrixA, **pMatrixB, **pResultMatrix;
     int NThreads; 
      pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory. 
 Type_Th_Args *pTh_Args=NULL;
 int rows_start,n_sub_rows; 	//Thread start at and how many.
  pthread_mutex_t pth_Sum_mutex; //To update Sum
 void *status;					//Status from thread.

    if (ExistArg("-h", argc, argv))
        ParametersError();

    if (!ExistArg("-r", argc, argv) || !ExistArg("-c", argc, argv)) {
        fputs("Parameters -r and -c are necessary.\n", stderr);
        ParametersError();
    } else {
        Rows = atoi(GetArg("-r", argc, argv));
        Cols = atoi(GetArg("-c", argc, argv));
        if (Rows < 1 || Cols < 1) {
            puts("Rows and Columns must be greater than 0.");
            exit(1);
        }
    }
if (!ExistArg("-nt",argc,argv))
    {
    fputs("Parameter -nt is neccesary.\n",stderr);
    ParametersError();
    }
 else
    {
     NThreads = atoi(GetArg("-nt",argc,argv)); 
     if (NThreads <0 || NThreads > Rows)
        {
         puts("NThreads must be > 0 and >= Rows.");
         exit(1);
        }
    }  

#if (PRINT == 1)
    printf("SumMatricesInt: Rows=%d, Cols=%d.\n", Rows, Cols);
#endif

    // Inicializar la semilla de números aleatorios
    srand48(0);
 /* Initialize mutex objects*/
 pthread_mutex_init(&pth_Sum_mutex, NULL);

 //Get mem for threads
 pThreads = (pthread_t *)    GetMem(NThreads, sizeof(pthread_t), 
                                    "Main: pThreads");
 pTh_Args = (Type_Th_Args *) GetMem(NThreads, sizeof(Type_Th_Args),
                                    "Main: pTh_Data");
 

    // Asignar memoria para las matrices usando GetMem2D
    pMatrixA = (int **) GetMem2D(Rows, Cols, sizeof(int), "Main:MatrixA");
    pMatrixB = (int **) GetMem2D(Rows, Cols, sizeof(int), "Main:MatrixB");
    pResultMatrix = (int **) GetMem2D(Rows, Cols, sizeof(int), "Main:ResultMatrix");

#if (DEBUG == 1)
    struct timeval tv1, tv2, tv3, tv4, tz;
    gettimeofday(&tv1, &tz);
#endif

    // Llenar las matrices con valores aleatorios
    InitMatrixInt(pMatrixA, Rows, Cols);
    InitMatrixInt(pMatrixB, Rows, Cols);

#if (DEBUG == 1)
    gettimeofday(&tv2, &tz);
#endif

#if (PRINT == 1)
    printf("Matrix A:\n");
    PrintMatrixInt(stdout, pMatrixA, Rows, Cols);
    printf("Matrix B:\n");
    PrintMatrixInt(stdout, pMatrixB, Rows, Cols);
#endif

#if (DEBUG == 1)
    gettimeofday(&tv3, &tz);
#endif
/*typedef struct th_args
{


 int **pResultMatrix


} Type_Th_Args;*/
for (int t=0; t<NThreads; t++) 
     {
 	  HowtoShareVector(t,NThreads, Rows, &rows_start, &n_sub_rows);
 	  pTh_Args[t].pMatrixA		= pMatrixA;
      pTh_Args[t].pMatrixB		= pMatrixB;
      pTh_Args[t].pResultMatrix	= pResultMatrix;
 	  pTh_Args[t].Rows			= Rows;
      pTh_Args[t].Cols			= Cols;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].ThreadId		= t;
      pTh_Args[t].pth_Sum_mutex = &pth_Sum_mutex;

      //By default threads are joinable.
      pthread_create(&pThreads[t],NULL, SumMatricesInt,(void *)&pTh_Args[t]);
     }

 //Do the threads join
 for (int t=0;t<NThreads; t++)
     {
      pthread_join(pThreads[t], (void **) &status);
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",t,*(int *)status);
      fflush(stdout);
      #endif
     }
    // Sumar las dos matrices
 //   SumMatricesInt(pMatrixA, pMatrixB, pResultMatrix, Rows, Cols);

#if (DEBUG == 1)
    gettimeofday(&tv4, &tz);
    double TInit = (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec) * 1e-6;
    printf("Tiempo de inicializar las matrices = %12.4g sec\n", TInit);
    double TSum = (tv4.tv_sec - tv3.tv_sec) + (tv4.tv_usec - tv3.tv_usec) * 1e-6;
    printf("Tiempo de sumar las matrices       = %12.4g sec\n", TSum);
    printf("Tiempo total = %12.4g sec\n", TInit + TSum);
#endif

#if (PRINT == 1)
    printf("Result Matrix (A + B):\n");
    PrintMatrixInt(stdout, pResultMatrix, Rows, Cols);
#endif

    // Liberar memoria
    Free2D((void **)pMatrixA, Rows);
    Free2D((void **)pMatrixB, Rows);
    Free2D((void **)pResultMatrix, Rows);
 pthread_mutex_destroy(&pth_Sum_mutex);
  free((void *)pTh_Args);
 free((void *)pThreads);
    return 0;
}
