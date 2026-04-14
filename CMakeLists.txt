cmake_minimum_required(VERSION 3.16)
project(EcosystemSimulation LANGUAGES CXX)

# Используем стандарт C++20.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции сборки:
# ENABLE_ASAN — включает AddressSanitizer;
# ENABLE_WARNINGS_AS_ERRORS — превращает предупреждения в ошибки.
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

# Настраиваем предупреждения и дополнительные флаги компилятора.
if (MSVC)
    add_compile_options(/W4 /EHsc /permissive-)

    if (ENABLE_WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif()

    if (ENABLE_ASAN)
        add_compile_options(/fsanitize=address)
        add_link_options(/fsanitize=address)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic)

    if (ENABLE_WARNINGS_AS_ERRORS)
        add_compile_options(-Werror)
    endif()

    if (ENABLE_ASAN)
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()
endif()

# Путь к установленной библиотеке SFML.
set(SFML_DIR "C:/Users/zador/Downloads/SFML-2.5.1-windows-vc15-64-bit/SFML-2.5.1/lib/cmake/SFML")

# Подключаем нужные компоненты SFML.
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Добавляем путь к заголовочным файлам проекта.
include_directories(include)

# Собираем исполняемый файл из всех исходников проекта.
add_executable(ecosystem_sim
    src/main.cpp
    src/Config.cpp
    src/Cell.cpp
    src/Entity.cpp
    src/Plant.cpp
    src/Animal.cpp
    src/Herbivore.cpp
    src/Predator.cpp
    src/Obstacle.cpp
    src/Grid.cpp
    src/SimulationEngine.cpp
    src/StatisticsCollector.cpp
    src/ConsoleRenderer.cpp
    src/Renderer.cpp
)

# Линкуем проект с библиотеками SFML.
target_link_libraries(ecosystem_sim
    sfml-graphics
    sfml-window
    sfml-system
)

# Копируем config.ini в папку сборки.
configure_file(${CMAKE_SOURCE_DIR}/config.ini
               ${CMAKE_BINARY_DIR}/config.ini
               COPYONLY)

# После сборки копируем DLL-файлы SFML рядом с exe.
add_custom_command(TARGET ecosystem_sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "C:/Users/zador/Downloads/SFML-2.5.1-windows-vc15-64-bit/SFML-2.5.1/bin"
    $<TARGET_FILE_DIR:ecosystem_sim>
)