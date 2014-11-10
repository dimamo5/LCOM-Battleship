#include "mouse.h"

static unsigned int hook_id;
unsigned long code;

static short x_total, y_total, l, t;

int mouse_subscribe_int() {
	unsigned int bit_sel = 3; // bit_sel is different from the one on the timer_subscribe and kbc_subscrive (just in case)
	hook_id = bit_sel;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)
			!= OK)
		return -1;
	if (sys_irqenable(&hook_id) != OK)
		return -1;
	return BIT(bit_sel);
}

int mouse_unsubscribe_int() {

	if (sys_irqdisable(&hook_id) != OK) {
		return 1;
	}
	if (sys_irqrmpolicy(&hook_id) != OK) {
		return 1;
	}

	return 0;
}

int mouse_int_handler() {
	if (sys_inb(KBD_BUFF, &code) != OK) { //sends content from the out buffer to the variable
		printf("Int Handler Error at sys_inb.");
		return 1;
	}
	return 0;
}

int kbd_out_status() {
	unsigned long status;

	if (sys_inb(KBC_STATUS, &status) != OK) { // reads the status
		printf("Error at kbd_out_status");
		return 1;
	}

	if (status & OUT_BUFFER_FULL) { // if the out buffer is ready to be read
		return 0;
	}

	return 1;
}

int kbd_in_status() {
	unsigned long status, lixo;

	if (sys_inb(KBC_STATUS, &status) != OK) { // reads the status
		printf("Error at kbd_in_status");
		return 1;
	}
	while (status & IN_BUFFER_FULL) { // compares if the In buffer is full or not
		tickdelay(micros_to_ticks(DELAY_US)); //20ms delay to let it process
		sys_inb(KBD_BUFF, &lixo);
		sys_inb(KBC_STATUS, &status);
	}
	return 0;
}
int send_cmd_config() {
	kbd_in_status();
	unsigned long resp;
	do {
		if (sys_outb(KBC_STATUS, MOUSE_WC) != OK)
			printf("Error at sys_outb.");
		if (sys_outb(KBD_BUFF, CMD_CONFIG) != OK)
			printf("Error at 2 sys_outb.");
		if (sys_inb(KBD_BUFF, &resp) != OK)
			printf("Error at sys_inb.");
		if (resp == ACK) {
			return 0;
		}

	} while (resp != ACK);

}

int enable_packets() {
	kbd_in_status();
	unsigned long resp;
	do {
		if (sys_outb(KBC_STATUS, ENABLE_MOUSE) != OK)
			printf("Error at sys_outb.");
		if (sys_outb(KBC_STATUS, MOUSE_WC) != OK)
			printf("Error at sys_outb.");
		if (sys_outb(KBD_BUFF, ENABLE_PACKETS) != OK)
			printf("Error at 2 sys_outb.");
		if (sys_inb(KBD_BUFF, &resp) != OK)
			printf("Error at sys_inb.");
		if (resp == ACK) {
			return 0;
		}
	} while (resp != ACK);
}

int disable_packets() {
	kbd_in_status();
	unsigned long resp;
	do {
		if (sys_outb(KBC_STATUS, MOUSE_WC) != OK)
			printf("Error at sys_outb.");
		if (sys_outb(KBD_BUFF, DISABLE_PACKETS) != OK)
			printf("Error at 2 sys_outb.");
		if (sys_inb(KBD_BUFF, &resp) != OK)
			printf("Error at sys_inb.");
		if (resp == ACK) {
			return 0;
		}
	} while (resp != ACK);
}

int check_first_byte() {
	if (BIT(3) & code) {
		return 1;
	} else
		return 0;
}

void print_packets(unsigned char* packet) {
	//B1=0x8	B2=0x12	B3=0x14	LB=0	MB=0	RB=0	XOV=0	YOV=0	X=18	Y=20
	printf(
			"B1=0x%X\tB2=0x%X\tB3=0x%X\tLB=%d\tMB=%d\tRB=%d\tXOV=%d\tYOV=%d\tX=%d\tY=%d\n\n",
			packet[0], packet[1], packet[2], (packet[0] & BIT(0)),
			(packet[0] & BIT(2)) >> 2, (packet[0] & BIT(1)) >> 1,
			(packet[0] & BIT(6)) >> 6, (packet[0] & BIT(7)) >> 7,
			(char) packet[1], (char) packet[2]);
}

int print_config(unsigned char* config) {

	if (config[0] & REMOTE_BIT) {
		printf("\nRemote (polled) mode\n");
	} else
		printf("\nStream Mode\n");

	if (config[0] & ENABLE_BIT) {
		printf("Data reporting enabled\n");
	} else
		printf("Data reporting disable\n");

	if (config[0] & SCALING_BIT) {
		printf("Scaling is 2:1\n");
	} else
		printf("Scaling is 1:1\n");

	if (config[0] & LEFT_BT_BIT) {
		printf(" Left button is currently pressed\n");
	} else
		printf("Left button is currently  released\n");

	if (config[0] & MIDDLE_BT_BIT) {
		printf(" Middle button is currently pressed\n");
	} else
		printf("Middle button is currently  released\n");

	if (config[0] & RIGHT_BT_BIT) {
		printf(" Right button is currently pressed\n");
	} else
		printf("Right button is currently  released\n");

	if (config[1] == 0) {
		printf("1 count per mm\n");
	} else if (config[1] == 1) {
		printf("2 count per mm\n");
	} else if (config[1] == 2) {
		printf("4 count per mm\n");
	} else if (config[1] == 3) {
		printf("8 count per mm\n");
	}

	printf("Sample rate: %d\n", config[2]);

}

