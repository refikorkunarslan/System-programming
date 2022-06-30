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

#define QUEUE_SIZE 15
int NCONSUMERS;
int NPRODUCERS;
int times;
struct producerTalent{
	char path[100];
	int ptime;

};
struct cunsomerTalent{
	int conId;
	int iter;
};
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
struct queueObject
{
	char buffer[QUEUE_SIZE];
	bool isAnyInQueue;
	unsigned beginIndex;
	unsigned endIndex;
};


int shmid;
int count=0;
int semid;
struct queueObject *queue;
struct sembuf sem[2];
enum { ONE, TWO, };

void* consumerTask(void *a);
void sem_init(int semid, int semaphore_number, int initial_value);
void* producerTask(void *a);
void sem_action(int semid, int sem_number, int sem_operation, struct sembuf * ptr);
int semAvailable(int semId, int semNum);


int main(int argc, char *argv[])
{

	struct producerTalent* pt;
	pt = malloc(sizeof(struct producerTalent));
	strcpy(pt->path,argv[6]);
	NCONSUMERS=atoi(argv[2]);
	times=atoi(argv[4]);
	pt->ptime=NCONSUMERS*times*2;
	key_t key = ftok(argv[0], 1);
	if(	(shmid = shmget(key, sizeof(struct queueObject), S_IRUSR | S_IWUSR | IPC_CREAT)) == -1)
	{
		perror("shmget\n");
	}
	if(	(semid = semget(key, 2, S_IRUSR | S_IWUSR | IPC_CREAT)) == -1)
	{
		perror("semget\n");
	}
	queue = shmat(shmid, NULL, 0);
	memset(queue->buffer, 0, QUEUE_SIZE * sizeof(pthread_t));
	queue->beginIndex = 0;
	queue->endIndex = 0;
	queue->isAnyInQueue = false;

	sem_init(semid, ONE, 0);
	sem_init(semid, TWO, 0);
	
	pthread_t thr;
	pthread_attr_t attr;
	int s;
	s = pthread_attr_init(&attr);
	if (s != 0)
		perror("pthread_attr_init");
	s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (s != 0)
		perror("pthread_attr_setdetachstate");
	s = pthread_create(&thr, &attr, producerTask, (void *) pt);
	if (s != 0)
		perror("pthread_create");
	s = pthread_attr_destroy(&attr);
	if (s != 0)
	perror("pthread_attr_destroy");
	
	pthread_t consthread[NCONSUMERS];

	struct cunsomerTalent ct[NCONSUMERS];
	for(int i = 0 ; i < NCONSUMERS; i++)
	{
		ct[i].iter=atoi(argv[4]);
		ct[i].conId=i;
		pthread_create(&consthread[i], NULL, &consumerTask, &ct[i] ); 
		
	}
	for (int i = 0; i < NCONSUMERS; i++)
	{
		if(!pthread_equal(pthread_self(),ct[i].conId))
		{
			pthread_join(consthread[i],NULL);
		}
	}
	free(pt);
	shmdt(queue);
	semctl(semid, 0, IPC_RMID);
	shmctl(shmid, IPC_RMID, NULL);
	

}
void* producerTask(void *a)
{
	time_t rawtime;
  struct tm * timeinfo;
	struct producerTalent * info = (struct producerTalent*)a;

	queue = shmat(shmid, NULL, 0);
	int fd;
	if ((fd = open(info->path, O_RDONLY)) == -1) {
		perror("open");
		exit(1);
	}
	
	char buf[1];
	for (int i = 0; i <info->ptime; ++i)
	{
		

		char ch;
		read(fd,buf,1);
		ch=buf[0];
		if (ch=='1')
		{
			int value1=semAvailable(semid, ONE);
			int value2=semAvailable(semid, TWO);
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			fprintf(stdout, "%sSupplier: read from input a ‘1’. Current amounts: %d x ‘1’, %d x ‘2’.\n",asctime(timeinfo),value1,value2 );
			sem_action(semid, ONE, 1, &sem[0]);
			value1=semAvailable(semid, ONE);
			value2=semAvailable(semid, TWO);

			if(value1>value2)
			{
				time ( &rawtime );
				timeinfo = localtime ( &rawtime );
				fprintf(stdout, "%sSupplier: delivered a ‘1’. Post-delivery amounts: %d x ‘1’, %d x ‘2’.\n",asctime(timeinfo),value1,value2 );

			}
			else
			{
				time ( &rawtime );
			timeinfo = localtime ( &rawtime );
				fprintf(stdout, "%sSupplier: delivered a ‘1’. Post-delivery amounts: %d x ‘1’, %d x ‘2’.\n",asctime(timeinfo),value1+1,value2+1 );
			}
			fprintf(stdout, "\n" );
				fflush(stdout);
				queue->buffer[queue->endIndex] = ch;
				queue->endIndex = (queue->endIndex + 1) % (QUEUE_SIZE);

			queue->isAnyInQueue = 1;

		}
		else
		{
			int value1=semAvailable(semid, ONE);
			int value2=semAvailable(semid, TWO);
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			fprintf(stdout, "%sSupplier: read from input a ‘2’. Current amounts: %d x ‘1’, %d x ‘2’.\n",asctime(timeinfo),value1,value2 );
			sem_action(semid, TWO, 1, &sem[1]);
			value1=semAvailable(semid, ONE);
			value2=semAvailable(semid, TWO);
			if(value2>value1)
			{
				time ( &rawtime );
			timeinfo = localtime ( &rawtime );
				fprintf(stdout, "%sSupplier: delivered a ‘2’. Post-delivery amounts: %d x ‘1’, %d x ‘2’.\n",asctime(timeinfo),value1,value2);
			}
			else
			{
				time ( &rawtime );
			timeinfo = localtime ( &rawtime );
				fprintf(stdout, "%sSupplier: delivered a ‘1’. Post-delivery amounts: %d x ‘1’, %d x ‘2’.\n",asctime(timeinfo),value1+1,value2+1 );
			}
			fprintf(stdout, "\n" );
			fflush(stdout);
			

				queue->buffer[queue->endIndex] = ch;
				queue->endIndex = (queue->endIndex + 1) % (QUEUE_SIZE);

			queue->isAnyInQueue = 1;

		}


	}
	time ( &rawtime );
			timeinfo = localtime ( &rawtime );
	fprintf(stdout, "%sThe Supplier has left.",asctime(timeinfo) );
	fprintf(stdout, "\n" );

	return 0;
}
void* consumerTask(void *a)
{

	time_t rawtime;
  struct tm * timeinfo;
	struct cunsomerTalent*  info = (struct cunsomerTalent*)a;

	
	int i=0;
	for (i = 0; i < info->iter; ++i)
	
	{

			sem[0].sem_op = -1;
			sem[0].sem_num = 0;
			sem[0].sem_flg = 0;

			sem[1].sem_op = -1;
			sem[1].sem_num = 1;
			sem[1].sem_flg = 0;
			semop(semid,sem, 2);
			
			int value1=semAvailable(semid, ONE);
			int value2=semAvailable(semid, TWO);
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			fprintf(stdout, "%sConsumer-%d at iteration %d (waiting). Current amounts: %d x ‘1’, %d x ‘2’.\n",asctime(timeinfo),info->conId,i,value1+1,value2+1);
			fprintf(stdout, "%sConsumer-%d at iteration %d (consumed). Post-consumption amounts: %d x ‘1’, %d x ‘2’.\n",asctime(timeinfo),info->conId,i,value1,value2);	
				fprintf(stdout, "\n" );
			fflush(stdout);
				queue->beginIndex = (queue->beginIndex + 1) % (QUEUE_SIZE);

			

			if(queue->beginIndex == queue->endIndex)
				queue->isAnyInQueue = 0;

	}
	time ( &rawtime );
			timeinfo = localtime ( &rawtime );
	fprintf(stdout, "%sConsumer-%d has left.\n",asctime(timeinfo),info->conId);	
	return 0;
}

void sem_init(int semid, int semaphore_number, int initial_value)
{
	union semun argument;
	argument.val = initial_value;
	if(semctl(semid, semaphore_number, SETVAL, argument) == -1)
	{
		perror("semctl\n");
	}
}
void sem_action(int semid, int sem_number, int sem_operation, struct sembuf * ptr)
{
	ptr->sem_num = sem_number;
	ptr->sem_op = sem_operation;
	ptr->sem_flg = SEM_UNDO;
  if(semop(semid, ptr, 1) == -1)
  {
		perror("semop: ");
		if(sem_operation > 0)
			perror("V\n");
		else
			perror("P\n");
  }
}


int semAvailable(int semId, int semNum) {
	int semValue = semctl(semId, semNum, GETVAL);
	return semValue;
}