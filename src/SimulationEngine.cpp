#include "SimulationEngine.hpp"

#include "Animal.hpp"
#include "Herbivore.hpp"
#include "Obstacle.hpp"
#include "Plant.hpp"
#include "Predator.hpp"

#include <algorithm>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

// Реализация ядра симуляции.

SimulationEngine::SimulationEngine(const Config& config)
    : config_(config),
      grid_(config.gridWidth, config.gridHeight),
      rng_(config.randomSeed),
      tick_(0) {
}

// Возвращает текущее поле.
const Grid& SimulationEngine::getGrid() const noexcept {
    return grid_;
}

// Возвращает номер текущего тика.
int SimulationEngine::getTick() const noexcept {
    return tick_;
}

// Возвращает реальное число потоков, которое можно использовать.
int SimulationEngine::effectiveThreadCount() const noexcept {
    int requested = (config_.threadCount > 0) ? config_.threadCount : 1;
    int limitedByHeight = std::max(1, grid_.getHeight());
    return std::max(1, std::min(requested, limitedByHeight));
}

// Генерирует случайную позицию на поле.
Position SimulationEngine::randomPosition() {
    std::uniform_int_distribution<int> xDist(0, grid_.getWidth() - 1);
    std::uniform_int_distribution<int> yDist(0, grid_.getHeight() - 1);
    return Position{xDist(rng_), yDist(rng_)};
}

// Пытается поставить сущность в случайную пустую клетку.
bool SimulationEngine::tryPlaceAtRandom(const std::shared_ptr<Entity>& entity) {
    for (int attempt = 0; attempt < 2000; ++attempt) {
        Position pos = randomPosition();
        if (grid_.isEmpty(pos)) {
            entity->setPosition(pos);
            grid_.placeEntity(entity);
            return true;
        }
    }
    return false;
}

// Случайно размещает растения.
void SimulationEngine::placeRandomPlants(int count) {
    for (int i = 0; i < count; ++i) {
        auto plant = std::make_shared<Plant>(Position{0, 0});
        tryPlaceAtRandom(plant);
    }
}

// Случайно размещает травоядных.
void SimulationEngine::placeRandomHerbivores(int count) {
    for (int i = 0; i < count; ++i) {
        auto herbivore = std::make_shared<Herbivore>(
            Position{0, 0},
            config_.herbivoreStartEnergy,
            config_.herbivoreMaxEnergy,
            config_.herbivoreMaxAge
        );
        tryPlaceAtRandom(herbivore);
    }
}

// Случайно размещает хищников.
void SimulationEngine::placeRandomPredators(int count) {
    for (int i = 0; i < count; ++i) {
        auto predator = std::make_shared<Predator>(
            Position{0, 0},
            config_.predatorStartEnergy,
            config_.predatorMaxEnergy,
            config_.predatorMaxAge
        );
        tryPlaceAtRandom(predator);
    }
}

// Случайно размещает препятствия.
void SimulationEngine::placeRandomObstacles(int count) {
    for (int i = 0; i < count; ++i) {
        auto obstacle = std::make_shared<Obstacle>(Position{0, 0});
        tryPlaceAtRandom(obstacle);
    }
}

// Полностью инициализирует новый случайный мир.
void SimulationEngine::initializeRandom() {
    grid_.clear();
    tick_ = 0;

    placeRandomObstacles(config_.initialObstacles);
    placeRandomPlants(config_.initialPlants);
    placeRandomHerbivores(config_.initialHerbivores);
    placeRandomPredators(config_.initialPredators);
}

// Ищет соседние клетки с объектом нужного типа.
std::vector<Position> SimulationEngine::findNeighborsOfType(Position pos, EntityType type) const {
    std::vector<Position> result;
    std::vector<Position> neighbors = grid_.getMooreNeighbors(pos);

    for (const auto& neighbor : neighbors) {
        if (grid_.at(neighbor).getType() == type) {
            result.push_back(neighbor);
        }
    }

    return result;
}

// Ищет пустые соседние клетки.
std::vector<Position> SimulationEngine::findEmptyNeighbors(Position pos) const {
    std::vector<Position> result;
    std::vector<Position> neighbors = grid_.getMooreNeighbors(pos);

    for (const auto& neighbor : neighbors) {
        if (grid_.isEmpty(neighbor)) {
            result.push_back(neighbor);
        }
    }

    return result;
}

