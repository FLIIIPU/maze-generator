#pragma once
#include"cell.h"
class MazeManager
{
private:
	int width, height;
	CellType type;
	std::vector<std::vector<cell>> maze;
	std::pair<int, int> player;
public:
	MazeManager(int width, int height, float cell_size, sf::Vector2u windowSize,CellType type);
	void generateDFS();
	void removeOneRandWall(int x,int y);
	void draw(sf::RenderWindow& window);
	void braidmaze(int chance);
	void markPoints() {
		maze[player.first][player.second].setFillColor(sf::Color::Green);
		maze[height - 1][width - 1].setFillColor(sf::Color::Red);
	}
	void handleMouse(sf::Vector2f mousePos);
};

