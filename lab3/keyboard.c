#include "keyboard.h"

static unsigned int hook_id;
static unsigned long code;

unsigned long counter_timer;

int kbd_subscribe_int() {
	unsigned int bit_sel = 2;
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

unsigned char get_command(int led1, int led2, int led3) {
	unsigned char command = 0x00;
	if (led1) {
		command |= LED_SCRLOCK;
	}
	if (led2) {
		command |= LED_NUMPAD;
	}
	if (led3) {
		command |= LED_CAPSLOCK;
	}
	return command;
}

int kbd_send_command(unsigned char com) {
	unsigned long resp;
	unsigned int counter = 1;
	do {
		if (counter == 1) {
			sys_outb(KBD_BUFF, SWITCH_LED);
			sys_inb(KBD_BUFF, &resp);
		}

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
	int m = 0;
	int led1 = 0; // led1 status - initialized as turned off
	int led2 = 0; // led2 status - initialized as turned off
	int led3 = 0; // led3 status - initialized as turned off
	unsigned int i = 0, r;

	while (m != n) {

		if (m == 0) {
			kbd_send_command(get_command(0, 0, 0)); // at start, every led is turned off
			m++; // increments m and goes to next iteration of the cycle
			continue;
		}

		if (led[m] == 0) {   // if the array element being analyzed is the 1st led (led1)
			if (led1 == 0) {
				led1 = 1;    // if it was off, change the status to 1- "on/going to be turned on"
			} else
				led1 = 0;    // if it was on, change the status to 0 - "off/going to be turned off"
		}
		if (led[m] == 1) {    // same as before, but with the 2nd led
			if (led2 == 0) {
				led2 = 1;
			} else
				led2 = 0;
		}
		if (led[m] == 2) {    // same as before, but with the 3rd led
			if (led3 == 0) {
				led3 = 1;
			} else
				led3 = 0;
		}
		kbd_send_command(get_command(led1, led2, led3));
		timer_test_int(1); // waits 1 second before the next led is turned on/off
		m++;
		continue;
	}

	return 0;
}

int kbd_scan_c() {
	int ipc_status, flagstop = 1;
	unsigned int i = 0, r;
	message msg;
	unsigned short byte2;
	short irq_set = kbd_subscribe_int();

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (flagstop) {

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
					if (code == SCAN_2BYTE) {
						byte2 = 1;
						break;
					}
					if (byte2) {
						code |= (SCAN_2BYTE << 8);
						byte2 = 0;
					}
					if (code & BREAKCODE) {
						printf("Breakcode: 0x%04X \n", code);
					} else {
						printf("Makecode: 0x%04X \n", code);
					}
					if (code == BC_ESC) {
						flagstop = 0;
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

int kbd_timed_scan(unsigned short sec) {
	int ipc_status, flag_stop = 1;
	unsigned int i = 0, r;
	unsigned int counter_milisec = 0;
	counter_timer = 0;
	message msg;
	unsigned short byte2;
	short irq_set = kbd_subscribe_int();
	short irq_set_time = timer_subscribe_int();

	timer_test_square(60);

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (flag_stop) {

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
					counter_timer = 0;
					counter_milisec = 0;
					kbd_int_handler();
					if (code == SCAN_2BYTE) {
						byte2 = 1;
						break;
					}
					if (byte2) {
						code |= (SCAN_2BYTE << 8);
						byte2 = 0;
					}
					if (code & BREAKCODE) {
						printf("Breakcode: 0x%04X \n", code);
					} else {
						printf("Makecode: 0x%04X \n", code);
					}
					if (code == BC_ESC) {
						flag_stop = 0;
						printf("Press ENTER to leave");
						break;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_time) {
					counter_milisec++;
					if (counter_milisec % 60 == 0) {
						counter_timer++;
					}
					if (sec <= counter_timer) {
						printf("Acabou o tempo");
						flag_stop = 0;
						break;
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

	if (timer_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}
	return 0;
}
