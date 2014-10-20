#ifndef __KBD_H
#define __KBD_H

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>


#define BIT(n) (0x01<<(n))
#define KBC_OUTBUF 0x60 //Output buffer from KBC
#define KBC_IRQ 1 //KBC IRQ line
#define BC_ESC 0x81 //Breakcode ESC



int kbd_int_handler();
int kbd_unsubscribe_int();
int kbd_subscribe_int();
int kbd_scan_c();

#endif
