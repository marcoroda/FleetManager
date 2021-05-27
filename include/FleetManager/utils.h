#pragma once

namespace Utils {

void menu();
void clear_console();

class Date {
public:
    Date() = default;
    Date(int year, int month, int day)
        : m_year(year)
        , m_month(month)
        , m_day(day)
    {
    }

private:
    int m_year;
    int m_month;
    int m_day;
};

}
