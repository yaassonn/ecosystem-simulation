#pragma once

// Сборщик статистики симуляции.
// Хранит данные по тикам и умеет сохранять их в CSV.

#include <string>
#include <vector>

// Статистика за один тик симуляции.
struct SimulationStats {
    int tick = 0;        // номер тика
    int plants = 0;      // число растений
    int herbivores = 0;  // число травоядных
    int predators = 0;   // число хищников
    int obstacles = 0;   // число препятствий
};

class StatisticsCollector {
public:
    // Добавляет новую запись статистики.
    void add(const SimulationStats& stats);

    // Возвращает все накопленные записи.
    const std::vector<SimulationStats>& getAll() const noexcept;

    // Сохраняет статистику в CSV-файл.
    bool saveCsv(const std::string& filename) const;

private:
    std::vector<SimulationStats> data_;
};