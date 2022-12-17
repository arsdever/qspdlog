#include "qobject"
#include "qtest"
#include "spdlog/spdlog.h"

#include "qspdlog/qspdlog.hpp"

class QSpdLogTest : public QObject {
  Q_OBJECT

public:
  QSpdLogTest() {
    widget = std::make_unique<QSpdLog>();
    logger = std::make_shared<spdlog::logger>("test");
  }

private slots:
  void checkMessageCountAllLevelsEnabled() {
    QVERIFY(widget != nullptr);
    widget->registerLogger(logger);
    logger->flush_on(spdlog::level::trace);
    logger->set_level(spdlog::level::trace);
    logger->trace("test");
    logger->debug("test");
    logger->info("test");
    logger->warn("test");
    logger->error("test");
    logger->critical("test");

    QCOMPARE(widget->model()->rowCount(), 6);
  }

private:
  std::unique_ptr<QSpdLog> widget;
  std::shared_ptr<spdlog::logger> logger;
};

QTEST_MAIN(QSpdLogTest);
#include "test_qspdlog.moc"