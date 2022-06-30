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
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>
#include <dirent.h>
#include <sys/file.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
int socket2 ;
#define MAX_STR_LEN 200

typedef struct link {
	char str[MAX_STR_LEN];
	int size2;
	struct link *next;
} nod;

typedef struct link_node {
	char node_str[MAX_STR_LEN];
	int size1;
	struct link *tar;
	struct link_node *next;
} node;

int compare_node(struct link_node *n1, struct link_node *n2);
struct link_node *add_node(struct link_node	*list, struct link_node *node);
void display_list(struct link_node *head);
void free_list(node *head);
void cleanInput(char* input);



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
int comparee_node(struct link *n1, struct link *n2) {
	return strcmp(n1->str, n2->str);
}
struct link *addd_node(struct link	*list, struct link *node){
	struct link *prev, *next;
	if (!list) {
		list = node;
	}
	else {
		prev = NULL;
		next = list;
		while (next && comparee_node(node,next)>0) {
			prev = next;
			next = next->next;
		}
		if (!next) {
			prev->next = node;
		}
		else {
			if (prev) {
				node->next = prev->next;
				prev->next = node;
			}
			else {
				node->next = list;
				list = node;
			}
		}
	}
	return list;
}

struct link_node *add_node(struct link_node	*list, struct link_node *node){
	struct link_node *prev, *next;
	if (!list) {
		list = node;
	}
	else {
		prev = NULL;
		next = list;
		while (next && compare_node(node,next)>0) {
			prev = next;
			next = next->next;
		}
		if (!next) {
			prev->next = node;
		}
		else {
			if (prev) {
				node->next = prev->next;
				prev->next = node;
			}
			else {
				node->next = list;
				list = node;
			}
		}
	}
	return list;
}


void free_list(node *head) {
	node *prev = head;
	node *cur = head;
	while (cur) {
		prev = cur;
		cur = prev->next;
		free(prev);
	}
}

int compare_node(struct link_node *n1, struct link_node *n2) {
	return strcmp(n1->node_str, n2->node_str);
}


void cleanInput(char* input) {
	int len = strlen(input);
	int i;
	for (i = 0; i < len-1; i++) {}
	input[i] = '\0';
}

void display_list(struct link_node *head) {
	int i=1;
	while (head) {
		head = head->next;
		i++;
	}
}




struct QNode {
    char city[10];
    struct link_node *date;

    struct QNode* next;
};
 
struct Queue {
    struct QNode *front, *rear;
};
 
struct QNode* newNode(char* cit)
{
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    strcpy(temp->city,cit);
  
    temp->next = NULL;
    return temp;
}

 
struct Queue* createQueue()
{
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;

     
    return q;
}
 
void enQueue(struct Queue* que,char* cit)
{
    // Create a new LL node
    struct QNode* temp = newNode(cit);
 
    // If queue is empty, then new node is front and rear both
    if (que->rear == NULL) {
        que->front = que->rear = temp;
        return;
    }

 
    // Add the new node at the end of queue and change rear
    que->rear->next = temp;
    que->rear = temp;
}

void deQueue(struct Queue* q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return;
 
    // Store previous front and move front one node ahead
    struct QNode* temp = q->front;
 
    q->front = q->front->next;
    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;
 
    free(temp);
}




