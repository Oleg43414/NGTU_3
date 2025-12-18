#include <SFML/Graphics.hpp>
#include <iostream>
#include "cave.h"
using namespace std;

int main() {

    int width, height, birth, death, chance;
    cout << "Width: "; cin >> width;
    cout << "Height: "; cin >> height;
    cout << "Birth limit: "; cin >> birth;
    cout << "Death limit: "; cin >> death;
    cout << "Chance to be alive (0-100): "; cin >> chance;

    CaveConfig config(width, height, birth, death, chance);
    Cave cave(config);
    cave.randomInitialize();

    int cellSize = max(1, min(50, min(600/width, 600/height)));
    
    int winWidth = max(width * cellSize, 300);
    int winHeight = max(height * cellSize, 300);

    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Cave Generator (step by step)");
    bool needStep = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                needStep = true;
        }
        if (needStep) {
            cave.doSimulationStep();
            needStep = false;
        }
        window.clear(sf::Color::White);
        const auto& grid = cave.getGrid();
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j) {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
                cell.setPosition(j * cellSize, i * cellSize);
                cell.setFillColor(grid[i][j] ? sf::Color::Black : sf::Color::White);
                window.draw(cell);
            }
        window.display();
    }
}
