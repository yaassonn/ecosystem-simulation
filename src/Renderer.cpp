#include "Renderer.hpp"

#include <algorithm>
#include <cassert>

// Реализация графического рендера на SFML.

Renderer::Renderer(int windowWidth, int windowHeight, int cellSize)
    : window_(sf::VideoMode(static_cast<unsigned int>(windowWidth),
                            static_cast<unsigned int>(windowHeight)),
              "Ecosystem Simulation"),
      cellSize_(cellSize) {
    assert(windowWidth > 0);
    assert(windowHeight > 0);
    assert(cellSize > 0);

    window_.setFramerateLimit(60);
}

// Проверяет, открыто ли окно.
bool Renderer::isOpen() const {
    return window_.isOpen();
}

// Обрабатывает события окна и нажатия клавиш.
void Renderer::processEvents(
    bool& paused,
    bool& restartRequested,
    bool& saveRequested,
    int& tickDelayMs
) {
    assert(tickDelayMs > 0);

    sf::Event event{};
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                paused = !paused;
            }

            if (event.key.code == sf::Keyboard::R) {
                restartRequested = true;
            }

            if (event.key.code == sf::Keyboard::S) {
                saveRequested = true;
            }

            if (event.key.code == sf::Keyboard::Up) {
                tickDelayMs = std::max(1, tickDelayMs - 10);
            }

            if (event.key.code == sf::Keyboard::Down) {
                tickDelayMs += 10;
            }
        }
    }
}

// Возвращает цвет для каждого типа сущности.
sf::Color Renderer::colorFor(EntityType type) const {
    switch (type) {
    case EntityType::Empty:
        return sf::Color(20, 20, 20);
    case EntityType::Plant:
        return sf::Color(40, 180, 40);
    case EntityType::Herbivore:
        return sf::Color(70, 130, 255);
    case EntityType::Predator:
        return sf::Color(220, 60, 60);
    case EntityType::Obstacle:
        return sf::Color(120, 120, 120);
    default:
        return sf::Color::Magenta;
    }
}

// Отрисовывает текущее состояние поля в окне.
void Renderer::render(const Grid& grid, const std::string& titleInfo) {
    window_.setTitle("Ecosystem Simulation | " + titleInfo);
    window_.clear(sf::Color::Black);

    sf::RectangleShape cellShape;
    cellShape.setSize(sf::Vector2f(
        static_cast<float>(cellSize_ - 1),
        static_cast<float>(cellSize_ - 1)));

    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            const EntityType type = grid.at(Position{x, y}).getType();

            cellShape.setPosition(
                static_cast<float>(x * cellSize_),
                static_cast<float>(y * cellSize_));
            cellShape.setFillColor(colorFor(type));

            window_.draw(cellShape);
        }
    }

    window_.display();
}