#pragma once

// Общие типы, которые используются во всём проекте.

#include <cstdint>

// Тип объекта, который может находиться в клетке поля.
enum class EntityType : std::uint8_t {
    Empty = 0,  // пустая клетка
    Plant,      // растение
    Herbivore,  // травоядное
    Predator,   // хищник
    Obstacle    // препятствие
};

// Координаты клетки на двумерном поле.
struct Position {
    int x = 0;  // координата по X
    int y = 0;  // координата по Y

    // Сравнение двух позиций на равенство.
    bool operator==(const Position& other) const noexcept {
        return x == other.x && y == other.y;
    }

    // Сравнение двух позиций на неравенство.
    bool operator!=(const Position& other) const noexcept {
        return !(*this == other);
    }
};