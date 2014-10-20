#include "test3.h"
#include "keyboard.h"

static void print_usage(char *argv[]);
static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);

int main(int argc, char **argv) {

	/* Initialize service */
	  sef_startup();

	  printf("lab3: KBC \n");

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
		 "\t service run %s -args \"scan <0->C/other->ASSEMBLY>\" \n"
		 "\t service run %s -args \"led <number of arguments Led's to toggle>\"\n"
		 "\t service run %s -args \"timed_scan <seconds without input to exit> \" \n",
		 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

  unsigned short ass,n,sec;
  unsigned short led[n];
  char *str;

  if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
  	  if( argc != 3 ) {
  		  printf("kbc:: wrong no of arguments for test of scan() \n");
  		  return 1;
  	  }
  	  ass=parse_ulong(argv[2],10);
  	  printf("kbc:: scan()\n"); /* Actually, it was already invoked */
  	  kbd_test_scan(ass);
  	  return 0;
    } else if (strncmp(argv[1], "led", strlen("led")) == 0) {
  	  if( argc < 3 ) {
  		  printf("kbc:: wrong no of arguments for test of led() \n");
  		  return 1;
  	  }
  	  sec=parse_ulong(argv[2],10);
  	  unsigned int i;
  	  for(i=0;i<n;i++){
  		  led[i]=parse_ulong(argv[3+i],10);
  	  }
  	  printf("kbc:: scan()\n"); /* Actually, it was already invoked */
  	  kbd_test_leds(sec,led);
  	  return 0;
    } else if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0) {
  	  if( argc != 3 ) {
  		  printf("kbc: wrong no of arguments for test of timed_scan() \n");
  		  return 1;
  	  }
  	  n=parse_ulong(argv[2],10);
  	  printf("kbc:: timed_scan()\n"); /* Actually, it was already invoked */
  	  kbd_test_timed_scan(n);
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
	  printf("kbc: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}
