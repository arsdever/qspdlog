#pragma once

#include <QTreeView>

namespace spdlog
{
class logger;
namespace sinks
{
class sink;
} // namespace sinks
} // namespace spdlog

class QSpdLogModel;
class QSpdLogProxyModel;

class QSpdLog : public QTreeView
{
public:
    /**
     * @brief Constructor
     *
     * @param parent The parent widget.
     */
    explicit QSpdLog(QWidget* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~QSpdLog();

public:
    /**
     * @brief Clear the contents of the model.
     *
     * The method will clear up all the cached messages. There's no way after
     * this to restore them.
     */
    void clear();

    /**
     * @brief Get the toolbar widget.
     *
     * The toolbar widget is constructed by the QSpdLog class and has all the
     * required connections already set up.
     *
     * The toolbar provides the controls for setting up the auto-scroll policy
     * and tools for filtering the messages.
     *
     * @return the toolbar widget
     */
    QWidget* toolbar() const;

    /**
     * @brief Get the sink.
     *
     * The sink should be used by the user to add it into any logger whose
     * output the user want's to see in the widget.
     *
     * @return std::shared_ptr<spdlog::sinks::sink> the sink of the widget
     */
    std::shared_ptr<spdlog::sinks::sink> sink();

private slots:
    void updateFiltering();
    void updateAutoScrollPolicy(int index);

private:
    QSpdLogModel* _sourceModel;
    QSpdLogProxyModel* _proxyModel;
    QWidget* _toolbar;
    bool _scrollIsAtBottom;
    QMetaObject::Connection _scrollConnection;
    std::shared_ptr<spdlog::sinks::sink> _sink;
};
