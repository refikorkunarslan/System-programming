#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>
#include "head.h"
#define SIZE 30
#define BLKSIZE 1024


char* inputPath = NULL;
char* outputPath = NULL;
int fd,fd2;
pid_t child_pid = -1;
const int row = 3, col = 3;
ssize_t readline (char *buf, size_t sz, char *fn, off_t *offset)
{
    int fd = open (fn, O_RDONLY);
    if (fd == -1) {
        fprintf (stderr, "%s() error: file open failed '%s'.\n",
                __func__, fn);
        return -1;
    }

    ssize_t nchr = 0;
    ssize_t idx = 0;
    char *p = NULL;

    /* position fd & read line */
    if ((nchr = lseek (fd, *offset, SEEK_SET)) != -1)
        nchr = read (fd, buf, sz);
    close (fd);

    if (nchr == -1) {   /* read error   */
        fprintf (stderr, "%s() error: read failure in '%s'.\n",
                __func__, fn);
        return nchr;
    }

    /* end of file - no chars read
    (not an error, but return -1 )*/
    if (nchr == 0) return -1;

    p = buf;    /* check each chacr */
    while (idx < nchr && *p != '\n') p++, idx++;
    *p = 0;

    if (idx == nchr) {  /* newline not found  */
        *offset += nchr;

        /* check file missing newline at end */
        return nchr < (ssize_t)sz ? nchr : 0;
    }

    *offset += idx + 1;

    return idx;
}
float frobeniusNorm(float mat[row][col],float mat1[row][col])
{
 
    float sumSq = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sumSq += pow(mat[i][j]-mat1[i][j], 2);
        }
    }
 
    float res = sqrt(sumSq);
    return res;
}
int size_file(char *path)
{
	struct stat filestat;
	lstat(path,&filestat);
	return filestat.st_size;
}
int main(int argc, char* argv[])
{

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_flags = 0;
	sa.sa_handler = &handler;

	if(sigaction(SIGINT, &sa, NULL) == -1){
		fprintf(stderr, "err: sigaction\n");
		exit(EXIT_FAILURE);
	}

	 inputPath = argv[2];
	 outputPath=argv[4];
	 fd = open(inputPath, O_RDONLY);
	 if(fd == -1)
	 {
		fprintf (stderr, "\nerrno = %d: %s (open inputPath)\n\n", errno, strerror (errno));
		exit(1);
	 }
	fprintf( stdout, "Process P reading %s\n",argv[2] );

    int processSize=size_file(argv[2])/30;


	pid_t pid[processSize];
 	pid_t  ppid;
 	ppid = getpid();
	
	int bytesread;
	int i = 0;
	int c=0;
	int ff=0;
	while(1 && !signal_flag)
	{
		ff=1;
		char wbuf[SIZE] = "";
		char buf1[BLKSIZE] = "";

		char temp[BLKSIZE] = "";
		bytesread = read(fd, wbuf, SIZE);
		
		if(bytesread<30)
		{
			break;
		}


		for(c = 0; c < SIZE; c += 3)
		{
			sprintf(temp, "(%d, %d, %d), ", (int)((unsigned char) wbuf[c]), (int)((unsigned char) wbuf[c+1]),(int)((unsigned char) wbuf[c+1])        );
			strcat(buf1,temp);
		}

		
		char *bff[]={buf1,NULL};
	     pid_t cur_pid = fork();
	     if(cur_pid == 0) {
	            pid[i] = cur_pid;
	           execve("h2_1",&argv[4],&bff[0]);
	            exit(0);
	       }
	       i++;




      }
  
    for(int k = 0; k < i; k++)
        wait(&pid[k]);
	if(signal_flag && ff==0)
	{
		fprintf( stdout, "The process was stopped before it was created. \n" );
 		kill(ppid, SIGINT);

	}
   char bfer[1024];
   float covarianceMatrix[processSize][3][3]; 
   int fd2;
   int r=0;
   int cc=0;
   int kk=0,zz=0,tt=0;
   int len=0;
   off_t offset = 0;
   if(!signal_flag)
   {
	    fd2 = open(argv[4], O_RDONLY,0666);
	   	if(fd2 == -1)
	   	{
	   		fprintf (stderr, "\nerrno = %d: %s (open outputPath)\n\n", errno, strerror (errno));
	   		exit(1);
	   	}
	   

		 while ((len = readline (bfer, 1024, argv[4], &offset)) != -1 && !signal_flag)
		{
			char temp[10];
			r=0;
			zz=0;
			cc=0;
			while(r<len)
			{
				if(bfer[r] ==' ')
				{
					covarianceMatrix[kk][zz][cc]=atof(temp);
					cc++;
					if(cc==3)
					{
						 memset(temp, ' ', 10);

						zz++;
						cc=0;
					}
					tt=0;

				}
				else
				{
					temp[tt]=bfer[r];
					tt++;
				}
				r++;
			}
			kk++;




		}
		if(!signal_flag)
		 {
			fprintf( stdout, "Reached EOF, collecting outputs from %s\n",argv[4] );
			float min=9999,mtemp=0;
			int pr1=0,pr2=0;
		
			for (int i = 0; i < processSize; i++)
			{

				for (int j = i+1; j < processSize; j++)
					{
						mtemp=frobeniusNorm(covarianceMatrix[i],covarianceMatrix[j]);
						if(mtemp<min)
						{
							pr1=i;
							pr2=j;
							min=mtemp;
						}
					}
			
			}
			fprintf( stdout, "The closest 2 matrices are R_%d and R_%d ,and their distance %.2f\n",pr1,pr2,min );
		}
	

		
            	

	    
       
   }


}
