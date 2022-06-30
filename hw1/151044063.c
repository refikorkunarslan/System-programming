#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include<sys/stat.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <sys/file.h>
int flg=0,flg1=0,flg2=0,flg3=0,flg4=0,flg5=0,flg6=0;

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

    /* position fd & read line */
    if ((nchr = lseek (fd, *offset, SEEK_SET)) != -1)
        nchr = read (fd, buf, sz);
    close (fd);

    if (nchr == -1) {   /* read error   */
        fprintf (stderr, "%s() error: read failure in '%s'.\n",
                __func__, fn);
        return nchr;
    }

    /* end of file - no chars read
    (not an error, but return -1 )*/
    if (nchr == 0) return -1;

    p = buf;    /* check each chacr */
    while (idx < nchr && *p != '\n') p++, idx++;
    *p = 0;

    if (idx == nchr) {  /* newline not found  */
        *offset += nchr;

        /* check file missing newline at end */
        return nchr < (ssize_t)sz ? nchr : 0;
    }

    *offset += idx + 1;

    return idx;
}
void str_replace(char *target, const char *needle, const char *replacement)
{
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {

        const char *p = strstr(tmp, needle);

        // walked past last occurrence of needle; copy remaining part
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        // copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // adjust pointers, move on
        tmp = p + needle_len;
    }

    // write altered string back to target
    strcpy(target, buffer);
}
void IgnoreCase(char* str1, char* str2)
{
    int i = 0;
    while (i < strlen(str1)) {
        if ((str1[i] >= 'A' && str1[i] <= 'Z')) {
            str2[i]=str1[i]+32;
        }
        else 
        {
            str2[i]=str1[i];
            
        } 
        i++;
 
    } 

 
} 
void insertCharact(char *source, char carac, size_t position) {
     
   int i,length=0;
   length=strlen(source);
   for(i=length;i>position-1;i--)
    { 
      
        source[i]=source[i-1];
      }


      source[position-1]=carac;
      source[length+1]='\0';

}
int regex(char *path,char*path1)
{
     int i=0,j=0;
     while( path[i] != '\0')
     {
        if(path[i] =='*')
        {
            if(path1[j] ==path[i-1])
            {
                j++;
            }
            else if(path[i+1]==path1[j] )
            {
                i++;
                
            }
            else
            {
                return 0;
            }


        }
        else
        {
            if(path[i]==path1[j] || path[i]==path1[j]+32 )
            {
                i++;
                j++;
            }
            else
            {
                return 0;
            }
        }

     }
     return 1;

    

}

