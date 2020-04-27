#include "GameOfLife.h"


bool Cell::isAlive() const{
	return _alive;
}

void Cell::kill(){
	_alive = false;
}


void Cell::born(){
	_alive = true;
}

Shape::Shape(int x, int y, int width, int height, std::shared_ptr<std::string[]> figure): _x(x), _y(y), _width(width), _height(height){

		if (figure == NULL){
		_figure = std::shared_ptr<std::string[]> (new std::string[_height]);
		for(int i = 0; i < _height; i++)
			for(int j = 0; j < _width; j++)
				_figure[i][j] = 'X';
		}else{
			_figure = figure;
		}
}

std::shared_ptr<std::string[]> Shape::getFigure(){
	return _figure;
}

void Shape::printShape(){
	std::cout << "Width = " << _width << std::endl;
	std::cout << "Height = " << _height << std::endl;
	for(int i = 0; i < _height; i++){
		for(int j = 0; j < _width; j++){
			std::cout << _figure[i][j];
		}
		std::cout << std::endl;
	}
}


Grid::Grid(int height, int width): _height(height), _width(width){

	for(int i = 0; i < height; i++){
		std::vector<Cell> row(width);
		Cells.push_back(row);
	}

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			Cells[i][j] = Cell();
		}
	}
}

std::string Grid::getGridCellStatus(int x, int y){
	if (x < 0 || y < 0 || x >= _width || y >= _height)
		return "No such cell";
	std::string status;
	Cells[x][y].isAlive() ? (status = "Alive") : (status = "Dead");
	return status;
}

void Grid::printTheGrid(){
	std::cout << std::endl;
	for(int i = 0; i < _width; i++){
		for(int j = 0; j < _height; j++)
			std::cout << ((Cells[i][j].isAlive()) ? 'X' : '.');
		std::cout << std::endl;
	}
}


void Grid::killCell(int x, int y){
	Cells[x][y].kill();
}

void Grid::bornCell(int x, int y){
	Cells[x][y].born();
}

int Grid::checkNumberOfLivingCellNeighbours(int x, int y){

	int neighbours = 0;
	
	for (int i = x - 1; i <= x + 1; i++)
		if (getGridCellStatus(i, y - 1) == "Alive")
			neighbours++;

	for (int i = x - 1; i <= x + 1; i++)
		if (getGridCellStatus(i, y + 1) == "Alive")
			neighbours++;

	if (getGridCellStatus(x - 1, y) == "Alive")
			neighbours++;
	

	if (getGridCellStatus(x + 1, y) == "Alive")
			neighbours++;

	return neighbours;
}

void Grid::updateGrid(){
	std::vector <std::vector <Cell>> updatedCells;

	updatedCells = Cells;

	for(int i = 0; i < _height; i++){
		for(int j = 0; j < _width; j++){
			int numOfNeighbours = checkNumberOfLivingCellNeighbours(i, j);
			if (numOfNeighbours == 3)
				updatedCells[i][j].born();
			else if ( numOfNeighbours < 2 || numOfNeighbours > 3){
				updatedCells[i][j].kill();
			}
		}
	}

	Cells = updatedCells;
}

void Grid::addShape(Shape &shape){
	if ( (( shape.getXCoordinate() - (shape.getWidth() / 2)) < 0) || (((shape.getWidth() / 2 ) + shape.getXCoordinate() ) >  _width)
		|| (( shape.getYCoordinate() - (shape.getHeight() / 2)) < 0) || (((shape.getHeight() / 2 ) + shape.getYCoordinate() ) >  _height)){
		std::cout << "Shape cant be added due to incorrect coordinates for this grid" << std::endl;
		return;
	}

	int moveHorizontallyFromMiddle = shape.getWidth() / 2;
	int moveVerticlyFromMiddle = shape.getHeight() / 2;

	std::shared_ptr<std::string[]> figure = shape.getFigure();

	for(int i = 0; i < shape.getHeight(); i++){
		for(int j = 0; j < shape.getWidth(); j++){
			if(figure[i][j] == 'X'){
				bornCell(shape.getYCoordinate() + i - moveVerticlyFromMiddle, shape.getXCoordinate() + j - moveHorizontallyFromMiddle);
			}else{
				killCell(shape.getYCoordinate() + i - moveVerticlyFromMiddle, shape.getXCoordinate() + j - moveHorizontallyFromMiddle);
			}
		}
	}
}


int Shape::getXCoordinate(){
	return _x;
}

int Shape::getYCoordinate(){
	return _y;
}

int Shape::getWidth(){
	return _width;
}

int Shape::getHeight(){
	return _height;
}

Blinker::Blinker(int x, int y): Shape(x, y, 3, 1){

}

GameOfLife::GameOfLife(Grid & grid): _grid(grid){

}

void GameOfLife::startTheGame(){

	while(true){
		_grid.updateGrid();
		_grid.printTheGrid();

		Sleep(1500);
	}
}