#include <qspdlog/qspdlog.hpp>

#include <qapplication>
#include <qpushbutton>
#include <qtimer>
#include <qsettings>
#include <qstylefactory>
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> createLogger(std::string name) {
  auto logger = std::make_shared<spdlog::logger>(name);
  logger->set_level(spdlog::level::trace);
  return logger;
}

void configureColorScheme() {
#ifdef Q_OS_WIN
  QSettings settings(
      "HKEY_CURRENT_"
      "USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
      QSettings::NativeFormat);
  if (settings.value("AppsUseLightTheme") == 0) {
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    QColor darkColor = QColor(45, 45, 45);
    QColor disabledColor = QColor(127, 127, 127);
    darkPalette.setColor(QPalette::Window, darkColor);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
    darkPalette.setColor(QPalette::AlternateBase, darkColor);
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    darkPalette.setColor(QPalette::Button, darkColor);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText,
                         disabledColor);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText,
                         disabledColor);

    qApp->setPalette(darkPalette);

    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; "
                        "border: 1px solid white; }");
  }
#endif
}

int main(int argc, char **argv) {
  Q_INIT_RESOURCE(qspdlog_resources);

  QApplication app(argc, argv);

  configureColorScheme();

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
