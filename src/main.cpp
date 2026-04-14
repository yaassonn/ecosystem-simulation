#include "Config.hpp"
#include "Renderer.hpp"
#include "SimulationEngine.hpp"
#include "StatisticsCollector.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

// Считывает целое число из заданного диапазона.
// Если ввод пустой или неверный, возвращается значение по умолчанию.
static int readIntInRange(
    const std::string& prompt,
    int left,
    int right,
    int defaultValue
) {
    std::cout << prompt << " [" << left << ".." << right
              << ", Enter = " << defaultValue << "]: ";

    std::string line;
    std::getline(std::cin, line);

    if (line.empty()) {
        return defaultValue;
    }

    try {
        int value = std::stoi(line);
        if (value < left || value > right) {
            std::cout << "Value out of range, default is used.\n";
            return defaultValue;
        }
        return value;
    } catch (...) {
        std::cout << "Invalid number, default is used.\n";
        return defaultValue;
    }
}

// Считывает режим среды из простого текстового меню.
static std::string readEnvironmentMode(const std::string& defaultMode) {
    std::cout << "\nChoose environment mode:\n";
    std::cout << "1 - normal\n";
    std::cout << "2 - drought\n";
    std::cout << "3 - overgrowth\n";
    std::cout << "Enter choice [1..3, Enter = current: " << defaultMode << "]: ";

    std::string line;
    std::getline(std::cin, line);

    if (line.empty()) {
        return defaultMode;
    }

    if (line == "1") {
        return "normal";
    }
    if (line == "2") {
        return "drought";
    }
    if (line == "3") {
        return "overgrowth";
    }

    std::cout << "Invalid choice, current mode is used.\n";
    return defaultMode;
}

// Точка входа в программу.
// Здесь загружается конфигурация, создаются основные объекты,
// запускается окно визуализации и основной цикл симуляции.
int main() {
    Config config;

    // Пытаемся загрузить параметры из файла.
    if (config.loadFromFile("config.ini")) {
        std::cout << "Config loaded from build folder.\n";
    } else {
        std::cout << "Warning: could not load config.ini, default values will be used.\n";
    }

    // Простое консольное меню перед запуском графики.
    std::cout << "\n==== Ecosystem Simulation Menu ====\n";
    config.environmentMode = readEnvironmentMode(config.environmentMode);
    config.threadCount = readIntInRange("Thread count", 1, 32, config.threadCount);
    config.tickDelayMs = readIntInRange("Tick delay (ms)", 1, 1000, config.tickDelayMs);
    config.ticksToRun = readIntInRange("Ticks to run in statistics", 1, 100000, config.ticksToRun);

    // Проверяем, что конфигурация корректна.
    std::string validationError;
    if (!config.validate(validationError)) {
        std::cerr << "Configuration error: " << validationError << '\n';
        return 1;
    }

    // Создаём ядро симуляции, сборщик статистики и рендерер.
    SimulationEngine engine(config);
    StatisticsCollector stats;
    Renderer renderer(
        config.gridWidth * config.cellSize,
        config.gridHeight * config.cellSize,
        config.cellSize
    );

    int currentTickDelayMs = config.tickDelayMs;
    bool paused = false;

    // Создаём начальное состояние мира.
    engine.initializeRandom();

    // Краткая подсказка по управлению.
    std::cout << "\nControls:\n";
    std::cout << "Space - pause/resume\n";
    std::cout << "R - restart simulation\n";
    std::cout << "S - save statistics\n";
    std::cout << "Up/Down - change speed\n\n";

    // Основной цикл приложения.
    while (renderer.isOpen()) {
        bool restartRequested = false;
        bool saveRequested = false;

        // Обрабатываем нажатия клавиш и события окна.
        renderer.processEvents(paused, restartRequested, saveRequested, currentTickDelayMs);

        // По запросу перезапускаем симуляцию.
        if (restartRequested) {
            engine.initializeRandom();
            stats = StatisticsCollector();
            paused = false;
            std::cout << "Simulation restarted.\n";
        }

        // По запросу сохраняем статистику в файл.
        if (saveRequested) {
            if (stats.saveCsv(config.statsOutput)) {
                std::cout << "Statistics saved to " << config.statsOutput << '\n';
            } else {
                std::cout << "Failed to save statistics file.\n";
            }
        }

        // Если симуляция не на паузе, выполняем один тик.
        if (!paused) {
            engine.step();

            SimulationStats s;
            s.tick = engine.getTick();
            s.plants = engine.countPlants();
            s.herbivores = engine.countHerbivores();
            s.predators = engine.countPredators();
            s.obstacles = engine.countObstacles();
            stats.add(s);
        }

        // Формируем строку для заголовка окна.
        std::ostringstream title;
        title << "tick=" << engine.getTick()
              << " plants=" << engine.countPlants()
              << " herbivores=" << engine.countHerbivores()
              << " predators=" << engine.countPredators()
              << " obstacles=" << engine.countObstacles()
              << " threads=" << config.threadCount
              << " delay=" << currentTickDelayMs << "ms"
              << (paused ? " [PAUSED]" : "");

        // Рисуем текущее состояние мира.
        renderer.render(engine.getGrid(), title.str());

        // Небольшая задержка между тиками.
        std::this_thread::sleep_for(std::chrono::milliseconds(currentTickDelayMs));
    }

    // При закрытии окна сохраняем статистику автоматически.
    if (stats.saveCsv(config.statsOutput)) {
        std::cout << "Statistics saved to " << config.statsOutput << '\n';
    } else {
        std::cout << "Failed to save statistics file.\n";
    }

    return 0;
}