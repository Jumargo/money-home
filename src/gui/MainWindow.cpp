#include "MainWindow.hpp"
#include "ExpenseModel.hpp"
#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QStandardPaths>
#include <QDir>

static std::string defaultDataDir(){
    const QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(base);
    return base.toStdString();
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), storage_(defaultDataDir()) {
    storage_.load();
    buildUi();
}

void MainWindow::buildUi(){
    setWindowTitle(QStringLiteral("Money Home"));
    resize(1100, 700);

    auto* central = new QWidget(this);
    auto* mainLayout = new QVBoxLayout();

    // Top controls
    auto* topBar = new QHBoxLayout();
    monthEdit_ = new QDateEdit(QDate::currentDate(), this);
    monthEdit_->setDisplayFormat("yyyy-MM");
    monthEdit_->setCalendarPopup(true);
    includeProrated_ = new QCheckBox("Incluir prorrateos", this);
    includeProrated_->setChecked(true);
    totalLabel_ = new QLabel("Total: 0.00", this);
    totalLabel_->setStyleSheet("font-weight:600;font-size:16px");
    topBar->addWidget(new QLabel("Mes:"));
    topBar->addWidget(monthEdit_);
    topBar->addSpacing(10);
    topBar->addWidget(includeProrated_);
    topBar->addStretch();
    topBar->addWidget(totalLabel_);

    // Table
    table_ = new QTableView(this);
    table_->setAlternatingRowColors(true);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model_ = new ExpenseModel(storage_, this);
    const auto ym = monthEdit_->date().toString("yyyy-MM");
    model_->setMonth(ym);
    model_->setIncludeProrated(includeProrated_->isChecked());
    table_->setModel(model_);
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Form to add expense
    auto* form = new QGroupBox("Añadir gasto", this);
    auto* formL = new QGridLayout();
    userBox_ = new QComboBox(this);
    categoryBox_ = new QComboBox(this);
    descEdit_ = new QLineEdit(this);
    dateEdit_ = new QDateEdit(QDate::currentDate(), this);
    dateEdit_->setDisplayFormat("yyyy-MM-dd");
    dateEdit_->setCalendarPopup(true);
    amountEdit_ = new QDoubleSpinBox(this);
    amountEdit_->setRange(0.01, 1e9);
    amountEdit_->setDecimals(2);
    amountEdit_->setPrefix("€ ");
    typeBox_ = new QComboBox(this);
    typeBox_->addItems({"Normal","Anual prorrateado"});
    auto* addBtn = new QPushButton("Añadir", this);
    auto* addCatBtn = new QPushButton("Nueva categoría", this);

    formL->addWidget(new QLabel("Usuario:"), 0,0); formL->addWidget(userBox_,0,1);
    formL->addWidget(new QLabel("Categoría:"), 0,2); formL->addWidget(categoryBox_,0,3); formL->addWidget(addCatBtn,0,4);
    formL->addWidget(new QLabel("Fecha:"), 1,0); formL->addWidget(dateEdit_,1,1);
    formL->addWidget(new QLabel("Importe:"),1,2); formL->addWidget(amountEdit_,1,3);
    formL->addWidget(new QLabel("Tipo:"),2,0); formL->addWidget(typeBox_,2,1);
    formL->addWidget(new QLabel("Descripción:"),2,2); formL->addWidget(descEdit_,2,3);
    formL->addWidget(addBtn,2,4);
    form->setLayout(formL);

    // Chart
    pie_ = new QPieSeries(this);
    auto* chart = new QChart();
    chart->setTitle("Gasto por categoría");
    chart->addSeries(pie_);
    chart->legend()->setAlignment(Qt::AlignRight);
    chartView_ = new QChartView(chart, this);
    chartView_->setRenderHint(QPainter::Antialiasing);
    chartView_->setMinimumHeight(260);

    auto* lower = new QHBoxLayout();
    lower->addWidget(form, 1);
    lower->addWidget(chartView_, 1);

    mainLayout->addLayout(topBar);
    mainLayout->addWidget(table_, 1);
    mainLayout->addLayout(lower);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    // Style
    qApp->setStyle("Fusion");
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, QColor(245,246,250));
    pal.setColor(QPalette::Base, QColor(255,255,255));
    pal.setColor(QPalette::AlternateBase, QColor(245,245,245));
    pal.setColor(QPalette::Highlight, QColor(52,120,246));
    pal.setColor(QPalette::Button, QColor(255,255,255));
    qApp->setPalette(pal);

    refreshUsers();
    refreshCategories();
    refreshSummaryAndChart();

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddExpense);
    connect(addCatBtn, &QPushButton::clicked, this, &MainWindow::onAddCategory);
    connect(monthEdit_, &QDateEdit::dateChanged, this, &MainWindow::onMonthChanged);
    connect(includeProrated_, &QCheckBox::stateChanged, this, &MainWindow::onIncludeProratedChanged);
}

void MainWindow::refreshUsers(){
    userBox_->clear();
    userBox_->addItems({"Yo","Pareja"});
}

void MainWindow::refreshCategories(){
    categoryBox_->clear();
    for(const auto& c : storage_.categories()) categoryBox_->addItem(QString::fromStdString(c.name));
}

void MainWindow::refreshSummaryAndChart(){
    // Update model/month and totals
    const auto ym = monthEdit_->date().toString("yyyy-MM");
    model_->setMonth(ym);
    model_->setIncludeProrated(includeProrated_->isChecked());
    totalLabel_->setText(QString("Total: %1").arg(QString::number(model_->total(),'f',2)));

    // Chart by category
    pie_->clear();
    auto byCat = model_->totalsByCategory();
    for (const auto& kv : byCat) {
        auto slice = pie_->append(kv.first, kv.second);
        if (kv.second <= 0.0) slice->setLabelVisible(false);
        else slice->setLabel(QString("%1 (%2)").arg(kv.first).arg(QString::number(kv.second,'f',0)));
    }
}

void MainWindow::onMonthChanged(){ refreshSummaryAndChart(); }
void MainWindow::onIncludeProratedChanged(int){ refreshSummaryAndChart(); }

void MainWindow::onAddCategory(){
    bool ok=false; auto name = QInputDialog::getText(this, "Nueva categoría", "Nombre:", QLineEdit::Normal, {}, &ok);
    if (!ok || name.trimmed().isEmpty()) return;
    if (storage_.addCategory(name.trimmed().toStdString())){
        storage_.save();
        refreshCategories();
    } else {
        QMessageBox::information(this, "Categoría", "Ya existe la categoría");
    }
}

void MainWindow::onAddExpense(){
    try{
        Expense e;
        e.id = std::to_string(std::time(nullptr));
        e.user = userBox_->currentText().toStdString();
        auto qd = dateEdit_->date(); e.date = Date{qd.year(), qd.month(), qd.day()};
        e.amount = amountEdit_->value();
        e.category = categoryBox_->currentText().toStdString();
        e.description = descEdit_->text().toStdString();
        e.type = (typeBox_->currentIndex()==1?ExpenseType::AnnualProrated:ExpenseType::Normal);
        if (e.amount <= 0.0) { QMessageBox::warning(this, "Validación", "El importe debe ser positivo"); return; }
        storage_.addExpense(e);
        storage_.save();
        descEdit_->clear(); amountEdit_->setValue(0.0);
        refreshSummaryAndChart();
    } catch(const std::exception& ex){ QMessageBox::critical(this, "Error", ex.what()); }
}
