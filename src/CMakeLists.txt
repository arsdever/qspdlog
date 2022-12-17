set(SOURCES qspdlog.cpp qspdlog_model.cpp)
set(HEADERS qspdlog_model.hpp qt_logger_sink.hpp)
set(RESOURCES qspdlog_resources.qrc)

add_library(qspdlog_lib STATIC ${HEADERS} ${SOURCES} ${RESOURCES})
add_library(qspdlog::lib ALIAS qspdlog_lib)