#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#ifdef _OPENMP
#  include <omp.h>
#endif

#define PRINT 0
#define BEST_CHUNK 0


/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]		");
 puts("\t  -r  <n rows image>			");
 puts("\t  -c  <n columns image>		");
 puts("\t  -mx <min  x Mandel's window>	");
 puts("\t  -my <min  y Mandel's window>	");
 puts("\t  -sx <size x Mandel's window>	");
 puts("\t  -sy <size y Mandel's window>	");
 puts("\t..-mi <max n of iterations>   ");
 puts("\t  -of <file> [Image]>	");
 puts("\t  -o graphical output	");

 exit(0);
}

/*---------------------------------------------------------------------------*/
void NIterToRGB(int NIter, char * pRed, char *pGreen, char *pBlue)
//Move the number of iterations to RGB using a pallete.
{
 const char PalleteRed  [16] =
            {66,25, 9, 4,  0, 12, 24, 57,134,211,241,248,255,204,153,106};
 const char PalleteGreen[16] =
            {30, 7, 1, 4,  7, 44, 82,125,181,236,233,201,170,128, 87, 52};
 const char PalleteBlue [16] =
            {15,26,47,73,100,138,177,209,229,248,191,95,   0,  0,  0,  3};

 NIter=NIter%16;
 *pRed   = PalleteRed  [NIter];
 *pGreen = PalleteGreen[NIter];
 *pBlue  = PalleteBlue [NIter];
}

