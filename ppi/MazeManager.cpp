#include "MazeManager.h"
#include<stack>
#include<algorithm>
#include<random>
#include<stdio.h>
MazeManager::MazeManager(int width, int height, float Size, sf::Vector2u windowSize,CellType type) : width(width), height(height), type(type) {
    sf::Vector2f offset;
    float totalW = (type == CellType::square) ? width * Size : width * (Size * 0.8);
    float totalH = (type == CellType::square) ? height * Size : height * (Size * 0.7);
    offset.x = (windowSize.x - totalW) / 2.f;
    offset.y = (windowSize.y - totalH) / 2.f;
    maze.reserve(height);
    for (int y = 0; y < height; ++y) {
        std::vector<cell> row;
        row.reserve(width);
        for (int x = 0; x < width; ++x) {
            row.emplace_back(x, y, Size, offset, type);
        }
        maze.emplace_back(std::move(row));
    }
}

void MazeManager::draw(sf::RenderWindow& window) {
	for (auto& row : maze) {
		for (auto& cell : row) {
			cell.draw(window);
		}
	}
}

void MazeManager::generateDFS() {
    std::random_device rando;
    std::mt19937 g(rando());
    std::stack<std::pair<int, int>> stack;

    stack.push({ 0, 0 });
    maze[0][0].setvisited();

    while (!stack.empty()) {
        auto [cx, cy] = stack.top();

        auto currentDirs = maze[cy][cx].getDirections();
        std::vector<Direction> neighbors;

        for (const auto& d : currentDirs) {
            int nx = cx + d.dx, ny = cy + d.dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && !maze[ny][nx].getvisited()) {
                neighbors.push_back(d);
            }
        }

        if (!neighbors.empty()) {
            std::shuffle(neighbors.begin(), neighbors.end(), g);
            const auto& chosen = neighbors[0];
            int nx = cx + chosen.dx, ny = cy + chosen.dy;

            maze[cy][cx].removeWall(chosen.wall);
            maze[ny][nx].removeWall(chosen.opposite);
            maze[ny][nx].setvisited();
            stack.push({ nx, ny });
        }
        else {
            stack.pop();
        }
    }
}
void MazeManager::braidmaze(int chance) {
    float forrando = chance / 100.0f;
    std::random_device rando;
    std::mt19937 g(rando());
    std::uniform_real_distribution<float> distr(0.0f, 1.0f);

    int deadEnd = (type == CellType::square) ? 3 : 5;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (maze[y][x].wallCount() == deadEnd) {
                if (distr(g) <= forrando) {
                    removeOneRandWall(x, y);
                }
            }
        }
    }
}
void MazeManager::removeOneRandWall(int x, int y) {
    auto currentDirs = maze[y][x].getDirections();
    std::vector<Direction> available;
    for (const auto& d : currentDirs) {
        int nx = x + d.dx, ny = y + d.dy;
        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            if (maze[y][x].checkWall(d.wall)) {
                available.push_back(d);
            }
        }
    }

    if (!available.empty()) {
        std::random_device rando;
        std::mt19937 g(rando());
        std::shuffle(available.begin(), available.end(), g);
        const auto& chosen = available[0];

        maze[y][x].removeWall(chosen.wall);
        maze[y + chosen.dy][x + chosen.dx].removeWall(chosen.opposite);
    }
}
void::MazeManager::handleMouse(sf::Vector2f mousePos) {
    int targetX = -1, targetY = -1;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (maze[y][x].getBounds().contains(mousePos)) {
                targetX = x;
                targetY = y;
                break;
            }
        }
        if (targetX != -1) break;
    }
    if (targetX == -1) return;
    int currY = player.first;
    int currX = player.second;
    bool canMove = false;
    auto currentDirs = maze[player.first][player.second].getDirections();
    for (const auto& d : currentDirs) {
        if (currX + d.dx == targetX && currY + d.dy == targetY) {
            if (!maze[currY][currX].checkWall(d.wall)) {
                canMove = true;
            }
            break;
       }
    }
    if (canMove) {
        if (currY == 0 && currX == 0) {
            maze[currY][currX].setFillColor(sf::Color::Green);
        }
        else {
            maze[currY][currX].setFillColor(sf::Color(100, 100, 100));
        }
        player.first = targetY;
        player.second = targetX;
        if (player.first == height - 1 && player.second == width - 1) {
            maze[player.first][player.second].setFillColor(sf::Color::White);
            printf("doszedles do mety\n");
        }
        else {
            maze[player.first][player.second].setFillColor(sf::Color::Blue);
        }
    }
}