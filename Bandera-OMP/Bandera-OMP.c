#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#ifdef _OPENMP
#  include <omp.h>
#endif

#define PRINT 1

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -c <n columns>		");
 puts("\t  -o <file>			");
 exit(0);
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols,i,j,chunk,tid;
 char ** ppRed, ** ppGreen, ** ppBlue;  //Matrices de RGB (0..255)
 char *FileName;
 FILE * FOut;
 char Command[256];
 bool GenImage=false;
 
 if (ExistArg("-h",argc,argv))
    ParametersError();  

 if (!ExistArg("-r",argc,argv))
    {
    fputs("Parameter -r is neccesary.\n",stderr);
    ParametersError();
    }
 else
    {
     Rows = atoi(GetArg("-r",argc,argv));
     if (Rows <=3)
        {
         puts("Rows<=3");
         exit(1);
        }
    } 

 if (!ExistArg("-c",argc,argv))
    {
    fputs("Parameter -c is neccesary.\n",stderr);
    ParametersError();
    }
 else
    {
     Cols = atoi(GetArg("-c",argc,argv)); 
     if (Cols <=3)
        {
         puts("Col<=3");
         exit(1);
        }
    }  

 if (ExistArg("-o",argc,argv))
    {
     GenImage=true;
     FileName = GetArg("-o",argc,argv);
    } 

# ifdef _OPENMP
#   pragma omp parallel default(none)
    { 
#     pragma omp single 
      printf("OpenMP-parallel with %1d threads\n", omp_get_num_threads());
    } /* end omp parallel */
# pragma omp barrier
# endif


 #if (PRINT==1)
 printf("Rows=%d, Cols=%d, Output=%s.\n", Rows, Cols, FileName);
 #endif

 /*Calloc de Getmen put data to zeroes*/
 ppRed   = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppRed");
 ppGreen = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppGreen");
 ppBlue  = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppBlue");


 # ifndef _OPENMP //T.CsPar
 struct timeval tv1,tv2; struct timezone tz;
 gettimeofday(&tv1, &tz);
 # endif
 
 # ifndef _OPENMP //Serializado
  for (i=0; i<Rows; i++)
     for (j=0; j<Cols; j++)
         {
          ppRed[i][j]=(char)255;
          if (i>=Rows/4 && i<=Rows*3/4)
             ppGreen[i][j]=(char)255;
         }
#endif   

 # ifndef _OPENMP //T.CsPar
   gettimeofday(&tv2, &tz);
   printf("T.CsPar (gettimeofday) = %g sec.\n", 
         (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6 );
 # endif
 
 # ifdef _OPENMP //CsPar: Codigo secuencial a paralelizar.
 //TODO 
 i= 0;
 j= 0;
 double wt1,wt2;
 int num_threads = omp_get_num_threads();
 int total_work = Rows * Cols;
 //chunk = (total_work + num_threads - 1) / num_threads; // Redondeo hacia arriba

 wt1=omp_get_wtime();

       
    #pragma omp parallel shared(ppGreen,ppRed) private(i,j,tid)
       {
 
      tid = omp_get_thread_num();

       #pragma omp for schedule(static,chunk) 
 	for (i=0; i<Rows; i++)  
 	 	      printf("Hello World from thread = %d\n", tid);
 	    for (j=0; j<Cols; j++)
   	      {

    	      ppRed[i][j]=(char)255;
    	      
    	      if (i>=Rows/4 && i<=Rows*3/4)
    	         ppGreen[i][j]=(char)255;
   	      }
   	      
       
  }

 wt2=omp_get_wtime();

 printf( "wall clock time (omp_get_wtime) = %12.4g sec\n", wt2-wt1 );
# endif


 if (GenImage)
	{
	 //Print to file
	 FOut=OpenFile(FileName,"wb");
	 for (int i=0;i<Rows;i++)
		 for (int j=0;j<Cols;j++)
		     {
		      fwrite(&ppRed  [i][j],sizeof(char),(size_t)1,FOut);
			  fwrite(&ppGreen[i][j],sizeof(char),(size_t)1,FOut);
			  fwrite(&ppBlue [i][j],sizeof(char),(size_t)1,FOut);
		     }

	 CloseFile(FOut);
	 /*rawtoppm and pnmtopng are in netpbm package*/
 	 sprintf(Command,"rawtoppm %d %d %s | pnmtopng > %s.png", 
                     Cols,Rows, FileName, FileName); 

	 #if (PRINT==1)
	 printf("Command=%s\n",Command);
	 #endif

	 system(Command);
	 /* eog from gnome visualize the image when it change*/
	}

 Free2D((void **)ppBlue, Rows);
 Free2D((void **)ppGreen,Rows);
 Free2D((void **)ppRed,  Rows);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/