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
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdatomic.h>
#include <sys/ipc.h>
#include<sys/time.h>
#include <time.h>
#include "fifo_seqnum.h"
#include <sys/mman.h>
#include<math.h>
#include <semaphore.h>
static int *glob_var;
static int *glob;
struct process {
pid_t rocIDs;
int x;
int z;
int m;
};
sig_atomic_t signal_flag = 0;
void handler (int signal_number){
	signal_flag = 1;
}

sem_t mutex;

static int *z_plus;
static int *child_plus;





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
char* parser(char *str)
{
		int i=0,k=0,z=0;

	char buffer[1024];
	char *buffer1 = (char*)malloc(1024*sizeof(char));
	
	
	while(str[i]!='\0')
		{
			
				while(str[i] !='\n')
				{
					 buffer1[k]=str[i];
					 k++;
					 i++;

				}
				
				z=commaCounter(buffer1);
				for (i = 0; i < z; ++i)
				{
							while(str[i] !='\n')
							{
								 buffer1[k]=str[i];
								 k++;
								 i++;

							}
							buffer[k]='\n';
					 
				}

				i++;
				break;
		}
		return buffer1;

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
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_flags = 0;
	sa.sa_handler = &handler;
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
int main(int argc, char *argv[],char *envp[])
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_flags = 0;
	sa.sa_handler = &handler;

	if(sigaction(SIGINT, &sa, NULL) == -1){
		fprintf(stderr, "err: sigaction\n");
		exit(EXIT_FAILURE);
	}
	time_t rawtime;
  struct tm * timeinfo;
	int fd=open(argv[0],O_WRONLY | O_APPEND,S_IWUSR);

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
	
	char* orkunn="Z:Server Z (";
	write(fd,orkunn,strlen(orkunn));

	write(fd,argv[0],strlen(argv[0]));

	char* orkunnn=", p=";
	write(fd,orkunnn,strlen(orkunnn));

	write(fd,argv[1],strlen(argv[1]));

	char* orkunnnn=", t=";
	write(fd,orkunnnn,strlen(orkunnnn));

	write(fd,argv[2],strlen(argv[2]));

	char* orkunnnnn=") started";
	write(fd,orkunnnnn,strlen(orkunnnnn));

	char* orkunnnnnn="\n";
	write(fd,orkunnnnnn,strlen(orkunnnnnn));


	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLKW,&lock);


	int serverFd, dummyFd, clientFd;
	int seqNum = 0;
	umask(0);
	if(mkfifo(argv[4], S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
		  perror("mkfifo ");
	
	serverFd = open(argv[4], O_RDONLY);
	if (serverFd == -1)
		  perror("open ");
	
	dummyFd = open(argv[4], O_WRONLY);
	if (dummyFd == -1)
		perror("open ");
	
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		perror("signal");
	int numberOfProcesses=atoi(argv[1]);
	struct process pro[numberOfProcesses];
	pid_t parentpid = getpid();
	
	 key_t key;
    char *virtualaddr;
    int get, put;
    int shmid;
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


	int pp;	
	int *p;

	pp = shmget(IPC_PRIVATE, numberOfProcesses*sizeof(int), IPC_CREAT | 0666);
	p = (int *) shmat(pp,NULL,0);

	z_plus = mmap(NULL, sizeof *z_plus,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	child_plus = mmap(NULL, sizeof *child_plus,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	*z_plus = 0;
	*child_plus = 0;

	int oo;	
	struct request *o;

	oo = shmget(IPC_PRIVATE, 50*sizeof(struct request), IPC_CREAT | 0666);
	o = (struct request*) shmat(oo,NULL,0);






    sem_init(&mutex,1,1);



    int i=1;

     i++;


if(!signal_flag)
{

	for(int i = 0; i < numberOfProcesses; i++){
		if (getpid() == parentpid)
		{
			z[i]=0;
			p[i]=0;

		    pid_t rocIDs = fork();
		    if(rocIDs < 0 ){
		        perror("Fork error!");
		        continue;	
		    }
		    else if (parentpid != getpid())
		    {
					x[i]=getpid();
		    }
		   
    	}

    }
}
    int keep_id;
    for (int i = 0; i < numberOfProcesses; ++i)
    {
    	if (getpid() == x[i])
    	{
    		keep_id = i;
    	}
    }

    int k=0;
  
  i=0;

  int cout=0;

    for (;;){
			char bf[256];
				if(signal_flag)
				{
						char* paras="(";
						write(fd,paras,strlen(paras));
						time ( &rawtime );
						timeinfo = localtime ( &rawtime );
						write(fd,asctime(timeinfo),strlen(asctime (timeinfo))-1);
						char* para11=") ";
						write(fd,para11,strlen(para11));

						char* orkunaa="Z:SIGINT received, exiting server Z. Total requests handled ";
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
        if(parentpid==getpid() && *z_plus==*child_plus && !signal_flag)
       {
		       		i=0;
		       		cout++;
		       		struct request req;

						//read(atoi(argv[3]), &bf, 256) ;
		       	int pipes = atoi(argv[3]);
		       	 *glob_var=*glob_var+1;
		       	if(!signal_flag)
		       	{
									if (read(pipes, &req, sizeof(struct request)) != sizeof(struct request)) {
														fprintf(stderr, "Error reading request; discarding\n");
														continue;
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

									char* orkuna="Z:Worker PID#";
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
									if(cout >numberOfProcesses)
									{
										cout=numberOfProcesses;

									}
									sprintf(strr4,"%d",cout);
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



								o[*z_plus].pid=req.pid;
								for (int i = 0; i < strlen(req.buf); ++i)
								{
										o[*z_plus].buf[i]=req.buf[i];
									
								}
								*z_plus=*z_plus+1;

									
				}		
				 if(parentpid==getpid() && *z_plus==(*child_plus +1) && !signal_flag )
       {

								int flag =0;
								i = 0;
								while (flag == 0)
								{
									if (i == numberOfProcesses)
									{
										i = i % numberOfProcesses;
									}
									if (z[i] == 0)
									{
				        		*child_plus = *child_plus + 1;
										z[i] = 1;
										flag += 1; 
									}
										i++;
								}

				}			




       
       else if (z[keep_id] == 1 && !signal_flag)
       {	

       		int indeks = *child_plus - 1;

       		char clientFifo[CLIENT_FIFO_NAME_LEN];
					// struct request req;
					struct response resp;
       		struct request newReq;
       		if(!signal_flag)
	       		{//	sem_wait(&mutex);
			       	snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,(long) o[indeks].pid);
			       	clientFd = open(clientFifo, O_WRONLY);
							if (clientFd == -1) {
									perror("open mk ");
									continue;
								}
								resp.invertible=matrixx(o[indeks].buf);
								resp.seqNum = seqNum;
								// strcpy(resp.buf1 , req.buf);
						}	
						if(!signal_flag)
	       		{
			       		sleep(atoi(argv[2]));
			       
			      }	 		
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
							char* orkun="Z:Worker PID#";
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
								seqNum += newReq.seqLen;
									 
		       		
		       		
			       		z[keep_id]=0;

					}
					*glob=*glob+1;

       		


       }
       
       

	}

	
if (getpid() == parentpid)
	{
		while(wait(NULL)>0);
	}
	
				
	shmctl(xx,IPC_RMID,NULL);

	shmdt((void *)z);
	shmctl(zz,IPC_RMID,NULL);

	shmdt((void *)p);
	shmctl(pp,IPC_RMID,NULL);

	shmdt((void *)o);
	shmctl(oo,IPC_RMID,NULL);
	
	close(serverFd);
	close(dummyFd);
	close(fd);
	
	return 0;
    

	



}





