#include "Animal.hpp"

#include <algorithm>

// Реализация общего поведения для всех животных.

Animal::Animal(Position position, int startEnergy, int maxEnergy, int maxAge)
    : Entity(position),
      energy_(startEnergy),
      maxEnergy_(maxEnergy),
      age_(0),
      maxAge_(maxAge) {
}

// Возвращает текущую энергию.
int Animal::getEnergy() const noexcept {
    return energy_;
}

// Возвращает максимальную энергию.
int Animal::getMaxEnergy() const noexcept {
    return maxEnergy_;
}

// Возвращает текущий возраст.
int Animal::getAge() const noexcept {
    return age_;
}

// Возвращает максимальный возраст.
int Animal::getMaxAge() const noexcept {
    return maxAge_;
}

// Увеличивает энергию, но не выше допустимого максимума.
void Animal::addEnergy(int value) noexcept {
    energy_ = std::min(maxEnergy_, energy_ + value);
}

// Уменьшает энергию.
void Animal::spendEnergy(int value) noexcept {
    energy_ -= value;
}

// Увеличивает возраст на один тик.
void Animal::incrementAge() noexcept {
    ++age_;
}

// Проверяет, хватает ли энергии для размножения.
bool Animal::canReproduce(int threshold) const noexcept {
    return energy_ >= threshold;
}

// Проверяет, умерло ли животное от голода.
bool Animal::isDeadByEnergy() const noexcept {
    return energy_ <= 0;
}

// Проверяет, умерло ли животное от старости.
bool Animal::isDeadByAge() const noexcept {
    return age_ >= maxAge_;
}