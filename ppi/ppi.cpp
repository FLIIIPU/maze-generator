// ppi.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "MazeManager.h"
#include<memory>
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

	std::cout << "select grid (input number): \n1-square\n2-hex"<<std::endl;
	int type=0;
	while (true) {
		std::cin >> type;
		if (type == 1 || type == 2) { break; }
		else { std::cout << "incorrect input, please try again\n"; }
	}
	std::unique_ptr<MazeManager> test;
	if (type == 1) {
		test=std::make_unique<MazeManager>(cols, rows, cellSize, windowSize, CellType::square);
	}
	else {
		test=std::make_unique<MazeManager>(cols, rows, cellSize, windowSize, CellType::hex);
	}
	std::cout << "input chance for deleting dead ends?(0-100)" << std::endl;
	int forbraiding;
	while (true) {
		std::cin >> forbraiding;
		if (forbraiding >= 0 && forbraiding <= 100) { break; }
		else { std::cout << "incorrect input, please try again\n"; }
	}
	test->generateDFS();
	test->braidmaze(forbraiding);
	test->markPoints();
	sf::RenderWindow window(sf::VideoMode(windowSize), "Lab Strzelczak");
	window.setFramerateLimit(60);
	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is < sf::Event::Closed>()) {
				window.close();
			}
			else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouseButton->button == sf::Mouse::Button::Left) {
					sf::Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
					test->handleMouse(mousePos);
				}
			}
		}
		window.clear(sf::Color::Black);

		test->draw(window);
		window.display();
	}

}