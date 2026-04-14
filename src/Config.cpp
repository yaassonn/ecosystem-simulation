#include "Config.hpp"

#include <cctype>
#include <fstream>
#include <string>
#include <unordered_map>

// Убирает пробелы в начале и конце строки.
std::string Config::trim(const std::string& value) {
    std::size_t start = 0;
    while (start < value.size() &&
           std::isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }

    std::size_t end = value.size();
    while (end > start &&
           std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }

    return value.substr(start, end - start);
}

// Загружает параметры из ini-файла.
bool Config::loadFromFile(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        return false;
    }

    std::unordered_map<std::string, std::string> values;
    std::string line;

    while (std::getline(input, line)) {
        line = trim(line);

        // Пропускаем пустые строки и комментарии.
        if (line.empty() || line[0] == '#') {
            continue;
        }

        const std::size_t pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }

        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));
        values[key] = value;
    }

    auto getInt = [&values](const std::string& key, int& field) {
        auto it = values.find(key);
        if (it != values.end()) {
            field = std::stoi(it->second);
        }
    };

    auto getUInt = [&values](const std::string& key, unsigned int& field) {
        auto it = values.find(key);
        if (it != values.end()) {
            field = static_cast<unsigned int>(std::stoul(it->second));
        }
    };

    auto getDouble = [&values](const std::string& key, double& field) {
        auto it = values.find(key);
        if (it != values.end()) {
            field = std::stod(it->second);
        }
    };

    auto getString = [&values](const std::string& key, std::string& field) {
        auto it = values.find(key);
        if (it != values.end()) {
            field = it->second;
        }
    };

    // Загружаем параметры окна и поля.
    getInt("window_width", windowWidth);
    getInt("window_height", windowHeight);
    getInt("grid_width", gridWidth);
    getInt("grid_height", gridHeight);
    getInt("cell_size", cellSize);

    // Загружаем стартовое количество объектов.
    getInt("initial_plants", initialPlants);
    getInt("initial_herbivores", initialHerbivores);
    getInt("initial_predators", initialPredators);
    getInt("initial_obstacles", initialObstacles);

    // Загружаем параметры роста растений.
    getDouble("plant_growth_probability", plantGrowthProbability);

    // Загружаем параметры травоядных.
    getInt("herbivore_max_energy", herbivoreMaxEnergy);
    getInt("herbivore_start_energy", herbivoreStartEnergy);
    getInt("herbivore_move_cost", herbivoreMoveCost);
    getInt("herbivore_idle_cost", herbivoreIdleCost);
    getInt("herbivore_food_gain", herbivoreFoodGain);
    getInt("herbivore_reproduce_threshold", herbivoreReproduceThreshold);
    getInt("herbivore_max_age", herbivoreMaxAge);

    // Загружаем параметры хищников.
    getInt("predator_max_energy", predatorMaxEnergy);
    getInt("predator_start_energy", predatorStartEnergy);
    getInt("predator_move_cost", predatorMoveCost);
    getInt("predator_idle_cost", predatorIdleCost);
    getInt("predator_food_gain", predatorFoodGain);
    getInt("predator_reproduce_threshold", predatorReproduceThreshold);
    getInt("predator_max_age", predatorMaxAge);

    // Загружаем параметры запуска.
    getInt("tick_delay_ms", tickDelayMs);
    getInt("thread_count", threadCount);
    getUInt("random_seed", randomSeed);

    // Загружаем параметры вывода статистики.
    getString("stats_output", statsOutput);
    getInt("ticks_to_run", ticksToRun);
    getInt("render_every_n_ticks", renderEveryNTicks);

    // Загружаем режим среды.
    getString("environment_mode", environmentMode);

    return true;
}

// Проверяет корректность загруженной конфигурации.
bool Config::validate(std::string& errorMessage) const {
    if (windowWidth <= 0 || windowHeight <= 0) {
        errorMessage = "Window size must be positive.";
        return false;
    }

    if (gridWidth <= 0 || gridHeight <= 0) {
        errorMessage = "Grid size must be positive.";
        return false;
    }

    if (cellSize <= 0) {
        errorMessage = "Cell size must be positive.";
        return false;
    }

    if (initialPlants < 0 || initialHerbivores < 0 ||
        initialPredators < 0 || initialObstacles < 0) {
        errorMessage = "Initial entity counts must be non-negative.";
        return false;
    }

    if (plantGrowthProbability < 0.0 || plantGrowthProbability > 1.0) {
        errorMessage = "plant_growth_probability must be in [0, 1].";
        return false;
    }

    if (herbivoreMaxEnergy <= 0 || herbivoreStartEnergy <= 0 ||
        herbivoreMoveCost < 0 || herbivoreIdleCost < 0 ||
        herbivoreFoodGain < 0 || herbivoreReproduceThreshold <= 0 ||
        herbivoreMaxAge <= 0) {
        errorMessage = "Invalid herbivore parameters.";
        return false;
    }

    if (predatorMaxEnergy <= 0 || predatorStartEnergy <= 0 ||
        predatorMoveCost < 0 || predatorIdleCost < 0 ||
        predatorFoodGain < 0 || predatorReproduceThreshold <= 0 ||
        predatorMaxAge <= 0) {
        errorMessage = "Invalid predator parameters.";
        return false;
    }

    if (tickDelayMs <= 0) {
        errorMessage = "tick_delay_ms must be positive.";
        return false;
    }

    if (threadCount <= 0) {
        errorMessage = "thread_count must be positive.";
        return false;
    }

    if (ticksToRun <= 0) {
        errorMessage = "ticks_to_run must be positive.";
        return false;
    }

    if (renderEveryNTicks < 0) {
        errorMessage = "render_every_n_ticks must be non-negative.";
        return false;
    }

    if (environmentMode != "normal" &&
        environmentMode != "drought" &&
        environmentMode != "overgrowth") {
        errorMessage = "environment_mode must be normal, drought or overgrowth.";
        return false;
    }

    if (statsOutput.empty()) {
        errorMessage = "stats_output must not be empty.";
        return false;
    }

    errorMessage.clear();
    return true;
}