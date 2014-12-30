#include <minix/drivers.h>
#include <minix/syslib.h>

#include "rtc.h"

static unsigned int hook_id;

//code fetched from the internet
unsigned char bcd_to_bin(unsigned char bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

unsigned long read_rtc_reg(unsigned long reg) {

	unsigned long data = 0;

	sys_outb(RTC_ADDR_REG, reg);
	sys_inb(RTC_DATA_REG, &data);
	return data;
}
/*
int rtc_subscribe_int() {
	unsigned int bit_sel = 4; // bit_sel is different from the timer, mouse and keyboard one.
	hook_id = bit_sel;
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK)
		return -1;

	if (sys_irqenable(&hook_id) != OK)
		return -1;

	unsigned char RegB = read_rtc_reg(11);
	RegB |= 1 << 6;
	//falta escrever

	unsigned char RegA = read_rtc_reg(10);
	RegA |= 1 << 0;
	RegA |= 1 << 1;
	RegA |= 1 << 2;
	RegA |= 1 << 3;
	//escrever no registo

	unsigned char RegC = read_rtc_reg(12);
	RegC |= 1 << 6;
	//escrever

	return BIT(bit_sel);
}

int rtc_unsubscribe_int() {

	if (sys_irqrmpolicy(&hook_id) != OK) {
		return 1;
	}

	return 0;
}
*/
void disable_ints() {
	__asm__("cli");
}

void enable_ints() {
	__asm__("sti");
}

void get_time(int *hour, int *min, int *sec) {
	unsigned char RegA, RegB;

	//disable interrupts
	//not sure
	disable_ints();

	//hour
	RegA = read_rtc_reg(10);

	while (RegA & 0x80 != 0) {
		RegA = read_rtc_reg(10);
	}

	*hour = read_rtc_reg(4);

	//minutes
	RegA = read_rtc_reg(10);

	while (RegA & 0x80 != 0) {
		RegA = read_rtc_reg(10);
	}

	*min = read_rtc_reg(2);

	//seconds
	while (RegA & 0x80 != 0) {
		RegA = read_rtc_reg(10);
	}

	*sec = read_rtc_reg(0);

	RegB = read_rtc_reg(11);

	if ((RegB & 0x02) == 0) {
		*hour = hour + 12;
	}

	if ((RegB & 0x04) == 0) {
		*hour = bcd_to_bin(*hour);
		*min = bcd_to_bin(*min);
		*sec = bcd_to_bin(*sec);
	}

	//enable interrupts
	//not sure
	enable_ints();
}

void get_date(int *year, int *month, int *day) {
	unsigned char RegB;

	//disable interrupts
	//not sure
	disable_ints();

	//year
	*year = read_rtc_reg(9);

	//month
	*month = read_rtc_reg(8);

	//day
	*day = read_rtc_reg(7);

	RegB = read_rtc_reg(11);

	if ((RegB & 0x04) == 0) {
		*year = bcd_to_bin(*year);
		*month = bcd_to_bin(*month);
		*day = bcd_to_bin(*day);
	}

	//enable interrupts
	//not sure
	enable_ints();

}

