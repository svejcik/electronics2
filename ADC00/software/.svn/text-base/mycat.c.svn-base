#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
int main(int argc,char *argv[])
{
  int f=0,n;
  char l[80];
  /*int address=0x37;*/
  /*u_int8_t address = 0x38;*/
  u_int8_t *address;
  address = (u_int8_t *) malloc(sizeof(u_int8_t));
  struct stat s;
  if(argc!=2)
    {
      printf("Mismatch argument");
      exit(1);
    }
  if(access(argv[1],F_OK))
    {
      printf("File Exist");
      exit(1);
    }
  if(stat(argv[1],&s)<0)
    {
      printf("Stat ERROR");
      exit(1);
    }
  if(S_ISREG(s.st_mode)<0)
    {
      printf("Not a Regular FILE");
      exit(1);
    }
  if(geteuid()==s.st_uid)
    if(s.st_mode & S_IRUSR)
      f=1;
    else if(getegid()==s.st_gid)
      if(s.st_mode & S_IRGRP)
	f=1;
      else if(s.st_mode & S_IROTH)
	f=1;
  if(!f)
    {
      printf("Permission denied");
      exit(1);
    }
  f=open(argv[1],O_RDWR);
  *address = 0x39;
    /*n=write(f, &address, 1*sizeof(u_int8_t));*/
  n=write(f, address, 1*sizeof(u_int8_t));
  write(1, address,1*sizeof(char));
  printf("\n");
  while((n=read(f,l,80))>0)
    write(1,l,n);
}
