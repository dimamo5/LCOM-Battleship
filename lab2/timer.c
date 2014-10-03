#include <minix/syslib.h>
#include <minix/drivers.h>
#include <timer.h>
#include <i8254.h>

int timer_set_square(unsigned long timer, unsigned long freq) {

	return 0;
}

int timer_subscribe_int(void ) {

	return 0;
}

int timer_unsubscribe_int() {

	return 0;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	if(timer<0||timer>2){
			return -1;
		}
	unsigned char temp=0xC0; //Initialize ReadBack Command
	temp |= TIMER_RB_SEL(timer) | TIMER_RB_STATUS_|TIMER_RB_COUNT_; // Read Back Command
	*st=temp; //execute previous command
	
	return 0;
}

int timer_display_conf(unsigned char conf) {
	unsigned char temp=*(0x43);
	
	return 0;
}

int timer_test_square(unsigned long freq) {
	
	return 0;
}

int timer_test_int(unsigned long time) {
	
	return 0;
}

int timer_test_config(unsigned long timer) {
	if(timer<0||timer>2){
		return -1;
	}
	//int timer_get_conf(unsigned char timer, unsigned char *st)
	//int timer_display_conf(unsigned char conf)
	return 0;
}
