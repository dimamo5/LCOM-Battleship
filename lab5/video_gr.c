#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>

#include "vbe.h"
/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8
#define Mi 0x100000

/* Private global variables */

static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned mem_phys;

unsigned get_hres() {
	return h_res;
}

unsigned get_vres() {
	return v_res;
}

unsigned get_memphys() {
	return mem_phys;
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
	bits_per_pixel = info_mode.BitsPerPixel;

	unsigned long vram = h_res * v_res * (bits_per_pixel / 8);
	/* Map memory */

	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = info_mode.PhysBasePtr;
	mr.mr_limit = mr.mr_base + vram;
	mem_phys = mr.mr_base;

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

void vg_set_pixel(unsigned short x, unsigned short y, unsigned long color) {
	unsigned char * mem_temp = video_mem;
	if ((x > h_res) || (y > v_res)) {
		printf("Erro");
	}
	mem_temp = mem_temp + h_res * y + x;
	(*mem_temp) = color;
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
	/*unsigned short dx = xf - xi;
	 unsigned short dy = yf - yi;
	 unsigned short x, y;

	 unsigned short d = 2 * dy - dx;
	 vg_set_pixel(xi, yi,color);
	 y = yi;

	 for (x = xi + 1; x < xf; x++) {
	 if (d > 0) {
	 y = y + 1;
	 vg_set_pixel(x, y,color);
	 d = d + (2 * dy - 2 * dx);
	 } else {
	 vg_set_pixel(x, y,color);
	 d = d + (2 * dy);
	 }

	 }*/
	int err, e2, dx = abs(xf - xi), dy = abs(yf - yi), sx, sy;
	/*int err = (dx > dy ? dx : -dy) / 2, e2;
	 int dx = abs(xf - xi), sx = xi < xf ? 1 : -1;
	 int dy = abs(yf - yi), sy = yi < yf ? 1 : -1;*/

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
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			xi += sx;
		}
		if (e2 < dy) {
			err += dx;
			yi += sy;
		}
	}

}

void aloca_pixmap(unsigned short xi, unsigned short yi, char *map, int width,
		int height) {
	char * mem_temp;
	unsigned int i = 0;
	// copy it to graphics memory
	unsigned short x_original = xi;

	for (i = 0; i < width * height; i++) {
		vg_set_pixel(xi, yi, map[i]);
		xi++;
		if (xi == width + x_original) {
			xi = x_original;
			yi++;
		}
	}
}

void display_vbe_info() {
	vbe_info_t info_vbe;

	short* mode_list_ptr = (short*) vbe_get_info(&info_vbe);

	// Endereço físico do Video Mode
	int farptr = info_vbe.VideoModePtr;

	// Transformacao de endereço fisico para virtual
	mode_list_ptr = (short *) ((int) mode_list_ptr
			+ ((farptr & 0xffff0000) >> 12) + (farptr & 0xffff));

	do {
		printf("0x%X\t", *mode_list_ptr);
		(short *) mode_list_ptr++;
	} while (*mode_list_ptr != -1);

	if (info_vbe.Capabilities[0] & BIT(0)) {
		printf("\n\nDAC width is switchable to 8 bits per primary color\n\n");
	} else {
		printf("\n\nDAC is fixed width, with 6 bits per primary color\n\n");
	}
	if (info_vbe.Capabilities[0] & BIT(1) >> 1) {
		printf("Controller is not VGA compatible\n\n");
	} else {
		printf("Controller is VGA compatible\n\n");
	}
	if (info_vbe.Capabilities[0] & BIT(2) >> 2) {
		printf("When programming large blocks of information to the RAMDAC\n"
				"use the blank bit in Function 09h\n\n");
	} else {
		printf(" Normal RAMDAC operation\n\n");
	}
	printf("\nVBE  Total memory: %d KB\n\n", info_vbe.TotalMemory * 64);

}
