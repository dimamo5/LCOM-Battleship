#include "test3.h"

static unsigned int hook_id;

int kbd_subscribe_int() {
	unsigned int bit_sel=1;
	hook_id=bit_sel;
	if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE , &hook_id) != OK)
		return -1;
	if(sys_irqenable(&hook_id) != OK)
		return -1;
	return BIT(bit_sel);
}

int kbd_unsubscribe_int() {

	if(sys_irqdisable(&hook_id) != OK){
		return 1;
	}
	if(sys_irqrmpolicy(&hook_id) != OK){
		return 1;
	}

	return 0;
}

int kbd_int_handler() {
	unsigned long code;
	sys_inb(KBC_OUTBUF,&code);
	if(code>>7){
		printf("Breakcode: 0x%X \n",code);
	}
	else{
		printf("Makecode: 0x%X \n",code);
	}
	if (code==81){
		return 0;
	}

	return 1;
}

int kbd_scan_c(){
	int ipc_status,int_leave=0;
	unsigned int i=0,r;
	message msg;
	short irq_set=kbd_subscribe_int();

	if(irq_set < 0){
		printf("Subscribe failed");
		return 1;
	}
	while(1){
		if(int_leave){
			break;
		}
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
				if(msg.NOTIFY_ARG & irq_set)
				{
					if(kbd_int_handler())
						continue;
					else int_leave=1;
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

	if(kbd_unsubscribe_int()!=0){
			printf("Unsubscribe failed");
		}
		return 0;
}

int kbd_test_scan(unsigned short ass) {
	if(!ass){
		kbd_scan_c();
	}
	//else kbd_scan_ass;
}


int kbd_test_leds(unsigned short n, unsigned short *leds) {
	/* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
	/* To be completed */
}