void listdir( char *name, int indent,char * sdd,char * tt)
{
    DIR *dir;
    struct dirent *entry;
    char path[1024];
    

    if (!(dir = opendir(name)))
        return;


    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
          //  printf("%*s[%s]\n", indent, "", entry->d_name);

            listdir(path, indent + 2,sdd,tt);
        } else {
            
            if(strcmp(entry->d_name,sdd) ==0)
            {
                strcat(name,"/"); 
              strcat(name,entry->d_name);
                int fd1 = open(name,O_WRONLY);
                 if (fd1 < 0) 
                    { 
                        printf("------------------\n");
                        perror("ERROR"); 
                               
                   }
                    struct flock a_read_mtx;
                    memset(&a_read_mtx, 0, sizeof(struct flock));
                    struct flock a_write_mtx;
                    memset(&a_write_mtx, 0, sizeof(struct flock));
                  fcntl(fd1, F_SETLKW, &a_read_mtx);
                   fcntl(fd1, F_SETLKW, &a_write_mtx);

                if(flg==1)
                {

                    char tt1[20],tt2[20];
                    int j=0,z=0,count=0;
                    while(j<strlen(tt))
                    {
                        if(tt[j] =='/')
                        {
                            j++;
                            z=0;
                            count++;


                        }
                        else
                        {
                            if(count==1)
                            {
                                tt1[z]=tt[j];
                                z++;

                            }
                            if(count==2)
                            {
                                tt2[z]=tt[j];
                                z++;

                            }
                                j++;
                        }
                        

                    }
                   
                    char line[100] = {0};
                    char line1[100] = {0};
                    off_t offset = 0;
                    ssize_t len = 0;
                    
                    while ((len = readline (line, 100, name, &offset)) != -1)
                    {
                         str_replace(line, tt1, tt2);
                         strcat(line1,line);
                         strcat(line1,"\n");


                    }
                     int fd1 = open(name,O_WRONLY);
                      if (fd1 < 0) 
                    { 
                          perror("ERROR"); 
                           
                    } 
                    write(fd1, line1, strlen(line1));
                          
                }
                if(flg1==1)
                {
                    char tt1[20],tt2[20];
                    int j=0,z=0,count=0;
                    while(j<strlen(tt))
                    {
                        if(tt[j] =='/')
                        {
                            j++;
                            z=0;
                            count++;


                        }
                        else
                        {
                            if(count==1)
                            {
                                tt1[z]=tt[j];
                                z++;

                            }
                            if(count==2)
                            {
                                tt2[z]=tt[j];
                                z++;

                            }
                                j++;
                        }
                        

                    }
                   
                    char line[100] = {0};
                    char line1[100] = {0};
                    char line2[100] = {0};
                    off_t offset = 0;
                    ssize_t len = 0;
                    
                    while ((len = readline (line, 100, name, &offset)) != -1)
                    {
                         str_replace(line, tt1, tt2);
                         strcat(line1,line);
                         strcat(line1,"\n");


                    }
                    IgnoreCase(line1,line2);
                     int fd1 = open(name,O_WRONLY);
                      if (fd1 < 0) 
                    { 
                          perror("ERROR"); 
                           
                    } 
                    write(fd1, line2, strlen(line2));
                    
                }
                if(flg2==1)
                {



                  

                }
                if(flg3==1)
                {
                    char tt1[20],tt2[20],ttemp[20];
                    int j=0,z=0,k=0,count=0,l=0;
                    while(j<strlen(tt))
                    {
                        if(tt[j] =='/')
                        {
                            j++;
                            z=0;
                            count++;


                        }
                        else
                        {
                            if(count==1)
                            {
                                tt1[z]=tt[j];
                                z++;

                            }
                            if(count==2)
                            {
                                tt2[z]=tt[j];
                                z++;

                            }
                                j++;
                        }
                        

                    }
                    int indexx=0,x=0;
                    char newtt[25];
                    while(k<strlen(tt1))
                    {
                        if(tt1[k] =='[')
                        {
                            indexx=k;
                            while(tt1[k+1] !=']')
                            {
                                ttemp[l]=tt1[k+1];
                                k++;
                                l++;
                            }


                        }
                        else
                        {
                            newtt[x]=tt1[k+1];
                            x++;
                       
                        }
                        k++;

                    }
                    
                    
                   
                    char line[100] = {0};
                    char line1[100] = {0};
                    off_t offset = 0;
                    ssize_t len = 0;
                  
                    int y=0;

                    char mm[25];
                    strcpy(mm,newtt);
                    
                


                            while ((len = readline (line, 100, name, &offset)) != -1)
                            {
                                while(y<strlen(ttemp))
                                     {

                                         insertCharact(newtt,ttemp[y],indexx+1);
                                         str_replace(line, newtt, tt2);
                                         
                                         strcpy(newtt,mm);
                                             y++;
                                     }
                                     strcat(line1,line);
                                         strcat(line1,"\n");
                                     y=0;

                            }
                      a_read_mtx.l_type = F_UNLCK;
                      fcntl(fd1, F_SETLKW, &a_read_mtx);
                    write(fd1, line1, strlen(line1));
                    
                    a_write_mtx.l_type = F_UNLCK;
                     fcntl(fd1, F_SETLKW, &a_write_mtx);
                    

                    


                    

                }
                if(flg4==1)
                {
                    char tt1[20],tt2[20];
                    int j=0,z=0,count=0;
                    while(j<strlen(tt))
                    {
                        if(tt[j] =='/')
                        {
                            j++;
                            z=0;
                            count++;


                        }
                        else
                        {
                            if(count==1)
                            {
                                tt1[z]=tt[j];
                                z++;

                            }
                            if(count==2)
                            {
                                tt2[z]=tt[j];
                                z++;

                            }
                                j++;
                        }


                    }
                  
                    char line[100] = {0};
                     
                    char line1[100] = {0};
                    off_t offset = 0;
                    ssize_t len = 0;
                    int ii=0;
                    int cc=0;
                    while ((len = readline (line, 100, name, &offset)) != -1)
                    {
                       cc=0;
                       ii=0;
                       int fdlg=0;
                       char ww[100] = {0};
                        while(ii<strlen(line))
                        {

                            if(line[ii] !=' ')
                            {
                               ww[ii]=line[ii];
                              
                              
                                
                            }
                            else
                            {
                                 cc++;
                            }

                            if(cc==1 && strcmp(ww,&tt1[1]) ==0)
                            {
                                str_replace(line, &tt1[1], tt2);
                              strcat(line1,line);
                                strcat(line1,"\n");
                                ii=strlen(line);
                                fdlg=1;

                            }
                            
                            ii++;
                        }
                        if(fdlg==0)
                            {
                                strcat(line1,line);
                                strcat(line1,"\n");
                             
                            
                            }
                         


                    }
                     int fd1 = open(name,O_WRONLY);
                      if (fd1 < 0) 
                    { 
                          perror("ERROR"); 
                           
                    } 
                    write(fd1, line1, strlen(line1));

                }
                if(flg5==1)
                {
                    char tt1[20],tt2[20];
                    int j=0,z=0,count=0;
                    while(j<strlen(tt))
                    {
                        if(tt[j] =='/')
                        {
                            j++;
                            z=0;
                            count++;


                        }
                        else
                        {
                            if(count==1)
                            {
                                if(tt[j] !='$')
                                {
                                tt1[z]=tt[j];
                                z++;

                                }

                            }
                            if(count==2)
                            {
                                tt2[z]=tt[j];
                                z++;

                            }
                                j++;
                        }


                    }
                    char line[100] = {0};
                     
                    char line1[100] = {0};
                    off_t offset = 0;
                    ssize_t len = 0;
                    int ii=0;
                    int cc=0;
                    int kk=0;
                    while ((len = readline (line, 100, name, &offset)) != -1)
                    {
                       cc=0;
                       ii=0;
                       kk=0;
                       int fdlg=0;
                       char ww[100] = {0};
                        while(ii<strlen(line))
                        {

                            if(line[ii] !='\0')
                            {

                               ww[kk]=line[ii];
                               kk++;
                              
                              
                                
                            }
                             if(line[ii+1] =='\0')
                            {

                               cc++;
                              
                              
                                
                            }
                            if(line[ii] ==' ')
                            {
                               memset(ww,0,100);
                                kk=0;
                                 
                            }
                           

                            if(cc==1 && strcmp(ww,tt1) ==0)
                            {
                                str_replace(line, tt1, tt2);
                              strcat(line1,line);
                                strcat(line1,"\n");
                                ii=strlen(line);
                                fdlg=1;

                            }
                            
                            ii++;
                        }
                        if(fdlg==0)
                            {
                                strcat(line1,line);
                                strcat(line1,"\n");
                             
                            
                            }
                         


                    }
                     int fd1 = open(name,O_WRONLY);
                      if (fd1 < 0) 
                    { 
                          perror("ERROR"); 
                           
                    } 
                    write(fd1, line1, strlen(line1));

                }
                 if(flg6==1)
                {

                    char tt1[20],tt2[20];
                    int j=0,z=0,count=0;
                    while(j<strlen(tt)-1)
                    {
                        if(tt[j] =='/')
                        {
                            j++;
                            z=0;
                            count++;


                        }
                        else
                        {
                            if(count==1)
                            {
                                    tt1[z]=tt[j];
                                    z++;
                                

                            }
                            if(count==2)
                            {
                                tt2[z]=tt[j];
                                z++;

                            }
                                j++;
                        }
                        

                    }
                   

                    char line[100] = {0};
                    char line1[100] = {0};
                    off_t offset = 0;
                    ssize_t len = 0;
                     int ii=0;
                    int kk=0;
                    int dd=0;
                    while ((len = readline (line, 100, name, &offset)) != -1)
                    {
                         
                         
                          
                       ii=0;
                       kk=0;
                       dd=0;
                       char ww[100] = {0};
                        while(ii<strlen(line)-1)
                        {

                            if(line[ii] !='\0')
                            {

                               ww[kk]=line[ii];
                               kk++;
                              
                              
                                
                            }

                          dd=regex(tt1,ww);
                          if(dd==1)
                        {
                            str_replace(line, ww, tt1);
                             int we=strlen(ww)-strlen(tt1);
                           ii=ii-we;
                         

                        }
                        


                        if(line[ii] ==' ')
                            {
                               memset(ww,0,100);
                                kk=0;
                                 
                            }
                           
                           
                            
                            ii++;
                        }

                         
                        

                       

                         str_replace(line, tt1, tt2);
                         strcat(line1,line);
                         strcat(line1,"\n");


                    }
                     //a_read_mtx.l_type = F_UNLCK;
                     //fcntl(fd1, F_SETLKW, &a_read_mtx);
                    int dx;
                    dx=write(fd1, line1, strlen(line1));
                   if(dx<0)
                   {
                    perror("Error");
                   }


                    
                          
                }


                    
                            break;
            }

                
        }

    }
    closedir(dir);
}
int serch(char * str,int xx)
{
    int i=0;
    while(i<strlen(str))
    {
        if(str[i]=='[' && xx !=1)
        {
            return 0;
        }
        if(str[i]=='^'&& xx !=2)
        {
            return 1;
        }
         if(str[i]=='$'&& xx !=3)
        {
            return 2;
        }
         if(str[i]=='*'&& xx !=4)
        {
            return 3;
        }

        i++;

    }
     return 4;
}

