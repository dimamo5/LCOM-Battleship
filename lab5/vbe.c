#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"



int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	struct reg86u reg;
	mmap_t address;

	lm_init();
	lm_alloc(sizeof(vmi_p), &address);

	reg.u.b.intno = 0x10; //BIOS service mode
	reg.u.w.ax = 0x4F01; //VBE

	reg.u.w.es = PB2BASE(address.phys);
	reg.u.w.di = PB2OFF(address.phys);

	reg.u.w.bx = 1 << LINEAR_MODEL_BIT | mode;

	sys_int86(&reg);


	lm_free(&address);

	return 0;

}

