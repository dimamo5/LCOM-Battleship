#include "sprite.h"

/** Creates a new sprite with pixmap "pic", random speeds
 * (not zero) and position (within the screen limits), and
 * draws it in memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(char *pic[]) {
//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL)
		return NULL;
// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}
	return sp;
}

void destroy_sprite(Sprite *sp) {
	if (sp == NULL)
		return;
	free(sp->map);
	free(sp);
	sp = NULL; // hopeless: pointer is passed by value
}
