#ifndef _BUTTON_H_
#define _BUTTON_H_

typedef struct {
	int x_ini, y_ini, x_final, y_final;
	int width, height;
	int mouse_hover;
	short color_border;
	unsigned short available;
} Button;

// Cria um "objeto" do botao, com as coordenadas especificadas
Button* newButton(int xi,int yi,int xf,int yf,short color);

void updateButton(Button* b);

// Apaga um "objeto" botao
void deleteButton(Button* botao);

#endif
