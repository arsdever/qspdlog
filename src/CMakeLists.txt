set(SOURCES qspdlog.cpp qspdlog_model.cpp)
set(HEADERS ${CMAKE_SOURCE_DIR}/include/qspdlog/qspdlog.hpp qspdlog_model.hpp qt_logger_sink.hpp)
set(RESOURCES resources.qrc)

add_library(qspdlog_lib STATIC ${HEADERS} ${SOURCES} ${RESOURCES})
add_library(qspdlog::lib ALIAS qspdlog_lib)