#include <minix/drivers.h>
#include <minix/syslib.h>

#include "rtc.h"

static unsigned int hook_id;

//code fetched from the internet
unsigned char bcd_to_bin(uchar bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

int rtc_subscribe_int() {
	unsigned int bit_sel = 4; // bit_sel is different from the timer, mouse and keyboard one.
	hook_id = bit_sel;
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK)
		return -1;
	if (sys_irqenable(&hook_id) != OK)
		return -1;
	return BIT(bit_sel);
}

int rtc_unsubscribe_int() {

	if (sys_irqdisable(&hook_id) != OK) {
		return 1;
	}
	if (sys_irqrmpolicy(&hook_id) != OK) {
		return 1;
	}

	return 0;
}

void disable_ints() {
	unsigned char RegB;

	sys_outb(RTC_ADDR_REG, 11);
	sys_inb(RTC_DATA_REG, RegB);

	RegB &= ~(1 << 6);
	RegB &= ~(1 << 5);
	RegB &= ~(1 << 4);

}

void enable_ints() {
	unsigned char RegB;

	sys_outb(RTC_ADDR_REG, 11);
	sys_inb(RTC_DATA_REG, RegB);

	RegB |= 1 << 6;
	RegB |= 1 << 5;
	RegB |= 1 << 4;
}

void get_time(int *hour, int *min, int *sec) {
	unsigned char RegA, RegB;

	//disable interrupts
	//not sure
	disable_ints();

	//hour
	sys_outb(RTC_ADDR_REG, 10);
	sys_inb(RTC_DATA_REG, RegA);

	while (RegA & 0x80 != 0) {
		sys_outb(RTC_ADDR_REG, 10);
		sys_inb(RTC_DATA_REG, RegA);
	}

	sys_outb(RTC_ADDR_REG, 4);
	sys_inb(RTC_DATA_REG, *hour);

	//minutes
	sys_outb(RTC_ADDR_REG, 10);
	sys_inb(RTC_DATA_REG, RegA);

	while (RegA & 0x80 != 0) {
		sys_outb(RTC_ADDR_REG, 10);
		sys_inb(RTC_DATA_REG, RegA);
	}

	sys_outb(RTC_ADDR_REG, 2);
	sys_inb(RTC_DATA_REG, *min);

	//seconds
	while (RegA & 0x80 != 0) {
		sys_outb(RTC_ADDR_REG, 10);
		sys_inb(RTC_DATA_REG, RegA);
	}

	sys_outb(RTC_ADDR_REG, 0);
	sys_inb(RTC_DATA_REG, *sec);

	sys_outb(RTC_ADDR_REG, 11);
	sys_inb(RTC_DATA_REG, RegB);

	if ((RegB & 0x02) == 0) {
		*hour = hour + 12;
	}

	if ((RegB & 0x04) == 0) {
		*hour = bcd2bin(*hour);
		*min = bcd2bin(*min);
		*sec = bcd2bin(*sec);
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
	sys_outb(RTC_ADDR_REG, 9);
	sys_inb(RTC_DATA_REG, *year);

	//month
	sys_outb(RTC_ADDR_REG, 8);
	sys_inb(RTC_DATA_REG, *month);

	//day
	sys_outb(RTC_ADDR_REG, 7);
	sys_inb(RTC_DATA_REG, *day);

	sys_outb(RTC_ADDR_REG, 11);
	sys_inb(RTC_DATA_REG, RegB);

	if ((RegB & 0x04) == 0) {
		*year = bcd2bin(*year);
		*month = bcd2bin(*month);
		*day = bcd2bin(*day);
	}

	//enable interrupts
	//not sure
	enable_ints();

}
