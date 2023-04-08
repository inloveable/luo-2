
#pragma once

#include <QAbstractListModel>
#include <memory>


struct CheckInfo;
class patientModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit patientModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:

    QList<std::unique_ptr<CheckInfo>> dataset;
};

