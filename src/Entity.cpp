#include "Entity.hpp"

// Реализация базового класса всех сущностей.

Entity::Entity(Position position)
    : position_(position) {
}

// Возвращает текущую позицию сущности.
Position Entity::getPosition() const noexcept {
    return position_;
}

// Меняет позицию сущности.
void Entity::setPosition(Position position) noexcept {
    position_ = position;
}

// Проверяет, жива ли сущность.
bool Entity::isAlive() const noexcept {
    return alive_;
}

// Помечает сущность как мёртвую.
void Entity::kill() noexcept {
    alive_ = false;
}