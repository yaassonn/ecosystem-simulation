#pragma once

// Класс игрового поля.
// Хранит двумерный массив клеток и даёт доступ к ним.

#include "Cell.hpp"
#include "Types.hpp"

#include <memory>
#include <vector>

class Entity;

class Grid {
public:
    // Создаёт поле заданного размера.
    Grid(int width, int height);

    // Возвращает ширину поля.
    int getWidth() const noexcept;

    // Возвращает высоту поля.
    int getHeight() const noexcept;

    // Проверяет, лежит ли позиция внутри поля.
    bool isInside(Position pos) const noexcept;

    // Приводит координаты к тороидальной форме.
    Position wrapPosition(Position pos) const noexcept;

    // Возвращает клетку по позиции.
    Cell& at(Position pos);

    // Возвращает константную клетку по позиции.
    const Cell& at(Position pos) const;

    // Проверяет, пуста ли клетка.
    bool isEmpty(Position pos) const;

    // Очищает клетку.
    void clearCell(Position pos);

    // Помещает сущность на поле.
    void placeEntity(const std::shared_ptr<Entity>& entity);

    // Возвращает соседей по окрестности Мура.
    std::vector<Position> getMooreNeighbors(Position pos) const;

    // Полностью очищает поле.
    void clear();

private:
    int width_;
    int height_;
    std::vector<std::vector<Cell>> cells_;
};