#include <minix/syslib.h>
#include <minix/drivers.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <machine/int86.h>
#include <minix/sysutil.h>

#include "vbe.h"
#include "graphics.h"
#include "keyboard_mouse.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */

/* Private global variables */

static short *video_mem; /* Process address to which VRAM is mapped */

/*buffer secondary and mouse buffer*/

static short *second_buffer;
static short *triple_buffer;

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bytes_per_pixel; /* Number of VRAM bits per pixel */

unsigned getHRes() {
	return h_res;
}

unsigned getVRes() {
	return v_res;
}

void * vg_init(unsigned short mode) {
	struct reg86u reg;
	reg.u.w.ax = VBE_SET_MODE; // VBE call, function 02 -- set VBE mode
	reg.u.w.bx = 1 << LINEAR_MODEL_BIT | mode; // set bit 14: linear framebuffer
	reg.u.b.intno = 0x10;
	if (sys_int86(&reg) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}
	if (reg.u.b.ah == FUNCTION_FAIL) {
		printf("Function call failed \n");
		return NULL;
	} else if (reg.u.b.ah == FUNCTION_NOT_SUPPORTED) {
		printf("Function is not supported in current HW configuration \n");
		return NULL;
	} else if (reg.u.b.ah == FUNCTION_INVALID) {
		printf("Function is invalid in current video mode \n");
		return NULL;
	}

	/* Obter info do modo */
	vbe_mode_info_t info_mode;
	vbe_get_mode_info(mode, &info_mode);

	h_res = info_mode.XResolution;
	v_res = info_mode.YResolution;
	bytes_per_pixel = info_mode.BitsPerPixel / 8;

	unsigned long vram = h_res * v_res * bytes_per_pixel;
	/* Map memory */

	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = info_mode.PhysBasePtr; //Endereço base da memoria
	mr.mr_limit = mr.mr_base + vram; //Endereço final da memoria - limite, fim do range a partir da posicao inicial

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_gr: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram);

	if (video_mem == MAP_FAILED)
		panic("video_gr couldn't map video memory");

	/* Initialize temporary buffer */
	second_buffer = (short *) malloc(h_res * v_res * bytes_per_pixel);
	triple_buffer = (short *) malloc(h_res * v_res * bytes_per_pixel);

	/* Save text mode resolution */

	return (void *) video_mem;
}

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
	free(second_buffer);
	free(triple_buffer);
}

void vg_set_pixel(unsigned short x, unsigned short y, unsigned short color) {
	unsigned short * mem_temp = second_buffer;

	mem_temp = second_buffer + h_res * y + x;
	*mem_temp = color;
}

void vg_fill(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned short color) {
	unsigned short x_original = x;
	unsigned short i;

	for (i = 0; i < width * height; i++) {
		vg_set_pixel(x, y, color);
		x++;
		if (x == width + x_original) {
			x = x_original;
			y++;
		}
	}
}

void vg_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {

	int err, err_temp, dx = abs(xf - xi), dy = abs(yf - yi), sx, sy;

	if (xi < xf) {
		sx = 1;
	} else {
		sx = -1;
	}

	if (yi < yf) {
		sy = 1;
	} else {
		sy = -1;
	}

	if (dx > dy) {
		err = dx / 2;
	} else {
		err = -dy / 2;
	}

	while (1) {
		vg_set_pixel(xi, yi, color);
		if (xi == xf && yi == yf)
			break;
		err_temp = err;
		if (err_temp > -dx) {
			err -= dy;
			xi += sx;
		}
		if (err_temp < dy) {
			err += dx;
			yi += sy;
		}
	}

}

