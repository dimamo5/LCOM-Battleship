#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "button.h"
#include "game.h"
#include "bitmap.h"
/** @defgroup graphics Graphics
 * @{
 *
 * Modulo com funcoes para escrever no ecra
 */
typedef enum {
	SMALL, BIG
} Board_size;

#define TRANS_COLOR rgb(255,0,255)
#define RED rgb(255,0,0)
#define GREEN rgb(0,255,0)
#define BLUE rgb(0,0,255)
#define WHITE rgb(255,255,255)

#define PART_SIZE 40
#define X_BOARD_HUM 50
#define Y_BOARD_HUM 250
#define X_BOARD_COM 500
#define Y_BOARD_COM 250

/**
 * @brief Initializes the video module in graphics mode
 *
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen,
 *  and the number of colors
 *
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

/**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 *
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
 * Resolucao Horizontal do Modo
 * @return resolucao horizontal
 */
unsigned getHRes();
/**
 * Resolucao Horizontal do Modo
 * @return resolucao vertical
 */
unsigned getVRes();
/**
 * Coloca no buffer um pixel na posicao x y na cor color
 * @param x posicao x do pixel
 * @param y posicao y do pixel
 * @param color cor do pixel
 */
void vg_set_pixel(unsigned short x, unsigned short y, unsigned short color);
/**
 * Desenha um rectangulo a uma determinada cor à volta de um botao
 * @param b botao que contem a cor e as coordenadas
 */
void drawRectangle(Button* b);

/**
 * Desenha uma linha com uma direcao cor e comprimento
 * @param x inicio da linha x
 * @param y inicio da linha y
 * @param length tamanho da linha
 * @param dir orientacao da linha
 * @param color cor da linha
 */
void drawLine(unsigned short x, unsigned short y, unsigned short length, char dir, unsigned short color);

/**
 * Coloca um pixmap no buffer
 * @param xi inicio do pixmap x
 * @param yi inicio do pixmap y
 * @param map array de cores
 * @param width comprimento da imagem
 * @param height altura da imagem
 */
void aloca_pixmap(unsigned short xi, unsigned short yi, unsigned short *map, int width, int height);

/**
 * Desenha o tabuleiro
 * @param x inicio do tabuleiro  x
 * @param y inicio do tabuleiro y
 *@param size tamanho do tabuleiro
 */
void draw_board(unsigned short x, unsigned short y, Board_size size);

/**
 * Desenha o cursor do rato (triple buffer)
 * @param map array com as cores
 * @param width comprimento do cursor
 * @param height comprimento do cursor
 */
void alocaMouse(unsigned short *map, int width, int height);

/**
 * Mete o segundo buffer na memoria
 */
void updateBufferSec();

/**
 * Limpa o segundo buffer
 */
void cleanBufferSec();

/**
 * Mete o terceiro buffer na memoria
 */
void updateBufferTriple();

/**
 * Desenha os dois tabuleiros de jogo(HUM/COM)
 * @param tab_hum tabuleiro do humano
 * @param tab_com tabuleiro do computador
 * @param b Bitmap com as imagens das naves
 * @param turn turno de que é a jogar
 */
void drawTabuleirosGame(tabuleiro tab_hum, tabuleiro tab_com, Bitmap* b, int turn);

/**
 * Transforma um RGB normal em RGB 565
 * @param r red color
 * @param g green color
 * @param b blue color
 * @return retorna a cor
 */
int rgb(unsigned char r, unsigned char g, unsigned char b);

/**
 * Desenha uma parte de uma nave no tabuleiro
 * @param x posicao onde desenha x
 * @param y posicao onde desenha y
 * @param p parte da nave
 * @param bmp Bitmap com as naves
 * @param ori Orientacao da peca
 */
void drawQuadricula(unsigned x, unsigned y, ship_part p, Bitmap* bmp, char ori);

/**
 * Desenha o tabueleiro e a nave que esta a ser colocada no tabuleiro
 * @param x posicao a desenhar o board x
 * @param y posicao a desenhar o board y
 * @param tab tabuleiro onde vai ser colocada a ship s
 * @param s ship a ser colocado no tabuleiro
 * @param b Bitmap com as imagens da nave
 */
void drawSetTabuleiro(unsigned x, unsigned y, tabuleiro tab, ship* s, Bitmap* b);

/**
 * Desenha a lista das ships que ainda faltam adicionar
 * @param x posicao x onde desenhar
 * @param y posicao y onde desenhar
 * @param count nr de naves ja colocadas
 * @param bmp Bitmap com a lista das naves
 */
void drawListShipSet(unsigned x, unsigned y, unsigned count, Bitmap* bmp);

/**
 * Desenha o contador do tempo
 * @param time tempo a desenhar no ecra
 * @param bmp Bitmap com os numeros (alarm stype)
 */
void drawClock(unsigned int time, Bitmap* bmp);

/**
 * Desenha uma parte do bitmap
 * @param x posicao x onde desenhar essa parte do bitmap
 * @param y posicao y onde desenhar essa parte do bitmap
 * @param x_rel posicao relativa(nr de quadrados ate a esa parte)
 * @param y_rel posicao relativa(nr de quadrados ate a esa parte)
 * @param x_size tamanho x de cada quadrado
 * @param y_size tamanho y de cada quadrado
 * @param bmp Bitmap a ser desenhado
 */
void drawPortionBitmap(unsigned int x, unsigned int y, unsigned short x_rel, unsigned short y_rel, unsigned short x_size,
		unsigned short y_size, Bitmap* bmp);

/**
 * Desenha Lista de Navios já destruidos
 * @param battle estrutura principal do jogo
 */
void drawDestroyedList(Battleship* battle);

/**
 * Desenha Imagem do Winner
 * @param winner ganhou 1- humano 2- computador
 * @param battle estrutura principal do jogo
 */
void drawWinner(int winner, Battleship* battle);

/**
 * Desenha o nr de turnos falhados
 * @param x posicao x onde desenha a imagem
 * @param y posicao y onde desenha a imagem
 * @param turns_missed nr de turnos passados
 * @param bmp_cross Bitmap Bitmap com uma cruz vermelho
 * @param bmp_turns_missed Bitmap com o texto de turns missed
 * */
void drawTurnsMissed(unsigned int x, unsigned int y, int turns_missed, Bitmap* bmp_cross, Bitmap* bmp_turns_missed);

/** @} end of graphics */

#endif
