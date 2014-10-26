#ifndef __KBD_H
#define __KBD_H

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>

#define DELAY_US    20000
#define BIT(n) (0x01<<(n))
#define KBD_BUFF 0x60 //Output buffer from KBC
#define KBC_STATUS 0x64 //Input buffer from KBC
#define KBC_IRQ 1 //KBC IRQ line
#define BC_ESC 0x81 //Breakcode ESC
#define RESEND 0xFE //
#define ERROR 0xFC //
#define ACK 0xFA //
#define LED_OFF 0x00 //Mascara a 0
#define LED_CAPSLOCK BIT(2) //argumento para ligar LED
#define LED_NUMPAD BIT(1)
#define LED_SCRLOCK BIT(0)
#define SWITCH_LED 0xED //comando para ligar LED
#define BUFFER_FULL BIT(1) //Buffer quando estiver full
#define SCAN_2BYTE 0x00E0
#define BREAKCODE BIT(7)


int kbd_int_handler();
int kbd_unsubscribe_int();
int kbd_subscribe_int();
int kbd_scan_c();
int kbd_analisa(int resp);
int kbd_send_command(unsigned char com);
unsigned char get_command(int led1,int led2,int led3);
int kbd_led(int n,unsigned short led[]);

#endif
