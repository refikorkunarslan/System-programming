#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define CLIENT_FIFO_TEMPLATE "refik.%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)
struct request {
pid_t pid;
int seqLen;
char buf[256];
int  matrixcounter;
};
struct response {
int seqNum;
char buf1[256];
int invertible;
int matsize;

};