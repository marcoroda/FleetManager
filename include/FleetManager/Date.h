#pragma once
#include <vector>

namespace Utils {
struct Date {
    Date() = default;
    Date(int year, int month, int day)
        : m_year(year)
        , m_month(month)
        , m_day(day) {};

    explicit Date(std::vector<int> date)
        : m_year(date.at(0))
        , m_month(date.at(1))
        , m_day(date.at(2)) {};

    int m_year;
    int m_month;
    int m_day;
};
}
