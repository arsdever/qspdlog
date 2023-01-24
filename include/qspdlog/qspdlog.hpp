#pragma once

#include <QWidget>

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
class QTreeView;
class QAbstractSpdLogToolBar;

enum class AutoScrollPolicy {
    AutoScrollPolicyDisabled =
        0, // Never scroll to the bottom, leave the scrollbar where it was.
    AutoScrollPolicyEnabled =
        1, // Always scroll to the bottom when new rows are inserted.
    AutoScrollPolicyEnabledIfBottom =
        2, // Scroll to the bottom only if the scrollbar was at the bottom
           // before inserting the new ones.
};

class QSpdLog : public QWidget
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
     * @brief Register a toolbar.
     *
     * The toolbar will be set up for the current instance. Being set up means
     * - all the actions from the toolbar will affect current instance
     * - the changes to one toolbar will be reflected in the other ones as well
     *
     * @param toolbar the toolbar
     */
    void registerToolbar(QAbstractSpdLogToolBar* toolbar);

    /**
     * @brief Remove a toolbar.
     *
     * The toolbar will be removed from the current instance. The toolbar will
     * not be deleted.
     *
     * @param toolbar the toolbar
     */
    void removeToolbar(QAbstractSpdLogToolBar* toolbar);

    /**
     * @brief Get the sink.
     *
     * The sink should be used by the user to add it into any logger whose
     * output the user want's to see in the widget.
     *
     * @return std::shared_ptr<spdlog::sinks::sink> the sink of the widget
     */
    std::shared_ptr<spdlog::sinks::sink> sink();

    /**
     * @brief Get the number of items in the widget.
     *
     * @return std::size_t the number of items in the widget
     */
    std::size_t itemsCount() const;
    
    /**
     * @brief Get the maximum amount of items in the widget.
     *
     * @return int the maximum number of items in the widget
     */
    std::optional<std::size_t> getMaxEntries() const;
    void setMaxEntries(std::optional<std::size_t> maxEntries);

private slots:
    void filterData(
        const QString& text, bool isRegularExpression, bool isCaseSensitive
    );
    void updateAutoScrollPolicy(int index);

private:
    QSpdLogModel* _sourceModel;
    QSpdLogProxyModel* _proxyModel;
    QTreeView* _view;
    bool _scrollIsAtBottom;
    QMetaObject::Connection _scrollConnection;
    std::shared_ptr<spdlog::sinks::sink> _sink;
    std::list<QAbstractSpdLogToolBar*> _toolbars;
};
