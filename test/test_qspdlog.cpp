#include <QAction>
#include <QComboBox>
#include <QLineEdit>
#include <QObject>
#include <QRegularExpression>
#include <QScrollArea>
#include <QScrollBar>
#include <QTest>
#include <QTreeView>

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

        QLineEdit* filterText =
            widget.toolbar()->findChild<QLineEdit*>("filterText");

        QCOMPARE(widget.itemsCount(), 2);
        QWidget* toolbar = widget.toolbar();
        QTest::keyClicks(filterText, "ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        filterText->setText("Another");
        QCOMPARE(widget.itemsCount(), 1);
        filterText->setText("");
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

        QLineEdit* filterText =
            widget.toolbar()->findChild<QLineEdit*>("filterText");
        QAction* caseSensitiveAction =
            widget.toolbar()->findChild<QAction*>("caseSensitiveAction");

        QCOMPARE(widget.itemsCount(), 2);
        QWidget* toolbar = widget.toolbar();
        filterText->setText("Ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        caseSensitiveAction->trigger();
        QCOMPARE(widget.itemsCount(), 0);
        filterText->setText("ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        filterText->setText("nonexistent");
        QCOMPARE(widget.itemsCount(), 0);
        filterText->setText("Ipsum");
        QCOMPARE(widget.itemsCount(), 0);
        caseSensitiveAction->trigger();
        QCOMPARE(widget.itemsCount(), 1);
        filterText->setText("");
        QCOMPARE(widget.itemsCount(), 2);
        caseSensitiveAction->trigger();
        QCOMPARE(widget.itemsCount(), 2);
    }

    void filterRegularExpressions()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("Lorem ipsum dolor sit amet, consectetur adipiscing elit");
        logger->info("Another message");

        QLineEdit* filterText =
            widget.toolbar()->findChild<QLineEdit*>("filterText");
        QAction* regexAction =
            widget.toolbar()->findChild<QAction*>("regexAction");

        QCOMPARE(widget.itemsCount(), 2);
        QWidget* toolbar = widget.toolbar();
        filterText->setText("ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        regexAction->trigger();
        QCOMPARE(widget.itemsCount(), 1);
        filterText->setText(".*");
        QCOMPARE(widget.itemsCount(), 2);
        filterText->setText(".*amet");
        QCOMPARE(widget.itemsCount(), 1);
        regexAction->trigger();
        QCOMPARE(widget.itemsCount(), 0);
        filterText->setText(".*");
        QCOMPARE(widget.itemsCount(), 0);
        regexAction->trigger();
        QCOMPARE(widget.itemsCount(), 2);
        filterText->setText("\(.*");
        QColor color = filterText->palette().color(QPalette::Text);
        QCOMPARE(color, Qt::red);
        QRegularExpression re("\(.*");
        QCOMPARE(filterText->toolTip(), re.errorString());
    }

    void autoScrollPolicyDefault()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);

        QComboBox* autoScrollPolicy =
            widget.toolbar()->findChild<QComboBox*>("autoScrollPolicySelection"
            );
        QTreeView* treeView = widget.findChild<QTreeView*>("qspdlogTreeView");
        QScrollBar* scrollBar = treeView->verticalScrollBar();

        treeView->resize(100, 100);

        autoScrollPolicy->setCurrentIndex(
            static_cast<int>(AutoScrollPolicy::AutoScrollPolicyDisabled)
        );

        QCOMPARE(scrollBar->value(), scrollBar->maximum());

        for (int i = 0; i < 10; ++i)
            logger->info("test");

        auto actualValue = scrollBar->value();
        treeView->scrollToBottom();
        auto maximumValue = scrollBar->value();

        QVERIFY(actualValue != maximumValue);
    }

    void autoScrollPolicyAutoScroll()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);

        QComboBox* autoScrollPolicy =
            widget.toolbar()->findChild<QComboBox*>("autoScrollPolicySelection"
            );
        QTreeView* treeView = widget.findChild<QTreeView*>("qspdlogTreeView");
        QScrollBar* scrollBar = treeView->verticalScrollBar();

        treeView->resize(100, 100);

        autoScrollPolicy->setCurrentIndex(
            static_cast<int>(AutoScrollPolicy::AutoScrollPolicyEnabled)
        );

        // fill the visible area
        for (int i = 0; i < 5; ++i)
            logger->info("test");

        QCOMPARE(scrollBar->value(), scrollBar->maximum());

        for (int i = 0; i < 3; ++i)
            logger->info("test");

        QCOMPARE(scrollBar->value(), scrollBar->maximum());
    }

    void autoScrollPolicySmartScroll()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);

        QComboBox* autoScrollPolicy =
            widget.toolbar()->findChild<QComboBox*>("autoScrollPolicySelection"
            );
        QTreeView* treeView = widget.findChild<QTreeView*>("qspdlogTreeView");
        QScrollBar* scrollBar = treeView->verticalScrollBar();

        treeView->resize(100, 100);

        autoScrollPolicy->setCurrentIndex(
            static_cast<int>(AutoScrollPolicy::AutoScrollPolicyEnabledIfBottom)
        );

        // fill the visible area
        for (int i = 0; i < 5; ++i)
            logger->info("test");

        QCOMPARE(scrollBar->value(), scrollBar->maximum());

        for (int i = 0; i < 3; ++i)
            logger->info("test");

        QCOMPARE(scrollBar->value(), scrollBar->maximum());

        scrollBar->setValue(0);
        for (int i = 0; i < 3; ++i)
            logger->info("test");

        QVERIFY(scrollBar->value() != scrollBar->maximum());

        scrollBar->setValue(scrollBar->maximum());
        for (int i = 0; i < 3; ++i)
            logger->info("test");

        QCOMPARE(scrollBar->value(), scrollBar->maximum());
    }
};

QTEST_MAIN(QSpdLogTest);
#include "test_qspdlog.moc"