int findd_dataset(char *str,struct Queue* q)
{
	//struct Queue *q = qq;
	int count=0;
	char tran[100];
	char place[100];
	char BeginDate[100];
	char EndDate[100];
	char city[100];
	char space=' ';
	

	size_t ct=sscanf(str,"%s%c%s%c%s%c%s%c%s", tran,&space,place,&space,BeginDate,&space,EndDate,&space,city);
	if(ct ==9)
	{
		while (q->rear->date !=NULL)
		{

			char tire='-';
			int day;
			int month;
			int year;

			int day1;
			int month1;
			int year1;
				
			int day2;
			int month2;
			int year2;
			

			sscanf(BeginDate,"%d%c%d%c%d",&day,&tire,&month,&tire,&year);
			sscanf(EndDate,"%d%c%d%c%d",&day2,&tire,&month2,&tire,&year2);
			sscanf(q->rear->date->node_str,"%d%c%d%c%d",&day1,&tire,&month1,&tire,&year1);

			if( (year1 >=year || (month1 >=month || (day1 >=day))) && (year2 >=year1 || (month2 >=month1 || (day2 >=day1))) )
			{
				while (q->rear->date->tar !=NULL)
				{
					char ttr[100];
					char plc[100];
					char cod[100];
					char msuq[100];
					char price[100];
					char space=' ';
					sscanf(q->rear->date->tar->str,"%s%c%s%c%s%c%s%c%s",ttr,&space,plc,&space,cod,&space,msuq,&space,price);
					if(strcmp(place,plc)==0)
					{
						count++;
					}
					q->rear->date->tar=q->rear->date->tar->next;
				
				}
				
			}
			q->rear->date=q->rear->date->next;
			
		}
		

	}
	else
	{
		while (q->rear !=NULL)
		{
			while (q->rear->date !=NULL)
			{
				char tire='-';
				int day;
				int month;
				int year;

				int day1;
				int month1;
				int year1;
					
				int day2;
				int month2;
				int year2;
				

				sscanf(BeginDate,"%d%c%d%c%d",&day,&tire,&month,&tire,&year);
				sscanf(EndDate,"%d%c%d%c%d",&day2,&tire,&month2,&tire,&year2);
				sscanf(q->rear->date->node_str,"%d%c%d%c%d",&day1,&tire,&month1,&tire,&year1);

				if( (year1 >=year || (month1 >=month || (day1 >=day))) && (year2 >=year1 || (month2 >=month1 || (day2 >=day1))) )
				{
					while (q->rear->date->tar !=NULL)
					{
						char ttr[100];
						char plc[100];
						char cod[100];
						char msuq[100];
						char price[100];
						char space=' ';
						sscanf(q->rear->date->tar->str,"%s%c%s%c%s%c%s%c%s",ttr,&space,plc,&space,cod,&space,msuq,&space,price);
						if(strcmp(place,plc)==0)
						{
							count++;
						}
						q->rear->date->tar=q->rear->date->tar->next;
					
					}
					
				}
				q->rear->date=q->rear->date->next;
				
			}
			q->rear=q->rear->next;
		}

	}
	return count;

}








