#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>
#include <math.h>
#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {
	char *vptr;
	vptr=video_mem;
	int i =0;
	for(i;i<scr_width*scr_lines;i++,vptr++){
		*vptr=ch;
		vptr++;
		*vptr=attr;
	}
}


void vt_blank() {
	vt_fill(0x00,0x00);

}

int vt_print_char(char ch, char attr, int r, int c) {
	char *vptr;
	if(r>=scr_lines || c>=scr_width || r<0||c<0){
		return -1;
	}
	vptr=video_mem;
	vptr=vptr+2*scr_width*r+2*c;
	*vptr=ch;
	vptr++;
	*vptr=attr;

	return 0;
}

int vt_print_string(char *str, char attr, int r, int c) {
	if(r>=scr_lines || c>=scr_width || r<0||c<0){
		return -1;
	}
	if(80-c<strlen(str)){
		return -1;
	}
	int lim=strlen(str);
	int i =0;
	char *vptr;
	vptr=video_mem;
	vptr=vptr+2*scr_width*r+2*c;
	for(i;i<lim;i++,vptr++){
		*vptr=*(str+i);
		vptr++;
		*vptr=attr;
	}

	return 0;

}

int vt_print_int(int num, char attr, int r, int c) {
	if(r>=scr_lines || c>=scr_width || r<0||c<0){
		return -1;
	}
	int i =0;
	char *vptr;
	vptr=video_mem;
	vptr=vptr+2*scr_width*r+2*c;
	int num_temp=num;
	int cont =0;
	do{num_temp=num_temp/10;
	cont++;
	}while(num_temp>0);

	vptr=vptr+2*(cont-1);
	for(i;i<cont;i++){
		*vptr=(num%10)+0x30;
		num=num/10;
		vptr++;
		*vptr=attr;
		vptr=vptr-3;
	}
	return 0;

}


int vt_draw_frame(int width, int height, char attr, int r, int c) {
	if(r>=scr_lines || c>=scr_width || r<0||c<0){
		return -1;
	}
	if(r+height>25||c+width>80){
		return -1;
	}
	char *vptr;
	vptr=video_mem;
	vptr=vptr+2*scr_width*r+2*c;
	vptr++;
	int i =0;
	for(i;i<width;i++){
		*vptr=attr;
		vptr=vptr+2;
	}
	i=0;
	vptr= vptr-2;
	for(i;i<height;i++){
		*vptr=attr;
		vptr=vptr+80*2;
	}
	i=0;
	vptr=vptr-(80*2)-2;
	for(i;i<width-1;i++){
		*vptr=attr;
		vptr=vptr-2;
	}
	i=0;
	vptr=vptr+2;
	for(i;i<height-1;i++){
		*vptr=attr;
		vptr=vptr-(80*2);
	}
	return 0;
}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes)(vi_p->vram_base);
	mr.mr_limit = mr.mr_base + vi_p->vram_size;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");

	/* Save text mode resolution */

	scr_lines = vi_p->scr_lines;
	scr_width = vi_p->scr_width;

	return video_mem;
}
