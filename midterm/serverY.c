#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include<sys/time.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "fifo_seqnum.h"
#include <math.h>
#include <sys/mman.h>
static int *glob_var;
static int *glob;
sig_atomic_t signal_flag = 0;
void handler (int signal_number){
	signal_flag = 1;
}

struct process {
pid_t rocIDs;
int x; /* keep the state for work or sleep*/
int z;
};
static void skeleton_daemon()
{
    pid_t pid;
    
    /* Fork off the parent process */
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
     /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    
   
    
    
    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
    
}
int determinant(int a[25][25], int k)
{

  int s = 1, det = 0, b[25][25];
  int i, j, m, n, c;
  if (k == 1)

    {

     return (a[0][0]);

    }

  else

    {

     det = 0;

     for (c = 0; c < k; c++)

       {

        m = 0;

        n = 0;

        for (i = 0;i < k; i++)

          {

            for (j = 0 ;j < k; j++)

              {

                b[i][j] = 0;

                if (i != 0 && j != c)

                 {

                   b[m][n] = a[i][j];

                   if (n < (k - 2))

                    n++;

                   else

                    {

                     n = 0;

                     m++;

                     }

                   }

               }

             }

          det = det + s * (a[0][c] * determinant(b, k - 1));

          s = -1 * s;

          }

    }

 

    return (det);

}


int matrixx(char *str)
{
	int i=0,k=0,l=0,z=0;
	int mat[25][25];
	while(i< strlen(str))
	{
		char temp[3];
		if(str[i] !=',' && str[i] !='\n' && str[i] !=' ')
		{
			temp[z]=str[i];
			z++;
		}
		else if(str[i] ==',')
		{
			mat[k][l]=atoi(temp);
			memset(temp, ' ', 3);
			z=0;
			l++;		
		}
		else if(str[i] =='\n')
		{
			z=0;
			mat[k][l]=atoi(temp);
			memset(temp, ' ', 3);
			k++;
			l=0;
		}
		
		i++;

	}
	int s;
	s=determinant(mat,k);
	return s;
	
}

struct flock lock;

