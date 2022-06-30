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
#include <semaphore.h>
#include <sys/mman.h>
#include <syslog.h>
static int *glob_var;
static int *glob;

struct Chef{
	char ingredient[2];
	char ingredient1[2];
	char supply[2];
	pid_t pid;
	int number;
};
sem_t * semaphores;
struct request {
char buf[256];
};

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
void Deliver(char ch1,char ch2){

	sem_wait(&semaphores[3]);
	write(STDOUT_FILENO,"the wholesaler delivers ",24);
	if (ch1 == 'M')
		write(STDOUT_FILENO,"milk",sizeof("milk"));
	else if (ch1 == 'S')
		write(STDOUT_FILENO,"sugar",sizeof("sugar"));
	else if (ch1 == 'F')
		write(STDOUT_FILENO,"flour",sizeof("flour"));
	else
		write(STDOUT_FILENO,"walnuts",sizeof("walnuts"));
	write(STDOUT_FILENO," and ",sizeof(" and "));

	if (ch2 == 'M')
		write(STDOUT_FILENO,"milk",sizeof("milk"));
	else if (ch2 == 'S')
		write(STDOUT_FILENO,"sugar",sizeof("sugar"));
	else if (ch2 == 'F')
		write(STDOUT_FILENO,"flour",sizeof("flour"));
	else
		write(STDOUT_FILENO,"walnuts",sizeof("milk"));
 	write(STDOUT_FILENO,"\n",1);
 	sem_post(&semaphores[3]);
}
void taken(int number,char ch1,char ch2)
{
			sem_wait(&semaphores[3]);
			write(STDOUT_FILENO,"chef", sizeof("chef"));

			char str[1];
			sprintf(str,"%d",number);
			write(STDOUT_FILENO,str, sizeof(str));

			write(STDOUT_FILENO," (", sizeof("("));
			char strr[1];
			sprintf(strr,"%d",getpid());
			write(STDOUT_FILENO,strr,strlen(strr));
			write(STDOUT_FILENO,")", sizeof(")"));
			write(STDOUT_FILENO," has taken the ", sizeof(" has taken the "));
			if (ch1 == 'M')
				write(STDOUT_FILENO,"milk",strlen("milk"));
			else if (ch1 == 'S')
				write(STDOUT_FILENO,"sugar",sizeof("sugar"));
			else if (ch1 == 'F')
				write(STDOUT_FILENO,"flour",sizeof("flour"));
			else
				write(STDOUT_FILENO,"walnuts",sizeof("walnuts"));
			
			

			write(STDOUT_FILENO,"\n",1);
			write(STDOUT_FILENO,"chef", sizeof("chef"));
			char strrr[1];
			sprintf(strrr,"%d",number);
			write(STDOUT_FILENO,strrr, sizeof(strrr));
			write(STDOUT_FILENO," (", sizeof("("));
			char strrrr[10];
			sprintf(strrrr,"%d",getpid());
			write(STDOUT_FILENO,strrrr,strlen(strrrr));
			write(STDOUT_FILENO,")", sizeof(")"));
			write(STDOUT_FILENO," has taken the ", sizeof(" has taken the "));
		
			if (ch2 == 'M')
				write(STDOUT_FILENO,"milk",sizeof("milk"));
			else if (ch2 == 'S')
				write(STDOUT_FILENO,"sugar",sizeof("sugar"));
			else if (ch2 == 'F')
				write(STDOUT_FILENO,"flour",sizeof("flour"));
			else
				write(STDOUT_FILENO,"walnuts",sizeof("walnuts"));
			write(STDOUT_FILENO,"\n",1);
			sem_post(&semaphores[3]);

}
void prepare(int number)
{
			sem_wait(&semaphores[3]);
			write(STDOUT_FILENO,"chef", sizeof("chef"));
			char strrrrr[1];
			sprintf(strrrrr,"%d",number);
			write(STDOUT_FILENO,strrrrr, sizeof(strrrrr));
			write(STDOUT_FILENO," (", sizeof("("));
			char strrrrrr[10];
			sprintf(strrrrrr,"%d",getpid());
			write(STDOUT_FILENO,strrrrrr,strlen(strrrrrr));
			write(STDOUT_FILENO,")", sizeof(")"));
			write(STDOUT_FILENO," is preparing the dessert", sizeof(" is preparing the dessert"));
			write(STDOUT_FILENO,"\n",1);
			sem_post(&semaphores[3]);
}
void Supply(int number,struct Chef chef){

	char str[10];
    int temp;

	sem_wait(&semaphores[3]);
	write(STDOUT_FILENO,"chef",sizeof("chef"));
 	temp = sprintf(str, "%d",number);
   	write(STDOUT_FILENO,str,temp);
   	write(STDOUT_FILENO," has an endless supply of ",sizeof(" has an endless supply of "));

	if (chef.supply[0] == 'M')
		write(STDOUT_FILENO,"milk",sizeof("milk"));
	else if (chef.supply[0] == 'S')
		write(STDOUT_FILENO,"sugar",sizeof("sugar"));
	else if (chef.supply[0] == 'F')
		write(STDOUT_FILENO,"flour",sizeof("flour"));
	else
		write(STDOUT_FILENO,"walnuts",sizeof("walnuts"));
   	write(STDOUT_FILENO," and ",sizeof("and"));
   	if (chef.supply[1] == 'M')
		write(STDOUT_FILENO,"milk",sizeof("milk"));
	else if (chef.supply[1] == 'S')
		write(STDOUT_FILENO,"sugar",sizeof("sugar"));
	else if (chef.supply[1] == 'F')
		write(STDOUT_FILENO,"flour",sizeof("flour"));
	else
		write(STDOUT_FILENO,"walnuts",sizeof("walnuts"));
   	write(STDOUT_FILENO," but lacks ",sizeof(" but lacks "));
   	if (chef.ingredient[0] == 'M')
		write(STDOUT_FILENO,"milk",sizeof("milk"));
	else if (chef.ingredient[0] == 'S')
		write(STDOUT_FILENO,"sugar",sizeof("sugar"));
	else if (chef.supply[0] == 'F')
		write(STDOUT_FILENO,"flour",sizeof("flour"));
	else
		write(STDOUT_FILENO,"walnuts",sizeof("walnuts"));
   	write(STDOUT_FILENO," and ",sizeof(" and "));
   	if (chef.supply[1] == 'M')
		write(STDOUT_FILENO,"milk",sizeof("milk"));
	else if (chef.supply[1] == 'S')
		write(STDOUT_FILENO,"sugar",sizeof("sugar"));
	else if (chef.supply[1] == 'F')
		write(STDOUT_FILENO,"flour",sizeof("flour"));
	else
		write(STDOUT_FILENO,"walnuts",sizeof("walnuts"));
   	write(STDOUT_FILENO,",",sizeof(","));
     

 	write(STDOUT_FILENO,"\n",1);

 	sem_post(&semaphores[3]);	

}
void finishDessert(int number)
{
			sem_wait(&semaphores[3]);
			write(STDOUT_FILENO,"chef", sizeof("chef"));
			char strrrrrrrr[1];
			sprintf(strrrrrrrr,"%d",number);
			write(STDOUT_FILENO,strrrrrrrr, sizeof(strrrrrrrr));
			write(STDOUT_FILENO," (", sizeof("("));
			char strrrrrrr[10];
			sprintf(strrrrrrr,"%d",getpid());
			write(STDOUT_FILENO,strrrrrrr,strlen(strrrrrrr));
			write(STDOUT_FILENO,")", sizeof(")"));
			write(STDOUT_FILENO," has delivered the dessert", sizeof(" has delivered the dessert"));
			write(STDOUT_FILENO,"\n",1);
			sem_post(&semaphores[3]);
}
void obtained()
{
		sem_wait(&semaphores[3]);
 		char str[5];
 		sprintf(str,"%d",getpid());
	    write(STDOUT_FILENO,"the wholesaler (", sizeof("the wholesaler ("));
	    write(STDOUT_FILENO,str, sizeof(str));
	    write(STDOUT_FILENO,") has obtained the dessert and left", sizeof(") has obtained the dessert and left"));
	    write(STDOUT_FILENO,"\n",1);
	    sem_post(&semaphores[3]);

}
void waiting(int number,char str ,char str1)
{
		sem_wait(&semaphores[3]);
 		write(STDOUT_FILENO,"chef", sizeof("chef"));
			char strrrrrrrr[1];
			sprintf(strrrrrrrr,"%d",number);
			write(STDOUT_FILENO,strrrrrrrr, sizeof(strrrrrrrr));
			write(STDOUT_FILENO," (", sizeof("("));
			char strrrrrrr[10];
			sprintf(strrrrrrr,"%d",getpid());
			write(STDOUT_FILENO,strrrrrrr,strlen(strrrrrrr));
			write(STDOUT_FILENO,")", sizeof(")"));
			write(STDOUT_FILENO," is waiting for ", sizeof(" is waiting for "));
			if (str == 'M')
					write(STDOUT_FILENO,"milk",sizeof("milk"));
				else if (str == 'S')
					write(STDOUT_FILENO,"sugar",sizeof("sugar"));
				else if (str == 'F')
					write(STDOUT_FILENO,"flour",sizeof("flour"));
				else
					write(STDOUT_FILENO,"walnuts",sizeof("walnuts"));
			write(STDOUT_FILENO," and ",sizeof(" and "));
			if (str1 == 'M')
					write(STDOUT_FILENO,"milk",sizeof("milk"));
				else if (str1 == 'S')
					write(STDOUT_FILENO,"sugar",sizeof("sugar"));
				else if (str1 == 'F')
					write(STDOUT_FILENO,"flour",sizeof("flour"));
				else
					write(STDOUT_FILENO,"walnuts",sizeof("walnuts"));

			write(STDOUT_FILENO,"\n",1);
	    sem_post(&semaphores[3]);

}
void waitingDessert()
{
		sem_wait(&semaphores[3]);
 		char str[5];
 		sprintf(str,"%d",getpid());
	    write(STDOUT_FILENO,"the wholesaler (", sizeof("the wholesaler ("));
	    write(STDOUT_FILENO,str, sizeof(str));
	    write(STDOUT_FILENO,") is waiting for the dessert", sizeof(") is waiting for the dessert"));
	    write(STDOUT_FILENO,"\n",1);
	    sem_post(&semaphores[3]);

}
void done(int a)
{
	sem_wait(&semaphores[3]);
 		write(STDOUT_FILENO,"the wholesaler ", sizeof("the wholesaler "));
			write(STDOUT_FILENO," (", sizeof("("));
			char strrrrrrr[10];
			sprintf(strrrrrrr,"%d",getpid());
			write(STDOUT_FILENO,strrrrrrr,strlen(strrrrrrr));
			write(STDOUT_FILENO,")", sizeof(")"));
			write(STDOUT_FILENO," is done ", sizeof(" is done "));
			write(STDOUT_FILENO," (", sizeof("("));
			write(STDOUT_FILENO,"total desserts: ", sizeof("total desserts: "));
			char strrrrrrrr[10];
			sprintf(strrrrrrrr,"%d",a);
			write(STDOUT_FILENO,strrrrrrrr,strlen(strrrrrrrr));
			write(STDOUT_FILENO,")", sizeof(")"));
			write(STDOUT_FILENO,"\n",1);
	sem_post(&semaphores[3]);

}

