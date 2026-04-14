#pragma once

// Класс хищника.
// Наследуется от Animal и охотится на травоядных.

#include "Animal.hpp"

class Predator : public Animal {
public:
    // Создаёт хищника с заданными параметрами.
    Predator(Position position, int startEnergy, int maxEnergy, int maxAge);

    // Возвращает тип сущности.
    EntityType getType() const noexcept override;

    // Создаёт копию хищника.
    std::shared_ptr<Entity> clone() const override;
};