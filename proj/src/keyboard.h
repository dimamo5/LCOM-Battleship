#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>

#define SC_ENTER 0x1C
#define SC_UP_ARROW 0x4B
#define SC_DOWN_ARROW 0x48
#define KBC_IRQ 1 //KBC IRQ line
#define KBD_BUFF 0x60 //Output buffer from KBC

int kbd_int_handler();
int kbd_unsubscribe_int();

/*verifica se a tecla enter, up_arrow ou down_arrow é premida
 * retorna 0 se a tecla for enter, 1 para up_arrow e 2 para down_arrow
 */
int check_codes();

#endif
