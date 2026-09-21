named pipe


Writer Program (Creates FIFO andWrites Data)
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
intmain(){
  
 //Create FIFO
   mkfifo("mypipe",0666);
   int fd=open("mypipe",O_WRONLY);
   char msg[] = "Hellofrom Writer Process";
   write(fd,msg,sizeof(msg));
   close(fd);
   printf("Datasent successfully.\n");
   return 0;
}
Reader Program
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
intmain(){
   char buffer[100];
  int fd=open("mypipe",O_RDONLY);
   read(fd,buffer,sizeof(buffer));
   printf("Received:%s\n",buffer);
   close(fd);   return 0;}

