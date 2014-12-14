#include "keyboard_mouse.h"
#include "timer.h"
#include "button.h"
#include "battleship.h"
#include "bitmap.h"
#include "graphics.h"

Battleship* startBattleship() {

	// Criar um "objeto" de jogo "battleship"
	Battleship* battle = (Battleship*) malloc(sizeof(Battleship));

	// Subscrever dispositivos
	battle->IRQ_SET_KEYBOARD = kbd_subscribe_int();
	battle->IRQ_SET_MOUSE = mouse_subscribe_int();
	battle->IRQ_SET_TIMER = timer_subscribe_int();

	getMouse();

	enable_packets();

	// Por frequencia do timer a 30 (30 fps)
	timer_set_square(0, 30);

	// Inicializar Variaveis
	// currentState inicial e o menu inicial
	battle->kb_code = 0;
	battle->currentState = MAIN_MENU_STATE;
	//battle->state = newMainMenuState();

	// finish initialization
	battle->done = 0, battle->draw = 1;

	return battle;
}

void updateBattleship(Battleship* battleship) {
	int ipc_status, r;
	message msg;

	if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
		printf("driver_receive failed with: %d", r);
	}

	if (is_ipc_notify(ipc_status)) // receive notification of interrupt request. returns true if msg received is notification and false otherwise
			{
		switch (_ENDPOINT_P(msg.m_source)) // m_source contains the endpoint of the msg and _ENDPOINT extracts the process identifier from process's endpoint
		{
		case HARDWARE:
			if (msg.NOTIFY_ARG & battleship->IRQ_SET_MOUSE) {
				updateMouse();
			}

			if (msg.NOTIFY_ARG & battleship->IRQ_SET_TIMER) {
				drawBattleship(battleship);
			}

			if (msg.NOTIFY_ARG & battleship->IRQ_SET_KEYBOARD) {
				battleship->kb_code = scan_kb_asm();
			}

			break;

		default:
			break;
		}
	} else {
		printf("Any interrupt received\n"); // Any interrupt received, so anything to do
	}

	if (battleship->kb_code == KEY_ESC) {
		battleship->done = 1;
	}
}

void drawBattleship(Battleship* battleship) {

	Bitmap* sky;
	sky = loadBitmap("/home/lcom/proj/img/teste1.bmp");
	Bitmap* death_star;
	death_star = loadBitmap("/home/lcom/proj/img/star.bmp");
	drawBitmap(sky, 50, 50);
	drawBitmap(death_star, 100, 50);
	drawRectangle(100, 50, death_star->bitmapInfo.width,
			death_star->bitmapInfo.height, 2, 0x019F);
	draw_board(200, 200, SMALL);

	switch (battleship->currentState) {

	case MAIN_MENU_STATE:
		//drawMainMenuState(battleship);
		break;
	case GAME_PLAY_SETSHIP_STATE:
//		drawPlaySetship(battleship);
		break;
	case GAME_PLAY_STATE:
//		drawPlay(battleship);
		break;
	case GAME_PAUSE_STATE:
//		drawPause(battleship);
		break;
	case HIGHSCORE_WRITE_STATE:
//		drawHighscoreWrite;
		break;
	case HIGHSCORE_MENU_STATE:
//		drawHighscoreMenu;
		break;

	default:
		break;
	}

	drawMouse();
}

void stopBattleship(Battleship* battleship) {

	if (mouse_unsubscribe_int() != OK) {
		printf("Unsubscribe mouse failed");
	}

	disable_packets();

	if (kbd_unsubscribe_int() != OK) {
		printf("Unsubscribe kbd failed");
	}
	if (timer_unsubscribe_int() != OK) {
		printf("Unsubscribe timer failed");
	}

	deleteMouse();

	free(battleship);
}

void changeState(Battleship* battleship, State programState) {

//Apagar o state atual
	deleteCurrentState(battleship);

//Fazer set ao state atual para aquele que se queria
	battleship->currentState = programState;

//Dependendo do estado que estamos a querer, vai-se criar esse estado
	switch (battleship->currentState) {

	case MAIN_MENU_STATE:
		//battleship->state = newMainMenuState(battleship);
		break;
	case GAME_PLAY_SETSHIP_STATE:
		//		battleship->state = newPlaySetship(battleship);
		break;
	case GAME_PLAY_STATE:
		//		battleship->state = newPlay(battleship);
		break;
	case GAME_PAUSE_STATE:
		//		battleship->state = newPause(battleship);
		break;
	case HIGHSCORE_WRITE_STATE:
		//		battleship->state = newHighscoreWrite(battleship);
		break;
	case HIGHSCORE_MENU_STATE:
		//		battleship->state = newHighscoreMenu(battleship);
		break;

	default:
		break;
	}

}

//Esta funcao verifica se e necessario chamar changeState e chama esta funcao, se necessario, consoante
//certas condicoes
void updateCurrentState(Battleship* battleship) {
	switch (battleship->currentState) {
	case MAIN_MENU_STATE:
		//deleteMainMenuState(battleship);
		break;
	case GAME_PLAY_SETSHIP_STATE:
		//		deletePlaySetship(battleship);
		break;
	case GAME_PLAY_STATE:
		//		deletePlay(battleship);
		break;
	case GAME_PAUSE_STATE:
		//		deletePause(battleship);
		break;
	case HIGHSCORE_WRITE_STATE:
		//		deleteHighscoreWrite(battleship);
		break;
	case HIGHSCORE_MENU_STATE:
		//		deleteHighscoreMenu(battleship);
		break;

	default:
		break;
	}

}

void deleteCurrentState(Battleship* battleship) {
	switch (battleship->currentState) {

	case MAIN_MENU_STATE:
		//deleteMainMenuState(battleship);
		break;
	case GAME_PLAY_SETSHIP_STATE:
//		deletePlaySetship(battleship);
		break;
	case GAME_PLAY_STATE:
//		deletePlay(battleship);
		break;
	case GAME_PAUSE_STATE:
//		deletePause(battleship);
		break;
	case HIGHSCORE_WRITE_STATE:
//		deleteHighscoreWrite(battleship);
		break;
	case HIGHSCORE_MENU_STATE:
//		deleteHighscoreMenu(battleship);
		break;

	default:
		break;
	}
}
