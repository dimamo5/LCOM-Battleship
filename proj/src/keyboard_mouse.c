#include "keyboard.h"


static unsigned int hook_id;
unsigned long code;

int kbd_subscribe_int() {
	unsigned int bit_sel = 2; // bit_sel is different from the one on the timer_subscribe
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

int check_codes_menus() {

	int ipc_status;
	unsigned int i = 0, r;
	message msg;
	short irq_set = kbd_subscribe_int();

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (code != SC_ENTER || code != SC_UP_ARROW || code != SC_DOWN_ARROW || code!= BC_ESC) {

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
					scan_asm();
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
	switch (code){
		case SC_ENTER:
			return 0;
			break;
		case SC_UP_ARROW:
			return 1;
			break;
		case SC_DOWN_ARROW:
			return 2;
			break;
		case BC_ESC:
			return 3;
			break;
	}
}

int check_codes_game() {

	int ipc_status;
	unsigned int i = 0, r;
	message msg;
	short irq_set = kbd_subscribe_int();

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (code != SC_LEFT_ARROW || code != SC_UP_ARROW || code != SC_DOWN_ARROW || code!= SC_RIGHT_ARROW || code!= SC_ENTER || code!= BC_ESC) {

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
					scan_asm();
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
	switch (code){
		case SC_ENTER:
			return 0;
			break;
		case SC_UP_ARROW:
			return 1;
			break;
		case SC_DOWN_ARROW:
			return 2;
			break;
		case SC_LEFT_ARROW:
			return 3;
			break;
		case SC_RIGHT_ARROW:
			return 4;
			break;
		case BC_ESC:
			return 5;
			break;
	}
}