int main(int argc, char *argv[])
{
	skeleton_daemon();
	time_t rawtime;
  struct tm * timeinfo;
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_flags = 0;
	sa.sa_handler = &handler;

	if(sigaction(SIGINT, &sa, NULL) == -1){
		fprintf(stderr, "err: sigaction\n");
		exit(EXIT_FAILURE);
	}
	int numberOfProcesses=atoi(argv[6]);
	int pfd[numberOfProcesses+2][2];
	char buf[1024];
	ssize_t numRead;
	int hmpipe=0;
	while(hmpipe<numberOfProcesses+2)
	{
		if (pipe(pfd[hmpipe]) == -1)
			perror("pipe");
		hmpipe++;
	}
	char pipeExec[10];
	char word1[10];
	char word2[10];
	char word3[10];
	char word4[10];
	char* envp[] = { NULL };
	sprintf(pipeExec,"%d",pfd[numberOfProcesses+1][0]);
	strcpy(word1,argv[4]);
	strcpy(word2,argv[8]);
	strcpy(word3,argv[10]);
	strcpy(word4,argv[2]);
	if(!signal_flag)
	{
		pid_t execpid=fork();
		if(execpid==0)
		{
			char* arg[] = { word1,word2,word3,pipeExec,word4,NULL };
			 int ret =execve("./serverZ",arg,envp);
			if(ret ==-1)
			{
				perror("execve failed");
			}
		}
	}	
	int fd=open(argv[4],O_WRONLY | O_CREAT| O_TRUNC |O_APPEND,0644);
		
	if(!signal_flag)
	{
			memset(&lock,0,sizeof(lock));
			lock.l_type=F_WRLCK;
			fcntl(fd,F_SETLKW,&lock);

			char* para="(";
			write(fd,para,strlen(para));
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			write(fd,asctime(timeinfo),strlen(asctime (timeinfo))-1);

			char* para1=") ";
			write(fd,para1,strlen(para1));
			
			char* orkunn="Server Y (";
			write(fd,orkunn,strlen(orkunn));

			write(fd,argv[4],strlen(argv[4]));

			char* orkunnn=", p=";
			write(fd,orkunnn,strlen(orkunnn));

			write(fd,argv[6],strlen(argv[6]));

			char* orkunnnn=", t=";
			write(fd,orkunnnn,strlen(orkunnnn));

			write(fd,argv[10],strlen(argv[10]));

			char* orkunnnnn=") started";
			write(fd,orkunnnnn,strlen(orkunnnnn));

			char* orkunnnnnn="\n";
			write(fd,orkunnnnnn,strlen(orkunnnnnn));


			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLKW,&lock);



			
	}
	memset(&lock,0,sizeof(lock));
			lock.l_type=F_WRLCK;
			fcntl(fd,F_SETLKW,&lock);
	char* para="(";
	write(fd,para,strlen(para));
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	write(fd,asctime(timeinfo),strlen(asctime (timeinfo))-1);

	char* para1=") ";
	write(fd,para1,strlen(para1));
	char* orkun="Instantiated server Z";
	write(fd,orkun,strlen(orkun));
	char* orkunnnnnnn="\n";
	write(fd,orkunnnnnnn,strlen(orkunnnnnnn));
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLKW,&lock);
		
	pid_t parentpid = getpid();
	//struct process pro[numberOfProcesses];
	glob_var=mmap(NULL,sizeof *glob_var,PROT_READ|PROT_WRITE,MAP_SHARED |MAP_ANONYMOUS,-1,0);
 *glob_var=0;

  glob=mmap(NULL,sizeof *glob,PROT_READ|PROT_WRITE,MAP_SHARED |MAP_ANONYMOUS,-1,0);
 *glob=0;
	
		int xx;	
		int *x;

		xx = shmget(IPC_PRIVATE, numberOfProcesses*sizeof(int), IPC_CREAT | 0666);
		x = (int *) shmat(xx,NULL,0);

		int zz;	
		int *z;

		zz = shmget(IPC_PRIVATE, numberOfProcesses*sizeof(int), IPC_CREAT | 0666);
		z = (int *) shmat(zz,NULL,0);
	

	for(int i = 0; i < numberOfProcesses; i++){
		if (getpid() == parentpid && !signal_flag)
		{
			z[i]=0;
		    pid_t rocIDs = fork();
		    if(rocIDs < 0 ){
		        perror("Fork error!");
		        continue;	
		    }
		    else if (getpid() == parentpid && !signal_flag){
		    	if (close(pfd[i][0]) == -1)
						perror("close - parent");
		    }
		    else if(rocIDs >= 0 && !signal_flag)
		    {

		    	x[i]=getpid();
		    	if (close(pfd[i][1]) == -1)
					perror("close - child");


		    }
    	}

    }


	int serverFd, dummyFd, clientFd;
	int seqNum = 0;
	umask(0);
	if(mkfifo(argv[2], S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
		  perror("mkfifo ");
	
	serverFd = open(argv[2], O_RDONLY);
	if (serverFd == -1)
		  perror("open ");
	
	dummyFd = open(argv[2], O_WRONLY);
	if (dummyFd == -1)
		perror("open ");
	
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		perror("signal");
	int i=0;
	int k=0;
	int m=0;

	int say ;
	for (int i = 0; i < numberOfProcesses; ++i)
	{
		if (getpid() == x[i])
		{
			say = i;
		}
	}
	i = 0;
	
	int count=1;
	int ssss=1;
	for (;;){
		if(signal_flag)
		{
					char* paras="(";
						write(fd,paras,strlen(paras));
						time ( &rawtime );
						timeinfo = localtime ( &rawtime );
						write(fd,asctime(timeinfo),strlen(asctime (timeinfo))-1);
						char* para11=") ";
						write(fd,para11,strlen(para11));

						char* orkunaa="SIGINT received, exiting server Z. Total requests handled ";
						write(fd,orkunaa,strlen(orkunaa));


						char strrr[10];
						sprintf(strrr,"%d",*glob_var);
						write(fd,strrr,strlen(strrr));

						char* orkunaaa=", ";
						write(fd,orkunaaa,strlen(orkunaaa));

						char strrrr[10];
						sprintf(strrrr,"%d",*glob);
						write(fd,strrrr,strlen(strrrr));

						char* orkunaaaaaa=" invertible, ";
						write(fd,orkunaaaaaa,strlen(orkunaaaaaa));

						char strrrrr[10];
						sprintf(strrrrr,"%d",*glob_var-*glob);
						write(fd,strrrrr,strlen(strrrrr));


						char* orkunaaaa=" , not invertible.";
						write(fd,orkunaaaa,strlen(orkunaaaa));





							char* orkun6="\n";
									write(fd,orkun6,strlen(orkun6));


			
			break;
		}
       if(parentpid==getpid() && !signal_flag)
       {
    		
			struct request req;
			i=0;
			*glob_var=*glob_var+1;
			if(!signal_flag)
			{	
				if (read(serverFd, &req, sizeof(struct request)) != sizeof(struct request)) {
										fprintf(stderr, "Error reading request; discarding\n");
										continue;
								}
			}
			int stop_turn = 1;

			while(i<numberOfProcesses && !signal_flag)
			{
				if(z[i]==0)
				{
					z[i]=1;	
					stop_turn = 0;
					if(!signal_flag)
					{
							if (write(pfd[i][1], &req, sizeof(struct request)) !=sizeof(struct request))
										perror("parent - partial/failed write");
					

						memset(&lock,0,sizeof(lock));
						lock.l_type=F_WRLCK;
						fcntl(fd,F_SETLKW,&lock);

								char* para="(";
					write(fd,para,strlen(para));
					time ( &rawtime );
					timeinfo = localtime ( &rawtime );
					write(fd,asctime(timeinfo),strlen(asctime (timeinfo))-1);

					char* para1=") ";
					write(fd,para1,strlen(para1));



						char* orkuna="Worker PID#";
						write(fd,orkuna,strlen(orkuna));

						char strr[10];
						sprintf(strr,"%d",getpid());
						write(fd,strr,strlen(strr));

						char* orkun1=" is handling client PID#";
						write(fd,orkun1,strlen(orkun1));

						char strr1[10];
						sprintf(strr1,"%d",req.pid);
						write(fd,strr1,strlen(strr1));

						char* orkun2=" matrix size ";
						write(fd,orkun2,strlen(orkun2));

						char strr2[10];
						sprintf(strr2,"%d",req.matrixcounter);
						write(fd,strr2,strlen(strr2));

						char* orkun3="X";
						write(fd,orkun3,strlen(orkun3));


						char strr3[10];
						sprintf(strr3,"%d",req.matrixcounter);
						write(fd,strr3,strlen(strr3));

						char* orkun4=", pool busy ";
						write(fd,orkun4,strlen(orkun4));

						char strr4[10];
						if(count >numberOfProcesses)
						{
							count=numberOfProcesses;

						}
						sprintf(strr4,"%d",count);
						write(fd,strr4,strlen(strr4));
						count++;

						char* orkun5="/";
						write(fd,orkun5,strlen(orkun5));

						char strr5[10];
						sprintf(strr5,"%d",numberOfProcesses);
						write(fd,strr5,strlen(strr5));

						char* orkun6="\n";
						write(fd,orkun6,strlen(orkun6));








						lock.l_type=F_UNLCK;
						fcntl(fd,F_SETLKW,&lock);

					}

					break;
				}
				i++;
					
			}	
			if(i==numberOfProcesses || stop_turn == 1)
			{
				if(!signal_flag)
				{
					if (write(pfd[numberOfProcesses+1][1],&req, sizeof(struct request)) !=sizeof(struct request))
											perror("parent - partial/failed write");
						memset(&lock,0,sizeof(lock));
						lock.l_type=F_WRLCK;
						fcntl(fd,F_SETLKW,&lock);

					char* para="(";
					write(fd,para,strlen(para));
					time ( &rawtime );
					timeinfo = localtime ( &rawtime );
					write(fd,asctime(timeinfo),strlen(asctime (timeinfo))-1);

					char* para1=") ";
					write(fd,para1,strlen(para1));

						char* orkunas="Forwarding request of client PID#";
						write(fd,orkunas,strlen(orkunas));

						char strr[10];
						sprintf(strr,"%d",req.pid);
						write(fd,strr,strlen(strr));

						char* orkun1="to serverZ, matrix size ";
						write(fd,orkun1,strlen(orkun1));

						char strr2[10];
						sprintf(strr2,"%d",req.matrixcounter);
						write(fd,strr2,strlen(strr2));

						char* orkun3="X";
						write(fd,orkun3,strlen(orkun3));


						char strr3[10];
						sprintf(strr3,"%d",req.matrixcounter);
						write(fd,strr3,strlen(strr3));
						
						char* orkun4=", pool busy ";
						write(fd,orkun4,strlen(orkun4));

						
						char strr4[10];
						sprintf(strr4,"%d",numberOfProcesses);
						write(fd,strr4,strlen(strr4));
						
						char* orkun5="/";
						write(fd,orkun5,strlen(orkun5));

						char strr5[10];
						sprintf(strr5,"%d",numberOfProcesses);
						write(fd,strr5,strlen(strr5));

						char* orkun6="\n";
							write(fd,orkun6,strlen(orkun6));


							lock.l_type=F_UNLCK;
							fcntl(fd,F_SETLKW,&lock);
				}

				
			}
			  

       }
       else if ( z[say]==1 && !signal_flag)
       {	

			char clientFifo[CLIENT_FIFO_NAME_LEN];
			// struct request req;
			struct response resp;
       		struct request newReq;
       		
       		if(!signal_flag)
       		{
				if (read(pfd[say][0], &newReq, sizeof(struct request)) != sizeof(struct request)) {
						fprintf(stderr, "Error reading request; discarding\n");
						// continue;
				}
       		}
       		snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,(long) newReq.pid);
       		if(!signal_flag)
       		{
				clientFd = open(clientFifo, O_WRONLY);
				if (clientFd == -1) {
					perror("open ");
					continue;
				}

			resp.invertible=matrixx(newReq.buf);
			}
			resp.seqNum = seqNum;
			if(!signal_flag)
			{
					memset(&lock,0,sizeof(lock));
					lock.l_type=F_WRLCK;
					fcntl(fd,F_SETLKW,&lock);

					char* para="(";
					write(fd,para,strlen(para));
					time ( &rawtime );
					timeinfo = localtime ( &rawtime );
					write(fd,asctime(timeinfo),strlen(asctime (timeinfo))-1);

					char* para1=") ";
					write(fd,para1,strlen(para1));

					char* orkun="Worker PID#";
					write(fd,orkun,strlen(orkun));

					char strr[10];
					sprintf(strr,"%d",getpid());
					write(fd,strr,strlen(strr));

					char* orkun1=" responding to client PID#";
					write(fd,orkun1,strlen(orkun1));

					char strr1[10];
					sprintf(strr1,"%d",newReq.pid);
					write(fd,strr1,strlen(strr1));

					char* orkun10=": the matrix IS";
					write(fd,orkun10,strlen(orkun10));
					if(resp.invertible==0)
					{
						char* orkun2=": NOT invertible.";
						write(fd,orkun2,strlen(orkun2));

					}
					else
					{
						char* orkun2=": invertible.";
						write(fd,orkun2,strlen(orkun2));

					}
					char* orkun11="\n";
						write(fd,orkun11,strlen(orkun11));


						lock.l_type=F_UNLCK;
						fcntl(fd,F_SETLKW,&lock);

			}
			if(!signal_flag)
			{
				if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
					fprintf(stderr, "Error writing to FIFO %s\n", clientFifo);
				if (close(clientFd) == -1)
					perror("close");
				*glob=*glob+1;
			}
			seqNum += newReq.seqLen;
			if(!signal_flag)
			{
       		
	       		sleep(atoi(argv[10]));
	       		z[i]=0;

       		}
				
       		


       }
       
	}
	if (getpid() == parentpid)
	{
		while(wait(NULL)>0);
	}
	shmdt((void *)z);
	shmctl(zz,IPC_RMID,NULL);

	shmdt((void *)z);
	shmctl(zz,IPC_RMID,NULL);
	close(serverFd);
	close(dummyFd);
	close(fd);
	return 0;



}
