#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>
#include "graphics.h"
#include "bitmap.h"

int main(int argc, char **argv) {
	sef_startup();
	vg_init(0x117);
	Bitmap* sky;
	sky= loadBitmap("/home/lcom/proj/img/teste1.bmp");
	Bitmap* death_star;
	death_star= loadBitmap("/home/lcom/proj/img/star.bmp");
	drawBitmap(sky,50,50);
	drawBitmap(death_star,200,200);
	drawRectangle(200,200,100,10,2,0xF800);
	tickdelay(micros_to_ticks(10000000));
	vg_exit();
	return 0;
}
