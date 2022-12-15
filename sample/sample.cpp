#include <qspdlog/qspdlog.hpp>

#include <qapplication>
#include <qpushbutton>
#include <qtimer>
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> createLogger(std::string name) {
  auto logger = std::make_shared<spdlog::logger>(name);
  logger->set_level(spdlog::level::trace);
  return logger;
}

int main(int argc, char **argv) {
  Q_INIT_RESOURCE(resources);

  QApplication app(argc, argv);

  auto logger = createLogger("main");

  QPushButton generate("Generate");
  generate.connect(&generate, &QPushButton::clicked, [&logger]() {
    // generate 10 messages with random levels
    for (int i = 0; i < 10; ++i) {
      logger->log(
          static_cast<spdlog::level::level_enum>(rand() % spdlog::level::off),
          "Message {}", i);
    }
  });
  generate.show();

  QSpdLog log;
  log.registerLogger(logger);
  log.show();
  log.move(generate.pos() + QPoint(0, generate.height() + 50));

  int result = app.exec();
  spdlog::shutdown();

  return result;
}
