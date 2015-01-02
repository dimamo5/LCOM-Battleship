#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "highscore.h"
#include "rtc.h"

Highscore_State* newHighscore(Battleship* battle) {
	Highscore_State * state = (Highscore_State*) malloc(sizeof(Highscore_State));

	get_time(&(state->year), &(state->month), &(state->day), &(state->hour), &(state->min));

	loadScores(state);

	state->intro_nome = loadBitmap("home/lcom/proj/img/intro_nome.bmp");
	state->show_scores = loadBitmap("home/lcom/proj/img/show_score.bmp");

	if (battle->highscore_winner > state->jogador_array[4].score) {
		state->score_player = battle->highscore_winner;
		state->show = 0;
	} else {
		state->show = 1;
	}

	state->fonts = loadBitmap("home/lcom/proj/img/fonts.bmp");

	state->done = 0;

	return state;
}

void drawHighscore(Battleship* battle) {
	char temp[20] = "";
	if (highscore->show) {
		unsigned int i;
		aloca_pixmap(0, 0, highscore->show_scores->Data, highscore->show_scores->bitmapInfo.width,
				highscore->show_scores->bitmapInfo.height);
		for (i = 0; i < 5; i++) {
			getStringJogador(battle, i, temp);
			drawString(100, 100 + 75 * i, temp, highscore->fonts);
		}
	} else {
		sprintf(temp, "%d", battle->highscore_winner);
		aloca_pixmap(0, 0, highscore->intro_nome->Data, highscore->intro_nome->bitmapInfo.width, highscore->intro_nome->bitmapInfo.height);
		drawString(400, 100, highscore->nome_player, highscore->fonts);
		drawString(700, 100, temp, highscore->fonts);
	}

}

State updateHighscore(Battleship* battle) {
	char char_adicionar;
	if (highscore->show) {
		if (battle->kb_code == KEY_ESC_BRK) {
			battle->kb_code = KEY_NONE;
			highscore->done = 1;
			return MAIN_MENU_STATE;
		}
	} else if (battle->kb_code == KEY_ENTER && strlen(highscore->nome_player) > 2) {
		Jogador j_temp;
		strcpy(j_temp.nome, highscore->nome_player);
		j_temp.score = highscore->score_player;
		dataToString(battle, j_temp.data);
		highscore->jogador_array[4] = j_temp;
		insertionSort(highscore->jogador_array, 5);
		saveScores(battle);
		highscore->show = 1;
		return HIGHSCORE_STATE;
	} else if (battle->kb_code != KEY_NONE) {
		char_adicionar = getKey(battle);
		battle->kb_code = KEY_NONE;
		if (char_adicionar == '#') {
			return HIGHSCORE_STATE;
		}
		if (char_adicionar == 0 && strlen(highscore->nome_player) > 0) {
			highscore->nome_player[strlen(highscore->nome_player) - 1] = 0;
		} else if (strlen(highscore->nome_player) < 5) {
			string_conc(highscore->nome_player, char_adicionar);
		}

	}
}

void deleteHighscore(Battleship* battle) {
	battle->highscore_winner = -1;
	deleteBitmap(highscore->fonts);
	deleteBitmap(highscore->show_scores);
	deleteBitmap(highscore->intro_nome);
	free(battle->state);
}

void loadScores(Highscore_State* state) {
	char temp[50];
	char *pos;
	unsigned int i;
	Jogador j_temp;

	FILE *f = fopen("home/lcom/proj/img/highscore.txt", "r");
	if (f == NULL)
		printf("Error opening file");

	for (i = 0; i < 5; i++) {
		if (NULL == fgets(j_temp.nome, 255, f)) {
			printf("Error reading file");
		}
		if ((pos = strchr(j_temp.nome, '\n')) != NULL)
			*pos = '\0';

		if (NULL == fgets(j_temp.data, 255, f)) {
			printf("Error reading file");
		}
		if ((pos = strchr(j_temp.data, '\n')) != NULL)
			*pos = '\0';
		if (NULL == fgets(temp, 255, f)) {
			printf("Error reading file");
		}
		if ((pos = strchr(temp, '\n')) != NULL)
			*pos = '\0';
		j_temp.score = atoi(temp);
		state->jogador_array[i] = j_temp;
	}

	fclose(f);
}

