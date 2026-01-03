#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include "bitmasks.h"
int fd;
void *buff;
int count,i,j;
struct timespec t,tsum;
ssize_t nRead=0;
/*int niterations = 200000000;*/
int niterations = 2000;
#define NREGISTERS 4
#define BYTESPERREGISTER 4
#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
volatile int STOP=FALSE;

float convert7822(int word, float vref){
  float vout;
        unsigned int frame0, frame1, frame2,frame3;
        int b0_11, b0_10, b0_09, b0_08, b0_07, b0_06, b0_05, b0_04, b0_03, b0_02, b0_01;
        int b1_11, b1_10, b1_09, b1_08, b1_07, b1_06, b1_05, b1_04, b1_03, b1_02, b1_01;
        int b_00;
        frame0 = word & 0x000000FF; /* bits 4,5,6,7,8,9,10,11 */
        frame1 = word & 0x0000FF00; /* bits 6,5,4,3,2,1,0,1 */
        frame2 = word & 0x00FF0000; /* 5 '0's then bits 11, 10, and 9 */
        frame3 = word & 0xFF000000; /* 0xFF or 0xFE */
        /*printf("0x%x 0x%x 0x%x\n", word, frame3, frame3 >> 24);*/
        if ((frame3 >> 24) < 248) {
          printf("0x%x\n", word);
          return -1; }	
        b0_11 = frame0 & MASK0;
        b0_10 = (frame0 & MASK1) >> 1;
        b0_09 = (frame0 & MASK2) >> 2;
        b0_08 = (frame0 & MASK3) >> 3;
        b0_07 = (frame0 & MASK4) >> 4;
        b0_06 = (frame0 & MASK5) >> 5;
        b0_05 = (frame0 & MASK6) >> 6;
        b0_04 = (frame0 & MASK7) >> 7;
        /*b0_03 = frame0 & MASK8;
          b0_02 = frame0 & MASK0;*/
        b0_01 = frame1 & MASK0;
        b_00 =  (frame1 & MASK1) >> 1;
        b1_01 = (frame1 & MASK2) >> 2;
        b1_02 = (frame1 & MASK3) >> 3;
        b1_03 = (frame1 & MASK4) >> 4;
        b1_04 = (frame1 & MASK5) >> 5;
        b1_05 = (frame1 & MASK6) >> 6;
        b1_06 = (frame1 & MASK7) >> 7;
        /*b1_07 = frame0 & MASK7;
	  b1_08 = frame0 & MASK7;*/
        b1_09 =  frame2 & MASK0;
        b1_10 = (frame2 & MASK1) >> 1;
        b1_11 = (frame2 & MASK2) >> 2;
        vout = vref*(b0_11*pow(2,11)+b0_10*pow(2,10) + b0_09*pow(2,9) + b0_08*pow(2,8) + b0_07*pow(2,7) + b0_06*pow(2,6) + b0_05*pow(2,5) + b0_04*pow(2,4)+b1_03*pow(2,3) + b1_02*pow(2,2) + b0_01*pow(2,1) + b_00*pow(2,0))/pow(2,12);
  return vout;
}

