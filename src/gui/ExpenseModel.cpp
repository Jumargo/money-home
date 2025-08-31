#include "ExpenseModel.hpp"
#include <QVariant>
#include <QStringList>
#include <QColor>
#include <iomanip>

ExpenseModel::ExpenseModel(Storage& storage, QObject* parent)
    : QAbstractTableModel(parent), storage_(storage) {}

int ExpenseModel::rowCount(const QModelIndex&) const { return static_cast<int>(rows_.size()); }
int ExpenseModel::columnCount(const QModelIndex&) const { return 6; }

QVariant ExpenseModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row()<0 || index.row()>=rowCount()) return {};
    const Row& r = rows_[index.row()];
    if (role == Qt::DisplayRole) {
        switch(index.column()){
            case 0: return r.date;
            case 1: return r.user;
            case 2: return r.category;
            case 3: return QString::number(r.amount, 'f', 2);
            case 4: return r.type;
            case 5: return r.desc;
        }
    }
    if (role == Qt::ForegroundRole && r.prorated) {
        return QColor(90,90,90);
    }
    return {};
}

QVariant ExpenseModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section){
            case 0: return "Fecha";
            case 1: return "Usuario";
            case 2: return "Categoría";
            case 3: return "Importe";
            case 4: return "Tipo";
            case 5: return "Descripción";
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void ExpenseModel::setMonth(const QString& yyyymm){ month_ = yyyymm; rebuild(); }
void ExpenseModel::setIncludeProrated(bool on){ includeProrated_ = on; rebuild(); }

void ExpenseModel::rebuild(){
    beginResetModel();
    rows_.clear(); total_=0.0; byCategory_.clear(); byUser_.clear();
    const auto ym = month_.toStdString();
    for(const auto& e : storage_.expenses()){
        const auto em = e.date.monthKey();
        if (e.type==ExpenseType::Normal) {
            if (em==ym) {
                Row r{QString::fromStdString(e.date.toString()), QString::fromStdString(e.user), QString::fromStdString(e.category), QString::fromStdString(e.description), "Normal"};
                r.amount = e.amount; r.prorated=false; r.type = "Normal";
                rows_.push_back(r);
                total_ += r.amount; byCategory_[r.category]+=r.amount; byUser_[r.user]+=r.amount;
            }
        } else {
            if (!includeProrated_) continue;
            int diff = monthsBetween(em, ym);
            if (diff>=1 && diff<=12) {
                double share = e.amount/12.0;
                Row r{QString::fromStdString(e.date.toString()), QString::fromStdString(e.user), QString::fromStdString(e.category), QString::fromStdString(e.description), "Prorrateo"};
                r.amount = share; r.prorated=true; r.type = "Prorrateo";
                rows_.push_back(r);
                total_ += r.amount; byCategory_[r.category]+=r.amount; byUser_[r.user]+=r.amount;
            }
        }
    }
    endResetModel();
}
