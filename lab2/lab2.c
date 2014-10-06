#include <minix/drivers.h>
#include "timer.h"

static void print_usage(char *argv[]);
static int proc_args(int argc, char *argv[]);

int main(int argc, char **argv) {

	/* Initialize service */
	  sef_startup();

	  printf("lab2: PC Timer \n");

	if ( argc == 1 ) {
	      print_usage(argv);
	      return 0;
	  } else {
	      proc_args(argc, argv);
	  }
	  return 0;
}

static void print_usage(char *argv[]) {
	  printf("Usage: one of the following tests:\n"
		 "\t service run %s -args \"int <time>\" \n"
		 "\t service run %s -args \"square <freq>\"\n"
		 "\t service run %s -args \"config <timer> \" \n",
		 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

  unsigned long timer, time, freq;
  char *str;

  if (strncmp(argv[1], "int", strlen("int")) == 0) {
  	  if( argc != 3 ) {
  		  printf("timer:: wrong no of arguments for test of int() \n");
  		  return 1;
  	  }
  	  time=*argv[2];
  	  printf("timer:: int()\n"); /* Actually, it was already invoked */
  	  timer_test_int(time);
  	  return 0;
    } else if (strncmp(argv[1], "square", strlen("square")) == 0) {
  	  if( argc != 3 ) {
  		  printf("timer:: wrong no of arguments for test of square() \n");
  		  return 1;
  	  }
  	  freq=*argv[2];
  	  printf("timer:: square()\n"); /* Actually, it was already invoked */
  	  timer_test_square(freq);
  	  return 0;
    } else if (strncmp(argv[1], "config", strlen("config")) == 0) {
  	  if( argc != 3 ) {
  		  printf("timer: wrong no of arguments for test of config() \n");
  		  return 1;
  	  }
  	  timer=(unsigned long) *argv[2] - 48;
  	  printf("timer:: config()\n"); /* Actually, it was already invoked */
  	  timer_test_config(timer);
  	  return 0;
    }
}
