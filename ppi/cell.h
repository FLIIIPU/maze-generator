#pragma once
#include <array>
#include <vector> // Dodane dla std::vector
#include <SFML/Graphics.hpp>

enum class CellType { square, hex };
enum Walls {
	TOP = 0, RIGHT = 1, DOWN = 2, LEFT = 3,
	H_RIGHT = 0, H_BR = 1, H_BL = 2, H_LEFT = 3, H_TL = 4, H_TR = 5
};

struct Direction {
	int dx, dy, wall, opposite;
};

class cell
{
private:
	int x, y;
	CellType type;
	std::array<bool, 6> walls = { true,true,true,true,true,true };
	bool visited = false;
	int weight = 1;
	cell* previous = nullptr;
	sf::ConvexShape shape;
public:
	cell(int x, int y, float size, sf::Vector2f offset, CellType type);
	void draw(sf::RenderWindow& window) const;
	void setFillColor(sf::Color color) { shape.setFillColor(color); }
	void setx(int a) { x = a; };
	void sety(int a) { y = a; };
	int getx() { return x; };
	int gety() { return y; };
	bool getvisited() { return visited; };
	void setvisited() { visited = true; };
	cell* getprevious() { return this->previous; };
	void setprevious(cell* newprevious) { this->previous = newprevious; }
	void drawLine(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2) const;
	void removeWall(int index) { walls[index] = false; }
	int wallCount() {
		int count = 0;
		int maxWalls = (type == CellType::square) ? 4 : 6;
		for (int i = 0; i < maxWalls; ++i) { if (walls[i]) count++; }
		return count;
	}
	bool checkWall(int index) const { return walls[index]; }
	std::vector<Direction> getDirections() const;
	sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
};