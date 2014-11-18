#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

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

/* Private global variables */

static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void * vg_init(unsigned short mode) {
	struct reg86u reg;
	reg.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg.u.w.bx = 1 << LINEAR_MODEL_BIT | mode; // set bit 14: linear framebuffer
	reg.u.b.intno = 0x10;
	if (sys_int86(&reg) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	/* Map memory */

	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = VRAM_PHYS_ADDR;
	mr.mr_limit = mr.mr_base + H_RES * V_RES;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, H_RES * V_RES);

	if (video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");

	/* Save text mode resolution */

	h_res = H_RES;
	v_res = V_RES;
	bits_per_pixel = BITS_PER_PIXEL;
	return video_mem;
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
	unsigned char * mem_temp = video_mem;
	unsigned short x_original = x;
	unsigned short i;
	if (x + width > h_res) {
		return;
	}
	if (y + height > v_res) {
		return;
	}
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
	int dx = abs(xf - xi), sx = xi < xf ? 1 : -1;
	int dy = abs(yf - yi), sy = yi < yf ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

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
