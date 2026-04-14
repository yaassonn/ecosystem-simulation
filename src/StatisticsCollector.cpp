#include "StatisticsCollector.hpp"

#include <fstream>

// Реализация сборщика статистики.

void StatisticsCollector::add(const SimulationStats& stats) {
    data_.push_back(stats);
}

// Возвращает все сохранённые записи статистики.
const std::vector<SimulationStats>& StatisticsCollector::getAll() const noexcept {
    return data_;
}

// Сохраняет статистику по тикам в CSV-файл.
bool StatisticsCollector::saveCsv(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        return false;
    }

    // Заголовок таблицы.
    out << "tick,plants,herbivores,predators,obstacles\n";

    // Записываем все накопленные строки статистики.
    for (const auto& row : data_) {
        out << row.tick << ','
            << row.plants << ','
            << row.herbivores << ','
            << row.predators << ','
            << row.obstacles << '\n';
    }

    return true;
}