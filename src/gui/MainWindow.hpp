#pragma once
#include <QMainWindow>
#include <memory>
#include "Storage.hpp"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>


class QTableView; class QComboBox; class QLineEdit; class QDateEdit; class QDoubleSpinBox; class QPushButton; class QCheckBox; class QLabel;
// QtCharts classes imported via QT_CHARTS_USE_NAMESPACE
class ExpenseModel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent=nullptr);

private slots:
    void onAddExpense();
    void onAddCategory();
    void onMonthChanged();
    void onIncludeProratedChanged(int);

private:
    Storage storage_;
    ExpenseModel* model_{nullptr};

    // UI widgets
    QTableView* table_{nullptr};
    QComboBox* userBox_{nullptr};
    QComboBox* categoryBox_{nullptr};
    QLineEdit* descEdit_{nullptr};
    QDateEdit* dateEdit_{nullptr};
    QDoubleSpinBox* amountEdit_{nullptr};
    QComboBox* typeBox_{nullptr};
    QDateEdit* monthEdit_{nullptr};
    QCheckBox* includeProrated_{nullptr};
    QLabel* totalLabel_{nullptr};

    // Chart
    QChartView* chartView_{nullptr};
    QPieSeries* pie_{nullptr};

    void buildUi();
    void refreshCategories();
    void refreshUsers();
    void refreshSummaryAndChart();
};
