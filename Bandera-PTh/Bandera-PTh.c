#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include "stdbool.h"


#define PRINT 1

/*---------------------------------------------------------------------------*/
//TODO: the structure can be modified.
typedef struct th_bandera_args
{
 char ** 	ppRed;
 char ** 	ppGreen;
 char ** 	ppBlue;
 int 		Rows;
 int 		Cols;
 int		GlobalRows;
 int		GlobalCols;
 int		Starti;
 int 		Endi;
 int 		Startj;
 int 		Endj;
 int 		ThreadId;
} Type_Th_Bandera_Args;


/*---------------------------------------------------------------------------*/
void * Th_Bandera_Function (void * pTh_Args)
{
 Type_Th_Bandera_Args * pMyData;
 
 pMyData=(Type_Th_Bandera_Args *) pTh_Args;
 
 //TODO: Print the assigned rows and columns
 #if (PRINT==1)
 printf("I am thread %d doing %d rows (%d,%d) and %d columns (%d,%d).\n", 
        pMyData->ThreadId, pMyData->Rows, pMyData->Starti, pMyData->Endi, pMyData->Cols, 
        pMyData->Startj, pMyData->Endj);
 #endif
 
 /* TODO: update the loops and references to ppRed, ppGreen and ppBlue
  for (int i= ?; i< ?; i ?)
     for (int j=?; j<?; j ?)
         {
          ppRed[i][j]=(char)255;
          if (i>Rows/4 && i<Rows*3/4)
             ppGreen[i][j]=(char)255;
         } 
 */
 
 int iniR= pMyData->Starti;
 int finR= pMyData->Endi;
 int iniC= pMyData->Startj;
 int finC= pMyData->Endj;
 int Row= pMyData->GlobalRows;
 for(int i=iniR; i<finR;i++)
    for(int j=iniC; j<finC;j++){
       pMyData->ppRed[i][j]=(char)255;
       if (i>Row/4 && i<Row*3/4)
          pMyData->ppGreen[i][j]=(char)255;
    }
 
 pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -c <n columns>		");
 puts("\t  -nt <Number of threads");
 puts("\t  .[-o <file>			");

 exit(0);
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int 		Rows, Cols;
 char ** 	ppRed, ** ppGreen, ** ppBlue;  //Matrices de RGB (0..255)
 char *		FileName;
 FILE * 	FOut;
 char 		Command[256];
 bool 		GenImage=false;  	//if output image is generated.
 int 		NThreads; 			//Number of threads.
 pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory. 
 //Vector of structs with data to threads. Dinamic memory allocation.
 Type_Th_Bandera_Args *pTh_Args=NULL;
 int rc;		//Return value  of pthread_join 
 void *status;	//status returned in pthread_join.
 
printf("1");
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


 if (!ExistArg("-nt",argc,argv))
    {
    fputs("Parameter -nt is neccesary.\n",stderr);
    ParametersError();
    }
 else
    {
     NThreads = atoi(GetArg("-nt",argc,argv)); 
     if (NThreads <1)
        {
         puts("NThreads must be > 0.");
         exit(1);
        }
    }  


 #if (PRINT==1)
 printf("Rows=%d, Cols=%d, Output=%s.\n", 
         Rows, Cols, FileName);
 #endif
 
 printf("Bandera-PTh main: NThreads=%d.\n",NThreads);

 //Get mem for threads
 pThreads = (pthread_t *) GetMem(NThreads, sizeof(pthread_t), 
                                 "Main: pThreads");
 pTh_Args = (Type_Th_Bandera_Args *) GetMem(NThreads, 
                                            sizeof(Type_Th_Bandera_Args),
                                            "Main: pTh_Data");

 /*Calloc de Getmen put data to zeroes*/
 //Get mem for Red, Green and Blue arrays.
 ppRed   = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppRed");
 ppGreen = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppGreen");
 ppBlue  = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppBlue");
 
 
 int auxC=0;
 //We create the Threads
  for (int t=0; t<NThreads; t++) 
      {
       pTh_Args[t].ThreadId=t;
       //TODO: Set the other values of pTh_Args. You can change them.
       //Balance Static.
       //All threads must do the almost same amount of work
       //Every pixel must be done.
       //Each pixel is visited just once.
       printf("1");
       pTh_Args[t].Rows= Rows/NThreads;
       pTh_Args[t].Cols= Cols/NThreads;
       pTh_Args[t].Starti= 0;
       pTh_Args[t].Startj= auxC;
       pTh_Args[t].Endi=Rows;
       pTh_Args[t].Endj= auxC + Cols/NThreads;
       pTh_Args[t].ppRed= ppRed;
       pTh_Args[t].ppGreen= ppGreen;
       pTh_Args[t].ppBlue= ppBlue;
       pTh_Args[t].GlobalRows=Rows;
       pTh_Args[t].GlobalCols=Cols;
       auxC+=Cols/NThreads;
       //By default threads are joinable.
       rc = pthread_create(&pThreads[t],NULL, 
                           Th_Bandera_Function, (void *)&pTh_Args[t]);
      }
  printf("1");
 //Wait all threads end.
 for (int t=0; t<NThreads; t++) 
     {
      //Compleately Static: I wait for threads in order.
      rc = pthread_join(pThreads[t], &status);
      if (rc) 
         {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
         }
      #if (PRINT==1)
      printf("Main: thread %d join with status %d\n",t,*(int *)status);
      #endif
     }


 if (GenImage)
    {
	 FOut=OpenFile(FileName,"wb");
	 for (int i=0;i<Rows;i++)
		 for (int j=0;j<Cols;j++)
		     {
		      fwrite(&ppRed  [i][j],sizeof(char),(size_t)1,FOut);
			  fwrite(&ppGreen[i][j],sizeof(char),(size_t)1,FOut);
			  fwrite(&ppBlue [i][j],sizeof(char),(size_t)1,FOut);
		     }

	 CloseFile(FOut);
	 
	 //rawtoppm and pnmtopng are in netpbm package
	 sprintf(Command,"rawtoppm %d %d %s | pnmtopng > %s.png", 
		             Cols,Rows, FileName, FileName); 

	 printf("Command=%s\n",Command);
	 //eog from gnome visualize the image when it change
	 system(Command);
    }

 Free2D((void **)ppBlue, Rows);
 Free2D((void **)ppGreen,Rows);
 Free2D((void **)ppRed,  Rows);
 
 free((void *)pThreads);
 free((void *)pTh_Args);

 
 pthread_exit(NULL);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
