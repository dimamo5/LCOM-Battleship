#include "mouse.h"

static unsigned int hook_id;
unsigned long code;

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

int enable_packets() {
	kbd_in_status();
	unsigned long resp;
	do {
		if (sys_outb(KBC_STATUS, MOUSE_WC) != OK)
			printf("Error at sys_outb.");
		if (sys_outb(KBD_BUFF, ENABLE_PACKETS) != OK)
			printf("Error at 2 sys_outb.");
		if (sys_inb(KBD_BUFF, &resp) != OK)
			printf("Error at sys_inb.");
		if (resp == ACK) {
			return 0;
		}
		sys_inb(KBD_BUFF, &resp);
		sys_inb(KBD_BUFF, &resp); //TODO ver destes ACK
		sys_inb(KBD_BUFF, &resp);
		sys_inb(KBD_BUFF, &resp);
		sys_inb(KBD_BUFF, &resp);
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
			(packet[0] & BIT(4)) >> 4, (packet[0] & BIT(5)) >> 5, packet[1],
			packet[2]);
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

	if (mouse_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}
	if (timer_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}

	return 0;
}

