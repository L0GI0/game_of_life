#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <typeinfo>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <SFML/Graphics.hpp>
#include <windows.h>


class Cell{
public:
    Cell(const sf::Vector2f & cellSize, float xPosition, float yPosition);
    Cell();
    bool isAlive() const;
    void kill();
    void born();
    void draw(sf::RenderWindow &) const;
private:
    bool _alive = false;
    sf::RectangleShape _cellShape;
};

class Shape{
public:
    Shape(int x, int y, int width, int height, std::shared_ptr<std::string[]> = NULL);
    int getXCoordinate();
    int getYCoordinate();
    int getWidth();
    int getHeight();
    void printShape();
    std::shared_ptr<std::string[]> getFigure();
    std::shared_ptr<std::string[]> _figure;
private:
    int _x;
    int _y;
    int _width;
    int _height;
    
};

class Blinker: public Shape{
public:
    Blinker(int x, int y);
private:

};

class Glider: public Shape{
public:
    Glider(int x, int y);

private:

};

class Grid{

public:
    Grid(int width, int height);
    Grid(int width, int height, sf::RenderWindow &);
    int getHeight();
    int getWidth();
    void killCell(int x, int y);
    void bornCell(int x, int y);
    void changeCellStatus(int x, int y);
    void resetAndResize(int width, int height, sf::RenderWindow& window);
    void setCellsFromRLE(std::string RLELine);
    int checkNumberOfLivingCellNeighbours(int x, int y);
    void updateGrid();
    void printTheGrid();
    void addShape(Shape &shape);
    void drawTheGrid(sf::RenderWindow &);
    std::string getGridCellStatus(int x, int y);

private:
    void setHeight(int height);
    void setWidth(int width);
    int _height;
    int _width;
    std::vector <std::vector <Cell>> Cells;
};

class GameOfLife{

public:

    GameOfLife(Grid& grid, sf::RenderWindow& window);
    void startTheGame(float speed);
    void saveToFile(std::string filename = "RLEFile.txt");
    void loadFromFile(std::string filename = "RLEFile.txt");
private:
    Grid & _grid;
    sf::RenderWindow& _window;
    int _menuHeight = 100;
};