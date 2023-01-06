#include <QAction>
#include <QLineEdit>
#include <QObject>
#include <QTest>

#include "qspdlog/qspdlog.hpp"
#include "spdlog/spdlog.h"

class QSpdLogTest : public QObject
{
    Q_OBJECT

public:
    QSpdLogTest() { }

private slots:
    void checkMessageCountAllLevelsEnabled()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->set_level(spdlog::level::trace);
        logger->trace("test");
        logger->debug("test");
        logger->info("test");
        logger->warn("test");
        logger->error("test");
        logger->critical("test");

        QCOMPARE(widget.itemsCount(), 6);
    }

    void disconnectionOfTheSink()
    {
        std::unique_ptr<QSpdLog> widget = std::make_unique<QSpdLog>();
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget->sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("test");
        QCOMPARE(widget->itemsCount(), 1);
        widget.reset();
        logger->info("test");
        logger->flush();
    }

    void addSinkToLoggerAndDestroy()
    {
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");
        {
            std::unique_ptr<QSpdLog> widget = std::make_unique<QSpdLog>();
            logger->sinks().push_back(widget->sink());
            logger->flush_on(spdlog::level::trace);
            logger->info("test");
            QCOMPARE(widget->itemsCount(), 1);
        }
        logger->info("test");
        logger->flush();
    }

    void clearLogHistory()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("test");
        QCOMPARE(widget.itemsCount(), 1);
        widget.clear();
        QCOMPARE(widget.itemsCount(), 0);
    }

    void filterMessage()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("Lorem ipsum dolor sit amet, consectetur adipiscing elit");
        logger->info("Another message");
        QCOMPARE(widget.itemsCount(), 2);
        QWidget* toolbar = widget.toolbar();
        QTest::keyClicks(toolbar->findChild<QLineEdit*>("filterText"), "ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        toolbar->findChild<QLineEdit*>("filterText")->setText("Another");
        QCOMPARE(widget.itemsCount(), 1);
        toolbar->findChild<QLineEdit*>("filterText")->setText("");
        QCOMPARE(widget.itemsCount(), 2);
    }

    void filterCaseDependant()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("Lorem ipsum dolor sit amet, consectetur adipiscing elit");
        logger->info("Another message");
        QCOMPARE(widget.itemsCount(), 2);
        QWidget* toolbar = widget.toolbar();
        toolbar->findChild<QLineEdit*>("filterText")->setText("Ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        toolbar->findChild<QAction*>("caseSensitiveAction")->trigger();
        QCOMPARE(widget.itemsCount(), 0);
        toolbar->findChild<QLineEdit*>("filterText")->setText("ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        toolbar->findChild<QLineEdit*>("filterText")->setText("nonexistent");
        QCOMPARE(widget.itemsCount(), 0);
        toolbar->findChild<QLineEdit*>("filterText")->setText("Ipsum");
        QCOMPARE(widget.itemsCount(), 0);
        toolbar->findChild<QAction*>("caseSensitiveAction")->trigger();
        QCOMPARE(widget.itemsCount(), 1);
        toolbar->findChild<QLineEdit*>("filterText")->setText("");
        QCOMPARE(widget.itemsCount(), 2);
        toolbar->findChild<QAction*>("caseSensitiveAction")->trigger();
        QCOMPARE(widget.itemsCount(), 2);
    }
};

QTEST_MAIN(QSpdLogTest);
#include "test_qspdlog.moc"