void wholesaler(int size,struct request *o,char* filepath)
{
	
	while(*glob_var<size-1)
	{
		
		
 		Deliver(o[*glob_var].buf[0],o[*glob_var].buf[1]);
 		
 		
 		 sem_post(&semaphores[0]);
 		
			

 		 sem_wait(&semaphores[5]);
 		  waitingDessert();
 		  sem_post(&semaphores[6]);
 		
 		
 		sem_wait(&semaphores[1]);


 		obtained();
 		
	    sem_post(&semaphores[2]);
		
	    done(*glob_var+1);
 		sem_wait(&semaphores[4]);
 		*glob_var=*glob_var+1;

 		write(STDOUT_FILENO,"\n",1);

 		
 		
	}
	
   	

}
void cheff(int size,struct request *o,struct Chef chef)
{
	Supply(chef.number,chef);

	while(*glob_var<size)
	 {
		sem_wait(&semaphores[0]);

		*glob=1;
		
			
	 	if((o[*glob_var].buf[0]==chef.ingredient[0] && o[*glob_var].buf[1]==chef.ingredient[1]) ||  (o[*glob_var].buf[0]==chef.ingredient[1] && o[*glob_var].buf[1]==chef.ingredient[0]) )
 	 	{	
 	 		
 	 		
		 	taken(chef.number,o[*glob_var].buf[0],o[*glob_var].buf[1]);
		 	prepare(chef.number);
		 	sem_post(&semaphores[5]);
		 	sem_wait(&semaphores[6]);
		 	finishDessert(chef.number);
			sem_post(&semaphores[1]);
			sem_wait(&semaphores[2]);
			waiting(chef.number,chef.ingredient[0] ,chef.ingredient[1]);
			sem_post(&semaphores[4]);

			
		


	 	}
	 	else
	 	{
			sem_post(&semaphores[0]);

	 		*glob=1;

	 	}
	 	if(*glob_var==size-2)
		{
			break;
		}

	}



}

