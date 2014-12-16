#include "keyboard_mouse.h"
#include "bitmap.h"
#include <stdio.h>
#include <minix/syslib.h>
#include <minix/drivers.h>

static unsigned int hook_id;
static unsigned long code;
Mouse* mouse = NULL;

void print_packets(unsigned long* packet);

Mouse* getMouse() {
	if (mouse == NULL) {
		mouse = (Mouse *) malloc(sizeof(Mouse));
		mouse->x = 0;
		mouse->y = 0;
		mouse->speedMultiplier = 1;
		mouse->bytesRead = 1;
		mouse->leftButtonDown = 0;
		mouse->rightButtonDown = 0;
		mouse->draw = 0;
	} else {
		return mouse;
	}
}

void deleteMouse() {
	free(mouse);
}

void updateMouse() {
	mouse_int_handler();

	if (mouse->bytesRead == 1) {
		if (!check_first_byte()) {
			mouse->bytesRead = 1;
			return;
		}
	}

	mouse->packets[mouse->bytesRead - 1] = code;

	if (mouse->bytesRead == 3) {
		print_packets(mouse->packets);

		mouse->x += (char) (mouse->packets[1] * mouse->speedMultiplier);

		if (mouse->x < 0) {
			mouse->x = 0;
		} else if (mouse->x > getHRes()) {
			mouse->x = getHRes();
		}
		mouse->y -= (char) (mouse->packets[2] * mouse->speedMultiplier);
		if (mouse->y < 0) {
			mouse->y = 0;
		} else if (mouse->y > getVRes()) {
			mouse->y = getVRes();
		}
		mouse->draw = 0;
		mouse->bytesRead = 1;
		mouse->leftButtonDown = mouse->packets[0] & BIT(0);
		mouse->rightButtonDown = mouse->packets[0] & BIT(1);

		return;
	}
	mouse->bytesRead++;
}

void drawMouse() {
	Bitmap* mouse_bmp;
	if (mouse->draw == 1) {
		return;
	}

	if (mouse->leftButtonDown || mouse->rightButtonDown) {
		mouse_bmp = loadBitmap("/home/lcom/proj/img/mouse_down.bmp");
	} else {
		mouse_bmp = loadBitmap("/home/lcom/proj/img/mouse_up.bmp");
	}

	alocaMouse(mouse_bmp->Data, mouse_bmp->bitmapInfo.width,
			mouse_bmp->bitmapInfo.height);

	updateBufferTriple();
	mouse->draw = 1;

}

int kbd_int_handler() {
	unsigned long code_kbd;
	if (sys_inb(KBD_BUFF, &code_kbd) != OK) { //sends content from the out buffer to the variable
		printf("Int Handler Error at sys_inb.");
		return 1;
	}
	return code_kbd;
}

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

	if (sys_irqrmpolicy(&hook_id) != OK) {
		return 1;
	}

	return 0;

}

//updates the keyboard (checks for input of arrows, esc or enter)
/*void check_up_codes_menus() {
 kb_temp kb; //temporario

 int ipc_status, r = 0;
 message msg;

 if (driver_receive(ANY, &msg, &ipc_status) != 0) {
 return;
 }
 if (is_ipc_notify(ipc_status)) // receive notification of interrupt request. returns true if msg received is notification and false otherwise
 {
 switch (_ENDPOINT_P(msg.m_source)) // m_source contains the endpoint of the msg and _ENDPOINT extracts the process identifier from process's endpoint
 {
 case HARDWARE:
 if (msg.NOTIFY_ARG & kb.IRQ_SET_KB) {
 scan_asm();
 kb.scancode = code;
 }
 break;

 default:
 break;
 }
 }

 switch (kb.scancode) {
 case SC_ENTER:
 kb.key_pressed_menu = 0;
 break;
 case SC_UP_ARROW:
 kb.key_pressed_menu = 1;
 break;
 case SC_DOWN_ARROW:
 kb.key_pressed_menu = 2;
 break;
 case BC_ESC:
 kb.key_pressed_menu = 3;
 break;
 }
 }

 int check_codes_game() {
 kb_temp kb; //temporario

 int ipc_status, r = 0;
 message msg;

 if (driver_receive(ANY, &msg, &ipc_status) != 0) {
 return;
 }
 if (is_ipc_notify(ipc_status)) // receive notification of interrupt request. returns true if msg received is notification and false otherwise
 {
 switch (_ENDPOINT_P(msg.m_source)) // m_source contains the endpoint of the msg and _ENDPOINT extracts the process identifier from process's endpoint
 {
 case HARDWARE:
 if (msg.NOTIFY_ARG & kb.IRQ_SET_KB) {
 scan_asm();
 kb.scancode = code;
 }
 break;

 default:
 break;
 }
 }

 switch (kb.scancode) {
 case SC_ENTER:
 kb.key_pressed_game = 0;
 break;
 case SC_UP_ARROW:
 kb.key_pressed_game = 1;
 break;
 case SC_DOWN_ARROW:
 kb.key_pressed_game = 2;
 break;
 case SC_LEFT_ARROW:
 kb.key_pressed_game = 3;
 break;
 case SC_RIGHT_ARROW:
 kb.key_pressed_game = 4;
 break;
 case BC_ESC:
 kb.key_pressed_game = 5;
 break;
 }
 }*/

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

void print_packets(unsigned long* packet) {
	//B1=0x8	B2=0x12	B3=0x14	LB=0	MB=0	RB=0	XOV=0	YOV=0	X=18	Y=20
	printf(
			"B1=0x%X\tB2=0x%X\tB3=0x%X\tLB=%d\tMB=%d\tRB=%d\tXOV=%d\tYOV=%d\tX=%d\tY=%d\n\n",
			packet[0], packet[1], packet[2], (packet[0] & BIT(0)),
			(packet[0] & BIT(2)) >> 2, (packet[0] & BIT(1)) >> 1,
			(packet[0] & BIT(6)) >> 6, (packet[0] & BIT(7)) >> 7,
			(char) packet[1], (char) packet[2]);
}
/*
int mouseInsideRect(Button* botao, Mouse* rato) {

	if (rato->x > botao->x_ini && rato->x < botao->x_final
			&& rato->y > botao->y_ini && rato->y < botao->y_final)
		return 1;

	return 0;
}
*/
