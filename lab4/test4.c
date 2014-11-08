#include "test4.h"
#include "mouse.h"

int test_packet(unsigned short cnt){
	return mouse_packet(cnt);
}	
	
int test_async(unsigned short idle_time) {
	return mouse_packet_async (idle_time);
}
	
int test_config(void) {
    /* To be completed ... */
}	
	
int test_gesture(short length, unsigned short tolerance) {
    /* To be completed ... */
}
