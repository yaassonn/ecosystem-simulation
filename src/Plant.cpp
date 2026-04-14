#include "Plant.hpp"

// Реализация класса растения.

Plant::Plant(Position position)
    : Entity(position) {
}

// Возвращает тип сущности.
EntityType Plant::getType() const noexcept {
    return EntityType::Plant;
}

// Создаёт копию растения.
std::shared_ptr<Entity> Plant::clone() const {
    return std::make_shared<Plant>(*this);
}