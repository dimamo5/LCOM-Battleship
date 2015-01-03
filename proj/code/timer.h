#ifndef __TIMER_H
#define __TIMER_H

/**
 * @defgroup timer Timer
 * @{
 * @brief Modulo com o timer
 */
#define FPS 30

/**
 * Atualiza o menu principal, atualizando as posicoes do rato e outras variaveis,
 * desenhando consoante os seus valores, ou ate alterando o estado
 * @return Retorna o bit de selecao
 */
int timer_subscribe_int();
/**
 * Atualiza o menu principal, atualizando as posicoes do rato e outras variaveis,
 * @return Retorna 0 se passou e 1 se falhou a fazer disable ou remove policy
 */
int timer_unsubscribe_int();
/**
 *
 * @param timer Timer que se pretende alterar. No projeto usamos o timer 0, por exemplo.
 * @param freq Frequencia para a qual se vai por o timer.
 * @return Retorna 1 se for introduzida frequencia invalida e 0 de outra maneira
 */
int timer_set_square(unsigned long timer, unsigned long freq);

/**@}fim do modulo Timer*/
#endif