// Случайно выбирает одну позицию из списка.
Position SimulationEngine::chooseRandomPosition(const std::vector<Position>& positions) {
    if (positions.empty()) {
        throw std::runtime_error("Cannot choose random position from empty vector");
    }

    std::uniform_int_distribution<std::size_t> dist(0, positions.size() - 1);
    return positions[dist(rng_)];
}

// Возвращает вероятность роста растений для выбранного режима среды.
double SimulationEngine::currentPlantGrowthProbability() const noexcept {
    if (config_.environmentMode == "drought") {
        return config_.plantGrowthProbability * 0.35;
    }
    if (config_.environmentMode == "overgrowth") {
        return std::min(1.0, config_.plantGrowthProbability * 2.0);
    }
    return config_.plantGrowthProbability;
}

// Выращивает новые растения на пустых клетках.
// Рост разбивается по строкам и выполняется в несколько потоков.
void SimulationEngine::growPlants() {
    const double growProbability = currentPlantGrowthProbability();
    const int numThreads = effectiveThreadCount();

    std::vector<std::thread> workers;
    workers.reserve(static_cast<std::size_t>(numThreads));

    const int totalRows = grid_.getHeight();
    const int rowsPerThread = totalRows / numThreads;
    const int extraRows = totalRows % numThreads;

    int startY = 0;
    for (int threadIndex = 0; threadIndex < numThreads; ++threadIndex) {
        const int blockSize = rowsPerThread + (threadIndex < extraRows ? 1 : 0);
        const int endY = startY + blockSize;

        workers.emplace_back([this, growProbability, startY, endY, threadIndex]() {
            std::mt19937 localRng(
                config_.randomSeed +
                static_cast<unsigned int>(tick_ * 1009 + threadIndex * 313)
            );
            std::uniform_real_distribution<double> probability(0.0, 1.0);

            for (int y = startY; y < endY; ++y) {
                for (int x = 0; x < grid_.getWidth(); ++x) {
                    Position pos{x, y};

                    if (grid_.isEmpty(pos) &&
                        probability(localRng) < growProbability) {
                        grid_.placeEntity(std::make_shared<Plant>(pos));
                    }
                }
            }
        });

        startY = endY;
    }

    for (auto& worker : workers) {
        worker.join();
    }
}

