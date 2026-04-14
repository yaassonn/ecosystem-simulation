#include "ConsoleRenderer.hpp"

#include <iostream>

// Выводит поле в консоль в упрощённом текстовом виде.
void ConsoleRenderer::render(const Grid& grid, int maxWidth, int maxHeight) {
    const int gridWidth = grid.getWidth();
    const int gridHeight = grid.getHeight();

    // Если поле слишком большое, уменьшаем детализацию вывода.
    const int stepX = (gridWidth > maxWidth) ? (gridWidth / maxWidth) : 1;
    const int stepY = (gridHeight > maxHeight) ? (gridHeight / maxHeight) : 1;

    for (int y = 0; y < gridHeight; y += stepY) {
        for (int x = 0; x < gridWidth; x += stepX) {
            const EntityType type = grid.at(Position{x, y}).getType();

            char symbol = '.';

            // Подбираем символ в зависимости от типа объекта.
            switch (type) {
            case EntityType::Empty:
                symbol = '.';
                break;
            case EntityType::Plant:
                symbol = '*';
                break;
            case EntityType::Herbivore:
                symbol = 'H';
                break;
            case EntityType::Predator:
                symbol = 'P';
                break;
            case EntityType::Obstacle:
                symbol = '#';
                break;
            }

            std::cout << symbol;
        }

        std::cout << '\n';
    }
}