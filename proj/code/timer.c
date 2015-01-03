#include "timer.h"
#include "i8254.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>

static unsigned int hook_id;

int timer_subscribe_int() {
	unsigned int bit_sel = 1;
	hook_id = bit_sel;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK)
		return -1;
	if (sys_irqenable(&hook_id) != OK)
		return -1;
	return BIT(bit_sel);
}

int timer_unsubscribe_int() {

	if (sys_irqdisable(&hook_id) != OK) {
		return 1;
	}
	if (sys_irqrmpolicy(&hook_id) != OK) {
		return 1;
	}

	return 0;
}

int timer_set_square(unsigned long timer, unsigned long freq) {
	if (freq < 1 || freq > TIMER_FREQ) {
		printf("ERROR FREQ");
		return 1;
	}
	unsigned long resultado_freq, control_word;
	resultado_freq = TIMER_FREQ / freq;
	control_word = TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN;
	unsigned short timer_end = TIMER_0 + timer;
	unsigned char freq_lsb = resultado_freq;
	unsigned char freq_msb = resultado_freq >> 8;
	switch (timer) {
	case 0:
		control_word |= TIMER_SEL0;
		break;
	case 1:
		control_word |= TIMER_SEL1;
		break;
	case 2:
		control_word |= TIMER_SEL2;
		break;
	default:
		break;
	}
	if (sys_outb(TIMER_CTRL, control_word) != OK)
		printf("Erro Control word");
	if (sys_outb(timer_end, freq_lsb) != OK)
		printf("Erro timer LSB");
	if (sys_outb(timer_end, freq_msb) != OK)
		printf("Erro timer MSB");

	return 0;
}
