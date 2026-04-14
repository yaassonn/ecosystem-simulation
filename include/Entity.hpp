#pragma once

// Базовый класс для всех объектов, которые могут находиться на поле.

#include "Types.hpp"

#include <memory>

class Entity {
public:
    // Создаёт сущность в заданной позиции.
    explicit Entity(Position position);

    virtual ~Entity() = default;

    // Возвращает текущую позицию сущности.
    Position getPosition() const noexcept;

    // Меняет позицию сущности.
    void setPosition(Position position) noexcept;

    // Проверяет, жива ли сущность.
    bool isAlive() const noexcept;

    // Помечает сущность как мёртвую.
    void kill() noexcept;

    // Возвращает тип сущности.
    virtual EntityType getType() const noexcept = 0;

    // Создаёт копию сущности.
    virtual std::shared_ptr<Entity> clone() const = 0;

protected:
    Position position_;  // текущая позиция
    bool alive_ = true;  // флаг жизненного состояния
};