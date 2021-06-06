#pragma once
#include <string>

class IRentable {
    virtual float calc_rental_fee(const float& days, const int& cat) = 0;
};
