#pragma once

// Ядро симуляции.
// Управляет полем, инициализацией мира, обновлением сущностей
// и подсчётом статистики.

#include "Config.hpp"
#include "Grid.hpp"

#include <random>
#include <vector>

class SimulationEngine {
public:
    // Создаёт симуляцию на основе конфигурации.
    explicit SimulationEngine(const Config& config);

    // Заполняет мир случайными сущностями.
    void initializeRandom();

    // Выполняет один тик симуляции.
    void step();

    // Возвращает текущее поле.
    const Grid& getGrid() const noexcept;

    // Подсчитывает количество растений.
    int countPlants() const;

    // Подсчитывает количество травоядных.
    int countHerbivores() const;

    // Подсчитывает количество хищников.
    int countPredators() const;

    // Подсчитывает количество препятствий.
    int countObstacles() const;

    // Возвращает номер текущего тика.
    int getTick() const noexcept;

private:
    // Случайно размещает растения.
    void placeRandomPlants(int count);

    // Случайно размещает травоядных.
    void placeRandomHerbivores(int count);

    // Случайно размещает хищников.
    void placeRandomPredators(int count);

    // Случайно размещает препятствия.
    void placeRandomObstacles(int count);

    // Выращивает новые растения.
    void growPlants();

    // Обновляет всех животных на поле.
    void updateAnimals();

    // Пытается поставить сущность в случайную пустую клетку.
    bool tryPlaceAtRandom(const std::shared_ptr<Entity>& entity);

    // Генерирует случайную позицию на поле.
    Position randomPosition();

    // Ищет соседние клетки с объектами нужного типа.
    std::vector<Position> findNeighborsOfType(Position pos, EntityType type) const;

    // Ищет пустые соседние клетки.
    std::vector<Position> findEmptyNeighbors(Position pos) const;

    // Случайно выбирает одну позицию из списка.
    Position chooseRandomPosition(const std::vector<Position>& positions);

    // Возвращает вероятность роста растений в зависимости от режима среды.
    double currentPlantGrowthProbability() const noexcept;

    // Параллельно считает количество объектов одного типа.
    int countTypeParallel(EntityType type) const;

    // Возвращает фактическое число потоков для расчётов.
    int effectiveThreadCount() const noexcept;

private:
    Config config_;
    Grid grid_;
    std::mt19937 rng_;
    int tick_ = 0;
};