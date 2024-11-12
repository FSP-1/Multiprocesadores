#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <omp.h>

#define PRINT 0
#define DEBUG 1

/*---------------------------------------------------------------------------*/
void ParametersError()
{
    puts("Options are:");
    puts("\t[ -h To show this help ]");
    puts("\t  -r <n rows>");
    puts("\t  -c <n columns>");
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
void SumMatricesInt(int **pMatrixA, int **pMatrixB, int **pResultMatrix, const int Rows, const int Cols)
{
    //#pragma omp parallel for default(none) shared(pMatrixA,Rows) collapse(2) 
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            pResultMatrix[i][j] = pMatrixA[i][j] + pMatrixB[i][j];
        }
    }
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

#if (PRINT == 1)
    printf("SumMatricesInt: Rows=%d, Cols=%d.\n", Rows, Cols);
#endif

    // Inicializar la semilla de números aleatorios
    srand48(0);

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

    // Sumar las dos matrices
    SumMatricesInt(pMatrixA, pMatrixB, pResultMatrix, Rows, Cols);

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

    return 0;
}
