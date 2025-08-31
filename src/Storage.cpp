#include "Storage.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>

using std::string; using std::vector;

static string sanitize(const string& s) {
    string out = s;
    for (char& c : out) { if (c == '\n' || c == '\r') c = ' '; }
    return out;
}

Storage::Storage(std::string dataDir) : dataDir_(std::move(dataDir)) {}

bool Storage::load() {
    std::filesystem::create_directories(dataDir_);
    bool ok1 = loadCategories();
    bool ok2 = loadExpenses();
    if (categories_.empty()) {
        // Default categories
        const char* defaults[] = {
            "Alquiler","Hipoteca","Supermercado","Restaurantes","Transporte","Gasolina",
            "Salud","Seguros","IBI","Internet","Luz","Agua","Ocio","Educación",
            "Mascotas","Regalos","Hogar","Ropa","Viajes","Impuestos","Suscripciones","Otros"
        };
        for (auto c : defaults) categories_.push_back(Category{c});
        saveCategories();
    }
    return ok1 && ok2;
}

bool Storage::save() const {
    std::filesystem::create_directories(dataDir_);
    bool ok1 = saveCategories();
    bool ok2 = saveExpenses();
    return ok1 && ok2;
}

vector<Expense>& Storage::expenses() { return expenses_; }
const vector<Expense>& Storage::expenses() const { return expenses_; }
vector<Category>& Storage::categories() { return categories_; }
const vector<Category>& Storage::categories() const { return categories_; }

bool Storage::addCategory(const string& name) {
    for (const auto& c : categories_) if (c.name == name) return false;
    categories_.push_back(Category{name});
    return true;
}

void Storage::addExpense(const Expense& e) {
    expenses_.push_back(e);
}

bool Storage::loadCategories() {
    categories_.clear();
    std::ifstream in(dataDir_ + "/categories.csv");
    if (!in.is_open()) return true; // no file yet
    string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        categories_.push_back(Category{line});
    }
    return true;
}

bool Storage::loadExpenses() {
    expenses_.clear();
    std::ifstream in(dataDir_ + "/expenses.csv");
    if (!in.is_open()) return true;
    string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        string id,user,dateS,amountS,category,description,typeS;
        std::getline(ss,id,';');
        std::getline(ss,user,';');
        std::getline(ss,dateS,';');
        std::getline(ss,amountS,';');
        std::getline(ss,category,';');
        std::getline(ss,description,';');
        std::getline(ss,typeS,';');
        Expense e;
        e.id = id;
        e.user = user;
        try { e.date = Date::parse(dateS); } catch(...) { continue; }
        e.amount = std::stod(amountS);
        e.category = category;
        e.description = description;
        e.type = (typeS == "AnnualProrated" ? ExpenseType::AnnualProrated : ExpenseType::Normal);
        expenses_.push_back(e);
    }
    return true;
}

bool Storage::saveCategories() const {
    std::ofstream out(dataDir_ + "/categories.csv", std::ios::trunc);
    if (!out.is_open()) return false;
    for (const auto& c : categories_) {
        out << sanitize(c.name) << "\n";
    }
    return true;
}

bool Storage::saveExpenses() const {
    std::ofstream out(dataDir_ + "/expenses.csv", std::ios::trunc);
    if (!out.is_open()) return false;
    for (const auto& e : expenses_) {
        out << sanitize(e.id) << ';'
            << sanitize(e.user) << ';'
            << e.date.toString() << ';'
            << std::fixed << std::setprecision(2) << e.amount << ';'
            << sanitize(e.category) << ';'
            << sanitize(e.description) << ';'
            << (e.type==ExpenseType::AnnualProrated?"AnnualProrated":"Normal")
            << "\n";
    }
    return true;
}

