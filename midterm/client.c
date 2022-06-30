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
#include<sys/time.h>
#include <time.h>

#include "fifo_seqnum.h"
sig_atomic_t signal_flag = 0;
void handler (int signal_number){
	signal_flag = 1;
}

static char clientFifo[CLIENT_FIFO_NAME_LEN];
static void removeFifo(void)
{
unlink(clientFifo);
}
int commaCounter(char *str)
{
	int count=0;
	for(int i=0;i<strlen(str);i++)
	{
		if(str[i]==',')
		{
			count++;
		}
	}
	return count;
}
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

    
    if ((nchr = lseek (fd, *offset, SEEK_SET)) != -1)
        nchr = read (fd, buf, sz);
    close (fd);

    if (nchr == -1) {   
        fprintf (stderr, "%s() error: read failure in '%s'.\n",
                __func__, fn);
        return nchr;
    }
    if (nchr == 0) return -1;

    p = buf;    /* check each chacr */
    while (idx < nchr && *p != '\n') p++, idx++;
    *p = 0;

    if (idx == nchr) {  
        *offset += nchr;

        return nchr < (ssize_t)sz ? nchr : 0;
    }

    *offset += idx + 1;

    return idx;
}


int main(int argc, char *argv[])
{

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_flags = 0;
	sa.sa_handler = &handler;
	if(sigaction(SIGINT, &sa, NULL) == -1){
		fprintf(stderr, "err: sigaction\n");
		exit(EXIT_FAILURE);
	}
	struct timeval st, et;

	int serverFd, clientFd;
	struct request reqq;
	struct response resp;
	  time_t rawtime;
  struct tm * timeinfo;
	int parent = getpid();
	 time_t start,end;
  double dif;

	char buffer[1024];
	char buffer2[1024];
	char buffer1[1024][1024]={0};
	
	int i=0,k=0,z=0;
	ssize_t len = 0;
	 off_t offset = 0;
	int first = 0;
	int fd = open(argv[4], O_RDONLY);
	int counter[100];
	while((len = readline (buffer, 1024, argv[4], &offset)) != -1 && !signal_flag)
  	{
  				z=commaCounter(buffer);

  				if (first == 0)
  				{
  					first = z;
  				}

  				if (z > 0)
  				{
	  				strcat(buffer1[k],buffer);					
  					if (i == first)
  					{
  						strcat(buffer1[k],"\n");
  						counter[k]=i+1;
  						k += 1;
  						i = 0;
  						first = 0;
  					}
  					else {
  						strcat(buffer1[k],"\n");	
  						i++;
  					}



  				}


				memset(&buffer,0,sizeof(buffer));
				

  	}
  	int matrix_size = k;


	 i=0;
	 k=0;
	 z=0;
	int flagg = 0;
	int killchild[matrix_size];

	if(!signal_flag)
   {
		for (int n = 0; n < matrix_size; ++n)
		{
			if (parent == getpid() && !signal_flag)
			{
					pid_t child= fork();
				    if(parent != getpid() && !signal_flag)
				    {
				    	gettimeofday(&st,NULL);
				    		 
							struct request req;
							struct response resp;
					  		char clientFifo[CLIENT_FIFO_NAME_LEN];

									 umask(0);
							snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,(long) getpid());
							if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
								perror("mkfifo");
							if (atexit(removeFifo) != 0)
								perror("atexit");
								  req.pid = getpid();
								req.seqLen = strlen(buffer1[n]); //(argc > 1) ? getLong(buffer1, GN_GT_0, "seq-len") : 1;
								strcpy(req.buf,buffer1[n]);
								req.matrixcounter=counter[n];
								

								serverFd = open(argv[2], O_WRONLY);
								if (serverFd == -1)
									perror("open");
								if(!signal_flag)
   								{
									if (write(serverFd, &req, sizeof(struct request)) !=sizeof(struct request))
											perror("Can't write to server");


									clientFd = open(clientFifo, O_RDONLY);
									if (read(clientFd, &resp, sizeof(struct response))!= sizeof(struct response))
										perror("Can't read response from server");
								}
								time ( &rawtime );
	  							timeinfo = localtime ( &rawtime );
								fprintf(stdout,"Client output %s\n",asctime (timeinfo));
								gettimeofday(&et,NULL);
								int elapsed = (et.tv_sec - st.tv_sec);
								int elapse = (et.tv_usec - st.tv_usec);
								if(!signal_flag)
   								{
									if(resp.invertible==0)
									{
										fprintf(stdout,"Client PID# %d (%s) is submitting a %dx%d matrix\n",req.pid,argv[4],req.matrixcounter,req.matrixcounter);
										fprintf(stdout,"Client %d: the matrix is invertible,total time %.d.%d seconds,goodbye \n",req.pid,elapsed,elapse);

									}
									else
									{
										fprintf(stdout,"Client PID# %d (%s) is submitting a %dx%d matrix\n",req.pid,argv[4],req.matrixcounter,req.matrixcounter);
										fprintf(stdout,"Client %d: the matrix is invertible,total time %.d.%d seconds,goodbye \n",req.pid,elapsed,elapse);
									}
								}

								unlink(clientFifo);
								exit(EXIT_SUCCESS);
								break;

					}

			}

			



		}
	}
	if (getpid() == parent)
	{
		while(wait(NULL)>0);
	}
	
	return 0;
	
}
