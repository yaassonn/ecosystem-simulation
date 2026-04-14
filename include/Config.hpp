#pragma once

// Конфигурация симуляции.
// Здесь хранятся все основные параметры мира, агентов и запуска.

#include <string>

class Config {
public:
    int windowWidth = 1200;
    int windowHeight = 900;

    int gridWidth = 120;
    int gridHeight = 90;
    int cellSize = 8;

    int initialPlants = 1500;
    int initialHerbivores = 120;
    int initialPredators = 40;
    int initialObstacles = 500;

    double plantGrowthProbability = 0.03;

    int herbivoreMaxEnergy = 20;
    int herbivoreStartEnergy = 12;
    int herbivoreMoveCost = 1;
    int herbivoreIdleCost = 1;
    int herbivoreFoodGain = 8;
    int herbivoreReproduceThreshold = 16;
    int herbivoreMaxAge = 120;

    int predatorMaxEnergy = 24;
    int predatorStartEnergy = 14;
    int predatorMoveCost = 1;
    int predatorIdleCost = 1;
    int predatorFoodGain = 10;
    int predatorReproduceThreshold = 18;
    int predatorMaxAge = 140;

    int tickDelayMs = 60;
    int threadCount = 4;
    unsigned int randomSeed = 42;

    std::string statsOutput = "simulation_stats.csv";
    int ticksToRun = 50;
    int renderEveryNTicks = 10;

    std::string environmentMode = "normal";

    // Загружает параметры из файла config.ini.
    bool loadFromFile(const std::string& filename);

    // Проверяет корректность параметров после загрузки.
    bool validate(std::string& errorMessage) const;

private:
    // Убирает пробелы в начале и конце строки.
    static std::string trim(const std::string& value);
};