char getKey(Battleship* battle) {
	switch (battle->kb_code) {
	case (KEY_A):
		return 'a';
	case (KEY_B):
		return 'b';
	case (KEY_C):
		return 'c';
	case (KEY_D):
		return 'd';
	case (KEY_E):
		return 'e';
	case (KEY_F):
		return 'f';
	case (KEY_G):
		return 'g';
	case (KEY_H):
		return 'h';
	case (KEY_I):
		return 'i';
	case (KEY_J):
		return 'j';
	case (KEY_K):
		return 'k';
	case (KEY_L):
		return 'l';
	case (KEY_M):
		return 'm';
	case (KEY_N):
		return 'n';
	case (KEY_O):
		return 'o';
	case (KEY_P):
		return 'p';
	case (KEY_Q):
		return 'q';
	case (KEY_R):
		return 'r';
	case (KEY_S):
		return 's';
	case (KEY_T):
		return 't';
	case (KEY_U):
		return 'u';
	case (KEY_V):
		return 'v';
	case (KEY_W):
		return 'w';
	case (KEY_X):
		return 'x';
	case (KEY_Y):
		return 'y';
	case (KEY_Z):
		return 'z';
	case (KEY_0):
		return '0';
	case (KEY_1):
		return '1';
	case (KEY_2):
		return '2';
	case (KEY_3):
		return '3';
	case (KEY_4):
		return '4';
	case (KEY_5):
		return '5';
	case (KEY_6):
		return '6';
	case (KEY_7):
		return '7';
	case (KEY_8):
		return '8';
	case (KEY_9):
		return '9';
	case (KEY_BKSP):
		return 0;
	default:
		return '#';
	}
}

void string_conc(char* str1, char ch) {
	unsigned int i, comp;
	comp = strlen(str1);
	str1[comp] = ch;
	str1[comp + 1] = 0;
}

void insertionSort(Jogador jogadores[], int tam) {
	int i, j;
	Jogador eleito;
	for (i = 1; i < tam; i++) {
		eleito = jogadores[i];
		j = i - 1;
		while ((j >= 0) && (eleito.score > jogadores[j].score)) {
			jogadores[j + 1] = jogadores[j];
			j--;
		}
		jogadores[j + 1] = eleito;
	}
}

void getStringJogador(Battleship* battle, unsigned int i, char * temp) {
	sprintf(temp, "%-5s  %s  %d", highscore->jogador_array[i].nome, highscore->jogador_array[i].data, highscore->jogador_array[i].score);
}

int entrouHighscore(Battleship* battle) {
	if (battle->highscore_winner > highscore->jogador_array[4].score) {
		return 1;
	} else
		return 0;
}

void dataToString(Battleship* battle, char *temp) {

	sprintf(temp, "20%.2d.%.2d.%.2d %.2d:%.2d", highscore->year, highscore->month, highscore->day, highscore->hour, highscore->day);
}

void saveScores(Battleship* battle) {
	unsigned int i;

	FILE *f = fopen("home/lcom/proj/img/highscore.txt", "w");
	if (f == NULL)
		printf("Error opening file");

	for (i = 0; i < 5; i++) {
		fprintf(f, "%s\n", highscore->jogador_array[i].nome);
		fprintf(f, "%s\n", highscore->jogador_array[i].data);
		if (i == 4) {
			fprintf(f, "%d", highscore->jogador_array[i].score);
		} else {
			fprintf(f, "%d\n", highscore->jogador_array[i].score);
		}
	}
	fclose(f);
}
