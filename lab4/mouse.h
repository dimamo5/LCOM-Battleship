#ifndef _MOUSE_H
#define _MOUSE_H

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>
#include "timer.h"
#include "i8254.h"




#define MOUSE_IRQ 12
#define MOUSE_WC 0xD4
#define ENABLE_PACKETS 0xF4
#define DELAY_US    20000
#define BIT(n) (0x01<<(n))
#define KBD_BUFF 0x60 //Output buffer from KBC
#define KBC_STATUS 0x64 //Input buffer from KBC
#define IN_BUFFER_FULL BIT(1)
#define OUT_BUFFER_FULL BIT(0)
#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC






int kbd_in_status();
int kbd_out_status();
int mouse_int_handler();
int mouse_unsubscribe_int();
int mouse_subscribe_int();
int enable_packets();
int mouse_packet(unsigned short number_packets);
void print_packets(unsigned char packet[]);
int check_first_byte();



#endif
