cmake_minimum_required(VERSION 3.10)

project(qspdlog)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# set QT version to 6 if not set already
if("${QT_VERSION}" STREQUAL "")
	message("Parent scope didn't tell the QT version to use. Defaulting it to Qt6")
	set(QT_VERSION 6)
else()
	message("Qt version set by the parent scope is Qt${QT_VERSION}")
endif()

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets)
find_package(spdlog REQUIRED)

add_subdirectory(include)

link_libraries(Qt::Widgets spdlog::spdlog qspdlog::interface)

add_subdirectory(src)
add_subdirectory(sample)