int mouse_packet(unsigned short number_packets) {
	int ipc_status;
	unsigned int number_packets_received = 0, r, byte_packets = 1;
	unsigned char packets[3];
	message msg;
	short irq_set = mouse_subscribe_int();

	enable_packets();

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (number_packets_received < number_packets) {

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

					mouse_int_handler();

					if (byte_packets == 1) {
						if (!check_first_byte()) {
							byte_packets = 1;
							continue;
						}
					}

					packets[byte_packets - 1] = code;

					if (byte_packets == 3) {
						byte_packets = 1;
						print_packets(packets);
						number_packets_received++;
						continue;
					}
					byte_packets++;
				}
				break;

			default:
				break;
			}
		} else {
			printf("Any interrupt received\n"); // Any interrupt received, so anything to do
		}
	}

	disable_packets();

	if (mouse_unsubscribe_int() != OK) {
		printf("Unsubscribe failed");
	}
	return 0;
}

int mouse_packet_async(unsigned short idle_time) {
	int ipc_status, flag_stop = 1;
	unsigned int number_packets_received = 0, r, byte_packets = 1,
			counter_milisec = 0, counter_timer = 0;
	unsigned char packets[3];
	message msg;
	short irq_set = mouse_subscribe_int();
	short irq_set_time = timer_subscribe_int();

	enable_packets();

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

					mouse_int_handler();

					if (byte_packets == 1) {
						if (!check_first_byte()) {
							byte_packets = 1;
							continue;
						}
					}

					packets[byte_packets - 1] = code;

					if (byte_packets == 3) {
						byte_packets = 1;
						print_packets(packets);
						number_packets_received++;
						continue;
					}
					byte_packets++;
				}

				if (msg.NOTIFY_ARG & irq_set_time) {
					counter_milisec++;
					if (counter_milisec % 60 == 0) {
						counter_timer++;
					}
					if (idle_time <= counter_timer) {
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

	disable_packets();

	if (mouse_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}
	if (timer_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}

	return 0;
}

int config() {
	int n = 0, ipc_status,r;
	message msg;
	unsigned char config[3];
	short irq_set = mouse_subscribe_int();

	disable_packets();

	send_cmd_config();

	while (n < 3) {
		/* Get a request message. */
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {

			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_int_handler();
					config[n] = code;
					n++;

				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	mouse_unsubscribe_int();
	print_config(config);

	return 0;
}

typedef enum {
	INIT, DRAW, COMP
} state_t;
typedef enum {
	LDOWN, LUP, MOVE
} ev_type_t;

typedef struct {
	ev_type_t type;
	signed char x;
	signed char y;
} event_t;

void check_hor_line(event_t *evt) {
	static state_t st = INIT; // initial state; keep state
	switch (st) {
	case INIT:
		x_total = 0;
		y_total = 0;
		if (evt->type == LDOWN)
			st = DRAW;
		break;
	case DRAW:
		if (evt->type == MOVE) {
			if ((abs(x_total + evt->x) < abs(x_total)) && (abs(y_total) > t)) {
				x_total = 0;
				y_total = 0;
			} else {
				x_total += evt->x;
				y_total += evt->y;

				if (abs(x_total) >= l) {
					st = COMP;
				}
			}

		} else if (evt->type == LUP) {
			st = INIT;
		}
		break;
	default:
		break;
	}
}

int check_ldown(unsigned char* packet) {
	if (packet[1] == 0 && packet[2] == 0) {
		if (packet[0] & BIT(0)) {
			return LDOWN;
		} else
			return LUP;
	} else
		return MOVE;
}

int mouse_gesture(short length, unsigned short tolerance) {
	int ipc_status;
	unsigned int number_packets_received = 0, r, byte_packets = 1;
	unsigned char packets[3];
	message msg;
	short irq_set = mouse_subscribe_int();
	x_total = 0;
	y_total = 0;
	unsigned short flagstop = 1; //0 inicio
	l = length;
	t = tolerance;
	event_t evento;

	enable_packets();

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

					mouse_int_handler();

					if (byte_packets == 1) {
						if (!check_first_byte()) {
							byte_packets = 1;
							continue;
						}
					}

					packets[byte_packets - 1] = code;

					if (byte_packets == 3) {
						byte_packets = 1;
						print_packets(packets);
						evento.type = check_ldown(packets);
						evento.x = packets[1];
						evento.y = packets[2];
						check_hor_line(&evento);
						if (abs(x_total) >= l && abs(y_total) < t) {
							flagstop = 0;
							continue;
						}
						number_packets_received++;
						continue;
					}
					byte_packets++;
				}
				break;

			default:
				break;
			}
		} else {
			printf("Any interrupt received\n"); // Any interrupt received, so anything to do
		}
	}

	disable_packets();

	if (mouse_unsubscribe_int() != OK) {
		printf("Unsubscribe failed");
	}
	return 0;
}
