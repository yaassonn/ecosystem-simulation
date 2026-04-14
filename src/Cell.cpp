#include "Cell.hpp"
#include "Entity.hpp"

// Реализация клетки поля.

bool Cell::isEmpty() const noexcept {
    return occupant_ == nullptr;
}

// Полностью очищает клетку.
void Cell::clear() noexcept {
    occupant_.reset();
}

// Возвращает тип объекта в клетке.
// Если клетка пуста, возвращается Empty.
EntityType Cell::getType() const noexcept {
    if (!occupant_) {
        return EntityType::Empty;
    }
    return occupant_->getType();
}

// Возвращает указатель на объект в клетке.
const std::shared_ptr<Entity>& Cell::getOccupant() const noexcept {
    return occupant_;
}

// Помещает новый объект в клетку.
void Cell::setOccupant(std::shared_ptr<Entity> entity) {
    occupant_ = std::move(entity);
}