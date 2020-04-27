#include "gameOfLife.h"


int main(){

	Grid grid(10, 10);

	Blinker blinker = Blinker(5, 5);

	grid.addShape(blinker);

	// grid.printTheGrid();

	// grid.updateGrid();

	// grid.printTheGrid();

	GameOfLife game = GameOfLife(grid);

	game.startTheGame();

	return 0;
}