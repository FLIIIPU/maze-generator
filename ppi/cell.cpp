#include "cell.h"
#include "cmath"
#define _USE_MATH_DEFINES
cell::cell(int x, int y, float size, sf::Vector2f offset, CellType type) :x(x), y(y),type(type) {
	if (type == CellType::square) {
		shape.setPointCount(4);
		shape.setPoint(0, { 0,0 });
		shape.setPoint(1, { size,0 });
		shape.setPoint(2, { size,size });
		shape.setPoint(3, { 0,size });
		shape.setPosition({ x * size + offset.x,y * size + offset.y });

	}
	else {
		shape.setPointCount(6);
		float radius = size / 2.0f;
		for (int i = 0; i < 6; ++i) {
			float angle_deg = 60.f * i - 30.f;
			float angle_rad = 3.14f / 180.f * angle_deg;
			shape.setPoint(i, { radius + radius * std::cos(angle_rad),radius + radius * std::sin(angle_rad) });
		}
		float width = std::sqrt(3.f) * radius;
		float height = 2.f * radius;
		float xpos = x * width + offset.x;
		if (y % 2 != 0) xpos += width / 2.f;
		float ypos = y * (height * 0.75f) + offset.y;
		shape.setPosition({ xpos, ypos });
	}
	shape.setFillColor(sf::Color::Transparent);
}
void cell::drawLine(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2) const {
	sf::Vertex line[] = { sf::Vertex(p1), sf::Vertex(p2) };
	window.draw(line, 2, sf::PrimitiveType::Lines);
}

void cell::draw(sf::RenderWindow& window) const {
	window.draw(shape);
	int points = shape.getPointCount();
	sf::Vector2f pos = shape.getPosition();
	for (int i = 0; i < points; ++i) {
		if (walls[i]) {
			sf::Vector2f p1 = shape.getPoint(i) + pos;
			sf::Vector2f p2 = shape.getPoint((i + 1) % points) + pos;
			drawLine(window, p1, p2);
		}
	}
}

std::vector<Direction> cell::getDirections() const {
	if (type == CellType::square) {//SQUARE
		return {
			{0, -1, TOP, DOWN},
			{1, 0, RIGHT, LEFT},
			{0, 1, DOWN, TOP},
			{-1, 0, LEFT, RIGHT}
		};
	}
	else { // HEX
		if (y % 2 == 0) {
			return { {1, 0, 0, 3}, {0, 1, 1, 4}, {-1, 1, 2, 5}, {-1, 0, 3, 0}, {-1, -1, 4, 1}, {0, -1, 5, 2} };
		}
		else {
			return { {1, 0, 0, 3}, {1, 1, 1, 4}, {0, 1, 2, 5}, {-1, 0, 3, 0}, {0, -1, 4, 1}, {1, -1, 5, 2} };
		}
	}
}