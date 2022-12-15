cmake_minimum_required(VERSION 3.10)

project(qspdlog)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(spdlog REQUIRED)

add_library(qspdlog_interface INTERFACE include/qspdlog/qspdlog.hpp)
add_library(qspdlog::interface ALIAS qspdlog_interface)

target_include_directories(qspdlog_interface INTERFACE ${CMAKE_SOURCE_DIR}/include)

link_libraries(Qt::Widgets spdlog::spdlog qspdlog::interface)

add_subdirectory(src)
add_subdirectory(sample)