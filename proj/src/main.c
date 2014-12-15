#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "graphics.h"
#include "bitmap.h"
#include "keyboard_mouse.h"
#include "battleship.h"

int main(int argc, char **argv) {
	sef_startup();
	//Handler em ASM
	sys_enable_iop(SELF);
	srand(time(NULL));
	vg_init(0x117);

	//	Battleship* battleship = (Battleship *) startBattleship();
	//
	//	while (!battleship->done) {
	//		updateBattleship(battleship);
	//
	////		if (!battleship->done && battleship->draw) {
	////			drawBattleship(battleship);
	////		}
	//	}
	//	stopBattleship(battleship);

	Bitmap* menu_background;
	menu_background = loadBitmap("/home/lcom/proj/img/mainmenu.bmp");
//	Bitmap* death_star;
//	death_star = loadBitmap("/home/lcom/proj/img/star.bmp");
	drawBitmap(menu_background,0,0);
//	drawBitmap(death_star, 100, 50);
//	drawRectangle(100, 50, death_star->bitmapInfo.width,
//			death_star->bitmapInfo.height, 2, 0x019F);
//	draw_board(200, 200, SMALL);
	int irq_kbd = kbd_subscribe_int();
	getMouse();

	int counter = 0;
	unsigned short kb_code;
	int ipc_status, r;
	message msg;
	short irq_set = mouse_subscribe_int();
	short irq_set_timer = timer_subscribe_int();
	timer_set_square(0, 30);
	enable_packets();

	if (irq_set < 0) {
		printf("Subscribe failed");
		return 1;
	}
	while (counter < 30 * 5) {

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
					updateMouse();
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					counter++;

					drawMouse();
				}
				if (msg.NOTIFY_ARG & irq_kbd) {
					kb_code = kbd_int_handler();
					//printf("0x%X", kb_code);
				}
				break;

			default:
				break;
			}
		} else {
			printf("Any interrupt received\n"); // Any interrupt received, so anything to do
		}
		if (kb_code == KEY_ESC) {
			break;
		}
	}
	deleteMouse();
	disable_packets();

	if (mouse_unsubscribe_int() != OK) {
		printf("Unsubscribe failed");
	}
	kbd_unsubscribe_int();
	if (timer_unsubscribe_int() != OK) {
		printf("Unsubscribe failed");
	}

	vg_exit();
	return 0;
}
