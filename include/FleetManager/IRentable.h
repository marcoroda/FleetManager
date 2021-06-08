#pragma once
#include <string>

class IRentable {
    virtual double calc_rental_fee(const double& days, const int& cat) = 0;
};
