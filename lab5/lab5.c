#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>

#include "test5.h"
#include "pixmap.h"
static void print_usage(char *argv[]);
static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);

int main(int argc, char **argv) {

	/* Initialize service */
	sef_startup();

	printf("lab5: VIDEO GRAPHICS MODE \n");

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
					"\t service run %s -args \"init <mode, delay>\" \n"
					"\t service run %s -args \"square <x, y, size, color>\"\n"
					"\t service run %s -args \"line <xi, yi, xf, yf, color>\" \n"
					"\t service run %s -args \"xpm <xi, yi, XPM image> \" \n"
					"\t service run %s -args \"move <xi, yi, XPM image, dir, pixels, time > \" \n"
					"\t service run %s -args \"controller\" \n",

			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned short mode, delay, x, y, size, xi, yi, xf, yf, hor, time;
	char * pixmap = "";
	short delta;
	unsigned long color;

	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("video_gr:: wrong no of arguments for test_init() \n");
			return 1;
		}
		mode = parse_ulong(argv[2], 16);
		delay = parse_ulong(argv[3], 10);
		printf("video_gr:: test_init()\n"); /* Actually, it was already invoked */
		test_init(mode, delay);
		return 0;
	} else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("video_gr:: wrong no of arguments for test_square() \n");
			return 1;
		}

		printf("video_gr:: test_square()\n"); /* Actually, it was already invoked */
		x = parse_ulong(argv[2], 10);
		y = parse_ulong(argv[3], 10);
		size = parse_ulong(argv[4], 10);
		color = parse_ulong(argv[5], 16);
		test_square(x, y, size, color);
		return 0;
	} else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf(
					"video_gr: wrong no of arguments for test of test_line() \n");
			return 1;
		}
		printf("video_gr:: test_line()\n"); /* Actually, it was already invoked */
		xi = parse_ulong(argv[2], 10);
		yi = parse_ulong(argv[3], 10);
		xf = parse_ulong(argv[4], 10);
		yf = parse_ulong(argv[5], 10);
		color = parse_ulong(argv[6], 16);
		test_line(xi, yi, xf, yf, color);
		return 0;
	} else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 5) {
			printf("video_gr:: wrong no of arguments for test_xpm() \n");
			return 1;
		}

		printf("video_gr:: test_xpm()\n");
		x = parse_ulong(argv[2], 10);
		y = parse_ulong(argv[3], 10);
		pixmap = argv[4];
		test_xpm(x, y, get_xpm(pixmap));
		return 0;
	} else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		if (argc != 2) {
			printf("video_gr:: wrong no of arguments for test_controller() \n");
			return 1;
		}

		printf("video_gr:: test_controller()\n");
		test_controller();
		return 0;
	} else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if (argc != 8) {
			printf("video_gr:: wrong no of arguments for test_move() \n");
			return 1;
		}
		printf("video_gr:: test_move()\n");
		xi = parse_ulong(argv[2], 10);
		yi = parse_ulong(argv[3], 10);
		pixmap = argv[4];
		hor = parse_ulong(argv[5], 10);
		delta = parse_long(argv[6], 10);
		time = parse_ulong(argv[7], 10);
		test_move(xi, yi, get_xpm(pixmap), hor, delta, time);
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

static long parse_long(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtol(str, &endptr, base);

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MAX;
	}

	if (endptr == str) {
		printf("video_txt: parse_long: no digits were found in %s \n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}

