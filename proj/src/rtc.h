#ifndef __RTC_H
#define __RTC_H

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

int write_rtc_reg(unsigned long internal_reg, unsigned long byte);

/*
 * @brief Subscreve as interrupcoes do RTC.
 *
 * @param exclusive se 1, subscreve em modo exclusivo, caso contrario nao
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int rtc_subscribe_int();

/*
 * @brief Desativa as interrupcoes do RTC.
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int rtc_unsubscribe_int();

/*
 * @brief Ativa as interrupcoes do RTC.
 *
 * @param ints ...
 */
int enable_ints(unsigned char ints);

/*
 * @brief Desativa as interrupcoes do RTC.
 *
 * @param ints ...
 */
int disable_ints(unsigned char ints);

/*
 * @brief Ativa a frequencia de onda do RTC.
 *
 * @return 0
 */
int rtc_enable_square_wave();

/*
 * @brief Ativa o alarme.
 *
 * @param delta_s intervalo de tempo que se pretende que o alarme seja ativado
 *
 * @return 0
 */
int set_alarm(unsigned long delta_s);

/**@}*/
#endif
