#include "test4.h"
#include "mouse.h"

static void print_usage(char *argv[]);
static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);

int main(int argc, char **argv) {

	/* Initialize service */
	sef_startup();

	printf("lab4: MOUSE \n");

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;
}

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following tests:\n"
					"\t service run %s -args \"packet <number of packets to receive>\" \n"
					"\t service run %s -args \"async <number of sec to exit>\"\n"
					"\t service run %s -args \"config\" \n"
					"\t service run %s -args \"gesture <length,tolerance> \" \n",

			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned short number_packets, idle_time, length, tolerance;

	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("mouse:: wrong no of arguments for test_packet() \n");
			return 1;
		}
		number_packets = parse_ulong(argv[2], 10);
		printf("mouse:: packets()\n"); /* Actually, it was already invoked */
		test_packet(number_packets);
		return 0;
	} else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
			printf("mouse:: wrong no of arguments for test_async() \n");
			return 1;
		}

		printf("mouse:: async()\n"); /* Actually, it was already invoked */
		idle_time = parse_ulong(argv[2], 10);
		mouse_packet_async(idle_time);
		return 0;
	} else if (strncmp(argv[1], "config", strlen("config")) == 0) {
		if (argc != 2) {
			printf("mouse: wrong no of arguments for test of test_config() \n");
			return 1;
		}
		printf("mouse:: config()\n"); /* Actually, it was already invoked */
		test_config();
		return 0;
	} else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if (argc != 4) {
			printf("mouse:: wrong no of arguments for test_gesture() \n");
			return 1;
		}

		printf("mouse:: gesture()\n"); /* Actually, it was already invoked */
		length = parse_ulong(argv[2], 10);
		tolerance = parse_ulong(argv[3], 10);
		mouse_gesture(length,tolerance);
		return 0;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
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
