// Simple CSV storage for expenses and categories
#pragma once
#include <string>
#include <vector>
#include "Models.hpp"

class Storage {
public:
    explicit Storage(std::string dataDir = "data");

    bool load();
    bool save() const;

    std::vector<Expense>& expenses();
    const std::vector<Expense>& expenses() const;

    std::vector<Category>& categories();
    const std::vector<Category>& categories() const;

    bool addCategory(const std::string& name);
    void addExpense(const Expense& e);

private:
    std::string dataDir_;
    std::vector<Expense> expenses_;
    std::vector<Category> categories_;

    bool loadExpenses();
    bool loadCategories();
    bool saveExpenses() const;
    bool saveCategories() const;
};

