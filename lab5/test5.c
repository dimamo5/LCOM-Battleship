#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>

#include "test5.h"
#include "keyboard.h"
#include "sprite.h"

void *test_init(unsigned short mode, unsigned short delay) {
	unsigned char* mem;
	mem = vg_init(mode);
	timer_test_int(delay);
	vg_exit();
	printf("Virtual Memory : 0x%X", mem);

}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	vg_init(0x105);
	vg_fill(x, y, size, size, color);

	// Inicio keyboard
	int ipc_status;
	unsigned long code;
	unsigned int i = 0, r;
	message msg;
	short irq_set = kbd_subscribe_int();

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (code != BC_ESC) {

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
					sys_inb(KBD_BUFF, &code);
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
	// Fim keyboard

	vg_exit();
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {
	vg_init(0x105);
	vg_line(xi, yi, xf, yf, color);
	// Inicio keyboard
	int ipc_status;
	unsigned long code;
	unsigned int i = 0, r;
	message msg;
	short irq_set = kbd_subscribe_int();

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (code != BC_ESC) {

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
					sys_inb(KBD_BUFF, &code);
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
	// Fim keyboard
	vg_exit();

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	vg_init(0x105);
	aloca_pixmap(xi,yi,xpm);
	timer_test_int(5);
	vg_exit();
}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {
	vg_init(0x105);

}
int test_controller(){
	display_vbe_info();
}
