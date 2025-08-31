#pragma once
#include <QAbstractTableModel>
#include <QString>
#include <vector>
#include "Storage.hpp"

class ExpenseModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ExpenseModel(Storage& storage, QObject* parent=nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setMonth(const QString& yyyymm);
    void setIncludeProrated(bool on);
    QString month() const { return month_; }
    bool includeProrated() const { return includeProrated_; }

    double total() const { return total_; }
    std::map<QString,double> totalsByCategory() const { return byCategory_; }
    std::map<QString,double> totalsByUser() const { return byUser_; }

private:
    struct Row { QString date,user,category,desc,type; double amount; bool prorated; };
    Storage& storage_;
    QString month_;
    bool includeProrated_{true};
    std::vector<Row> rows_;
    double total_{0.0};
    std::map<QString,double> byCategory_;
    std::map<QString,double> byUser_;

    void rebuild();
};

