#include <qspdlog/qspdlog.hpp>

#include <qapplication>
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> createLogger(std::string name) {
  auto logger = std::make_shared<spdlog::logger>(name);
  logger->set_level(spdlog::level::trace);
  return logger;
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  auto logger = createLogger("main");

  QSpdLog log;
  log.registerLogger(logger);
  log.show();

  logger->info("Hello, World!");

  int result = app.exec();
  spdlog::shutdown();

  return result;
}
