#ifndef __RTC_H
#define __RTC_H

#include "keyboard_mouse.h"

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_IRQ 8

/*registos a ler 0 2 4 7 8 9
 */

/** @defgroup rtc rtc
 /* @{
 *
 * Classe que gere o real-time clock
 */

unsigned long read_rtc_reg(unsigned long reg);

/*
 * @brief Ativa as interrupcoes do RTC.
 *
 * @param ints ...
 */
void enable_ints();

/*
 * @brief Desativa as interrupcoes do RTC.
 *
 * @param ints ...
 */
void disable_ints();

/**@}*/

void get_time(int *year, int *month, int *day, int *hour, int *min);
unsigned char bcd_to_bin(unsigned char bcd);

#endif
