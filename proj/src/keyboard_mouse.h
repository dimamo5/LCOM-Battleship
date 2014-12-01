#ifndef __KEYBOARD_MOUSE_H
#define __KEYBOARD_MOUSE_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>

//this struct is from difusal.blogspot.pt
typedef struct {
    int x, y;
    int xSign, ySign;
    int deltaX, deltaY;
    double speedMultiplier;

    int byteBeingRead;
    unsigned long packet[3];

    int leftButtonDown;
    int middleButtonDown;
    int rightButtonDown;

    int leftButtonReleased;
    int rightButtonReleased;
    int middleButtonReleased;

    int size;
    int color1, color2;

    int hasBeenUpdated;
    int draw;
} Mouse;

typedef struct{
	int IRQ_SET_KB;
	int done, key_pressed_menu,key_pressed_game;
	unsigned long scancode;
}kb_temp;

#define SC_ENTER 0x001C
#define SC_LEFT_ARROW 0xE04B
#define SC_UP_ARROW 0xE048
#define SC_DOWN_ARROW 0xE050
#define SC_RIGHT_ARROW 0xE04D
#define BC_ESC 0x0001
#define KBC_IRQ 1 //KBC IRQ line
#define KBD_BUFF 0x60 //Output buffer from KBC
#define BIT(n) (0x01<<(n))

int kbd_int_handler();
int kbd_unsubscribe_int();

/*verifica se a tecla enter, up_arrow ou down_arrow é premida
 * retorna 0 se a tecla for enter, 1 para up_arrow e 2 para down_arrow
 */
int check_codes_menus();
int check_codes_game();



#endif
