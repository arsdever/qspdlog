find_package(Qt${QT_VERSION} REQUIRED COMPONENTS Test)

add_executable(qspdlog_test_ui test_qspdlog.cpp)
add_executable(qspdlog::test::ui ALIAS qspdlog_test_ui)

target_link_libraries(qspdlog_test_ui PUBLIC Qt::Test qspdlog::lib)

add_test(NAME qspdlog_test_ui COMMAND qspdlog_test_ui)
