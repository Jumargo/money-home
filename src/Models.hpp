// Basic domain models
#pragma once
#include <string>
#include "Date.hpp"

enum class ExpenseType { Normal, AnnualProrated };

struct Expense {
    std::string id;           // simple unique id (timestamp-based)
    std::string user;         // "Yo" | "Pareja" | custom
    Date date;                // YYYY-MM-DD
    double amount{0.0};       // positive
    std::string category;     // category name
    std::string description;  // optional
    ExpenseType type{ExpenseType::Normal};
};

struct Category {
    std::string name;
};