int main(int argc, char *argv[])
{
	 int myCounterFd;
    if ((myCounterFd = open ("/tmp/myappname.counter264", O_CREAT | O_RDWR, 0666)) < 0) {
        return 0;
    }
    if (flock (myCounterFd, LOCK_EX | LOCK_NB) < 0) {
        return -1;
    }
    FILE* fp = fdopen(myCounterFd, "w+");
    int count;
    size_t error;
    error = fread(&count, sizeof(count), 1, fp);

    if (error < 1) {
        count = 1;
        fwrite(&count, sizeof(count), 1, fp);
    } else{
        count++;
        rewind(fp);
        fwrite(&count, sizeof(count), 1, fp);
    }

    fflush(fp);
    flock (myCounterFd, LOCK_UN);
    close(myCounterFd);

	struct Queue* q = createQueue();
	char *filename=argv[2];
	char *betweenNumber=argv[4];
	char *ip=argv[6];
	int port=atoi(argv[8]);
	open (filename, O_RDONLY);
	struct dirent *pDirent;
	DIR *pDir;
	pDir = opendir (filename);
	int i=1;
	
	char input[MAX_STR_LEN];
	node *head, *newNode;
	head = NULL;

	char input1[MAX_STR_LEN];
	node  *newNode1;

	char input2[MAX_STR_LEN];
	nod  *newNode2;


	while ((pDirent = readdir(pDir)) != NULL) {
				cleanInput(input);
				strcpy(input,pDirent->d_name);
				newNode = (node*)malloc(sizeof(node));
				strcpy(newNode->node_str, input);
				newNode->next = NULL;
				head = add_node(head, newNode);
    }
    int begin=0,end=0;
    char a='-';
    sscanf(betweenNumber,"%d%c%d", &begin,&a,&end );
   	i=0;
   	while (i<begin+1) {
		head = head->next;
		i++;
	}
    

	char city[100];
	char path[100];
	char templ[100];
	char beginCity[100];
	char EndCity[100];
	int k=0;
	for (int i = begin; i <=end; ++i)
	{
		k=0;
		if(i==begin)
		{
			strcpy(beginCity,head->node_str);
		}
		if(i==end)
		{
			strcpy(EndCity,head->node_str);
		}
		memset(city, 0, 100);
		memset(path, 0, 100);
		strcpy(city,head->node_str);
		enQueue(q, head->node_str);

		DIR *pDir;
		strcat(path,filename);
		strcat(path,"/");
		strcat(path,head->node_str);
		pDir = opendir (path);

		while ((pDirent = readdir(pDir)) != NULL) {
			if(strcmp(pDirent->d_name,".") !=0 || strcmp(pDirent->d_name,"..") !=0 )
			{
	            if(pDirent->d_name[0] !='.')
	            {
		           	 cleanInput(input1);
					strcpy(input1,pDirent->d_name);
					newNode1 = (node*)malloc(sizeof(node));
					strcpy(newNode1->node_str, input1);
					newNode1->next = NULL;
					q->rear->date = add_node(q->rear->date, newNode1);
					q->rear->date->size1 =q->rear->date->size1+1 ;

		           
		            strcat(path,"/");
		            memset(templ, 0, 100);
		            strcpy(templ,path);
					strcat(templ,pDirent->d_name);
					 open(templ, O_RDONLY);
					
					off_t offset = 0;
					char* buffer=(char*)malloc(sizeof(char)*100);
					ssize_t len = 0;
					while((len = readline (buffer,100, templ, &offset)) != -1)
					{
						cleanInput(input2);
						strcpy(input2,buffer);
						newNode2 = (nod*)malloc(sizeof(nod));
						strcpy(newNode2->str, input2);
						newNode2->next = NULL;
						q->rear->date->tar= addd_node(q->rear->date->tar, newNode2);
						q->rear->date->tar->size2=q->rear->date->tar->size2+1;
					}

		          
	             }
	              k++;
        	}
        }
        head=head->next;
		
	}

	
    closedir (pDir);

    pid_t pid = syscall(SYS_getpid);
    fprintf(stdout,"Servant %d loaded dataset, cities %s-%s\n",pid,beginCity,EndCity);

    
   int socket1 = 0;
   struct sockaddr_in serverAddr;
   if((socket1 = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		{
			perror("Socket Creation Error");
			exit(EXIT_FAILURE);
		}
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_port = htons(port);
	if(inet_pton(AF_INET, ip, &serverAddr.sin_addr)<=0)  
		{
			perror("Invalid Adress");
			exit(EXIT_FAILURE);
		}
	if(connect(socket1, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
		{
				perror("connection failed");
				exit(EXIT_FAILURE);
		}
		
	
	char str1[100]="a ";
	strcat(str1,ip);
	strcat(str1," ");

	char str[100];
	sprintf(str, "%d", port+count);
	strcat(str," ");
	strcat(str1,str);
	strcat(str1," ");
	strcat(str1,beginCity);
	strcat(str1," ");
	strcat(str1,EndCity);
	
	send(socket1,str1, strlen(str1), 0);
	
	int opt1 = 1;
	struct sockaddr_in serverAddrr;
	if((socket2 = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{
		perror("Socket failed");
		exit(EXIT_FAILURE); 
	} 
	if(setsockopt(socket2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt1, sizeof(opt1))) 
	{
		perror("Setsockopt Failed");
		exit(EXIT_FAILURE); 
	}
	serverAddrr.sin_family = AF_INET;; 
	serverAddrr.sin_addr.s_addr = INADDR_ANY; 
	serverAddrr.sin_port = htons(port+count);
	fprintf(stdout,"Servant %d listening at port %d\n",pid,port+count);
	int addrlen = sizeof(serverAddrr);
	if(bind(socket2, (struct sockaddr *)&serverAddrr, sizeof(serverAddrr))<0) 
	{
		perror("Bind Failed");
		exit(EXIT_FAILURE); 
	}

	if (listen(socket2, 4096) < 0) 
	{
		perror("Listen Failed");
		exit(EXIT_FAILURE); 
	}

	int sck;
    for(;;)
    {
    		
    	addrlen = sizeof(serverAddr);
    	
 		sck=accept(socket2, (struct sockaddr *)&serverAddrr, (socklen_t*)&addrlen);
 		if(sck ==-1)
		{
			perror("Accept Servant");
		}
    	
    	char reqLenStr[100];
    	memset(reqLenStr, 0, 100);
	    read(sck, reqLenStr, 100);
	    int sum=findd_dataset(reqLenStr,q);
	    fprintf(stdout,"The server’s response to “%s” is %d\n",reqLenStr,sum);
	    
	    

	   
	
    	

    }
    free(q);
    free(head);
	free(newNode);
	free(newNode1);
	free(newNode2);




}


