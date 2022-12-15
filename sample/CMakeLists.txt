add_executable(qspdlog_sample sample.cpp ../src/resources.qrc)

target_link_libraries(qspdlog_sample Qt::Core Qt::Gui Qt::Widgets qspdlog::lib)