#include "keyboard.h"


static unsigned int hook_id;
static unsigned long code;

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
	sys_inb(KBC_OUTBUF,&code);
	return 1;
}

int kbd_scan_c(){
	int ipc_status,int_cont=1;
	unsigned int i=0,r;
	message msg;
	short irq_set=kbd_subscribe_int();

	if(irq_set < 0){
		printf("Subscribe failed");
		return 1;
	}
	while(int_cont){

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
					if(code>>7){
							printf("Breakcode: 0x%X \n",code);
						}
						else{
							printf("Makecode: 0x%X \n",code);
						}
						if (code==BC_ESC){
							int_cont=0;
							printf("Press ENTER to leave");
							return 0;
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

	if(kbd_unsubscribe_int()!=0){
			printf("Unsubscribe failed");
		}
		return 0;
}

