#include <minix/drivers.h>
#include "timer.h"

static void print_usage(char *argv[]);
static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);

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
  	  freq=parse_ulong(argv[2],10);
  	  printf("%lu",freq);
  	  printf("timer:: square()\n"); /* Actually, it was already invoked */
  	  timer_test_square(freq);
  	  return 0;
    } else if (strncmp(argv[1], "config", strlen("config")) == 0) {
  	  if( argc != 3 ) {
  		  printf("timer: wrong no of arguments for test of config() \n");
  		  return 1;
  	  }
  	  timer=parse_ulong(argv[2],10);
  	  printf("timer:: config()\n"); /* Actually, it was already invoked */
  	  timer_test_config(timer);
  	  return 0;
    }
}

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}
