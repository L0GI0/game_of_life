#include "gameOfLife.h"

Cell::Cell(const sf::Vector2f & cellSize, float xPosition, float yPosition) {
	_cellShape.setSize(cellSize);
	_cellShape.setPosition(xPosition, yPosition);
	_cellShape.setFillColor(sf::Color::Black);
	_cellShape.setOutlineColor(sf::Color::Black);
	_cellShape.setOutlineThickness(1.0);
}

Cell::Cell() {

}

bool Cell::isAlive() const{
	return _alive;
}

void Cell::kill(){
	_cellShape.setFillColor(sf::Color::Black);
	_alive = false;
}


void Cell::born(){
	_cellShape.setFillColor(sf::Color::White);
	_alive = true;
}

void Cell::draw(sf::RenderWindow& window) const {
	window.draw(_cellShape);
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


Grid::Grid(int width, int height, sf::RenderWindow & window): _height(height), _width(width){

	float cells_width =  window.getSize().x / _width;
	float cells_height = (window.getSize().y - (window.getSize().y * 0.1))  / _height;

	for(int i = 0; i < width; i++){
		std::vector<Cell> row(height);
		Cells.push_back(row);
	}

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			Cells[i][j] = Cell(sf::Vector2f(cells_width, cells_height), cells_width * i, (cells_height * j) + (window.getSize().y * 0.1));
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

void Grid::resetAndResize(int width, int height, sf::RenderWindow& window){
	setWidth(width);
	setHeight(height);

	float cells_width = window.getSize().x / _width;
	float cells_height = (window.getSize().y - (window.getSize().y * 0.1)) / _height;

	Cells.clear();

	for(int i = 0; i < _width; i++){
		std::vector<Cell> row(_height);
		Cells.push_back(row);
	}

	for(int i = 0; i < _width; i++){
		for(int j = 0; j < _height; j++){
			Cells[i][j] = Cell(sf::Vector2f(cells_width, cells_height), cells_width * i, (cells_height * j) + (window.getSize().y * 0.1));
		}
	}
}

void Grid::setCellsFromRLE(std::string RLELine){
	std::cout << "Setting Grid form RLE \n";

	for (int j = 0; j < _width; j++) {
		int endRowIndex = RLELine.find_first_of("$");
		std::string row = RLELine.substr(0, endRowIndex);
		RLELine = RLELine.substr(endRowIndex + 1, RLELine.length());
		int currentRowIndex = 0;
		while (row.length() != 0) {
			int numOfCellsToChange = 0;
			int firstAlpha = row.find_first_not_of("0123456789");

			if (firstAlpha != 0) {
				numOfCellsToChange = std::stoi(row.substr(0, firstAlpha));
			}
			else {
				numOfCellsToChange = 1;
			}
			if (firstAlpha != 0) {
			}
			char cellsStatus = row[firstAlpha];
			for (int i = currentRowIndex; i < currentRowIndex + numOfCellsToChange; i++) {
				if (cellsStatus == 'o') {
					Cells[j][i].born();
				}
				else {
					Cells[j][i].kill();
				}
			}
			currentRowIndex += numOfCellsToChange;
			row = row.substr(firstAlpha + 1, row.length());
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
	for(int i = 0; i < _height; i++){
		for(int j = 0; j < _width; j++)
			std::cout << ((Cells[j][i].isAlive()) ? 'X' : '.') << " ";
		std::cout << std::endl;
	}
}

void Grid::drawTheGrid(sf::RenderWindow & window){
	std::cout << std::endl;
	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++)
			Cells[j][i].draw(window);
	}
}

void Grid::killCell(int x, int y){
	Cells[x][y].kill();
}

void Grid::bornCell(int x, int y){
	Cells[x][y].born();
}

void Grid::changeCellStatus(int x, int y) {
	if (Cells[x][y].isAlive())
		Cells[x][y].kill();
	else
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

GameOfLife::GameOfLife(Grid & grid, sf::RenderWindow & window): _grid(grid), _window(window){

}

void GameOfLife::loadFromFile(std::string filename) {

	std::cout << "File name wen loading = " << filename << std::endl;
	std::ifstream RLEFile(filename);
	if (!RLEFile.is_open()) {
		std::cout << "Cant open the " << filename << " file" << std::endl;
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

	int gridWidth = std::stoi(gridSizeLine.substr(xGridSizeIndex + 2, 2));
	int gridHeight = std::stoi(gridSizeLine.substr(yGridSizeIndex + 2, 2));

	std::cout << "Grid size = " << gridWidth << "x" << gridHeight << std::endl;

	_grid.resetAndResize(gridWidth, gridHeight, _window);

	std::string RLELine;

	do {
		std::getline(RLEFile, RLELine);
		RLELine.erase(remove_if(RLELine.begin(), RLELine.end(), isspace), RLELine.end());
	} while (RLELine[0] == '#');

	std::cout << "RLE Line = " << RLELine << std::endl;
	_grid.setCellsFromRLE(RLELine);

}

void GameOfLife::saveToFile(std::string filename) {

	int aliveCount = 0;
	int deadCount = 0;
	std::ofstream RLEFile(filename);

	if (!RLEFile.is_open()) {
		std::cout << "Cant open the " << filename << " file" << std::endl;
		return;
	}
	RLEFile << "x = " << _grid.getWidth() << ", y = " << _grid.getHeight() << std::endl;
	for (int i = 0; i < _grid.getWidth(); i++) {
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
		if (deadCount != 0) {
			RLEFile << ((deadCount == 1) ? "" : std::to_string(deadCount)) << 'd';
			deadCount = 0;
		}
		else if (aliveCount != 0) {
			RLEFile << ((aliveCount == 1) ? "" : std::to_string(aliveCount)) << 'o';
			aliveCount = 0;
		}
		RLEFile << "$";
	}
}

void GameOfLife::startTheGame(float speed){
	bool isPaused = false;
	_grid.drawTheGrid(_window);
	sf::RectangleShape topMenu;
	topMenu.setSize(sf::Vector2f(_window.getSize().x, _window.getSize().y * 0.1));
	_menuHeight = _window.getSize().y * 0.1;
	topMenu.setPosition(0, 0);
	topMenu.setFillColor(sf::Color::Cyan);
	sf::Font font;
	font.loadFromFile("arial.ttf");

	sf::Text menuText;
	sf::Text pauseText;
	sf::Text loadText;
	sf::Text saveText;

	menuText.setFont(font);
	menuText.setString("Menu");
	menuText.setCharacterSize(24);
	menuText.setFillColor(sf::Color::Black);
	menuText.setPosition(sf::Vector2f((_window.getSize().x / 2) - 4, 0));

	pauseText.setFont(font);
	pauseText.setString("p - pause the game");
	pauseText.setCharacterSize(14);
	pauseText.setFillColor(sf::Color::Black);
	pauseText.setPosition(sf::Vector2f(50, 40));

	loadText.setFont(font);
	loadText.setString("l - load the game from RLEFile.txt");
	loadText.setCharacterSize(14);
	loadText.setFillColor(sf::Color::Black);
	loadText.setPosition(sf::Vector2f((_window.getSize().x / 2) - std::string("l - load the game from RLEFile.txt").length() * 2.5, 40));

	saveText.setFont(font);
	saveText.setString("s - save the game to RLEFile.txt");
	saveText.setCharacterSize(14);
	saveText.setFillColor(sf::Color::Black);
	saveText.setPosition(sf::Vector2f((_window.getSize().x) - (std::string("s - save the game to RLEFile.txt").length() + 12) * 7, 40));
	_window.display();

	while (_window.isOpen()) {
		sf::Event event;
		_window.draw(topMenu);
		_window.draw(menuText);
		_window.draw(pauseText);
		_window.draw(loadText);
		_window.draw(saveText);
		while (_window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				_window.close();

			if (event.type == sf::Event::Resized) {
				_menuHeight = _window.getSize().y * 0.1;
				std::cout << "menu size = " << _menuHeight << std::endl;
				break;
			}

			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
				if (mousePosition.x >= 0 & mousePosition.x / floor(_window.getSize().x / _grid.getWidth()) < _grid.getWidth() &
					mousePosition.y >= _menuHeight & ((mousePosition.y - _menuHeight) / floor(((_window.getSize().y - _menuHeight) / _grid.getHeight()))) < _grid.getHeight()) {
					_grid.changeCellStatus(int(floor(mousePosition.x / floor(_window.getSize().x / _grid.getWidth()))), int((mousePosition.y - _menuHeight) / ((_window.getSize().y - _menuHeight) / _grid.getHeight())));
					_grid.drawTheGrid(_window);
					_window.display();
					break;
				}
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
				if (!isPaused) {
					isPaused = true;
					std::cout << "Game paused" << std::endl;
				}
				else {
					isPaused = false;
					std::cout << "Game unpaused" << std::endl;
				}
				Sleep(100);
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				saveToFile();
				std::cout << "Save to file" << std::endl;
				break;
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
				loadFromFile();
				_grid.drawTheGrid(_window);
				_window.display();
				isPaused = false;
			}
		}

		if (!isPaused) {
			_grid.updateGrid();
			_grid.drawTheGrid(_window);
			_window.display();
		}
	}
}