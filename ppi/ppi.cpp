// ppi.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "MazeManager.h"
int main() {
	const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	unsigned int screenW = desktop.size.x;
	unsigned int screenH = desktop.size.y;
	float Width = screenW * 0.8f;
	float Height = screenH * 0.8f;
	std::cout << "how many rows do you want?" << std::endl;
	int rows;
	std::cin >> rows;
	std::cout << "how many cols do you want?" << std::endl;
	int cols;
	std::cin >> cols;
	float cellSize = std::floor(Height / rows);
	sf::Vector2u windowSize = { (unsigned int)(cols * cellSize + 100),(unsigned int)(rows * cellSize + 100) };
	sf::RenderWindow window(sf::VideoMode(windowSize), "Labirynt PPI Strzelczak");
	window.setFramerateLimit(60);
	MazeManager test(cols,rows,cellSize,windowSize,CellType::hex);
	test.generateDFS();
	test.braidmaze(1.0f);
	test.markPoints();
	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is < sf::Event::Closed>()) {
				window.close();
			}
			else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouseButton->button == sf::Mouse::Button::Left) {
					sf::Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
					test.handleMouse(mousePos);
				}
			}
		}
		window.clear(sf::Color::Black);

		test.draw(window);
		window.display();
	}

}