#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h> 
#include<sys/time.h>
#include <time.h>
#include <math.h>
#include <sys/socket.h>
#include <arpa/inet.h>
struct ope{
    char *strRequest;
    int threadNumber;
    int reqsize;
    int portN;
    char* ipN;
    
};
static int arrived=0;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
ssize_t readline (char *buf, size_t sz, char *fn, off_t *offset);
void* threadShow(void *a);
int RequestSize(char *reqfile);
int main(int argc, char *argv[])
{
	char *reqfile=argv[2];
	char *ip=argv[6];
	int port=atoi(argv[4]);

	int reqSize=RequestSize(reqfile);


	int fd = open (reqfile, O_RDONLY);
	if(fd == -1)
     {
        perror("open");
        exit(1);
     }


	off_t offset = 0;
	char* buffer=(char*)malloc(sizeof(char)*100);
	ssize_t len = 0;

	pthread_t consthread[reqSize];
	 struct ope op[reqSize];
	 int i=0;
	while((len = readline (buffer,100, reqfile, &offset)) != -1)
  	{
  		if(buffer[0] !=' ' && buffer[0] !='\n' && buffer[0] !='\0')
  		{
  			op[i].threadNumber=i;
  			op[i].strRequest=(char*)malloc(sizeof(char )*100);
  			strcpy(op[i].strRequest,buffer);
  			op[i].reqsize=reqSize;
  			op[i].portN=port;
  			op[i].ipN=ip;
  			pthread_create(&consthread[i], NULL, &threadShow, &op[i] );
  			fprintf(stdout,"Client-Thread-%d: Thread-%d has been created\n",op[i].threadNumber,op[i].threadNumber);
  			i++;
  		}

  	}
  	for (int i = 0; i < reqSize; i++)
     {
     		 if(!pthread_equal(pthread_self(), op[i].threadNumber))
                {
                    pthread_join(consthread[i],NULL);
                }
     }

     fprintf(stdout,"All threads have terminated, goodbye.\n");
	free(buffer);

}

void* threadShow(void *a)
{

	 struct ope * info = (struct ope*)a;
	

        int socket1 = 0;
		struct sockaddr_in serverAddr; 

		if((socket1 = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		{
			perror("Socket Creation Error");
			exit(EXIT_FAILURE);
		}
		serverAddr.sin_family = AF_INET; 
		serverAddr.sin_port = htons(info->portN);
			if(inet_pton(AF_INET, info->ipN, &serverAddr.sin_addr)<=0)  
		{
			perror("Invalid Adress");
			exit(EXIT_FAILURE);
		}
		if(connect(socket1, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
		{
				perror("connection failed");
				exit(EXIT_FAILURE);
		}
		pthread_mutex_lock(&mutex1);
        ++arrived;
        if(arrived < info->reqsize)
        {
            pthread_cond_wait(&cond,&mutex1);
        }
        else
        {
            pthread_cond_broadcast(&cond);
        }
        
        pthread_mutex_unlock(&mutex1);

        fprintf(stdout,"I am requesting “%s”\n",info->strRequest);
        
        send(socket1, info->strRequest, strlen(info->strRequest), 0);




         fprintf(stdout,"Terminating %d\n",info->threadNumber);
        
        


        return 0;
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
int RequestSize(char *reqfile)
{
	int fd = open (reqfile, O_RDONLY);
	if(fd == -1)
     {
        perror("open");
        exit(1);
     }
	off_t offset = 0;
	char* buffer=(char*)malloc(sizeof(char)*100);
	ssize_t len = 0;
	int count=0;
	while((len = readline (buffer,100, reqfile, &offset)) != -1)
  	{
  		if(buffer[0] !=' ' && buffer[0] !='\n' && buffer[0] !='\0')
  		{

  			count++;

  		}

  	}
  	close(fd);
  	free(buffer);
  	return count;

}