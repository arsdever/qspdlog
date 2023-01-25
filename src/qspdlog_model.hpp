#pragma once

#include <QAbstractListModel>
#include <deque>

class QSpdLogModel : public QAbstractListModel
{
public:
    Q_OBJECT
public:
    struct entry_t {
        std::chrono::duration<double> time;
        int level;
        std::string message;
    };

public:
    QSpdLogModel(QObject* parent = nullptr);
    ~QSpdLogModel() override = default;

    void addEntry(entry_t entry);
    void clear();

    void setMaxEntries(std::optional<std::size_t> maxEntries);
    std::optional<std::size_t> getMaxEntries() const;

#pragma region QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole)
        const override;
    QVariant headerData(
        int section, Qt::Orientation orientation, int role = Qt::DisplayRole
    ) const override;
#pragma endregion

private:
    std::deque<entry_t> _items;
    std::optional<std::size_t> _maxEntries;
};
