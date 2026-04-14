#pragma once

// Класс препятствия.
// Такие объекты занимают клетки поля и мешают передвижению животных.

#include "Entity.hpp"

class Obstacle : public Entity {
public:
    // Создаёт препятствие в заданной позиции.
    explicit Obstacle(Position position);

    // Возвращает тип сущности.
    EntityType getType() const noexcept override;

    // Создаёт копию препятствия.
    std::shared_ptr<Entity> clone() const override;
};