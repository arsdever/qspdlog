#include <QAction>
#include <QApplication>
#include <QSettings>
#include <QStyleFactory>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <thread>

#include "qspdlog/qabstract_spdlog_toolbar.hpp"
#include "qspdlog/qspdlog.hpp"
#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> createLogger(std::string name)
{
    auto logger = std::make_shared<spdlog::logger>(name);
    logger->set_level(spdlog::level::trace);
    return logger;
}

void configureColorScheme()
{
#ifdef Q_OS_WIN
    QSettings settings(
        "HKEY_CURRENT_"
        "USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personaliz"
        "e",
        QSettings::NativeFormat
    );
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
        darkPalette.setColor(
            QPalette::Disabled, QPalette::ButtonText, disabledColor
        );
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        darkPalette.setColor(
            QPalette::Disabled, QPalette::HighlightedText, disabledColor
        );

        qApp->setPalette(darkPalette);

        qApp->setStyleSheet(
            "QToolTip { color: #ffffff; background-color: #2a82da; "
            "border: 1px solid white; }"
        );
    }
#endif
}

void configureToolbar(
    QToolBar& toolbar, QSpdLog& logView, std::shared_ptr<spdlog::logger> logger
)
{
    QAction* clearAction = toolbar.addAction("Clear");
    QAction* generateAction = toolbar.addAction("Generate");
    QAction* generateMultipleAction = toolbar.addAction("GenerateMultiple");

    generateAction->connect(
        generateAction,
        &QAction::triggered,
        [ logger ](bool) {
        // generate 10 messages with random levels
        for (int i = 0; i < 10; ++i)
            logger->log(
                static_cast<spdlog::level::level_enum>(
                    rand() % spdlog::level::off
                ),
                "Message {}",
                i
            );
        });

    generateMultipleAction->connect(
        generateMultipleAction,
        &QAction::triggered,
        [ &logView, logger ](bool) {
        // create 10 threads and generate 10 messages with random levels
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([ &logView, i, logger ]() {
                auto threadLocalLogger =
                    createLogger(fmt::format("thread {}", i));
                threadLocalLogger->sinks().push_back(logView.sink());
                logger->info("Thread {} started", i);
                for (int i = 0; i < 10; ++i) {
                    threadLocalLogger->log(
                        static_cast<spdlog::level::level_enum>(
                            rand() % spdlog::level::off
                        ),
                        "Message {}",
                        i
                    );
                }
                logger->info("Thread {} finished", i);
            });
        }

        for (auto& thread : threads)
            thread.join();
        });

    clearAction->connect(clearAction, &QAction::triggered, [ &logView ](bool) {
        logView.clear();
    });
}

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(qspdlog_resources);

    QApplication app(argc, argv);

    configureColorScheme();

    auto logger = createLogger("main");

    QToolBar toolbar("Manipulation toolbar");
    toolbar.show();

    QSpdLog log;
    log.show();
    log.move(toolbar.pos() + QPoint(0, toolbar.height() + 50));

    QAbstractSpdLogToolBar* logToolbar = createToolBar();
    log.registerToolbar(logToolbar);
    dynamic_cast<QWidget*>(logToolbar)->show();

    logger->sinks().push_back(log.sink());

    configureToolbar(toolbar, log, logger);

    int result = app.exec();
    spdlog::shutdown();

    return result;
}