void drawRectangle(Button* b) {
	if (!b->available || !b->mouse_hover) {
		return;
	}
	short border = 0;
	short width = b->width;
	short height = b->height;
	short x_temp, y_temp;

	for (border; border < 2; border++) {
		x_temp = b->x_ini - border;
		y_temp = b->y_ini - border;

		for (x_temp; x_temp <= width + b->x_ini; x_temp++) {
			vg_set_pixel(x_temp, y_temp, b->color_border);
			vg_set_pixel(x_temp, y_temp + height, b->color_border);
		}
		x_temp = b->x_ini - border;
		y_temp = b->y_ini - border;
		for (y_temp; y_temp <= height + b->y_ini; y_temp++) {
			vg_set_pixel(x_temp, y_temp, b->color_border);
			vg_set_pixel(x_temp + width, y_temp, b->color_border);
		}

		width += 2;
		height += 2;
	}
}
void aloca_pixmap(unsigned short xi, unsigned short yi, unsigned short *map, int width, int height) {
	unsigned int i = 0;
	unsigned short x_original = xi;
	for (i = 0; i < width * height; i++) {
		if (map[i] != TRANS_COLOR) {
			vg_set_pixel(xi, yi + height, map[i]);
		}
		xi++;
		if (xi == width + x_original) {
			xi = x_original;
			yi--;
		}
	}

}

void drawLine(unsigned short x, unsigned short y, unsigned short length, char dir, unsigned short color) {
	unsigned short i;
	if (dir == 'h') {
		for (i = 0; i < length; i++) {
			vg_set_pixel(x + i, y, color);
		}
	} else if (dir == 'v') {
		for (i = 0; i < length; i++) {
			vg_set_pixel(x, y + i, color);
		}
	}
}
void draw_board(unsigned short x, unsigned short y, Board_size size) {
	unsigned short x_temp = x - 2;
	unsigned short y_temp = y - 2;
	unsigned int i = 0;

	for (; i < 11; i++) {
		if (i == 0 || i == 10) {
			drawLine(x_temp, y_temp, 412, 'h', WHITE);
			drawLine(x_temp, y_temp + 1, 412, 'h', WHITE);
		} else {
			drawLine(x_temp, y_temp, 412, 'h', WHITE);

		}
		y_temp += 41;

	}

	i = 0;
	x_temp = x - 2;
	y_temp = y - 2;

	for (; i < 11; i++) {
		if (i == 0 || i == 10) {
			drawLine(x_temp, y_temp, 412, 'v', WHITE);
			drawLine(x_temp + 1, y_temp, 412, 'v', WHITE);
		} else {
			drawLine(x_temp, y_temp, 412, 'v', WHITE);

		}
		x_temp += 41;
	}

}
void drawSetTabuleiro(unsigned x, unsigned y, tabuleiro tab, ship* s) {
	Bitmap * b = loadBitmap("home/lcom/proj/img/mapanaves.bmp");
	ship_part f;
	f.t_part = FULL;
	f.t_ship = NOTHING;
	f.hit = 0;
	unsigned short x_temp = 0;
	unsigned short y_temp = 0;

	unsigned short i = 0;
	draw_board(x, y, BIG);

	for (i; i < 100; i++) {
		drawQuadricula(x + (i % 10) * 41, y + (i / 10) * 41, *(tab.tab_array[i % 10][i / 10]), b, s->direction);
	}

	for (i = 0; i < s->size; i++) {
		if (tab.tab_array[s->x_central][s->y_central]->t_ship == NOTHING) {
			if (s->t_ship == DEATH_STAR) {
				drawQuadricula(x + 41 * s->x_central, y + y * s->y_central, s->parts_array[0], b, s->direction);
				drawQuadricula(x + 41 * s->x_central + 41, y + y * s->y_central, s->parts_array[1], b, s->direction);
				drawQuadricula(x + 41 * s->x_central, y + y * s->y_central + 41, s->parts_array[2], b, s->direction);
				drawQuadricula(x + 41 * s->x_central + 41, y + y * s->y_central + 41, s->parts_array[3], b, s->direction);
			} else if (s->direction == 'h') {
				drawQuadricula(x + 41 * s->x_central + i * 41, y + y * s->y_central, s->parts_array[i], b, s->direction);
			} else if (s->direction == 'v') {
				drawQuadricula(x + 41 * s->x_central + i * 41, y + y * s->y_central + i * 41, s->parts_array[i], b, s->direction);
			}
		} else {
			if (s->direction == 'h') {
				drawQuadricula(x + 41 * s->x_central + i * 41, y + y * s->y_central, f, b, s->direction);
			} else if (s->direction == 'v') {
				drawQuadricula(x + 41 * s->x_central + i * 41, y + y * s->y_central + i * 41, f, b, s->direction);
			}
		}
	}

}
void alocaMouse(unsigned short *map, int width, int height) {
	memcpy(triple_buffer, second_buffer, v_res * h_res * bytes_per_pixel);

	short * mem_temp;
	Mouse* mouse = getMouse();
	unsigned int i = 0;
	unsigned short xi = mouse->x, yi = mouse->y + height;
	for (i = 0; i < width * height; i++) {
		if (map[i] != TRANS_COLOR) {
			mem_temp = triple_buffer + h_res * yi + xi;
			*mem_temp = map[i];
		}
		xi++;
		if (xi == width + mouse->x) {
			xi = mouse->x;
			yi--;
		}
	}

}
void cleanBufferSec() {
	memset(second_buffer, 0, v_res * h_res * bytes_per_pixel);
}

