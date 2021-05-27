#pragma once

#include "IRentable.h"
#include <string>

namespace Rentable {

class Van : public IRentable {
public:
    Van() = default;

    Van(std::string brand,
        std::string model,
        int year,
        std::string plate_number,
        int hp,
        int doors)
        : m_brand(std::move(brand))
        , m_model(std::move(model))
        , m_year(year)
        , m_plate_number(std::move(plate_number))
        , m_hp(hp)
        , m_doors(doors) {};

    [[nodiscard]] std::string brand() const { return m_brand; }
    void set_brand(const std::string& brand) { m_brand = brand; }
    [[nodiscard]] std::string model() const { return m_model; }
    void set_model(const std::string& model) { m_model; }
    [[nodiscard]] int year() const { return m_year; }
    void set_year(const int& year) { m_year = year; }
    [[nodiscard]] std::string plate_number() const { return m_plate_number; }
    void set_plate_number(const std::string& plate_number) { m_plate_number = plate_number; }
    [[nodiscard]] int hp() const { return m_hp; }
    void set_hp(const int& hp) { m_hp = hp; }
    [[nodiscard]] int doors() const { return m_doors; }
    void set_doors(const int& doors) { m_doors = doors; }

    void print_van_info() const;

    virtual double calc_rental_fee(const int& days, const int& cat, const std::string& type_rentable) override;
private:
    std::string m_brand;
    std::string m_model;
    std::string m_plate_number;
    int m_hp {};
    int m_year {};
    int m_doors {};
    //        bool m_is_rented;
    //        int cat;
    //        int m_status;
    //        bool m_has_gps;
    //        Date m_last_ITV;
    //        bool m_is_accident_ready;
    //        Insurance m_insurance_name;
    //        Date m_last_insurance_date;
};
} // namespace van
