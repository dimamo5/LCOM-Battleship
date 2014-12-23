#include "game.h"
#include "battleship.h"

game* newGame() {
}
void drawGame(Battleship* battle) {

}
State updateGame(Battleship* battle) {

}
void deleteGame(Battleship* battle) {

}

SetShipState* newPlaySetship() {
	SetShipState* state = (SetShipState*) malloc(sizeof(SetShipState));

	ship_part water;
	water.t_part = WATER;
	water.hit = 0;
	water.selected = 0;
}
void drawPlaySetship(Battleship* battle){

}
State updatePlaySetship(Battleship* battle){

}
void deletePlaySetship(Battleship* battle){

}
