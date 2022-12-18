set(SOURCES qspdlog.cpp qspdlog_model.cpp qspdlog_proxy_model.cpp)
set(HEADERS qspdlog_model.hpp qt_logger_sink.hpp qspdlog_proxy_model.hpp)
set(RESOURCES qspdlog_resources.qrc)

add_library(qspdlog_lib STATIC ${HEADERS} ${SOURCES} ${RESOURCES})
add_library(qspdlog::lib ALIAS qspdlog_lib)