int main(){

  count = NREGISTERS;
  
  int nframe03fail = 0;
  int nframe13fail = 0;
  int nframe23fail = 0;
  int nframe33fail = 0;
  int ch;
  int mode = -1;
  char sel[1];
  initscr();			/* Start curses mode 		*/
  raw();				/* Line buffering disabled	*/
  keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
  noecho();			/* Don't echo() while we do getch */
  
  while (mode!=3){
    printw("Type of read\n");
    printw("  0-Read once\n");
    printw("  1-Read fixed number of iterations\n");
    printw("  2-Periodic read for fixed number of iterations\n");
    printw("  3-Exit\n");
    ch = getch();			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
    refresh();			/* Print it on to the real screen */
    getch();			/* Wait for user input */
    
    intrflush(stdscr, FALSE);
    keypad(stdscr,TRUE);
    endwin();
    sprintf(sel, "%c", ch);  /* int ascii code to character */
    mode=atoi(sel); /* back to int*/
    
    switch (mode)
      {
      case 0:      
	fd = open("/dev/ttyS0",O_RDONLY);
	buff = (void *) malloc(NREGISTERS*BYTESPERREGISTER*sizeof(char));
        nRead = 0;
        while (nRead != 16){
	  nRead = read(fd, buff, NREGISTERS*BYTESPERREGISTER);
        }
	if (nRead >= 0){
	  for (j = 0; j < NREGISTERS; j +=1) {
	    printf("%i %i 0x%x\n",j, nRead, ((int *) buff)[j]);
	  }
	} else {
	  printf("Error reading Serial Port registers\n");}
	close(fd);
	free(buff);
	break;

      case 1:
	/*Note that this one is fairly inefficient since we are opening and closing
	  the file on each iteration. This is due to the implementation of the
	  read operation for this particular file. In next iteration, will have a
	  separate file with some maximum buffer size */
        for (i = 0; i < niterations; i+=1){
	  fd = open("/dev/ttyS0",O_RDONLY);
	  buff = (void *) malloc(NREGISTERS*BYTESPERREGISTER*sizeof(char));
	  nRead = read(fd, buff, NREGISTERS*BYTESPERREGISTER);
	  if (nRead >= 0){
	    for (j = 0; j < NREGISTERS; j +=1) {
	      printf("%i 0x%x\n",j,((int *) buff)[j]);
	    }
	  } else {
	    printf("Error reading Serial Port registers\n");}
	  close(fd);
	  free(buff);
	}
	break;

      case 2:
        t.tv_sec = 0;
        t.tv_nsec = 5000000;
        t.tv_sec = 0;
        t.tv_nsec = 0;
        tsum.tv_sec = 0;
        tsum.tv_nsec = 0;
        int row, col;
        int word0, word1, word2, word3;
        int frame0, frame1, frame2,frame3;
        float vref=5.0;
        float vout;
        /* new stuff */
        int c, res;
        struct termios oldtio, newtio;
        char buf2[255];

	fd = open("/dev/ttyS0",O_RDWR | O_NOCTTY);
        if (fd < 0) {perror(MODEMDEVICE); exit(-1); }
        tcgetattr(fd, &oldtio);
        bzero(&newtio, sizeof(newtio));
        newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
        newtio.c_iflag = IGNPAR | ICRNL;
        newtio.c_oflag = 0;
        newtio.c_lflag = ICANON;
        newtio.c_cc[VINTR]     = 0;
        newtio.c_cc[VQUIT]     = 0;
        newtio.c_cc[VERASE]     = 0;
        newtio.c_cc[VKILL]     = 0;
        newtio.c_cc[VEOF]     = 4;
        newtio.c_cc[VTIME]     = 0;
        newtio.c_cc[VMIN]     = 1;
        newtio.c_cc[VSWTC]     = 0;
        newtio.c_cc[VSTART]     = 0;
        newtio.c_cc[VSTOP]     = 0;
        newtio.c_cc[VSUSP]     = 0;
        newtio.c_cc[VEOL]     = 0;
        newtio.c_cc[VREPRINT]     = 0;
        newtio.c_cc[VDISCARD]     = 0;
        newtio.c_cc[VWERASE]     = 0;
        newtio.c_cc[VLNEXT]     = 0;
        newtio.c_cc[VEOL2]     = 0;
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd, TCSANOW, &newtio);
        getmaxyx(stdscr, row, col);
	buff = (void *) malloc(NREGISTERS*BYTESPERREGISTER*sizeof(char));
        for (i = 0; i < niterations; i+=1){
          tsum.tv_sec += t.tv_sec;
          tsum.tv_nsec += t.tv_nsec;
	  nRead = read(fd, buff, NREGISTERS*BYTESPERREGISTER);
	  /*if (nRead >= 0){*/
	  if (nRead == NREGISTERS*BYTESPERREGISTER){
            word0 = ((int *) buff)[0];
            word1 = ((int *) buff)[1];
            word2 = ((int *) buff)[2];
            word3 = ((int *) buff)[3];
            vout = convert7822(word0, vref);
            if (vout < 0) {
              nframe03fail += 1;}
	    mvprintw(3*row/4, ((col - 3)/2), "%i 0x%x %5.2f %6.5f %i\n",i,((int *) buff)[0],(float) tsum.tv_sec, vout, nframe03fail);
            vout = convert7822(word1, vref);
            if (vout < 0) {
              nframe13fail += 1;}
	    mvprintw(3*row/4 + 1, ((col - 3)/2), "%i 0x%x %5.2f %6.5f %i\n",i,((int *) buff)[1],(float) tsum.tv_sec, vout, nframe13fail);
            vout = convert7822(word2, vref);
            if (vout < 0) {
              nframe23fail += 1;}
	    mvprintw(3*row/4 + 2, ((col - 3)/2), "%i 0x%x %5.2f %6.5f %i\n",i,((int *) buff)[2],(float) tsum.tv_sec, vout, nframe23fail);
            vout = convert7822(word3, vref);
            if (vout < 0) {
              nframe33fail += 1;}
	    mvprintw(3*row/4 + 3, ((col - 3)/2), "%i 0x%x %5.2f %6.5f %i\n",i,((int *) buff)[3],(float) tsum.tv_sec, vout, nframe33fail);
	    /*printw("%i 0x%x %5.2f \n",i,((int *) buff)[2],(float) tsum.tv_sec);*/
	    refresh();			/* Print it on to the real screen */
	  } else {
	    /*printf("Error reading Serial Port registers\n");*/}
	  /*nanosleep(&t,NULL);*/
          refresh();
	}
	close(fd);
        tcsetattr(fd, TCSANOW, &oldtio);
	free(buff);
	break;

      }
  }
  return 0;
}
