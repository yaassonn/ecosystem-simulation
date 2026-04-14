#include "Obstacle.hpp"

// Реализация класса препятствия.

Obstacle::Obstacle(Position position)
    : Entity(position) {
}

// Возвращает тип сущности.
EntityType Obstacle::getType() const noexcept {
    return EntityType::Obstacle;
}

// Создаёт копию препятствия.
std::shared_ptr<Entity> Obstacle::clone() const {
    return std::make_shared<Obstacle>(*this);
}