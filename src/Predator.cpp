#include "Predator.hpp"

// Реализация класса хищника.

Predator::Predator(Position position, int startEnergy, int maxEnergy, int maxAge)
    : Animal(position, startEnergy, maxEnergy, maxAge) {
}

// Возвращает тип сущности.
EntityType Predator::getType() const noexcept {
    return EntityType::Predator;
}

// Создаёт копию хищника.
std::shared_ptr<Entity> Predator::clone() const {
    return std::make_shared<Predator>(*this);
}