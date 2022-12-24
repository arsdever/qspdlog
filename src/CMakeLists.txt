set(SOURCES qspdlog.cpp qspdlog_model.cpp qspdlog_proxy_model.cpp
            qspdlog_toolbar.cpp)
set(HEADERS qspdlog_model.hpp qt_logger_sink.hpp qspdlog_proxy_model.hpp
            qspdlog_toolbar.hpp)
set(RESOURCES qspdlog_resources.qrc)

add_library(qspdlog_lib STATIC ${HEADERS} ${SOURCES} ${RESOURCES})
add_library(qspdlog::lib ALIAS qspdlog_lib)

target_link_libraries(qspdlog_lib PUBLIC qspdlog::interface)
