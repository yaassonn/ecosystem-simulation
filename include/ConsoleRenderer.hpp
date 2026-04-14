#pragma once

// Простой текстовый рендер поля в консоли.
// Используется для отладки и для запуска без графического окна.

#include "Grid.hpp"

class ConsoleRenderer {
public:
    // Выводит поле в консоль в упрощённом виде.
    static void render(const Grid& grid, int maxWidth = 60, int maxHeight = 25);
};