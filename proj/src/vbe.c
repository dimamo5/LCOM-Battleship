#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	struct reg86u reg;
	mmap_t address;

	lm_init();
	lm_alloc(sizeof(vbe_mode_info_t), &address);

	reg.u.w.cx = mode;

	reg.u.b.intno = 0x10; //BIOS service mode
	reg.u.w.ax = VBE_MODE_INFO; //VBE

	reg.u.w.es = PB2BASE(address.phys);
	reg.u.w.di = PB2OFF(address.phys);

	sys_int86(&reg);

	if (reg.u.b.ah == FUNCTION_FAIL) {
		printf("Function call failed \n");
		return -1;
	} else if (reg.u.b.ah == FUNCTION_NOT_SUPPORTED) {
		printf("Function is not supported in current HW configuration \n");
		return -1;
	} else if (reg.u.b.ah == FUNCTION_INVALID) {
		printf("Function is invalid in current video mode \n");
		return -1;
	}

	*vmi_p = *(vbe_mode_info_t *) address.virtual;
	lm_free(&address);

	return 0;

}