void updateBufferSec() {
	memcpy(video_mem, second_buffer, v_res * h_res * bytes_per_pixel);
}

void updateBufferTriple() {
	memcpy(video_mem, triple_buffer, v_res * h_res * bytes_per_pixel);
}

int rgb(unsigned char r, unsigned char g, unsigned char b) {
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
		return -1;
	}
	int red = r * 31 / 255;
	int green = g * 63 / 255;
	int blue = b * 31 / 255;

	return (red << 11) | (green << 5) | blue;
}

void drawQuadricula(unsigned x, unsigned y, ship_part p, Bitmap* bmp, char ori) {

	unsigned short pos_x, pos_y, i;

	switch (p.t_ship) {
	case NOTHING:
		if (p.t_part == WATER) {
			return;
		} else if (p.t_part == FULL) {
			pos_x = 3;
			pos_y = 2;
		}
		break;
	case DEATH_STAR:
		if (p.t_part == UPPER_LEFT) {
			pos_x = 1;
			pos_y = 3;
		} else if (p.t_part == UPPER_RIGHT) {
			pos_x = 2;
			pos_y = 3;
		} else if (p.t_part == BOTTOM_LEFT) {
			pos_x = 1;
			pos_y = 4;
		} else if (p.t_part == BOTTOM_RIGHT) {
			pos_x = 2;
			pos_y = 4;
		}
		break;

	case BATTLESHIP:
		if (p.t_part == FIRST) {
			pos_x = 0;
			pos_y = 0;
		} else if (p.t_part == SECOND) {
			pos_x = 0;
			pos_y = 1;
		} else if (p.t_part == THIRD) {
			pos_x = 0;
			pos_y = 2;
		} else if (p.t_part == FOURTH) {
			pos_x = 0;
			pos_y = 3;
		} else if (p.t_part == FIFTH) {
			pos_x = 0;
			pos_y = 4;
		}
		break;

	case CRUSER:
		if (p.t_part == FIRST) {
			pos_x = 2;
			pos_y = 0;
		} else if (p.t_part == SECOND) {
			pos_x = 2;
			pos_y = 1;
		} else if (p.t_part == THIRD) {
			pos_x = 2;
			pos_y = 2;
		}
		break;

	case FIGHTER:
		if (p.t_part == FIRST) {
			pos_x = 1;
			pos_y = 1;
		} else if (p.t_part == SECOND) {
			pos_x = 1;
			pos_y = 2;
		}
		break;

	case ESCAPE_SHUTTLE:
		if (p.t_part == FIRST) {
			pos_x = 1;
			pos_y = 0;
		}
		break;
	}
	pos_x = 40 * pos_x; //Mudar de posicao relativa na imagens para a real
	pos_y = 40 * pos_y;

	int pos = (bmp->bitmapInfo.height - 1 - pos_y) * bmp->bitmapInfo.width + pos_x;

	for (i = 0; i < 40 * 40; i++) {
		vg_set_pixel(x + i % 40, y + i / 40, bmp->Data[pos]);
		//bmp->Data[(pos_x + i % 40) + (bmp->bitmapInfo.width * ((bmp->bitmapInfo.height - 1) - pos_y))
		//		- (bmp->bitmapInfo.width * i / 40)]);
		pos++;
		if (i % 40 == 0 && i > 0) {
			pos -= (bmp->bitmapInfo.width + 40);
		}
	}
}
