#ifndef __KEYBOARD_MOUSE_H
#define __KEYBOARD_MOUSE_H

#include "button.h"
#include "bitmap.h"

//this struct is from difusal.blogspot.pt

typedef struct {
	int x, y;
	double speedMultiplier;

	int bytesRead;
	long packets[3];

	int leftButtonDown;
	int rightButtonDown;
	Bitmap* mouse_up;
	Bitmap* mouse_down;
	int hasBeenUpdated;
	int draw;
} Mouse;

#define ENABLE_PACKETS 	0xF4
#define ENABLE_MOUSE	0xA8
#define KBC_IRQ 1 //KBC IRQ line
#define KBD_BUFF 0x60 //Output buffer from KBC
#define KBC_STATUS 		0x64 //Input buffer from KBC
#define BIT(n) (0x01<<(n))
#define ACK 			0xFA
#define NACK 			0xFE
#define ERROR 			0xFC
#define MOUSE_IRQ 		12
#define MOUSE_WC 		0xD4
#define DISABLE_PACKETS 0xF5
#define IN_BUFFER_FULL 	BIT(1)
#define DELAY_US    	20000

typedef enum {
	KEY_NONE = 0x0000,
	KEY_ESC = 0x0001,
	KEY_ESC_BRK = 0x0081,
	KEY_1 = 0x0002,
	KEY_2 = 0x0003,
	KEY_3 = 0x0004,
	KEY_4 = 0x0005,
	KEY_5 = 0x0006,
	KEY_6 = 0x0007,
	KEY_7 = 0x0008,
	KEY_8 = 0x0009,
	KEY_9 = 0x000A,
	KEY_0 = 0x000B,
	KEY_BKSP = 0x000E,
	KEY_TAB = 0x000F,
	KEY_Q = 0x0010,
	KEY_W = 0x0011,
	KEY_E = 0x0012,
	KEY_R = 0x0013,
	KEY_R_BRK = 0x0093,
	KEY_T = 0x0014,
	KEY_Y = 0x0015,
	KEY_U = 0x0016,
	KEY_I = 0x0017,
	KEY_O = 0x0018,
	KEY_P = 0x0019,
	KEY_P_BRK = 0X0099,
	KEY_ENTER = 0x001C,
	KEY_ENTER_BRK = 0x009C,
	KEY_L_CTRL = 0x001D,
	KEY_A = 0x001E,
	KEY_S = 0x001F,
	KEY_D = 0x0020,
	KEY_F = 0x0021,
	KEY_G = 0x0022,
	KEY_H = 0x0023,
	KEY_J = 0x0024,
	KEY_K = 0x0025,
	KEY_L = 0x0026,
	KEY_BACKSLASH = 0x0029,
	KEY_L_SHIFT = 0x002A,
	KEY_Z = 0x002C,
	KEY_X = 0x002D,
	KEY_C = 0x002E,
	KEY_V = 0x002F,
	KEY_B = 0x0030,
	KEY_N = 0x0031,
	KEY_M = 0x0032,
	KEY_R_SHIFT = 0x0036,
	KEY_SPACE = 0x0039,
	KEY_R_CTRL = 0xE01D,
	KEY_NUM_SLASH = 0xE035,
	KEY_ALT_GR = 0xE038,
	KEY_ARR_UP = 0xE048,
	KEY_ARR_LEFT = 0xE04B,
	KEY_ARR_RIGHT = 0xE04D,
	KEY_ARR_DOWN = 0xE050,
	KEY_ARR_UP_BRK = 0xE0C8,
	KEY_ARR_LEFT_BRK = 0xE0CB,
	KEY_ARR_RIGHT_BRK = 0xE0CD,
	KEY_ARR_DOWN_BRK = 0xE0D0,
	KEY_PGDN = 0xE051,
	KEY_DEL = 0xE053,
} KEY;

int kbd_int_handler();
int kbd_subscribe_int();
int kbd_unsubscribe_int();

int mouse_int_handler();
int mouse_subscribe_int();
int mouse_unsubscribe_int();

/*verifica se a tecla enter, up_arrow ou down_arrow é premida
 * retorna 0 se a tecla for enter, 1 para up_arrow e 2 para down_arrow
 */
//int check_codes_menus();
//int check_codes_game();
Mouse* getMouse();
void resetMouse();
void deleteMouse();
void updateMouse();
int mouse_int_handler();
int enable_packets();
int disable_packets();
int check_first_byte();
void drawMouse();
int mouseInsideButton(Button* botao, Mouse* rato);

#endif
