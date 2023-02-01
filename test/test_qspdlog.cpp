#include <QAction>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QScrollBar>
#include <QSettings>
#include <QTest>
#include <QTimer>
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
    QAction* style() override { return _style; }
    QComboBox* autoScrollPolicy() override { return _autoScrollPolicy; }

#pragma endregion

public:
    QLineEdit* _filter = new QLineEdit;
    QAction* _regex = new QAction;
    QAction* _caseSensitive = new QAction;
    QAction* _clearHistory = new QAction;
    QAction* _style = new QAction;
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

    void limitLogTest()
    {
        QSpdLog widget;
        QCOMPARE(widget.getMaxEntries(), std::nullopt);
        widget.setMaxEntries(20);
        QCOMPARE(widget.getMaxEntries(), 20);
        auto logger = std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("test");
        QCOMPARE(widget.itemsCount(), 1);
        for (int i = 0; i < 100; i++)
            logger->info("test {0}", i);
        logger->flush();
        QCOMPARE(widget.itemsCount(), 20);
        widget.setMaxEntries(std::nullopt);
        QCOMPARE(widget.getMaxEntries(), std::nullopt);
        for (int i = 0; i < 50; i++)
            logger->info("test2 {0}", i);
        QCOMPARE(widget.itemsCount(), 70);
        widget.setMaxEntries(20);
        QCOMPARE(widget.getMaxEntries(), 20);
        QCOMPARE(widget.itemsCount(), 20);
    }

    void backgroundForegroundColorTest()
    {
        QSpdLog widget;
        QCOMPARE(widget.getLoggerBackground("test"), std::nullopt);
        widget.setLoggerBackground("test", QBrush(Qt::red));
        QCOMPARE(widget.getLoggerBackground("test"), QBrush(Qt::red));
        QCOMPARE(widget.getLoggerBackground("test2"), std::nullopt);
        widget.setLoggerBackground("test", std::nullopt);
        QCOMPARE(widget.getLoggerBackground("test"), std::nullopt);

        QCOMPARE(widget.getLoggerForeground("test"), std::nullopt);
        widget.setLoggerForeground("test", Qt::white);
        QCOMPARE(widget.getLoggerForeground("test"), Qt::white);
        QCOMPARE(widget.getLoggerForeground("test2"), std::nullopt);
        widget.setLoggerForeground("test", std::nullopt);
        QCOMPARE(widget.getLoggerForeground("test"), std::nullopt);
    }

    void runToolbarTests()
    {
        std::vector<std::unique_ptr<QAbstractSpdLogToolBar>> toolbars;
        toolbars.push_back(std::make_unique<QTestToolBar>());
        toolbars.push_back(
            std::unique_ptr<QAbstractSpdLogToolBar>(createToolBar())
        );

        for (auto& toolbar : toolbars) {
            filterMessageAndCompletionHistory(toolbar.get());
            filterCaseDependant(toolbar.get());
            filterRegularExpressions(toolbar.get());
            autoScrollPolicyDefault(toolbar.get());
            autoScrollPolicyAutoScroll(toolbar.get());
            autoScrollPolicySmartScroll(toolbar.get());
            toolbar->setParent(nullptr);
        }
    }

    void filterMessageAndCompletionHistory(QAbstractSpdLogToolBar* toolbar)
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("Lorem ipsum dolor sit amet, consectetur adipiscing elit");
        logger->info("Another message");
        widget.registerToolbar(toolbar);

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

    void filterCaseDependant(QAbstractSpdLogToolBar* toolbar)
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("Lorem ipsum dolor sit amet, consectetur adipiscing elit");
        logger->info("Another message");
        widget.registerToolbar(toolbar);

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

    void filterRegularExpressions(QAbstractSpdLogToolBar* toolbar)
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->flush_on(spdlog::level::trace);
        logger->info("Lorem ipsum dolor sit amet, consectetur adipiscing elit");
        logger->info("Another message");
        widget.registerToolbar(toolbar);

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
        // TODO: base implementation of the toolbar should change the color of
        // the invalid regex text filter->setText("\(.*"); QColor color =
        // filter->palette().color(QPalette::Text); QCOMPARE(color, Qt::red);
        // QRegularExpression re("\(.*");
        // QCOMPARE(filter->toolTip(), re.errorString());
    }

    void autoScrollPolicyDefault(QAbstractSpdLogToolBar* toolbar)
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
        widget.registerToolbar(toolbar);

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

    void autoScrollPolicyAutoScroll(QAbstractSpdLogToolBar* toolbar)
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
        widget.registerToolbar(toolbar);

        QComboBox* autoScrollPolicy = toolbar->autoScrollPolicy();
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

    void autoScrollPolicySmartScroll(QAbstractSpdLogToolBar* toolbar)
    {
        QSpdLog widget;
        std::shared_ptr<spdlog::logger> logger =
            std::make_shared<spdlog::logger>("test");

        logger->sinks().push_back(widget.sink());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
        widget.registerToolbar(toolbar);

        QComboBox* autoScrollPolicy = toolbar->autoScrollPolicy();
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

    void headerColumnShowHide()
    {
        QSpdLog widget;
        QTreeView* treeView = widget.findChild<QTreeView*>("qspdlogTreeView");
        QHeaderView* headerView = treeView->header();
        QCOMPARE(headerView->count(), 4);
        QMetaObject::invokeMethod(
            headerView,
            [] {
            QList<QWidget*> topLevelWidgets = QApplication::topLevelWidgets();
            auto widgetsIt = std::find_if(
                topLevelWidgets.begin(),
                topLevelWidgets.end(),
                [](QWidget* widget) {
                return widget->objectName() == "qspdlogHeaderContextMenu";
                });
            QVERIFY(widgetsIt != topLevelWidgets.end());
            QMenu* menu = qobject_cast<QMenu*>(*widgetsIt);
            QTest::mouseClick(
                menu, Qt::LeftButton, Qt::NoModifier, QPoint(5, 5)
            );
            },
            Qt::QueuedConnection);
        headerView->customContextMenuRequested(QPoint(5, 5));
        QCOMPARE(headerView->count(), 4);
        QCOMPARE(headerView->hiddenSectionCount(), 1);
        QMetaObject::invokeMethod(
            headerView,
            [] {
            QList<QWidget*> topLevelWidgets = QApplication::topLevelWidgets();
            auto widgetsIt = std::find_if(
                topLevelWidgets.begin(),
                topLevelWidgets.end(),
                [](QWidget* widget) {
                return widget->objectName() == "qspdlogHeaderContextMenu";
                });
            QVERIFY(widgetsIt != topLevelWidgets.end());
            QMenu* menu = qobject_cast<QMenu*>(*widgetsIt);
            QTest::mouseClick(
                menu, Qt::LeftButton, Qt::NoModifier, QPoint(5, 25)
            );
            },
            Qt::QueuedConnection);
        headerView->customContextMenuRequested(QPoint(5, 5));
        QCOMPARE(headerView->count(), 4);
        QCOMPARE(headerView->hiddenSectionCount(), 2);
        QMetaObject::invokeMethod(
            headerView,
            [] {
            QList<QWidget*> topLevelWidgets = QApplication::topLevelWidgets();
            auto widgetsIt = std::find_if(
                topLevelWidgets.begin(),
                topLevelWidgets.end(),
                [](QWidget* widget) {
                return widget->objectName() == "qspdlogHeaderContextMenu";
                });
            QVERIFY(widgetsIt != topLevelWidgets.end());
            QMenu* menu = qobject_cast<QMenu*>(*widgetsIt);
            QTest::mouseClick(
                menu, Qt::LeftButton, Qt::NoModifier, QPoint(5, 5)
            );
            },
            Qt::QueuedConnection);
        headerView->customContextMenuRequested(QPoint(5, 5));
        QCOMPARE(headerView->count(), 4);
        QCOMPARE(headerView->hiddenSectionCount(), 1);
    }

    void setStyleFromToolbar()
    {
        QSpdLog widget;
        auto testLogger = std::make_shared<spdlog::logger>("test");
        auto testLogger1 = std::make_shared<spdlog::logger>("test1");

        testLogger->sinks().push_back(widget.sink());
        testLogger1->sinks().push_back(widget.sink());

        std::unique_ptr<QAbstractSpdLogToolBar> toolbar(createToolBar());
        widget.registerToolbar(toolbar.get());
        QAction* style = toolbar->style();

        auto dialogManipThread =
            manipulateStyleDialog("test", "#ff0000", "#00ff00", true);
        style->trigger();
        dialogManipThread.join();

        testLogger->info("test");
        testLogger1->info("test1");

        const QTreeView* treeView =
            widget.findChild<const QTreeView*>("qspdlogTreeView");
        const QAbstractItemModel* model = treeView->model();
        QModelIndex index = model->index(0, 3);
        QCOMPARE(
            model->data(index, Qt::DisplayRole).value<QString>(),
            QString("test")
        );
        QCOMPARE(
            model->data(index, Qt::BackgroundRole).value<QColor>(),
            QColor("#ff0000")
        );
        QCOMPARE(
            model->data(index, Qt::ForegroundRole).value<QColor>(),
            QColor("#00ff00")
        );

        index = model->index(1, 3);
        QCOMPARE(
            model->data(index, Qt::DisplayRole).value<QString>(),
            QString("test1")
        );
        QCOMPARE(
            model->data(index, Qt::BackgroundRole).value<QColor>(), QColor {}
        );
        QCOMPARE(
            model->data(index, Qt::ForegroundRole).value<QColor>(), QColor {}
        );

        dialogManipThread =
            manipulateStyleDialog("test", std::nullopt, std::nullopt, true);
        style->trigger();
        dialogManipThread.join();

        index = model->index(0, 3);
        QCOMPARE(
            model->data(index, Qt::DisplayRole).value<QString>(),
            QString("test")
        );
        QCOMPARE(
            model->data(index, Qt::BackgroundRole).value<QColor>(),
            QColor("#ff0000")
        );
        QCOMPARE(
            model->data(index, Qt::ForegroundRole).value<QColor>(),
            QColor("#00ff00")
        );

        dialogManipThread = manipulateStyleDialog("test", "", "", true);
        style->trigger();
        dialogManipThread.join();

        index = model->index(0, 3);
        QCOMPARE(
            model->data(index, Qt::DisplayRole).value<QString>(),
            QString("test")
        );
        QCOMPARE(
            model->data(index, Qt::BackgroundRole).value<QColor>(), QColor {}
        );
        QCOMPARE(
            model->data(index, Qt::ForegroundRole).value<QColor>(), QColor {}
        );

        dialogManipThread = manipulateStyleDialog("test", "", "", false);
        style->trigger();
        dialogManipThread.join();

        index = model->index(0, 3);
        QCOMPARE(
            model->data(index, Qt::DisplayRole).value<QString>(),
            QString("test")
        );
        QCOMPARE(
            model->data(index, Qt::BackgroundRole).value<QColor>(), QColor {}
        );
        QCOMPARE(
            model->data(index, Qt::ForegroundRole).value<QColor>(), QColor {}
        );
        index = model->index(1, 3);
        QCOMPARE(
            model->data(index, Qt::DisplayRole).value<QString>(),
            QString("test1")
        );
        QCOMPARE(
            model->data(index, Qt::BackgroundRole).value<QColor>(), QColor {}
        );
        QCOMPARE(
            model->data(index, Qt::ForegroundRole).value<QColor>(), QColor {}
        );
    }

private:
    std::thread manipulateStyleDialog(
        std::optional<QString> name,
        std::optional<QString> background,
        std::optional<QString> foreground,
        bool accept
    ) const
    {
        return std::thread([ n = std::move(name),
                             bg = std::move(background),
                             fg = std::move(foreground),
                             accept ] {
            QDialog* dialog;
            bool success = QTest::qWaitFor(
                [ &dialog ]() -> bool {
                    auto widgets = qApp->topLevelWidgets();
                    auto it = std::find_if(
                        widgets.begin(),
                        widgets.end(),
                        [](QWidget* widget) {
                    return widget->objectName() == "qspdlogStyleDialog";
                        });

                    if (it == widgets.end())
                        return false;

                    dialog = qobject_cast<QDialog*>(*it);
                    return true;
                },
                1000
            );

            QVERIFY(success);

            QMetaObject::invokeMethod(
                dialog,
                [ name = std::move(n),
                  background = std::move(bg),
                  foreground = std::move(fg),
                  accept,
                  dialog ] {
                QVERIFY(dialog);
                QLineEdit* loggerNameEdit =
                    dialog->findChild<QLineEdit*>("loggerNameEdit");
                QVERIFY(loggerNameEdit);
                QLineEdit* backgroundColorEdit =
                    dialog->findChild<QLineEdit*>("backgroundColorEdit");
                QVERIFY(backgroundColorEdit);
                QLineEdit* textColorEdit =
                    dialog->findChild<QLineEdit*>("textColorEdit");
                QVERIFY(textColorEdit);

                if (name)
                    loggerNameEdit->setText(name.value());

                if (background)
                    backgroundColorEdit->setText(background.value());

                if (foreground)
                    textColorEdit->setText(foreground.value());

                QDialogButtonBox* buttonBox =
                    dialog->findChild<QDialogButtonBox*>("buttonBox");
                QVERIFY(buttonBox);
                if (accept)
                    buttonBox->button(QDialogButtonBox::Ok)->click();
                else
                    buttonBox->button(QDialogButtonBox::Cancel)->click();
                },
                Qt::QueuedConnection);
        });
    }
};

QTEST_MAIN(QSpdLogTest);
#include "test_qspdlog.moc"
