#pragma once

// Базовый класс для всех животных в симуляции.
// Хранит общие параметры: энергию, возраст и ограничения по ним.

#include "Entity.hpp"

class Animal : public Entity {
public:
    // Создаёт животное с заданной позицией, стартовой энергией
    // и ограничениями по энергии и возрасту.
    Animal(Position position, int startEnergy, int maxEnergy, int maxAge);

    // Возвращает текущую энергию.
    int getEnergy() const noexcept;

    // Возвращает максимально возможную энергию.
    int getMaxEnergy() const noexcept;

    // Возвращает текущий возраст.
    int getAge() const noexcept;

    // Возвращает предельный возраст.
    int getMaxAge() const noexcept;

    // Увеличивает энергию, не выходя за максимум.
    void addEnergy(int value) noexcept;

    // Уменьшает энергию на указанное значение.
    void spendEnergy(int value) noexcept;

    // Увеличивает возраст на один тик.
    void incrementAge() noexcept;

    // Проверяет, хватает ли энергии для размножения.
    bool canReproduce(int threshold) const noexcept;

    // Проверяет, погибло ли животное от нехватки энергии.
    bool isDeadByEnergy() const noexcept;

    // Проверяет, погибло ли животное от старости.
    bool isDeadByAge() const noexcept;

protected:
    int energy_;     // текущая энергия
    int maxEnergy_;  // максимальная энергия
    int age_;        // текущий возраст
    int maxAge_;     // максимальный возраст
};