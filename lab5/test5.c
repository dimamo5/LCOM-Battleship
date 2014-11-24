#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>

#include "test5.h"
#include "keyboard.h"
#include "sprite.h"

void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);
	timer_test_int(delay);
	vg_exit();
	printf("Physical Memory : 0x%X", get_memphys());

}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	vg_init(0x105);

	//Verificacao dos limites do quadrado:
	// verifica se o quadrado se encontra completamente dentro da resoluçao usada
	if (x + size > get_hres() || y + size > get_vres()) {
		vg_exit();
		printf("Ultrapassou os limites!");
		return;
	}

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
	if (xi > get_hres() || xf > get_hres() || yi > get_vres()
			|| yf > get_vres()) {
		printf("Ultrapassou os limites!");
		vg_exit();
		return;
	}

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
	if (xpm == NULL) {
		printf("Nao existe tal XPM!");
		return -1;
	}
	vg_init(0x105);
	int width, height;
	char * map;
	map = (char *) read_xpm(xpm, &width, &height);
	if (xi + width > get_hres() || yi + height > get_vres()) {
		vg_exit();
		printf("Ultrapassou os limites!");
		return;
	}
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
	if (xpm == NULL) {
		printf("Nao existe tal XPM!");
		return -1;
	}
	vg_init(0x105);

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

	if (hor) {
		if ((xi + delta + st->width > get_hres() || xi + delta < 0)) {
			vg_exit();
			printf("Passou da resolucao horizontal do ecra!");
			return -1;
		}
	} else if ((yi + delta + st->height > get_vres() || yi + delta < 0)) {
		vg_exit();
		printf("Passou da resolucao vertical do ecra!");
		return -1;
	}

	aloca_pixmap((unsigned short) st->x, (unsigned short) st->y, st->map,
			st->width, st->height);

	double x_ant = xi, y_ant = yi;
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
					vg_fill((unsigned short) x_ant, (unsigned short) y_ant,
							st->width, st->height, 0x00);
					st->x += st->xspeed;
					st->y += st->yspeed;
					aloca_pixmap((unsigned short) st->x, (unsigned short) st->y,
							st->map, st->width, st->height);
					x_ant = st->x;
					y_ant = st->y;
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
	destroy_sprite(st);

	vg_exit();

	if (kbd_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}

	if (timer_unsubscribe_int() != 0) {
		printf("Unsubscribe failed");
	}

	return 0;
}

int test_controller() {
	display_vbe_info();
	return 0;
}
