/* run as ./pport_track 0 20 0 20. Run with display permissions on (e.g. as user, not root) */
/*mount -t debugfs none /sys/kernel/debug  */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <curses.h>
#include <time.h>
int fd;
int device = 0;
int address = 0;
int nWritten = 0;
int count = 0;
int itest = 0;
void *buff;
struct timespec t,tsum;
ssize_t nRead = 0;
int niterations = 20;
#define BUFFSIZE 64
#define NREGISTERS 5
#define BYTESPERREGISTER 4
#define MAXREADS 600
int main(int argc, char *argv[]){
  int i, j;
  const void *buf;
  /* gnuplot specifications: */
  double xmin, xmax, ymin, ymax;
  double xmin_start, xmax_start, ymin_start, ymax_start;
  double data[BUFFSIZE][2] = {0};
  FILE *Gplt;  
  /* ncurses-related quantities: */
  int ch;
  int mode = -1;
  int secondsDelay = 1;
  int nanosecondsDelay = 0;
  char sel[4];
  char option[1];
  char nreadsTomake[80];
  int readsTomake;

  xmin_start = xmin = atof(argv[1]);
  xmax_start = xmax = atof(argv[2]);
  ymin_start = ymin = atof(argv[3]);
  ymax_start = ymax = atof(argv[4]);
  Gplt = popen("gnuplot -geometry 300x300","w");
  setlinebuf(Gplt);
  fprintf(Gplt, "set xrange[%f:%f]\n", xmin, xmax);
  fprintf(Gplt, "set yrange[%f:%f]\n", ymin, ymax);
  
  initscr();			/* Start curses mode 		*/
  raw();			/* Line buffering disabled	*/
  cbreak();
  keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
  noecho();			/* Don't echo() while we do getch */
  
  
  while (mode!=4){
    mvprintw(0,0,"Type of read\n");
    printw( "  0-Monitor Single Device\n");
    printw( "  1-Alternate Device Monitoring\n");
    printw( "  2-Select Device (currently %i) \n",device);
    printw( "  3-Select Address for Device %i (currently %i) \n", device, address);
    printw( "  4-Change delay between reads\n");
    printw( "  9-Exit\n");
    printw(">");    

    /* option 1: this sequence is very prompt with no deletions */
    refresh();
    ch = getch();		     
    sprintf(sel, "%c", ch);  /* int ascii code to character */
    mode=atoi(sel); /* back to int*/
    /* option 2: this sequence is more interactive */
    /*
    refresh();
    echo();
    getnstr(option, 1);
    sprintf(&(sel[0]), "%c", option[0]);
    sscanf(sel,"%d", &mode);
    noecho();
    */
    mvprintw(6,0,"\n\n");
    switch (mode)
      {

      case 0:
	/* this operation will do periodic reads and print out the last 64 of them. */
        /* Currently, it runs for 10 minutes */
        t.tv_sec = secondsDelay;
        t.tv_nsec = nanosecondsDelay;
        tsum.tv_sec = 0;
        tsum.tv_nsec = 0;
        xmin = xmin_start;
        xmax = xmax_start;
        ymin = ymin_start;
        ymax = ymax_start;
        j=0;
	buff = (void *) malloc(NREGISTERS*BYTESPERREGISTER*sizeof(char));
	printw("Enter number of reads\n");
        refresh();
        echo();
	getstr(nreadsTomake);			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
        noecho();
        refresh();
	sscanf(nreadsTomake, "%d", &readsTomake); /* back to int*/
        if (readsTomake>MAXREADS) {
          readsTomake=MAXREADS;
        }
	while (j<readsTomake) {
          j++;
	  /*fd = open("/sys/kernel/debug/pport_monitor/pport_0",O_RDONLY);*/
          tsum.tv_sec += t.tv_sec;
          tsum.tv_nsec += t.tv_nsec;
          nRead = 1;
	  /*nRead = read(fd, buff, NREGISTERS*BYTESPERREGISTER);*/
	  if (tsum.tv_sec > xmax) {
	    xmax = tsum.tv_sec + (xmax - xmin);
	    xmin = tsum.tv_sec;
	    fprintf(Gplt,"set xrange[%f:%f]\n", xmin, xmax);
	  }

	  for (i=0; i < BUFFSIZE-1; i++){
	    data[i][0]=data[i+1][0];
	    data[i][1]=data[i+1][1];
	  }
          /*
	  data[BUFFSIZE-1][0] = (float) ((int *) buff)[2];
	  data[BUFFSIZE-1][1] = (float) tsum.tv_sec;
          */
	  if (nRead >= 0){
	    fprintf(Gplt, "plot '-' w lp pt 6 ps 1 lt 1\n");
	    for (i = 0; i < BUFFSIZE; i++){
	      fprintf(Gplt,"%1f %1f \n", data[i][1], data[i][0]);
	    }
	    fprintf(Gplt,"e\n");
	    /*refresh();*/			/* Print it on to the real screen */
	  } else {
	    printf("Error reading Parallel Port registers\n");}
	  nanosleep(&t,NULL);
          refresh();
	  /*close(fd);*/
	}
	free(buff);
	break;
      case 1:
	/* this operation will do periodic reads and print out the last 64 of them. */
        /* Currently, it runs for 10 minutes */
        xmin = xmin_start;
        xmax = xmax_start;
        ymin = ymin_start;
        ymax = ymax_start;

        t.tv_sec = 1;
        t.tv_nsec = 0;
        tsum.tv_sec = 0;
        tsum.tv_nsec = 0;
        j=0;
	buff = (void *) malloc(NREGISTERS*BYTESPERREGISTER*sizeof(char));
	while (j<128) {
          j++;
          /*
          if (device == 0) {
	    fd = open("/sys/kernel/debug/pport_monitor/pport_0_registers",O_RDONLY);}
	  else if (device == 1){
	    fd = open("/sys/kernel/debug/pport_monitor/pport_1_registers",O_RDONLY);
	  } else {
	    printf("Unknown device\n");
            break;
	  }
          */
          tsum.tv_sec += t.tv_sec;
          tsum.tv_nsec += t.tv_nsec;
	  /*nRead = read(fd, buff, NREGISTERS*BYTESPERREGISTER);*/
          nRead = 1;
	  if (tsum.tv_sec > xmax) {
	    xmax = tsum.tv_sec + (xmax - xmin);
	    xmin = tsum.tv_sec;
	    fprintf(Gplt,"set xrange[%f:%f]\n", xmin, xmax);
	  }

	  for (i=0; i < BUFFSIZE-1; i++){
	    data[i][0]=data[i+1][0];
	    data[i][1]=data[i+1][1];
	  }

	  data[BUFFSIZE-1][0] = (float) ((int *) buff)[2];
	  data[BUFFSIZE-1][1] = (float) tsum.tv_sec;

	  if (nRead >= 0){
	    fprintf(Gplt, "plot '-' w lp pt 6 ps 1 lt 1\n");
	    for (i = 0; i < BUFFSIZE; i++){
	      fprintf(Gplt,"%1f %1f \n", data[i][1], data[i][0]);            
	    }
	    fprintf(Gplt,"e\n");
	  } else {
	    printf("Error reading Parallel Port registers\n");}
	  nanosleep(&t,NULL);
          refresh();
	  /*close(fd);*/
	}
	free(buff);
	break;
      case 2:
        device = 0;
	printw("Enter Device Number (0 or 1)\n");
	ch = getch();			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
	refresh();			/* Print it on to the real screen */
    
	sprintf(sel, "%c", ch);  /* int ascii code to character */
	device=atoi(sel); /* back to int*/
        clear();
        break;

      case 3:
        address = 0;
        count = 0;
        nWritten = 0;
	buff = (void *) malloc(sizeof(int));

	printw("Enter Address (0 or 1)\n");
	ch = getch();			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
    
	sprintf(sel, "%c", ch);  /* int ascii code to character */
	address=atoi(sel); /* back to int*/
        nRead = 1;
        /*
        if (device == 0) {
          printf ("device 0\n");
          fd = 0;
          fd = open("/sys/kernel/debug/pport_monitor/pport_dev0_address",O_RDWR);}
	else {
          fd = open("/sys/kernel/debug/pport_monitor/pport_dev1_address",O_RDWR);}
        nRead = read(fd, buff, sizeof(int));
        */
        clear();
        printf("Value read %i\n", (*(char *) buff));
        printf("nbytes read %i\n",nRead);
        getch();
        buf = &address;
        printf("Intend to write %i\n",*(int *)buf);
        sprintf((char *) buf,"%d",address);
        nWritten = 1;
        /*nWritten = write(fd, buf, sizeof(int));*/
        printf("Bytes written for address: %i\n",nWritten);
        free(buff);
        /*close(fd);*/
        
      case 9:
	endwin();
        break;
      }
  }
  return 0;
}
