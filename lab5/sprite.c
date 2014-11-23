#include "sprite.h"

/** Creates a new sprite with pixmap "pic", random speeds
 * (not zero) and position (within the screen limits), and
 * draws it in memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(char *pic[]) {
//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == 0)
		return 0;
// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if (sp->map == 0) {
		free(sp);
		return 0;
	}
	return sp;
}

void destroy_sprite(Sprite *sp) {
	if (sp == 0)
		return;
	free(sp->map);
	free(sp);
}
