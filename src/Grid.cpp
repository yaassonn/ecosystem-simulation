#include "Grid.hpp"
#include "Entity.hpp"

#include <cassert>
#include <stdexcept>

// Реализация игрового поля и операций с клетками.

Grid::Grid(int width, int height)
    : width_(width), height_(height), cells_(height, std::vector<Cell>(width)) {
    if (width_ <= 0 || height_ <= 0) {
        throw std::invalid_argument("Grid dimensions must be positive");
    }
}

// Возвращает ширину поля.
int Grid::getWidth() const noexcept {
    return width_;
}

// Возвращает высоту поля.
int Grid::getHeight() const noexcept {
    return height_;
}

// Проверяет, лежит ли позиция внутри поля.
bool Grid::isInside(Position pos) const noexcept {
    return pos.x >= 0 && pos.x < width_ && pos.y >= 0 && pos.y < height_;
}

// Преобразует координаты с учётом тороидального замыкания.
Position Grid::wrapPosition(Position pos) const noexcept {
    assert(width_ > 0);
    assert(height_ > 0);

    Position wrapped = pos;
    wrapped.x = ((wrapped.x % width_) + width_) % width_;
    wrapped.y = ((wrapped.y % height_) + height_) % height_;

    return wrapped;
}

// Возвращает изменяемую ссылку на клетку по позиции.
Cell& Grid::at(Position pos) {
    Position wrapped = wrapPosition(pos);
    return cells_[wrapped.y][wrapped.x];
}

// Возвращает константную ссылку на клетку по позиции.
const Cell& Grid::at(Position pos) const {
    Position wrapped = wrapPosition(pos);
    return cells_[wrapped.y][wrapped.x];
}

// Проверяет, пуста ли клетка.
bool Grid::isEmpty(Position pos) const {
    return at(pos).isEmpty();
}

// Очищает клетку по заданной позиции.
void Grid::clearCell(Position pos) {
    at(pos).clear();
}

// Помещает сущность на поле.
void Grid::placeEntity(const std::shared_ptr<Entity>& entity) {
    if (!entity) {
        throw std::invalid_argument("Cannot place null entity");
    }

    Position pos = wrapPosition(entity->getPosition());
    entity->setPosition(pos);
    at(pos).setOccupant(entity);
}

// Возвращает соседние клетки по окрестности Мура.
std::vector<Position> Grid::getMooreNeighbors(Position pos) const {
    std::vector<Position> neighbors;
    neighbors.reserve(8);

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            neighbors.push_back(wrapPosition(Position{pos.x + dx, pos.y + dy}));
        }
    }

    return neighbors;
}

// Полностью очищает всё поле.
void Grid::clear() {
    for (auto& row : cells_) {
        for (auto& cell : row) {
            cell.clear();
        }
    }
}