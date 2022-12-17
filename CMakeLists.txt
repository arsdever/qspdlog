cmake_minimum_required(VERSION 3.10)

project(qspdlog)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(spdlog REQUIRED)

add_subdirectory(include)

link_libraries(Qt::Widgets spdlog::spdlog qspdlog::interface)

add_subdirectory(src)
add_subdirectory(sample)