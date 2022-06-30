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
#include <limits.h>
struct ope{
    char *strRequest;
    int threadNumber;
    int reqsize;
    int portN;
    char* ipN;
    
};
struct serverStruct{
    int serverThreadNumber;
};
int ddx;
int num=0;
char buff[100];
int socket2[100] ;
int socket1=-1;
ssize_t readline (char *buf, size_t sz, char *fn, off_t *offset);
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void* serverPool(void *a);

typedef struct node{
	char array[100];
	struct node *next;
}node;

typedef struct{
	node *front;
	node *rear;
}Queue;

int flag=0;
int empty(Queue *q)
{

	if(q->front==NULL)
		return 0;
	else
		return 1;
}
void enqueue(Queue *q, char* item)
{
	node* tempNode = (node *) malloc(sizeof(node));
	tempNode->next = NULL;
	strcpy(tempNode->array, item);

	if(empty(q)!=0)
		q->rear->next = tempNode;
	else
		q->front = tempNode;
	
	q->rear = tempNode;
}


char* dequeue(Queue *q)
{										
	char *client=malloc(sizeof(char*)*100);
	node *tempNode;

	if(empty(q)!=0)
	{
		strcpy(client,q->front->array);
		tempNode = q->front;
		q->front = q->front->next;
		
		free(tempNode);
		
		if(q->front==NULL)
			q->rear=NULL;
	}
	return client;
}
Queue queue1;
int main(int argc, char *argv[])
{

	queue1.front = NULL;
	queue1.rear = NULL;
	int port=atoi(argv[2]);
	int opt1 = 1;
	int numberThreads=atoi(argv[4]);
    pthread_t consthread[numberThreads];
    struct serverStruct ss[numberThreads];
	for(int i = 0 ; i < numberThreads; i++)
	{
		pthread_create(&consthread[i], NULL, &serverPool, &ss[i] ); 
		
	}



	struct sockaddr_in serverAddr;
	if((socket1 = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{
		perror("Socket failed");
		exit(EXIT_FAILURE); 
	} 
	if(setsockopt(socket1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt1, sizeof(opt1))) 
	{
		perror("Setsockopt Failed");
		exit(EXIT_FAILURE); 
	}
	serverAddr.sin_family = AF_INET;; 
	serverAddr.sin_addr.s_addr = INADDR_ANY; 
	serverAddr.sin_port = htons(port);
	int addrlen = sizeof(serverAddr);
	if(bind(socket1, (struct sockaddr *)&serverAddr, sizeof(serverAddr))<0) 
	{
		perror("Bind Failed");
		exit(EXIT_FAILURE); 
	}

	if (listen(socket1, 4096) < 0) 
	{
		perror("Listen Failed");
		exit(EXIT_FAILURE); 
	}

	
	int sck;

	for (;;) {
		
		addrlen = sizeof(serverAddr);
		sck=accept(socket1, (struct sockaddr *)&serverAddr, (socklen_t*)&addrlen);
		if(sck ==-1)
		{
			perror("Accept");
			exit(EXIT_FAILURE);
		}
		char reqLenStr[100];
		memset(reqLenStr,0,100);
		read(sck, reqLenStr, 100);
		pthread_mutex_lock(&mutex1);
		pthread_mutex_unlock(&mutex1);
		if(reqLenStr[0]=='t')
		{
				fprintf(stdout,"Request arrived “%s”\n",reqLenStr);
			char tran[100];
			char place[100];
			char BeginDate[100];
			char EndDate[100];
			char city[100];
			char space=' ';
			enqueue(&queue1,reqLenStr);
				pthread_cond_signal(&cond);
			sscanf(reqLenStr,"%s%c%s%c%s%c%s%c%s", tran,&space,place,&space,BeginDate,&space,EndDate,&space,city);
			
			

		}
		else
		{
			char space=' ';
			char mode;
			char between[100];
			char between1[100];
			char servantPort[100];
			char ip[50];
			sscanf(reqLenStr,"%c%c%s%c%s%c%s%c%s", &mode,&space,ip,&space,servantPort,&space,between,&space,between1);
			fprintf(stdout,"Present at port %s handling cities %s-%s \n",servantPort,between,between1);
			struct sockaddr_in serverAddr; 
			socket2[num]=0;
			if((socket2[num] = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
			{
				perror("Socket Creation Error");
				exit(EXIT_FAILURE);
			}
			serverAddr.sin_family = AF_INET;
			serverAddr.sin_port = htons(atoi(servantPort));
			if(inet_pton(AF_INET, ip, &serverAddr.sin_addr)<=0)  
			{
				perror("Invalid Adress");
				exit(EXIT_FAILURE);
			}
			if(connect(socket2[num], (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
			{
					perror("connection failed");
					exit(EXIT_FAILURE);
			}
			
			num++;

		}
		

		
		





	
		
	
	}
  




}
void* serverPool(void *a)
{
		
		for(;;)
		{
			  pthread_mutex_lock(&mutex1);
				
			  
			  if(empty(&queue1) ==0)
			  {
			  	 pthread_cond_wait(&cond, &mutex1);

			  }
			 
			 pthread_mutex_unlock(&mutex1);
			char *orkun=dequeue(&queue1);
			
			write(socket2[ddx],orkun, strlen(orkun));
			 ddx++;
			  	 
		}



	

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