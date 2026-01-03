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
int address = 48; /* ascii for 0 */
int nWritten = 0;
int count = 0;
int secondsDelay = 1;
int nanosecondsDelay = 0;
void *buff;
struct timespec t,tsum;
ssize_t nRead = 0;
int niterations = 20;
#define BUFFSIZE 64
#define NREGISTERS 4
#define BYTESPERREGISTER 4
#define MAXREADS 1024

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
  char sel[4];
  char option[1];
  char nreadsRequested[80];
  int nreadsTomake;
  
  xmin_start = xmin = atof(argv[1]);
  xmax_start = xmax = atof(argv[2]);
  ymin_start = ymin = atof(argv[3]);
  ymax_start = ymax = atof(argv[4]);
  Gplt = popen("gnuplot -geometry 300x300","w");
  setlinebuf(Gplt);
  fprintf(Gplt, "set xrange[%f:%f]\n", xmin, xmax);
  fprintf(Gplt, "set yrange[%f:%f]\n", ymin, ymax);
  
  initscr();			/* Start curses mode 		*/
  raw();				/* Line buffering disabled	*/
  keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
  noecho();			/* Don't echo() while we do getch */
  
  while (mode!=9){
    mvprintw(0, 0, "Type of read\n");
    printw("  0-Monitor Single Device\n");
    printw("  1-Alternate Device Monitoring\n");
    printw("  2-Select Device (currently %i) \n",device);
    printw("  3-Select Address for Device %i (currently %c) \n", device, address);
    printw("  4-Change delay between reads \n");
    printw("  5-Enable writing data to file \n");
    printw("  6-Enable writing data to database \n");
    printw("  9-Exit\n");
    printw(">");
    /*
    refresh();
    ch = getch();
    sprintf(sel, "%c", ch);
    mode=atoi(sel);
    */
    refresh();
    echo();
    getnstr(option, 1);
    sprintf(&(sel[0]), "%c", option[0]);
    sscanf(sel,"%d", &mode);
    noecho();


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
	getstr(nreadsRequested);			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
        noecho();
        refresh();
	sscanf(nreadsRequested, "%d", &nreadsTomake); /* back to int*/
        if (nreadsTomake>MAXREADS) {
          nreadsTomake=MAXREADS;
        }
	while (j<nreadsTomake) {
          j++;
	  fd = open("/sys/kernel/debug/pport_monitor/pport_0",O_RDONLY);
          tsum.tv_sec += t.tv_sec;
          tsum.tv_nsec += t.tv_nsec;
	  nRead = read(fd, buff, NREGISTERS*BYTESPERREGISTER);
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
	    /*refresh();*/			/* Print it on to the real screen */
	  } else {
	    printf("Error reading Parallel Port registers\n");}
	  nanosleep(&t,NULL);
          refresh();
	  close(fd);
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

        t.tv_sec = secondsDelay;
        t.tv_nsec = nanosecondsDelay;
        tsum.tv_sec = 0;
        tsum.tv_nsec = 0;
        j=0;
	buff = (void *) malloc(NREGISTERS*BYTESPERREGISTER*sizeof(char));
	while (j<128) {
          j++;
          if (device == 0) {
	    fd = open("/sys/kernel/debug/pport_monitor/pport_0_registers",O_RDONLY);}
	  else if (device == 1){
	    fd = open("/sys/kernel/debug/pport_monitor/pport_1_registers",O_RDONLY);
	  } else {
	    printf("Unknown device\n");
            break;
	  }
          tsum.tv_sec += t.tv_sec;
          tsum.tv_nsec += t.tv_nsec;
	  nRead = read(fd, buff, NREGISTERS*BYTESPERREGISTER);
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
	  close(fd);
	}
	free(buff);
	break;
      case 2:
        device = 0;
	printw("Enter Device Number (0 or 1)\n");
	ch = getch();			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
        refresh();
	sprintf(sel, "%c", ch);  /* int ascii code to character */
	device=atoi(sel); /* back to int*/
        clear();
        break;
      case 3:
        address = 48; /* ascii 0 */
        count = 0;
        nWritten = 0;
	buff = (void *) malloc(sizeof(int));

	printw("Enter Address (0 or 1)\n");
        echo();
	getstr(sel);
        noecho();
        refresh();
        sscanf(sel, "%d", &address);
        if (device == 0) {
          printw ("device 0\n");
          fd = 0;
          fd = open("/sys/kernel/debug/pport_monitor/pport_dev0_address",O_RDWR);}
	else {
          fd = open("/sys/kernel/debug/pport_monitor/pport_dev1_address",O_RDWR);}
        nRead = read(fd, buff, sizeof(int));
        clear();
        printw("Original Value read %c\n", (*(int *) buff));
        printw("nbytes read %d\n",nRead);
        refresh();
        buf = &address;
        printw("Attempt to set address to  %d\n",*(int *)buf);
        sprintf((char *) buf,"%d",address);
        nWritten = write(fd, buf, sizeof(int));
        printw("Bytes written for address: %i\n",nWritten);
        ch = getch();
        free(buff);
        close(fd);
        break;
      case 4:
        printw("Not finished with implementation. Only reading 1-9 sec/nsec\n");
        printw("Enter seconds between read\n");
        ch = getch();
        sprintf(&(sel[0]),"%c",ch);
        secondsDelay = atoi(sel);
        printw("Delay is now: %d\n", secondsDelay);
        printw("Enter nanoseconds between read\n");
        ch = getch();
        sprintf(&(sel[0]),"%c",ch);
        nanosecondsDelay = atoi(sel);
        printw("Delay is now: %d\n", nanosecondsDelay);
        refresh();
        getch();
        mvprintw(6,0,"\n\n");
        refresh();
        break;
      case 5:
        printw("Not yet implemented\n");
        break;
      case 6:
        printw("Not yet implemented\n");
        break;
      case 9:
        endwin();
        break;
      }
  }
  return 0;
}
