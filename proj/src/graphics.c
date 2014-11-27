#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>

#include "vbe.h"
#include "graphics.h"
/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */

/* Private global variables */

static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bytes_per_pixel; /* Number of VRAM bits per pixel */

unsigned getHRes() {
	return h_res;
}

unsigned getVRes() {
	return v_res;
}
char * getVideoMem() {
	return video_mem;
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
}

void vg_set_pixel(unsigned short x, unsigned short y, unsigned short color) {
	unsigned char * mem_temp = video_mem;

	mem_temp = mem_temp + bytes_per_pixel * (h_res * y + x);
	(*(unsigned short *) mem_temp) = color;
}

void vg_fill(unsigned short x, unsigned short y, unsigned short width,
		unsigned short height, unsigned long color) {
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

void vg_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {

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

void drawRectangle(unsigned short x, unsigned short y, unsigned short width,
		unsigned short height, unsigned short border, unsigned short color) {

	for (border; border; border--) {
		unsigned short x_temp = x;
		unsigned short y_temp = y;

		for (x_temp; x_temp <= width + x; x_temp++) {
			vg_set_pixel(x_temp, y, color);
			vg_set_pixel(x_temp, y + height, color);
		}
		for (y_temp; y_temp <= height + y; y_temp++) {
			vg_set_pixel(x, y_temp, color);
			vg_set_pixel(x + width, y_temp, color);
		}

		x --;
		y --;
		width += 2;
		height += 2;
	}
}
void aloca_pixmap(unsigned short xi, unsigned short yi, unsigned short *map,
		int width, int height) {
	unsigned int i = 0;
	// copy it to graphics memory
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
