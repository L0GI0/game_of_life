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
		for (int i = 0; i < _height; i++)
			for(int j = 0; j < _width; j++)
				_figure[i].push_back('X');
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


Grid::Grid(int width, int height): _height(height), _width(width){

	for(int i = 0; i < width; i++){
		std::vector<Cell> row(height);
		Cells.push_back(row);
	}

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			Cells[i][j] = Cell();
		}
	}
}

int Grid::getHeight(){
	return _height;
}

int Grid::getWidth(){
	return _width;
}

void Grid::setWidth(int width){
	if (width < 10 || width > 100){
		std::cout << "Invalid grid width" << std::endl;
		return;
	}
	_width = width;
}
    
void Grid::setHeight(int height){
	if (height < 10 || height > 100){
		std::cout << "Invalid grid height" << std::endl;
		return;
	}
	_height = height;
}

void Grid::resetAndResize(int width, int height){
	setWidth(width);
	setHeight(height);

	Cells.clear();

	for(int i = 0; i < _width; i++){
		std::vector<Cell> row(_height);
		Cells.push_back(row);
	}

	for(int i = 0; i < _width; i++){
		for(int j = 0; j < _height; j++){
			Cells[i][j] = Cell();
		}
	}
}

void Grid::setCellsFromRLE(std::string RLELine){
	std::cout << "Setting Grid form RLE \n";
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
	for(int i = 0; i < _height; i++){
		for(int j = 0; j < _width; j++)
			std::cout << ((Cells[j][i].isAlive()) ? 'X' : '.') << " ";
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

	for(int i = 0; i < _width; i++){
		for(int j = 0; j < _height; j++){
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
				bornCell(shape.getXCoordinate() + j - moveHorizontallyFromMiddle, shape.getYCoordinate() + i - moveVerticlyFromMiddle);
			}else{
				killCell(shape.getXCoordinate() + j - moveHorizontallyFromMiddle, shape.getYCoordinate() + i - moveVerticlyFromMiddle);
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

Glider::Glider(int x, int y): Shape(x, y, 3, 3, ((std::shared_ptr<std::string[]>) 
												new std::string[3] {".X.",
										  	  		 		        "..X",
																	"XXX"})){
}

GameOfLife::GameOfLife(Grid & grid): _grid(grid){

}

void GameOfLife::loadFromFile(std::string filename) {

	std::ifstream RLEFile(filename);

	if (!RLEFile.is_open()) {
		std::cout << "Cant open the file" << std::endl;
		return;
	}

	std::string gridSizeLine;


	do {
		std::getline(RLEFile, gridSizeLine);
		gridSizeLine.erase(remove_if(gridSizeLine.begin(), gridSizeLine.end(), isspace), gridSizeLine.end());
	} while (gridSizeLine[0] == '#');

	boost::to_lower(gridSizeLine);

	size_t xGridSizeIndex = gridSizeLine.find("x=", 0);
	size_t yGridSizeIndex = gridSizeLine.find("y=", 0);
	
	std::cout << "Grid size line : " << std::endl << gridSizeLine << std::endl;

	std::cout << "Grid size indexes = " << xGridSizeIndex << " " << yGridSizeIndex << std::endl;

	std::cout << "Grid size substrings " << gridSizeLine.substr(xGridSizeIndex + 2, 2) << " " << gridSizeLine.substr(yGridSizeIndex + 2, 2) << std::endl;

	int gridWidth = std::stoi(gridSizeLine.substr(xGridSizeIndex + 2, 2));
	int gridHeight = std::stoi(gridSizeLine.substr(yGridSizeIndex + 2, 2));

	std::cout << "Grid size =" << gridWidth << "x" << gridHeight << std::endl;

	_grid.resetAndResize(gridWidth, gridHeight);

	std::string RLELine;

	do {
		std::getline(RLEFile, RLELine);
		RLELine.erase(remove_if(RLELine.begin(), RLELine.end(), isspace), RLELine.end());
	} while (RLELine[0] == '#');	

	_grid.setCellsFromRLE(RLELine);

}

void GameOfLife::startTheGame(float speed){


	int count = 0;

	while(true){
		_grid.printTheGrid();
		_grid.updateGrid();

		if (count == 5) {
			saveToFile();
			loadFromFile();
			return;
		}

		count++;
		Sleep(speed);
	}
}

void GameOfLife::saveToFile(std::string filename){

	int aliveCount = 0;
	int deadCount = 0;
 	std::ofstream RLEFile(filename);

 	if(!RLEFile.is_open()){
 		std::cout << "Cant open the file" << std::endl;
 		return;
 	}
	RLEFile << "x = " << _grid.getWidth() << ", y = " << _grid.getHeight() << std::endl;
	for(int i = 0; i < _grid.getWidth(); i++){
		for (int j = 0; j < _grid.getHeight(); j++) {
			if (_grid.getGridCellStatus(i, j) == "Alive") {
			aliveCount++;
			if (deadCount != 0) {
				RLEFile << ((deadCount == 1) ? "" : std::to_string(deadCount)) << 'd';
				deadCount = 0;
				}
			}
			else {
				deadCount++;
				if (aliveCount != 0) {
					RLEFile << ((aliveCount == 1) ? "" : std::to_string(aliveCount)) << 'o';
					aliveCount = 0;
				}
			}
		}
		if (deadCount != 0){
			RLEFile << ((deadCount == 1) ? "" : std::to_string(deadCount)) << 'd';
			deadCount = 0;
		}else if (aliveCount != 0) {
			RLEFile << ((aliveCount == 1) ? "" : std::to_string(aliveCount)) << 'o';
			aliveCount = 0;
		}
		RLEFile << "$";
	}
}

