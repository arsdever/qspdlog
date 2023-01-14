#include <QAction>
#include <QComboBox>
#include <QLineEdit>
#include <QObject>
#include <QRegularExpression>
#include <QScrollArea>
#include <QScrollBar>
#include <QSettings>
#include <QTest>
#include <QTreeView>

#include "qspdlog/qabstract_spdlog_toolbar.hpp"
#include "qspdlog/qspdlog.hpp"
#include "spdlog/spdlog.h"

class QTestToolBar : public QAbstractSpdLogToolBar
{
public:
    explicit QTestToolBar()
    {
        _autoScrollPolicy->addItems(
            { "Manual Scroll", "Scroll To Bottom", "Smart Scroll" }
        );
        _regex->setCheckable(true);
        _caseSensitive->setCheckable(true);
    }
    ~QTestToolBar() override = default;

#pragma region QAbstractSpdLogToolBar

public:
    QLineEdit* filter() override { return _filter; }
    QAction* regex() override { return _regex; }
    QAction* caseSensitive() override { return _caseSensitive; }
    QAction* clearHistory() override { return _clearHistory; }
    QComboBox* autoScrollPolicy() override { return _autoScrollPolicy; }

#pragma endregion

public:
    QLineEdit* _filter = new QLineEdit;
    QAction* _regex = new QAction;
    QAction* _caseSensitive = new QAction;
    QAction* _clearHistory = new QAction;
    QComboBox* _autoScrollPolicy = new QComboBox;
};

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

    void filterMessageAndCompletionHistory()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("Lorem ipsum dolor sit amet, consectetur adipiscing elit");
        logger->info("Another message");

        // setup the toolbar
        auto toolbar = std::make_unique<QTestToolBar>();
        widget.registerToolbar(toolbar.get());

        QCOMPARE(widget.itemsCount(), 2);
        QTest::keyClicks(toolbar->filter(), "ipsum");
        QTest::keyClick(toolbar->filter(), Qt::Key_Enter);
        QCOMPARE(widget.itemsCount(), 1);
        toolbar->filter()->setText("Another");
        QCOMPARE(widget.itemsCount(), 1);
        toolbar->filter()->setText("");
        QTest::keyClick(toolbar->filter(), Qt::Key_Enter);
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

        // setup the toolbar
        auto toolbar = std::make_unique<QTestToolBar>();
        widget.registerToolbar(toolbar.get());

        QLineEdit* filter = toolbar->filter();
        QAction* caseSensitive = toolbar->caseSensitive();

        QCOMPARE(widget.itemsCount(), 2);
        filter->setText("Ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        caseSensitive->trigger();
        QCOMPARE(widget.itemsCount(), 0);
        filter->setText("ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        filter->setText("nonexistent");
        QCOMPARE(widget.itemsCount(), 0);
        filter->setText("Ipsum");
        QCOMPARE(widget.itemsCount(), 0);
        caseSensitive->trigger();
        QCOMPARE(widget.itemsCount(), 1);
        filter->setText("");
        QCOMPARE(widget.itemsCount(), 2);
        caseSensitive->trigger();
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

        // setup the toolbar
        auto toolbar = std::make_unique<QTestToolBar>();
        widget.registerToolbar(toolbar.get());

        QLineEdit* filter = toolbar->filter();
        QAction* regex = toolbar->regex();

        QCOMPARE(widget.itemsCount(), 2);
        filter->setText("ipsum");
        QCOMPARE(widget.itemsCount(), 1);
        regex->trigger();
        QCOMPARE(widget.itemsCount(), 1);
        filter->setText(".*");
        QCOMPARE(widget.itemsCount(), 2);
        filter->setText(".*amet");
        QCOMPARE(widget.itemsCount(), 1);
        regex->trigger();
        QCOMPARE(widget.itemsCount(), 0);
        filter->setText(".*");
        QCOMPARE(widget.itemsCount(), 0);
        regex->trigger();
        QCOMPARE(widget.itemsCount(), 2);
        // TODO: base implementation of the toolbar should change the color of the invalid regex text
        // filter->setText("\(.*");
        // QColor color = filter->palette().color(QPalette::Text);
        // QCOMPARE(color, Qt::red);
        // QRegularExpression re("\(.*");
        // QCOMPARE(filter->toolTip(), re.errorString());
    }

    void autoScrollPolicyDefault()
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);

        // setup the toolbar
        auto toolbar = std::make_unique<QTestToolBar>();
        widget.registerToolbar(toolbar.get());

        QComboBox* autoScrollPolicy = toolbar->autoScrollPolicy();
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

        // setup the toolbar
        auto toolbar = std::make_unique<QTestToolBar>();
        widget.registerToolbar(toolbar.get());

        QComboBox* autoScrollPolicy = toolbar->autoScrollPolicy();
        QTreeView* treeView =
        widget.findChild<QTreeView*>("qspdlogTreeView"); QScrollBar*
        scrollBar = treeView->verticalScrollBar();

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

        // setup the toolbar
        auto toolbar = std::make_unique<QTestToolBar>();
        widget.registerToolbar(toolbar.get());

        QComboBox* autoScrollPolicy = toolbar->autoScrollPolicy();
        QTreeView* treeView =
        widget.findChild<QTreeView*>("qspdlogTreeView"); QScrollBar*
        scrollBar = treeView->verticalScrollBar();

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
