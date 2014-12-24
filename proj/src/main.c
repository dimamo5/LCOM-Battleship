#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "graphics.h"
#include "bitmap.h"
#include "keyboard_mouse.h"
#include "battleship.h"

int main(int argc, char **argv) {
	sef_startup();
	//Handler em ASM
	sys_enable_iop(SELF);
	srand(time(NULL));
	vg_init(0x117);

	Battleship* battleship = (Battleship *) startBattleship();

	while (!battleship->done) {
		updateBattleship(battleship); //A funcao da update as variaveis e chama drawBattleship()

		if (battleship->timer_ticked) {
			drawBattleship(battleship);
		}
		if (battleship->kb_code == KEY_ESC) {
			battleship->done = 1;
		}
	}
	stopBattleship(battleship);

	return 0;
}
