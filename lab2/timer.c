#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "timer.h"
#include "i8254.h"

unsigned int counter;

static int hook_id;

int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned long resultado_freq,control_word;
	resultado_freq = TIMER_FREQ / freq;
	control_word = TIMER_LSB_MSB|TIMER_SQR_WAVE|TIMER_BIN;
	unsigned long freq_lsb = resultado_freq;
	unsigned long freq_msb = resultado_freq >> 8;

	if (timer == 0)
	{
		control_word |= TIMER_SEL0;
		if(sys_outb(TIMER_CTRL,control_word)!=OK)
			printf("erro1");
		if(sys_outb(TIMER_0, freq_lsb)!=OK)
			printf("erro2");
		if(sys_outb(TIMER_0, freq_msb)!=OK)
			printf("erro1");
	}
	else if (timer == 1)
	{
		control_word |= TIMER_SEL1;
		sys_outb(TIMER_CTRL, control_word);
		sys_outb(TIMER_0, freq_lsb);
		sys_outb(TIMER_0, freq_msb);
	}
	else if (timer == 2)
	{
		control_word |= TIMER_SEL2;
		sys_outb(TIMER_CTRL, control_word);
		sys_outb(TIMER_0, freq_lsb);
		sys_outb(TIMER_0, freq_msb);
	}

	return 1;
}

int timer_subscribe_int(void ) {
	hook_id=BIT(1);
	int hook_id_temp=hook_id;
    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE , &hook_id) != OK)
    	return -1;
    if(sys_irqenable(&hook_id) != OK)
        return -1;
    hook_id=hook_id_temp;
	return hook_id;
}

int timer_unsubscribe_int() {

    if(sys_irqrmpolicy(&hook_id) != OK)
    	return 1;
    if(sys_irqdisable(&hook_id) != OK)
        return 1;

	return 0;
}

void timer_int_handler() {
	++counter;
}


int timer_get_conf(unsigned long timer, unsigned char *st) {

	unsigned char temp; //Initialize ReadBack Command
	temp = TIMER_RB_CMD | TIMER_RB_SEL(timer) |TIMER_RB_COUNT_; // Read Back Command
	sys_outb(TIMER_CTRL,temp); //execute previous command
	unsigned char timer_sel=TIMER_0+timer;
	unsigned long temp_st; // Auxiliar long variable, to be filled
	sys_inb(timer_sel,&temp_st); // There is no need for a cast this way
	*st = temp_st;  // The original variable st is filled as supposed and the unimportant bits are truncated while passing a long to a char
	return 0;
}

int timer_display_conf(unsigned char conf) {
	if(conf & TIMER_BCD){
		printf("\nBCD \n");
	}
	else printf("\nBINARY \n");

	if((conf & TIMER_SQR_WAVE)>>1==3){
		printf("MODE 3 \n");
	}
	else if ((conf & TIMER_RATE_GEN)>>1==2)
		printf("MODE 2 \n");
	else if ((conf & TIMER_HW_RTRIG)>>1==1)
		printf("MODE 1 \n");
	else if ((conf & TIMER_INTR)>>1==0)
		printf("MODE 0 \n");

	if ((conf & TIMER_LSB_MSB)>>4==3){
		printf("LSB followed by MSB \n");
	}
	else if((conf & TIMER_MSB)>>4==2){
		printf("MSB \n");
	}
	else if ((conf & TIMER_LSB)>>4==1){
		printf("LSB \n");
	}

	if ((conf & BIT(6))>>(6)){
		printf("NULL COUNTER = 1 \n");
	}
	else printf("NULL COUNTER = 0 \n");

	if((conf & BIT(7))>>(7)){
		printf("OUT PIN = 1 \n\n");
	}
	else printf("OUT PIN = 0 \n\n");

	return 0;
}

int timer_test_square(unsigned long freq) {
	timer_set_square(0,freq);
	return 0;
}

int timer_test_int(unsigned long time) {

	int ipc_status;
	counter=0;
	unsigned int i=0,r;
	message msg;
	short irq_set=timer_subscribe_int();
	timer_test_square(60); //Por para a frequencia normal

	while(counter < time)
	{
		/* ANY -> receives msg from any process
		 *  2nd and 3rd arguments are the addresses of variables of type message and int
		 */
		if((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if( is_ipc_notify(ipc_status) ) // receive notification of interrupt request. returns true if msg received is notification and false otherwise
		{
			switch(_ENDPOINT_P(msg.m_source)) // m_source contains the endpoint of the msg and _ENDPOINT extracts the process identifier from process's endpoint
			{
			case HARDWARE:
				if(msg.NOTIFY_ARG && irq_set)
				{
                    i++;
                    if(i%60==0){
                    	timer_int_handler();
                    	printf("Interrupt %d \n",counter); // message printed when interrupt received
                    }
				}
				break;

			default:
				break;
			}
		}
		else
		{
			printf("Any interrupt received\n");     // Any interrupt received, so anything to do
		}
	}
	timer_unsubscribe_int();

	return 0;
}

int timer_test_config(unsigned long timer) {
	if(timer<0||timer>2){
		printf("Timer tem de ser entre 0 e 2 \n");
		return -1;
	}
	unsigned char test;
	timer_get_conf(timer,&test);
	timer_display_conf(test);
	return 0;
}
