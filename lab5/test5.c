void *test_init(unsigned short mode, unsigned short delay) {
	unsigned char* mem;
	mem = vg_init(0x105);
	timer_test_int(delay);
	vg_exit();
	printf("0x%X", mem);

}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	vg_init(0x105);
	vg_fill(x, y, size, size, color);
	timer_test_int(5);
	vg_exit();
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {
	vg_init(0x105);
	vg_line(xi, yi, xf, yf, color);
	timer_test_int(5);
	vg_exit();

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	/* To be completed */

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {

	/* To be completed */

}