int main(int argc, char *argv[]) {
    int i=0;
   
    char path[150];
    strcpy(path,argv[2]);
    int j=0;
    int k=0;
        char temp[100];

    while(j !=strlen(path)+1)
    {
        if(path[j]=='/')
        {
            memset(temp,0,100);
            k=0;
        }
        else
        {

            temp[k]=path[j];
            k++;
        }
        j++;

    }
    
   char tt[150];
    strcpy(tt,argv[1]);
    int count=0;
    int xx=0;
    while(i<strlen(tt))
    {
        if(tt[i]=='/')
        {

            if(count==2)
            {
                if(tt[i+1]=='\0' && serch(tt,xx)==4)
                {
                    flg=1;
                }
                if(tt[i+1]=='i')
                {

                    flg1=1;
                }
                if(tt[i+3]==';')
                {
                    flg2=1;
                }
                if(serch(tt,xx)==0)
                {
                    xx=1;
                    flg3=1;
                }
                if(serch(tt,xx)==1)
                {
                    xx=2;

                    flg4=1;
                }
                if(serch(tt,xx)==2)
                {
                    xx=3;
                    flg5=1;
                }
                if(serch(tt,xx)==3)
                {
                    xx=4;

                    flg6=1;
                }





            }
            count++;
        }

        
          i++;

    }
    listdir(".", 0,temp,tt);
    return 0;
}