#pragma once

// Класс растения.
// Растения служат источником пищи для травоядных.

#include "Entity.hpp"

class Plant : public Entity {
public:
    // Создаёт растение в заданной позиции.
    explicit Plant(Position position);

    // Возвращает тип сущности.
    EntityType getType() const noexcept override;

    // Создаёт копию растения.
    std::shared_ptr<Entity> clone() const override;
};