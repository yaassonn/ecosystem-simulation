#pragma once

// Класс графического рендера.
// Отвечает за окно SFML, обработку событий и отрисовку поля.

#include "Grid.hpp"

#include <SFML/Graphics.hpp>
#include <string>

class Renderer {
public:
    // Создаёт окно нужного размера и запоминает размер клетки.
    Renderer(int windowWidth, int windowHeight, int cellSize);

    // Проверяет, открыто ли окно.
    bool isOpen() const;

    // Обрабатывает события окна и клавиатуры.
    void processEvents(
        bool& paused,
        bool& restartRequested,
        bool& saveRequested,
        int& tickDelayMs
    );

    // Рисует текущее состояние поля.
    void render(const Grid& grid, const std::string& titleInfo);

private:
    // Возвращает цвет для каждого типа сущности.
    sf::Color colorFor(EntityType type) const;

private:
    sf::RenderWindow window_;
    int cellSize_;
};