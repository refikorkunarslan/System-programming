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
#define PI 3.14159265

char inpath1[25];
char inpath2[25];
char outpath[25];
int N;
int M;

struct ope{
    int **arr;
    int **arr1;
    int conId;
    int tt;
    int Mnum;
    
};
int result[1024][1024];
float real[1024][1024];
float image[1024][1024];
float amplitudeOut[1024][1024];

static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int arrived=0;
static int arrived1=0;
void* calc(void *a);
long long current_timestamp();
sig_atomic_t signal_flag = 0;
void signalHandler(){
 signal_flag = 1;
}
int size_file(char *path)
{
    struct stat filestat;
    lstat(path,&filestat);
    return filestat.st_size;
}
int main(int argc, char *argv[])
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sa.sa_flags = 0;
    sa.sa_handler = &signalHandler;

    if(sigaction(SIGINT, &sa, NULL) == -1){
        fprintf(stderr, "err: sigaction\n");
        exit(EXIT_FAILURE);
    }

     signal(SIGINT, signalHandler);
    
    strcpy(inpath2,argv[4]);
    strcpy(outpath,argv[6]);
    strcpy(inpath1,argv[2]);
    if(size_file(inpath1)< (pow(2,N)*pow(2,N)) || size_file(inpath2)< (pow(2,N)*pow(2,N)))
    {
            fprintf(stdout, "insufficient content");
            return 0;
    }
    N=atoi(argv[8]);
    M=atoi(argv[10]);
    int fd,fd1;
    FILE *fd2;
    int matrix1[(int)pow(2,N)][(int)pow(2,N)];
    int matrix2[(int)pow(2,N)][(int)pow(2,N)];
    fd2= fopen(outpath, "w+");
    
    fprintf(stdout, "Two matrices of size %dx%d have been read. The number of threads is %d\n",(int)pow(2,N),(int)pow(2,N),M );
     
    char buf[1];
    char bf;
    fd= open(inpath1, O_RDONLY);
    if(fd == -1)
     {
        perror("open");
        exit(1);
     }
     for (int i = 0; i <pow(2,N); ++i)
     {
         for (int j = 0; j <pow(2,N); ++j)
         {
             read(fd,buf,1);
                bf=buf[0];
               
                if(bf !=10)
                {
                     matrix1[i][j]=(int)((unsigned char) bf);              
                }
    
             
             
         }
     }
   
   
    fd1= open(inpath2, O_RDONLY);
    if(fd1 == -1)
     {
        perror("open");
        exit(1);
     }
  
    for (int i = 0; i <pow(2,N); ++i)
     {
         for (int j = 0; j <pow(2,N); ++j)
         {
             read(fd1,buf,1);
                bf=buf[0];
               
                if(bf !=10)
                {
                     matrix2[i][j]=(int)((unsigned char) bf);              
                }
    
             
             
         }
     }
    pthread_t consthread[M];
    struct ope op[M];
    for (int zz = 0; zz < M; ++zz)
    {
        
         op[zz].Mnum=M;
         op[zz].tt=pow(2,N);
         op[zz].arr=malloc(sizeof(int *)* pow(2,N)); 
         op[zz].arr1=malloc(sizeof(int *)* pow(2,N)); 
         for (int i = 0; i < pow(2,N); ++i)
         {
             op[zz].arr[i]=(int *)malloc(sizeof(int *)* pow(2,N));
             op[zz].arr1[i]=(int *)malloc(sizeof(int *)* pow(2,N));
         }


    }
    for (int zz = 0; zz < M; ++zz)
    {
    
         for (int i = 0; i <pow(2,N); ++i)
         {
             for (int j = 0; j <=pow(2,N); ++j)
             {
                 
                op[zz].arr[i][j]=matrix1[i][j];
                op[zz].arr1[i][j]=matrix2[i][j];
                 
                 
             }
         }


    }
    if(!signal_flag)
    {
       
              
            for(int i = 0 ; i < M; i++)
            {
                op[i].conId=i;
                pthread_create(&consthread[i], NULL, &calc, &op[i] ); 
                
            }
            for (int i = 0; i < M; i++)
            {
                if(!pthread_equal(pthread_self(), op[i].conId))
                {
                    pthread_join(consthread[i],NULL);
                }
            }

                fprintf(fd2,"REAL PART\n");

             for (int i = 0; i < pow(2,N); ++i)
            {
                for (int j = 0; j < pow(2,N); ++j)
                {
                    fprintf(fd2,"%.3f ",pow(2,N)*real[i][j]);
                    fprintf(fd2,",");

                }
                fprintf(fd2,"\n");
            }

                fprintf(fd2,"IMAGINARY PART\n");

             for (int i = 0; i < pow(2,N); ++i)
            {
                for (int j = 0; j < pow(2,N); ++j)
                {
                    fprintf(fd2,"%.3f ",pow(2,N)*image[i][j]);
                    fprintf(fd2,",");
                }
                fprintf(fd2,"\n");

            }
            

            for (int zz = 0; zz < M; ++zz)
            {
                for (int i = 0; i < pow(2,N); ++i)
                {
                    free(op[zz].arr[i]);
                    free(op[zz].arr1[i]);
                }
                free(op[zz].arr);
                free(op[zz].arr1);

            }

      }
     pthread_mutex_destroy(&mutex1);
    close(fd);
    close(fd1);
    fclose(fd2);
   
   
}
void* calc(void *a)
{
 struct timeval st, et;
 gettimeofday(&st,NULL);
    struct ope * info = (struct ope*)a;
    for (int i = 0; i < info->tt; i++) {
        for (int j=info->conId*(pow(2,N)/info->Mnum) ; j <info->conId*(pow(2,N)/info->Mnum)+(pow(2,N)/info->Mnum); j++) {
            result[i][j] = 0;
 
            for (int k = 0; k < info->tt; k++) {
                result[i][j] += info->arr[i][k] * info->arr1[k][j];
            }
 
        }
 
    }
   
     if(!signal_flag)
    {
        pthread_mutex_lock(&mutex1);
        ++arrived;
        gettimeofday(&et,NULL);
        float elapse = (et.tv_usec - st.tv_usec);
        fprintf(stdout, "Thread %d has reached the rendezvous point in %.5f seconds.\n",info->conId,elapse/100000);
        if(arrived < info->Mnum)
        {
            pthread_cond_wait(&cond,&mutex1);
        }
        else
        {
            pthread_cond_broadcast(&cond);
        }
        arrived=0;
        fprintf(stdout, "Thread %d is advancing to the second part\n",info->conId);
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_lock(&mutex1);
        ++arrived1;
        if(arrived1< info->Mnum)
        {
            pthread_cond_wait(&cond,&mutex1);
        }
        else
        {
            pthread_cond_broadcast(&cond);
        }
       
        pthread_mutex_unlock(&mutex1);
    }
     if(!signal_flag)
    {
         for (int yWave = 0; yWave < pow(2,N); yWave++)

            {

                for (int xWave = info->conId*(pow(2,N)/info->Mnum); xWave < info->conId*(pow(2,N)/info->Mnum)+(pow(2,N)/info->Mnum); xWave++)

                {
                    real[yWave][xWave]=0;
                    image[yWave][xWave]=0;


                    for (int ySpace = 0; ySpace < pow(2,N); ySpace++)

                    {

                        for (int xSpace = 0; xSpace < pow(2,N); xSpace++)

                        {

                               real[yWave][xWave] += (result[ySpace][xSpace] * cos( 2 * PI * ((1.0 * xWave * xSpace / pow(2,N)) + (1.0* yWave * ySpace / pow(2,N))))) / sqrt(pow(2,N) * pow(2,N));
                            image[yWave][xWave] -= (result[ySpace][xSpace] * sin(

                                    2 * PI * ((1.0 * xWave * xSpace / pow(2,N)) + (1.0

                                            * yWave * ySpace / pow(2,N))))) / sqrt(

                                    pow(2,N) * pow(2,N));

                            amplitudeOut[yWave][xWave] = sqrt(

                                    real[yWave][xWave] * real[yWave][xWave]

                                            + image[yWave][xWave]

                                                    * image[yWave][xWave]);

                        }

                    }

                }

            }
        }
    gettimeofday(&et,NULL);
    float elapse = (et.tv_usec - st.tv_usec);;
    fprintf(stdout, "Thread %d has has finished the second part in %.5f seconds.\n",info->conId,elapse/100000);
    return 0;

}
