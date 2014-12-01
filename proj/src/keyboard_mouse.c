#include "keyboard_mouse.h"

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

//updates the keyboard (checks for input of arrows, esc or enter)
void check_up_codes_menus() {
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
}
