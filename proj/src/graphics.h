#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "button.h"
#include "game.h"
#include "bitmap.h"

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
#define Y_BOARD_HUM 50
#define X_BOARD_COM 500
#define Y_BOARD_COM 250

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

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

void vg_set_pixel(unsigned short x, unsigned short y, unsigned short color);

void updateBufferSec();

int rgb(unsigned char r, unsigned char g, unsigned char b);

void drawQuadricula(unsigned x, unsigned y, ship_part p, Bitmap* bmp, char ori);

void drawSetTabuleiro(unsigned x, unsigned y, tabuleiro tab, ship* s);

void drawListShipSet(unsigned x, unsigned y, unsigned count, Bitmap* bmp);

void drawPortionBitmap(unsigned int x, unsigned int y, unsigned short x_rel, unsigned short y_rel, unsigned short x_size,
		unsigned short y_size, Bitmap* bmp);

/** @} end of video_gr */

#endif /* __VIDEO_GR_H */
