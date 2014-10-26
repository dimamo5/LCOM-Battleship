#include "keyboard.h"

static unsigned int hook_id;
static unsigned long code;

int kbd_subscribe_int() {
	unsigned int bit_sel = 1;
	hook_id = bit_sel;
	if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK)
		return -1;
	if (sys_irqenable(&hook_id) != OK)
		return -1;
	return BIT(bit_sel);
}

int kbd_unsubscribe_int() {

	if (sys_irqdisable(&hook_id) != OK) {
		return 1;
	}
	if (sys_irqrmpolicy(&hook_id) != OK) {
		return 1;
	}

	return 0;
}

int kbd_int_handler() {
	sys_inb(KBD_BUFF, &code);
	return 1;
}

int kbd_status() {
	unsigned long status, lixo;
	sys_inb(KBC_STATUS, &status);
	if (status & BUFFER_FULL) {
		tickdelay(micros_to_ticks(DELAY_US));
		sys_inb(KBD_BUFF, &lixo);
	}
	return 1;
}

unsigned char get_command(int led1,int led2,int led3){
	unsigned char command=0x00;
	if(led1){
		command|=LED_SCRLOCK;
	}
	if(led2){
		command |=LED_NUMPAD;
	}
	if(led3){
		command |=LED_CAPSLOCK;
	}
	return command;
}

int kbd_send_command(unsigned char com) {
	unsigned long resp;
	unsigned int counter = 1;
	do { if(counter==1){
		sys_outb(KBD_BUFF, SWITCH_LED);
		sys_inb(KBD_BUFF, &resp);}

		while (kbd_analisa(resp) == 0) {
			if (kbd_analisa(resp) == 0 && counter == 1) {
				sys_outb(KBD_BUFF, SWITCH_LED);
				sys_inb(KBD_BUFF, &resp);
			} else if (kbd_analisa(resp) == 0 && counter == 2) {
				sys_outb(KBD_BUFF, com);
				sys_inb(KBD_BUFF, &resp);

			}
		}

		if (kbd_analisa(resp) == 2 && counter == 1) { //acknowledged after 1st cycle
			sys_outb(KBD_BUFF, com);
			sys_inb(KBD_BUFF, &resp);
			counter++; // Counter passa para 2, ou seja, vai para o 2o ciclo write/read
		}
		if (kbd_analisa(resp) == 2 && counter == 2) { //acknowledged after 2nd cycle
			break;
		}

		else if (kbd_analisa(resp) == 1) { // Error, start from the beggining of the loop
			counter = 1;
			continue;

		}
	} while (1);
	return 1;

}

int kbd_analisa(int resp) {
	if (resp == RESEND) {
		return 0;
	} else if (resp == ERROR) {
		return 1;
	} else if (resp == ACK) {
		return 2;
	}
}

int kbd_led(int n, unsigned short led[]) {
	int ipc_status, m = 0;
	int led1,led2,led3;
	unsigned int i = 0, r;
	message msg;
	short irq_set = kbd_subscribe_int();

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (m != n) {

		/* ANY -> receives msg from any process
		 *  2nd and 3rd arguments are the addresses of variables of type message and int
		 */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) // receive notification of interrupt request. returns true if msg received is notification and false otherwise
				{
			switch (_ENDPOINT_P(msg.m_source)) // m_source contains the endpoint of the msg and _ENDPOINT extracts the process identifier from process's endpoint
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					if(m=0){
						kbd_send_command(get_command(0,0,0));//tudo desligado
						continue;
					}
					if(led[m]==0){
						led1=!led1;
					}
					if(led[m]==1){
						led2=!led2;
					}
					if(led[m]==2){
						led3=!led3;
					}
					kbd_send_command(get_command(led1,led2,led3));
					m++;
					continue;
				}
				break;

			default:
				break;
			}
		} else {
			printf("Any interrupt received\n"); // Any interrupt received, so anything to do
		}
	}

	if (kbd_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}
	return 0;
}

int kbd_scan_c() {
	int ipc_status, int_cont = 1;
	unsigned int i = 0, r;
	message msg;
	unsigned short byte2;
	short irq_set = kbd_subscribe_int();

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (int_cont) {

		/* ANY -> receives msg from any process
		 *  2nd and 3rd arguments are the addresses of variables of type message and int
		 */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) // receive notification of interrupt request. returns true if msg received is notification and false otherwise
				{
			switch (_ENDPOINT_P(msg.m_source)) // m_source contains the endpoint of the msg and _ENDPOINT extracts the process identifier from process's endpoint
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					kbd_int_handler();
					if(code==SCAN_2BYTE){
						byte2=1;
						break;
					}
					if(byte2){
						code|=(SCAN_2BYTE<<8);
						byte2=0;
					}
						if (code&BREAKCODE) {
							printf("Breakcode: 0x%04X \n", code);
						} else {
							printf("Makecode: 0x%04X \n", code);
						}
						if (code == BC_ESC) {
							int_cont = 0;
							printf("Press ENTER to leave");
							return 0;
						}
					}
				break;

			default:
				break;
			}
		} else {
			printf("Any interrupt received\n"); // Any interrupt received, so anything to do
		}
	}

	if (kbd_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}
	return 0;
}