/*---------------------------------------------------------------------------*/
 void PixelMandel(char **ppRed, char ** ppGreen, char ** ppBlue,
                  const double MinX, const double MinY, 
                  const double IncX, const double IncY,
                  const int MaxNIter, const int i, const int j)
{
 int 	 NIter; 			//Number of iterations.
 double zr,zi,cr,ci;
 double zrs,zis;

 zi=ci=MinY+i*IncY;
 zr=cr=MinX+j*IncX;
 zrs=zis=0.0;
 NIter=0;
 while (zrs+zis<=4.0 && NIter<=MaxNIter)
       {
        zrs=zr*zr;
        zis=zi*zi;
        zi=2.0*zr*zi+ci;
        zr=zrs-zis+cr;
        NIter++;
       }
       
 NIterToRGB(NIter,&(ppRed[i][j]),&(ppGreen[i][j]),&(ppBlue[i][j]));
 
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int     MaxNIter;			//Max number of Iterations.
 int     Rows, Cols;		//Rows and Cols of the Image.
 double  MinX, MinY;		//Min x and y of the mandelbrot window.
 double  SizeX, SizeY; 		//Size in x and y of the Mandelbrot window.
 char ** ppRed=NULL, ** ppGreen=NULL, ** ppBlue=NULL;  //RGB matrices (0..255)
 char *  FileName;
 FILE *  FOut;
 char    Command[256];
 char    GenImage=False; 	//If graphical output is generated 
 double  IncX, IncY; 		//Pixes width in x and y.


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

 if (!ExistArg("-mx",argc,argv))
    {
    fputs("Parameter -mx is neccesary.\n",stderr);
    ParametersError();
    }
 else
     MinX = atof(GetArg("-mx",argc,argv)); 

 if (!ExistArg("-my",argc,argv))
    {
    fputs("Parameter -my is neccesary.\n",stderr);
    ParametersError();
    }
 else
     MinY = atof(GetArg("-my",argc,argv)); 


 if (!ExistArg("-sx",argc,argv))
    {
    fputs("Parameter -sx is neccesary.\n",stderr);
    ParametersError();
    }
 else
     SizeX = atof(GetArg("-sx",argc,argv)); 

 if (!ExistArg("-sy",argc,argv))
    {
    fputs("Parameter -sy is neccesary.\n",stderr);
    ParametersError();
    }
 else
     SizeY = atof(GetArg("-sy",argc,argv)); 


 if (!ExistArg("-mi",argc,argv))
    {
    fputs("Parameter -mi is neccesary.\n",stderr);
    ParametersError();
    }
 else
    {
     MaxNIter = atoi(GetArg("-mi",argc,argv));
     if (MaxNIter <1)
        {
         puts("Max. number of Iterations < 1");
         exit(1);
        }
    } 

 if (!ExistArg("-of",argc,argv))
     FileName="Image";
 else
     FileName = GetArg("-of",argc,argv);
     
 if (ExistArg("-o",argc,argv)) 
    GenImage=True; 

 #if (PRINT==1)
 printf("Img.Rows=%d, Img.Cols=%d,\n", Rows, Cols);
 printf("Mandel.MinX=%f, Mandel.MinY=%f,\n",
         MinX, MinY);
 printf("Mandel.SizeX=%f, Mandel.SizeY=%f,\n",
         SizeX,SizeY);
 printf("InGray=%d, Max.N.iterations=%d,  Output=%s.\n",InGray, MaxNIter, FileName);
 #endif


# ifdef _OPENMP
#   pragma omp parallel default(none)
    { 
#     pragma omp single 
     {
      printf("------------\n");
      printf("OpenMP-parallel with %1d threads\n", omp_get_num_threads());
     }
    } /* end omp parallel */
# pragma omp barrier    
# endif 


 //GetMem for ppRed, ppGreen and ppBlue. It put data to zeroes
 ppRed   = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppRed");
 ppGreen = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppGreen");
 ppBlue  = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppBlue");

 IncX=SizeX/(double)Cols;
 IncY=SizeY/(double)Rows;


 # ifndef _OPENMP //T.CsPar 
 struct timeval tv1,tv2; struct timezone tz;
 gettimeofday(&tv1, &tz);

 //Iterations per pixel
 for (int i=0;i<Rows;i++)
     for (int j=0;j<Cols;j++)
         PixelMandel(ppRed, ppGreen,  ppBlue, 
                     MinX, MinY, IncX, IncY, MaxNIter, 
                     i, j);

   gettimeofday(&tv2, &tz);
   printf("T.CsPar (gettimeofday) = %g sec.\n", 
         (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6 );
 # endif


 #if (BEST_CHUNK==0)
 # ifdef _OPENMP //CsPar: Codigo secuencial a paralelizar.
 double wt1,wt2;
 wt1=omp_get_wtime();
#pragma omp parallel for collapse(2) shared(ppRed, ppGreen, ppBlue, Rows, Cols, MinX, MinY, IncX, IncY, MaxNIter) schedule(dynamic)  
    
	for (int i = 0; i < Rows; i++)
    	    for (int j = 0; j < Cols; j++)
        	PixelMandel(ppRed, ppGreen, ppBlue, 
       			    MinX, MinY, IncX, IncY, MaxNIter, 
        	    	    i, j);
   	      
     

 wt2=omp_get_wtime();

 printf( "wall clock time (omp_get_wtime) = %12.4g sec\n", wt2-wt1 );
# endif
# endif

 #if (BEST_CHUNK==1)
# ifdef _OPENMP //CsPar: Codigo secuencial a paralelizar.
 //TODO Parallelize with OpenMP # ifdef _OPENMP
 int total_iterations = Rows * Cols;
 int num_threads;
 double wt1,wt2;

    #pragma omp parallel shared(ppRed, ppGreen, ppBlue, MinX, MinY, IncX, IncY, MaxNIter) private(num_threads)
    { 
    int min_chunk = 1;
    num_threads = omp_get_num_threads();
    int max_chunk = total_iterations / num_threads;
    int best_chunk = min_chunk;
    double best_time = 1e9; // Un valor alto para comenzar

    while (max_chunk - min_chunk > 1) {
        int med_chunk = (min_chunk + max_chunk) / 2;

        // Medir el tiempo con el chunk actual
        wt1 = omp_get_wtime();
        #pragma omp  for schedule(dynamic, med_chunk) collapse(2)
        for (int i = 0; i < Rows; i++)
            for (int j = 0; j < Cols; j++)
                PixelMandel(ppRed, ppGreen, ppBlue, MinX, MinY, IncX, IncY, MaxNIter, i, j);
                
        wt2 = omp_get_wtime();

        double elapsed_time = wt2 - wt1;
        printf("Chunk: %d, Time: %f sec\n", med_chunk, elapsed_time);
        
        // Evaluar el tiempo
        if (elapsed_time < best_time) {
            best_time = elapsed_time;
            best_chunk = med_chunk; // Guardar el mejor chunk encontrado
        }

        // Actualizar los límites
        if (elapsed_time < best_time) {
            max_chunk = med_chunk;
        } else {
            min_chunk = med_chunk;
        }
    }

    printf("Best chunk: %d, Best time: %f sec\n", best_chunk, best_time); 
    }
# endif
 #endif


 if (GenImage)
    { 
	 //Write to the output file
	 FOut=OpenFile(FileName,"wb");
	 
	 for (int i=0;i<Rows;i++)
	     for (int j=0;j<Cols;j++)
	         {
			  fwrite(&ppRed  [i][j],sizeof(char),(size_t)1,FOut);
			  fwrite(&ppGreen[i][j],sizeof(char),(size_t)1,FOut);
			  fwrite(&ppBlue [i][j],sizeof(char),(size_t)1,FOut);
	         }

	 CloseFile(FOut);

	 //rawtopgm and pnmtopng are in netpbm package
	 //Generate the comand to move from raw to png
	 sprintf(Command,"rawtoppm %d %d %s | pnmtopng > %s.png", 
	                 Cols,Rows, FileName, FileName);
	 printf("Command=%s\n",Command);
	 system(Command);
	 // eog from gnome visualize the image when it change
    }

 //Free memory for ppBlue, ppGreen and ppRed.
 Free2D((void **)ppBlue, Rows);
 Free2D((void **)ppGreen,Rows);
 Free2D((void **)ppRed,  Rows);
 
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
