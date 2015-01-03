#ifndef _BUTTON_H_
#define _BUTTON_H_
/**
 * @defgroup button Button
 * @{
 * Classe Botao
 */
/**
 * @name botao Botao
 * @{
 * Estrutura Botao
 */
typedef struct {
	int x_ini, y_ini, x_final, y_final; /**<@brief Posições dos cantos do botao*/
	int width, height; /**<@brief Altura e Largura*/
	int mouse_hover;/**<@brief Flag mouse esta em cima do botao*/
	short color_border;/**<@brief Cor da borda*/
	unsigned short available; /**<@brief Botao disponivel para carregar*/
} Button;
/**@}fim da estrutura mouse */

/**
 *  Construtor "objeto" do botao com as coordenadas especificadas e cor
 *  @param xi x inicial
 *  @param xf x final
 *  @param yi y inicial
 *  @param yf y inicial
 *  @param color cor da borda quando mouse esta em cima do botao
 *  @return Poiter para botao
 */
Button* newButton(int xi,int yi,int xf,int yf,short color);

/**
 * Actualiza a flag do mouse hover
 * @param b botao a verificar
 */
void updateButton(Button* b);

/**
 * Apaga e liberta memoria alocadoa pelo botao
 * @param botao botao a ser actualizado
 */
void deleteButton(Button* botao);
/**@} fim da classe botao */
#endif
