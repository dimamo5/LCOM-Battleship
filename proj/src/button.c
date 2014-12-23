#include "button.h"
#include "graphics.h"
#include "keyboard_mouse.h"

Button* newButton(int xi, int yi, int xf, int yf, short color) {
	Button* botao = (Button*) malloc(sizeof(Button));
	botao->x_ini = xi;
	botao->y_ini = yi;
	botao->x_final = xf;
	botao->y_final = yf;
	botao->color_border = color;
	botao->width = xf - xi;
	botao->height = yf - yi;
	botao->available = 1;
	botao->mouse_hover = 0;

	return botao;
}

void updateButton(Button* b) {
	Mouse* mouse = getMouse();
	if (mouse->x > b->x_ini && mouse->x < b->x_final && mouse->y > b->y_ini
			&& mouse->y < b->y_final) {
		b->mouse_hover = 1;
	}
	drawRectangle(b->x_ini, b->y_ini, b->width, b->height, 2, b->color_border);
}

void deleteButton(Button* botao) {
	free(botao);
}
