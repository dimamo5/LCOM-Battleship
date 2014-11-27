#include <minix/drivers.h>
#include <minix/syslib.h>

#include "rtc.h"




unsigned long read_rtc_reg(unsigned long reg) {

	unsigned long data = 0;

	sys_outb(RTC_ADDR_REG, reg);
	sys_inb(RTC_DATA_REG, &data);
	return data;
}
