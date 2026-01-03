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
#include "bitmasks.h"
int fd;
void *buff;
int count,i,j;
struct timespec t,tsum;
ssize_t nRead=0;
/*int niterations = 10000000;*/
int niterations = 200000;
#define NREGISTERS 4
#define MAXREGISTERS 4
#define BYTESPERREGISTER 4
#define NAVG 2000
int words[NREGISTERS];
float voutsum[NREGISTERS];
int nframe3fail[NREGISTERS];
int nframe3failTot[NREGISTERS];
int registerAddress[NREGISTERS]={0x00000070, 0x000000B0, 0x000000D0, 0x000000E0};
typedef struct
{
  float vout;
  unsigned int address;
  int status;
} results7822type;

results7822type convert7822(int word, float vref){
  unsigned int frame0, frame1, frame2,frame3;
  int b0_11, b0_10, b0_09, b0_08, b0_07, b0_06, b0_05, b0_04, b0_03, b0_02, b0_01;
  int b1_11, b1_10, b1_09, b1_08, b1_07, b1_06, b1_05, b1_04, b1_03, b1_02, b1_01;
  int b_00;
  results7822type results7822 = {0.0, 0x00000000, 0};
  frame0 =  word & 0x000000FF; /* bits 4,5,6,7,8,9,10,11 */
  frame1 = (word & 0x0000FF00) >> 8; /* bits 6,5,4,3,2,1,0,1 */
  frame2 = (word & 0x00FF0000) >> 16; /* 5 '0's then bits 11, 10, and 9 */
  frame3 = (word & 0xFF000000) >> 24; /* 0xFF or 0xFE */
  /*printf("0x%x 0x%x 0x%x\n", word, frame3, frame3 >> 24);*/
  results7822.address = word;
  /* Only one copy of bits 2,3,7, and 8 are
     kept. The second copy is used for start and stop bits. */
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
  b1_09 = frame2 & MASK0;
  b1_10 = (frame2 & MASK1) >> 1;
  b1_11 = (frame2 & MASK2) >> 2;
  if ((b1_11 != b1_11) || (b1_10 != b1_10) || (b1_09 != b1_09)) {
    results7822.status = -1;}
  if (b1_01 != b0_01) {
    results7822.status = -1;}
  if (b1_04 != b0_04)  {
    results7822.status = -1;}
  if (b1_05 != b0_05)  {
    results7822.status = -1;}
  if (b1_06 != b0_06)  {
    results7822.status = -1;}
  /* calculate using first 12 bits */
  /*if (results7822.status == 0) {
    results7822.vout = vref*(b0_11*pow(2,11)+b0_10*pow(2,10) + b0_09*pow(2,9) + b0_08*pow(2,8) + b0_07*pow(2,7) + b0_06*pow(2,6) + b0_05*pow(2,5) + b0_04*pow(2,4)+b1_03*pow(2,3) + b1_02*pow(2,2) + b0_01*pow(2,1) + b_00*pow(2,0))/pow(2,12);
    }*/
  /* calculate using last 12 bits */
  if (results7822.status == 0) {
    results7822.vout = vref*(b1_11*pow(2,11)+b1_10*pow(2,10) + b1_09*pow(2,9) + b0_08*pow(2,8) + b0_07*pow(2,7) + b1_06*pow(2,6) + b1_05*pow(2,5) + b1_04*pow(2,4)+b1_03*pow(2,3) + b1_02*pow(2,2) + b1_01*pow(2,1) + b_00*pow(2,0))/pow(2,12);
  }
  return results7822;
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
        int nsucc = 0;
        int nbadaddr = 0;
        int reg;
        float vref=5.0;
        float vout;
        float vavg = 9999.9999;
        int firstfail = 0;
        int nTotBytesRead = 0;
        int colctr;
        unsigned int addressbits = 0;
        int ireg = 0;
        results7822type results={0.0, 0x0, 0};
        for (i=0; i< NREGISTERS; i+=1){
          voutsum[i] = 0.0;
	  nframe3fail[i] = 0;
          nframe3failTot[i] = 0;
        }
	fd = open("/dev/ttyS0",O_RDONLY);
        getmaxyx(stdscr, row, col);
        colctr = (col - 3) / 4;
        mvprintw(3 * row / 4 - 2, colctr, 
		 "         Iteration       Raw     CumSeconds    AvgVout   Frame3Fails (per %i )  Frame3Fail Rate (Total)\n", NAVG);
	buff = (void *) malloc(MAXREGISTERS * BYTESPERREGISTER * sizeof(char));
        for (i = 0; i < niterations; i+=1){
	  nRead = read(fd, buff, NREGISTERS * BYTESPERREGISTER);
	  if ( nRead > 0 ) {
            nTotBytesRead += nRead;
            for (reg = 0; reg < NREGISTERS; reg += 1) {
              results = convert7822(((int *) buff)[reg], vref);
              addressbits = results.address;
              mvprintw(3 * row / 4 - NREGISTERS - 12 + reg, colctr, "Address 0x%x \n", results.address);
	    }	  
            if (i % NAVG == 0) {
	      refresh();
	      nbadaddr = 0;
	    }
	  }
	  else {
	    mvprintw(3 * row / 4 + 5, colctr, "Error reading Serial Port registers - read %i at iteration %i \n", nRead, i);
	    if (i% NAVG == 0) refresh();
	  }
	  /*nanosleep(&t,NULL);*/
	}
	close(fd);
	free(buff);
	break;

      }
  }
  return 0;
}