int main(int argc, char *argv[])
{
	glob_var=mmap(NULL,sizeof *glob_var,PROT_READ|PROT_WRITE,MAP_SHARED |MAP_ANONYMOUS,-1,0);
 	*glob_var=0;
 	glob=mmap(NULL,sizeof *glob,PROT_READ|PROT_WRITE,MAP_SHARED |MAP_ANONYMOUS,-1,0);
 	*glob=0;
	int semaphore_shared = shm_open(argv[4], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (semaphore_shared == -1) {
        perror("semaphore_shared");
        return -1;
    }
    if( ftruncate(semaphore_shared, sizeof(sem_t)  * 7)==-1)
    {
    	perror("ftruncate semaphore");
        return -1;
    }
    

    semaphores = mmap(NULL, sizeof(sem_t) * 7, PROT_READ | PROT_WRITE, MAP_SHARED, semaphore_shared, 0);

	sem_init(&semaphores[0], 1, 0);
	sem_init(&semaphores[1], 1, 0);
	sem_init(&semaphores[2], 1, 0);
	sem_init(&semaphores[3], 1, 1);
	sem_init(&semaphores[4], 1, 0);
	sem_init(&semaphores[5], 1, 0);
	sem_init(&semaphores[6], 1, 0);


	struct Chef chef[6];
	int oo;	
	struct request *o;

	oo = shmget(IPC_PRIVATE, 150*sizeof(struct request), IPC_CREAT | 0666);
	o = (struct request*) shmat(oo,NULL,0);



	
	
	ssize_t len = 0;
	 off_t offset = 0;
	 char* buffer=(char*)malloc(sizeof(char)*5);
	 int i=0;
	while((len = readline (buffer, 5, argv[2], &offset)) != -1)
  	{
  		strcpy(o[i].buf,buffer);
  		i++;

  	}
	
  	int HMingredient=i;
  	pid_t parentpid=getpid();
  	for(int i = 0; i < 6; i++){
		if (getpid() == parentpid)
		{
		    pid_t rocIDs = fork();
		    if(rocIDs < 0 ){
		        perror("Fork error!");
		        exit(0);
		    }
		    else if (parentpid != getpid())
		    {	
		    	if(i==0)
		    	{
		    		chef[i].number=i+1;
		    		chef[i].pid=getpid();
		    		strcpy(chef[i].ingredient,"WS");
		    		strcpy(chef[i].supply,"MF");
		    		cheff(HMingredient,o,chef[0]);

		    	}
		    	if(i==1)
		    	{
		    		chef[i].number=i+1;
		    		chef[i].pid=getpid();

		    		strcpy(chef[i].ingredient,"FW");
		    		strcpy(chef[i].supply,"MS");

		    		cheff(HMingredient,o,chef[1]);

		    	}
		    	if(i==2)
		    	{
		    		chef[i].number=i+1;
		    		chef[i].pid=getpid();

		    		strcpy(chef[i].ingredient,"FS");
		    		strcpy(chef[i].supply,"WM");

		    		cheff(HMingredient,o,chef[2]);

		    	}
		    	if(i==3)
		    	{
		    		chef[i].number=i+1;
		    		chef[i].pid=getpid();

		    		strcpy(chef[i].ingredient,"MF");
		    		strcpy(chef[i].supply,"WS");

		    		cheff(HMingredient,o,chef[3]);

		    	}
		    	if(i==4)
		    	{
		    		chef[i].number=i+1;
		    		chef[i].pid=getpid();

		    		strcpy(chef[i].ingredient,"MW");
		    		strcpy(chef[i].supply,"FS");
		    		cheff(HMingredient,o,chef[4]);

		    	}
		    	if(i==5)
		    	{

		    		chef[i].number=i+1;
		    		chef[i].pid=getpid();

		    		strcpy(chef[i].ingredient,"MS");
		    		strcpy(chef[i].supply,"FW");

		    		cheff(HMingredient,o,chef[5]);

		    	}
		    	
		    }
		   
    	}
    }
    if(getpid()==parentpid)
    {
    	wholesaler(HMingredient,o,argv[2]);
    }
     if(getpid()==parentpid)
    {
	    for(int k = 0; k < 6; k++)
	        wait(NULL);
    }
      for(int k = 0; k < 6; k++)
	        sem_destroy(&semaphores[k]);
	
	munmap(semaphores, sizeof(sem_t) * 6);
	shmdt((void *)o);
	shmctl(oo,IPC_RMID,NULL);
  	return 0;

}