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
	int width, height;
	char * map;
	map = (char *) read_xpm(xpm, &width, &height);
	aloca_pixmap(xi, yi, map, width, height);
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

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {
	vg_init(0x105);
	printf("passou");
	if (xi > get_hres() || yi > get_vres()) {
		vg_exit();
		printf("Passou da resolucao do ecra!");
		return -1;
	}

	Sprite* st = create_sprite(xpm);
	st->x = xi;
	st->y = yi;
	if (hor) {
		st->xspeed = (double) delta / (time * 60); //pixel per tick
		printf("%d", st->xspeed);
		st->yspeed = 0;
	} else {
		st->yspeed = (double) delta / (time * 60);
		st->xspeed = 0;
	}

	aloca_pixmap((unsigned short) st->x, (unsigned short) st->y, st->map,st->width,st->height);
/*
	int ipc_status, flag_stop = 1;
	unsigned int r, counter_milisec = 0, counter_timer = 0;
	unsigned long code;
	message msg;
	short irq_set_kbd = kbd_subscribe_int();
	short irq_set_time = timer_subscribe_int();

	timer_test_square(60);

	if (irq_set_kbd < 0 || irq_set_time < 0) {
		vg_exit();
		printf("Subscribe failed");
		return 1;
	}
	while (flag_stop) {
		printf("entrou no ciclo");
		// ANY -> receives msg from any process
		 //  2nd and 3rd arguments are the addresses of variables of type message and int
		 //
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) // receive notification of interrupt request. returns true if msg received is notification and false otherwise
				{
			switch (_ENDPOINT_P(msg.m_source)) // m_source contains the endpoint of the msg and _ENDPOINT extracts the process identifier from process's endpoint
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_kbd) {
					sys_inb(KBD_BUFF, &code);

					if (code == BC_ESC) {
						flag_stop = 0;
						printf("Premiu ESC");
						break;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_time) {
					counter_milisec++;
					st->x += st->xspeed;
					st->y += st->yspeed;
					aloca_pixmap((unsigned short) st->x, (unsigned short) st->y,
							st->map);
					if (counter_milisec % 60 == 0) {
						counter_timer++;
						printf("%d seconds", counter_timer);
					}
					if (time == counter_timer) {
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

	vg_exit();

	if (kbd_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}

	if (timer_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}
*/
	return 0;
}

int test_controller() {
	display_vbe_info();
}
