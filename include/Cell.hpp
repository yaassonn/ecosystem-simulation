#pragma once

// Клетка поля. Может быть пустой или содержать одну сущность.

#include "Types.hpp"

#include <memory>

class Entity;

class Cell {
public:
    Cell() = default;

    // Проверяет, пуста ли клетка.
    bool isEmpty() const noexcept;

    // Очищает клетку.
    void clear() noexcept;

    // Возвращает тип объекта в клетке.
    EntityType getType() const noexcept;

    // Возвращает указатель на объект в клетке.
    const std::shared_ptr<Entity>& getOccupant() const noexcept;

    // Помещает объект в клетку.
    void setOccupant(std::shared_ptr<Entity> entity);

private:
    std::shared_ptr<Entity> occupant_;
};