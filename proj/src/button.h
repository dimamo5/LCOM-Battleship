typedef struct {
	int x_ini, y_ini, x_final, y_final;
	int width, height;
	int mouse_hover;
	short color_border;

} button;

// Cria um "objeto" do botao, com as coordenadas especificadas
button* newButton(xi, yi, xf, yf,color);

void updateButton(button* b);

// Apaga um "objeto" botao
void deleteButton(button* botao);
