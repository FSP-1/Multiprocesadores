#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"


#define DEBUG 0
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
    int *pVector;

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

    // Count occurrences of the specified number
    occurrences = CountOccurrences(pVector, Rows, number);

    // Print the result
    printf("The number %d appears %d times in the vector.\n", number, occurrences);

    // Free memory
    free((void *)pVector);

    return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
