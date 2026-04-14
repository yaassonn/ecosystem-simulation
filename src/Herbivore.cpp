#include "Herbivore.hpp"

// Реализация класса травоядного.

Herbivore::Herbivore(Position position, int startEnergy, int maxEnergy, int maxAge)
    : Animal(position, startEnergy, maxEnergy, maxAge) {
}

// Возвращает тип сущности.
EntityType Herbivore::getType() const noexcept {
    return EntityType::Herbivore;
}

// Создаёт копию травоядного.
std::shared_ptr<Entity> Herbivore::clone() const {
    return std::make_shared<Herbivore>(*this);
}