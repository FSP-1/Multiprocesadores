#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include <mpi.h>
#include "utils.h"



#define PRINT 1

/*---------------------------------------------------------------------------*/
void ParametersError()
{
    puts("Options are:");
    puts("\t[ -h To show this help ]");
    puts("\t  -r <n rows>			");
    puts("\t  -n <number to search>		");
    exit(0);
}

/*---------------------------------------------------------------------------*/
// Init values of a vector of size Rows with random numbers in [0,9]
void InitVectorInt(int *pVector, const int Rows)
{ 
    for (int i = 0; i < Rows; i++)
        pVector[i] = lrand48() % 10;
}

/*---------------------------------------------------------------------------*/
// Count occurrences of a specific number in the array
int CountOccurrences(int *pVector, const int Rows, int number)
{
    int count = 0;
    for (int i = 0; i < Rows; i++)
    {
        if (pVector[i] == number)
        {
            count++;
        }
    }
    return count;
}

/*---------------------------------------------------------------------------*/
// Print a vector of size Rows
void PrintVectorInt(int *pVector, const int Rows, char *String)
{
    printf("%s -----------------------------------------\n", String);
    for (int i = 0; i < Rows; i++)
    {
        printf("%2d ", pVector[i]);
    }
    puts("");
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    int Rows, number, occurrences;
    int *pVector=NULL;
 int * pSubVector=NULL;

 int NTasks, MyRank; 	//MPI number of task and Rank for current task.
 int * pPosSubV;			//Vector with position each task starts in Vectors.
 int * pSizeSubV;			//Vector with size of subvector.
 int Rem; 				//Remainder
 int AddPos;			//Adding positions.
 //MPI Initialization
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
 MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
  if (MyRank==0)	//Task 0--
    {

      if (ExistArg("-h", argc, argv))
         ParametersError();

      if (!ExistArg("-r", argc, argv) || !ExistArg("-n", argc, argv))
      {
         fputs("Parameters -r and -n are necessary.\n", stderr);
         ParametersError();
      }

      Rows = atoi(GetArg("-r", argc, argv));
      number = atoi(GetArg("-n", argc, argv));

      if (Rows < 1 || number < 0 || number > 9)
      {
         puts("Invalid input: Ensure Rows > 0 and number is between 0 and 9.");
         exit(1);
      }

      printf("Rows = %d, Searching for number = %d.\n", Rows, number);

      // Init seed of random number generator
      srand48(time(NULL));

      // Allocate memory
      pVector = (int *)GetMem(Rows, sizeof(int), "Main:Vector");

      // Fill vector with random numbers
      InitVectorInt(pVector, Rows);

      // Print the generated vector
      #if (PRINT == 1)
      PrintVectorInt(pVector, Rows, "Generated Vector");
      #endif
    }

     MPI_Bcast(&Rows,1,MPI_INT,0,MPI_COMM_WORLD);
      MPI_Bcast(&number,1,MPI_INT,0,MPI_COMM_WORLD);

//Get men and initialize DisplacV and SizeSubV
 pPosSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:DisplacV");
 pSizeSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:SizeSubV");
 
 Rem=Rows%NTasks; 
 AddPos=0;
 for (int i=0; i<NTasks; i++) 
     {
      pSizeSubV[i] = Rows/NTasks;
      if (Rem > 0) 
         {
          pSizeSubV[i]++;
          Rem--;
         }

     pPosSubV[i] = AddPos;
     AddPos += pSizeSubV[i];
    }

 #if (PRINT==1)
 printf("Task %d out of %d, responsible elements: [%d,%d].\n",
      MyRank,NTasks,pPosSubV[MyRank],pPosSubV[MyRank]+pSizeSubV[MyRank]-1);
 #endif
 
 pSubVector  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector1");

 //All do de Scatterv
 MPI_Scatterv(pVector,   pSizeSubV, pPosSubV, MPI_INT, 
              pSubVector,pSizeSubV[MyRank],MPI_INT, 
              0, MPI_COMM_WORLD);

 #if (PRINT==1)
 printf("Task %d scater vectors:\n", MyRank);
 PrintVectorInt(pSubVector,pSizeSubV[MyRank],"SubVector:");
 #endif

    // Count occurrences of the specified number
    occurrences = CountOccurrences(pSubVector, pSizeSubV[MyRank], number);
int occurrences_global=0;
MPI_Reduce(&occurrences,&occurrences_global,  1,
                      MPI_INT, MPI_SUM, 0,
                      MPI_COMM_WORLD);
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubVector);


 if (MyRank==0)
    {
     #if (PRINT==1)
    printf("The number %d appears %d times in the vector.\n", number, occurrences_global);
     #endif
    // Free memory
    free((void *)pVector);
    }
     MPI_Finalize();

    return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
