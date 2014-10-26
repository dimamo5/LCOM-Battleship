#include "test3.h"
#include "keyboard.h"

int kbd_test_scan(unsigned short ass) {
	if(!ass){
		kbd_scan_c();
	}
	//else kbd_scan_ass;
}


int kbd_test_leds(unsigned short n, unsigned short *leds) {
	kbd_led(n,leds);
	return 1;
}
int kbd_test_timed_scan(unsigned short n) {
	kbd_timed_scan(n);
}
