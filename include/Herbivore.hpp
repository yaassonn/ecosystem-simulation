#pragma once

// Класс травоядного животного.
// Наследуется от Animal и задаёт конкретный тип сущности.

#include "Animal.hpp"

class Herbivore : public Animal {
public:
    // Создаёт травоядное с заданными параметрами.
    Herbivore(Position position, int startEnergy, int maxEnergy, int maxAge);

    // Возвращает тип сущности.
    EntityType getType() const noexcept override;

    // Создаёт копию травоядного.
    std::shared_ptr<Entity> clone() const override;
};