// Обновляет состояние всех животных:
// движение, питание, старение, смерть и размножение.
void SimulationEngine::updateAnimals() {
    std::vector<std::shared_ptr<Entity>> survivors;
    std::vector<std::shared_ptr<Entity>> offspring;
    std::vector<Position> processedPositions;

    for (int y = 0; y < grid_.getHeight(); ++y) {
        for (int x = 0; x < grid_.getWidth(); ++x) {
            Position currentPos{x, y};

            if (std::find(processedPositions.begin(), processedPositions.end(), currentPos) != processedPositions.end()) {
                continue;
            }

            Cell& cell = grid_.at(currentPos);
            if (cell.isEmpty()) {
                continue;
            }

            auto occupant = cell.getOccupant();
            if (!occupant) {
                continue;
            }

            EntityType type = occupant->getType();
            if (type != EntityType::Herbivore && type != EntityType::Predator) {
                continue;
            }

            auto animal = std::dynamic_pointer_cast<Animal>(occupant);
            if (!animal) {
                continue;
            }

            Position newPos = currentPos;
            bool moved = false;

            // Логика травоядного: ищем растение, иначе идём в пустую клетку.
            if (type == EntityType::Herbivore) {
                auto foodCells = findNeighborsOfType(currentPos, EntityType::Plant);

                if (!foodCells.empty()) {
                    Position target = chooseRandomPosition(foodCells);
                    grid_.clearCell(target);
                    animal->addEnergy(config_.herbivoreFoodGain);
                    newPos = target;
                    moved = true;
                } else {
                    auto emptyCells = findEmptyNeighbors(currentPos);
                    if (!emptyCells.empty()) {
                        newPos = chooseRandomPosition(emptyCells);
                        moved = true;
                    }
                }

                animal->spendEnergy(moved ? config_.herbivoreMoveCost : config_.herbivoreIdleCost);
            } else {
                // Логика хищника: ищем травоядное, иначе идём в пустую клетку.
                auto foodCells = findNeighborsOfType(currentPos, EntityType::Herbivore);

                if (!foodCells.empty()) {
                    Position target = chooseRandomPosition(foodCells);
                    grid_.clearCell(target);
                    animal->addEnergy(config_.predatorFoodGain);
                    newPos = target;
                    moved = true;
                } else {
                    auto emptyCells = findEmptyNeighbors(currentPos);
                    if (!emptyCells.empty()) {
                        newPos = chooseRandomPosition(emptyCells);
                        moved = true;
                    }
                }

                animal->spendEnergy(moved ? config_.predatorMoveCost : config_.predatorIdleCost);
            }

            animal->incrementAge();
            grid_.clearCell(currentPos);

            // Удаляем животное, если оно погибло.
            if (animal->isDeadByAge() || animal->isDeadByEnergy()) {
                continue;
            }

            animal->setPosition(newPos);
            survivors.push_back(animal);
            processedPositions.push_back(newPos);

            auto emptyNeighbors = findEmptyNeighbors(newPos);

            // Размножение травоядных.
            if (type == EntityType::Herbivore &&
                animal->canReproduce(config_.herbivoreReproduceThreshold) &&
                !emptyNeighbors.empty()) {

                Position childPos = chooseRandomPosition(emptyNeighbors);
                int childEnergy = animal->getEnergy() / 2;
                animal->spendEnergy(childEnergy);

                auto child = std::make_shared<Herbivore>(
                    childPos,
                    childEnergy,
                    config_.herbivoreMaxEnergy,
                    config_.herbivoreMaxAge
                );
                offspring.push_back(child);
            }

            // Размножение хищников.
            if (type == EntityType::Predator &&
                animal->canReproduce(config_.predatorReproduceThreshold) &&
                !emptyNeighbors.empty()) {

                Position childPos = chooseRandomPosition(emptyNeighbors);
                int childEnergy = animal->getEnergy() / 2;
                animal->spendEnergy(childEnergy);

                auto child = std::make_shared<Predator>(
                    childPos,
                    childEnergy,
                    config_.predatorMaxEnergy,
                    config_.predatorMaxAge
                );
                offspring.push_back(child);
            }
        }
    }

    // Удаляем старые позиции всех животных.
    for (int y = 0; y < grid_.getHeight(); ++y) {
        for (int x = 0; x < grid_.getWidth(); ++x) {
            Position pos{x, y};
            EntityType type = grid_.at(pos).getType();
            if (type == EntityType::Herbivore || type == EntityType::Predator) {
                grid_.clearCell(pos);
            }
        }
    }

    // Ставим выживших животных на новые позиции.
    for (const auto& entity : survivors) {
        if (grid_.isEmpty(entity->getPosition())) {
            grid_.placeEntity(entity);
        }
    }

    // Ставим потомков.
    for (const auto& entity : offspring) {
        if (grid_.isEmpty(entity->getPosition())) {
            grid_.placeEntity(entity);
        }
    }
}

// Выполняет один шаг симуляции.
void SimulationEngine::step() {
    ++tick_;
    updateAnimals();
    growPlants();
}

// Параллельно считает количество сущностей заданного типа.
int SimulationEngine::countTypeParallel(EntityType type) const {
    const int numThreads = effectiveThreadCount();

    std::vector<std::thread> workers;
    workers.reserve(static_cast<std::size_t>(numThreads));

    std::mutex sumMutex;
    int total = 0;

    const int totalRows = grid_.getHeight();
    const int rowsPerThread = totalRows / numThreads;
    const int extraRows = totalRows % numThreads;

    int startY = 0;
    for (int threadIndex = 0; threadIndex < numThreads; ++threadIndex) {
        const int blockSize = rowsPerThread + (threadIndex < extraRows ? 1 : 0);
        const int endY = startY + blockSize;

        workers.emplace_back([this, type, startY, endY, &sumMutex, &total]() {
            int localCount = 0;

            for (int y = startY; y < endY; ++y) {
                for (int x = 0; x < grid_.getWidth(); ++x) {
                    if (grid_.at(Position{x, y}).getType() == type) {
                        ++localCount;
                    }
                }
            }

            std::lock_guard<std::mutex> lock(sumMutex);
            total += localCount;
        });

        startY = endY;
    }

    for (auto& worker : workers) {
        worker.join();
    }

    return total;
}

// Возвращает количество растений.
int SimulationEngine::countPlants() const {
    return countTypeParallel(EntityType::Plant);
}

// Возвращает количество травоядных.
int SimulationEngine::countHerbivores() const {
    return countTypeParallel(EntityType::Herbivore);
}

// Возвращает количество хищников.
int SimulationEngine::countPredators() const {
    return countTypeParallel(EntityType::Predator);
}

// Возвращает количество препятствий.
int SimulationEngine::countObstacles() const {
    return countTypeParallel(EntityType::Obstacle);
}