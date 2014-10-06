#include <minix/syslib.h>
#include <minix/drivers.h>
#include "timer.h"
#include "i8254.h"

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
	unsigned char temp=0xC0;//Initialize ReadBack Command
	temp |= TIMER_RB_SEL(timer) | TIMER_RB_STATUS_|TIMER_RB_COUNT_; // Read Back Command
	sys_outb(TIMER_CTRL,temp);//execute previous command
	printf("0x%X \n",temp);
	if()
	sys_inb(,(unsigned long *)st);

	return 0;
}

int timer_display_conf(unsigned char conf) {
	int i =0;
	int bcd,mode,t_access,null_counter,output;
	unsigned char conf_temp;
		conf_temp=conf;
		conf_temp &= 0x01;
		bcd=conf_temp;
		conf_temp=conf;
		conf_temp>>1;
		conf_temp &= 0x07;
		mode=conf_temp;
		conf_temp=conf;
		conf_temp>>4;
		conf_temp &= 0x02;
		t_access=conf_temp;
		conf_temp=conf;
		conf_temp>>6;
		conf_temp &=0x01;
		null_counter=conf_temp;
		conf_temp>>7;
		conf_temp &=0x01;
		output=conf_temp;

		if(bcd){
			printf("bcd \n");
		}
		else printf("binary \n");
		if(mode==3){
			printf("mode 3 \n");
		}
		else if(mode==2){
			printf("mode 2 \n");
		}
		else if(mode==1){
			printf("mode 1 \n");
		}
		else if(mode==0){
			printf("mode 0 \n");
		}
		if (t_access==3){
			printf("LSB followed by MSB \n");
		}
		else if(t_access==2){
			printf("MSB \n");
		}
		else if (t_access==1){
			printf("LSB \n");
		}
		if(null_counter){
			printf("null counter \n");
		}
		else printf("counter available \n");
		if(output){
			printf("out pin = 1 \n");
		}
		else printf("out pin = 0 \n");
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
	unsigned char test;
	timer_get_conf(timer,&test);
	timer_display_conf(test);
	return 0;
}
