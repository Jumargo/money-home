// Simple date handling for YYYY-MM-DD and month math
#pragma once
#include <string>
#include <stdexcept>
#include <cstdio>

struct Date {
    int year{0};
    int month{0};
    int day{0};

    static Date parse(const std::string& s) {
        Date d{};
        if (std::sscanf(s.c_str(), "%d-%d-%d", &d.year, &d.month, &d.day) != 3) {
            throw std::runtime_error("Fecha inválida, usa YYYY-MM-DD");
        }
        if (d.month < 1 || d.month > 12 || d.day < 1 || d.day > 31) {
            throw std::runtime_error("Fecha fuera de rango");
        }
        return d;
    }

    std::string toString() const {
        char buf[11];
        std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d", year, month, day);
        return std::string(buf);
    }

    std::string monthKey() const {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%04d-%02d", year, month);
        return std::string(buf);
    }
};

inline int monthsBetween(const std::string& fromYYYYMM, const std::string& toYYYYMM) {
    int fy=0,fm=0; int ty=0,tm=0;
    if (std::sscanf(fromYYYYMM.c_str(), "%d-%d", &fy, &fm) != 2) return 0;
    if (std::sscanf(toYYYYMM.c_str(), "%d-%d", &ty, &tm) != 2) return 0;
    return (ty*12 + tm) - (fy*12 + fm);
}

inline std::string addMonths(const std::string& yyyymm, int delta) {
    int y=0,m=0; std::sscanf(yyyymm.c_str(), "%d-%d", &y, &m);
    int total = y*12 + (m-1) + delta;
    int ny = total / 12;
    int nm = (total % 12) + 1;
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%04d-%02d", ny, nm);
    return std::string(buf);
}

