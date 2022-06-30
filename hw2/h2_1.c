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
#include <ctype.h> 
#include "head.h"
struct Point
{
   int x, y, z;
};

struct Point pt[10]; 
float covarianceMatrix[3][3];  
void computeCovarianceMatrix() {
	double means[3] = {0, 0, 0};
	for (int i = 0; i < 10; i++)
		means[0] += pt[i].x,
		means[1] += pt[i].y,
		means[2] += pt[i].z;
	means[0] /= 10, means[1] /= 10, means[2] /= 10;
	
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			covarianceMatrix[i][j] = 0.0;
			for (int k = 0; k < 10; k++)
			{
				int a=0,b=0;
				if(i==0)
				{
					a=pt[k].x;

				}
				if(i==1)
				{
					a=pt[k].y;

				}
				if(i==2)
				{
					a=pt[k].z;

				}
				if(j==0)
				{
					b=pt[k].x;

				}
				if(j==1)
				{
					b=pt[k].y;

				}
				if(j==2)
				{
					b=pt[k].z;

				}
				covarianceMatrix[i][j] += (means[i] - a) *(means[j] - b);
				
             
             }                                             
			covarianceMatrix[i][j] /= 10 - 1;
		}	
}

int main(int argc, char* argv[],char *envp[])
{
		struct sigaction sa;
		memset(&sa, 0, sizeof(sa));

		sa.sa_flags = 0;
		sa.sa_handler = &handler;

		if(sigaction(SIGINT, &sa, NULL) == -1){
			fprintf(stderr, "err: sigaction\n");
			exit(EXIT_FAILURE);
		}

	int i=0,count=0,k=0;
	
	while(i< strlen(envp[0]) && !signal_flag)
	{

		char temp[3],flag=0,flag1=0;
		if(envp[0][i] =='(')
		{
			i++;
		}
		else if(envp[0][i] ==')')
		{
			i++;
		}
		else if(envp[0][i] ==' ')
		{
			i++;
		}
		else if(envp[0][i] ==',')
		{
			i++;
		}
		else
		{
			if(count==0)
			{

				temp[0]=envp[0][i];
				if(isdigit(envp[0][i+1]))
				{
					temp[1]=envp[0][i+1];
					flag=1;

				}
				if(isdigit(envp[0][i+2]))
				{
					temp[2]=envp[0][i+2];
					flag1=1;

				}
				if(flag==1 && flag1==1)
				{
					i =i+3;
				}
				if(flag==0 && flag1==1)
				{
					i =i+2;
				}
				if(flag==1 && flag1==0)
				{
					i =i+2;
				}
				if(flag==0 && flag1==0)
				{
					i =i+1;
				}
				pt[k].x=atoi(temp);
				count++;

			}
			else if(count==1)
			{

				temp[0]=envp[0][i];
				if(isdigit(envp[0][i+1]))
				{
					temp[1]=envp[0][i+1];
					flag=1;

				}
				if(isdigit(envp[0][i+2]))
				{
					temp[2]=envp[0][i+2];
					flag1=1;

				}
				if(flag==1 && flag1==1)
				{
					i =i+3;
				}
				if(flag==0 && flag1==1)
				{
					i =i+2;
				}
				if(flag==1 && flag1==0)
				{
					i =i+2;
				}
				if(flag==0 && flag1==0)
				{
					i =i+1;
				}
				pt[k].y=atoi(temp);
				count++;
				
			}
			else if(count==2)
			{

				temp[0]=envp[0][i];
				if(isdigit(envp[0][i+1]))
				{
					temp[1]=envp[0][i+1];
					flag=1;

				}
				if(isdigit(envp[0][i+2]))
				{
					temp[2]=envp[0][i+2];
					flag1=1;

				}
					if(flag==1 && flag1==1)
				{
					i =i+3;
				}
				if(flag==0 && flag1==1)
				{
					i =i+2;
				}
				if(flag==1 && flag1==0)
				{
					i =i+2;
				}
				if(flag==0 && flag1==0)
				{
					i =i+1;
				}
				pt[k].z=atoi(temp);
				count=0;
				k++;
				
			}
			
			
		}

	}
	if(!signal_flag)
	{
		computeCovarianceMatrix();



	struct flock lock;
	fprintf( stdout, "Created R_%d with %s\n",getpid()-getppid(),envp[0] );

	int fd2 = open(argv[0], O_WRONLY ,0666);
	if(fd2 == -1)
	{
		fprintf (stderr, "\nerrno = %d: %s (open outputPath)\n\n", errno, strerror (errno));
		exit(1);
	}
	memset(&lock,0,sizeof(lock));
	lock.l_type = F_WRLCK;
	fcntl(fd2, F_SETLKW, &lock);

   lseek(fd2, 0, SEEK_END);
   for (int i = 0; i < 3; i++)
	{
		char buf[20];
		

		for (int j = 0; j < 3; j++)
		{
			gcvt(covarianceMatrix[i][j], 6, buf);
			 write(fd2,buf,strlen(buf));
			 write(fd2," ",1);
		}

	}   
	write(fd2,"\n",1);

   

		lock.l_type = F_UNLCK;
		fcntl(fd2, F_SETLKW, &lock);
		close(fd2);

	}
	else
	{

	
		 if(sigaction(SIGINT,&sa,NULL) == 0)
	    		fprintf( stdout, "Terminate child procces: %d\n",getpid()); 
	    	signal_flag = 1;
	
	}

	



	
	//printf("%s\n",argv[0]);
}
