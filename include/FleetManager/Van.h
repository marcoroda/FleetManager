#pragma once

#include <FleetManager/Date.h>
#include <FleetManager/IRentable.h>
#include <string>

namespace Rentable {

class Van : public IRentable {
public:
    explicit Van(std::string brand,
        std::string model,
        int year,
        std::string plate_number,
        int hp,
        int doors,
        bool is_rented,
        int cat,
        bool has_gps,
        Utils::Date last_ITV,
        bool is_accident_ready,
        std::string insurance_name,
        Utils::Date last_insurance_date)
        : m_brand(std::move(brand))
        , m_model(std::move(model))
        , m_year(year)
        , m_plate_number(std::move(plate_number))
        , m_hp(hp)
        , m_doors(doors)
        , m_is_rented(is_rented)
        , m_cat(cat)
        , m_has_gps(has_gps)
        , m_last_ITV(last_ITV)
        , m_is_accident_ready(is_accident_ready)
        , m_insurance_name(std::move(insurance_name))
        , m_last_insurance_date(last_insurance_date) {};

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
    [[nodiscard]] bool is_rented() const { return m_is_rented; }
    void set_is_rented(const bool& is_rented) { m_is_rented = is_rented; }
    [[nodiscard]] bool cat() const { return m_cat; }
    void set_cat(const int& cat) { m_cat = cat; }
    [[nodiscard]] bool has_gps() const { return m_has_gps; }
    void set_has_gps(const bool& has_gps) { m_has_gps = has_gps; }
    [[nodiscard]] Utils::Date last_ITV() const { return m_last_ITV; }
    void set_last_ITV(const Utils::Date& date) { m_last_ITV = date; }
    [[nodiscard]] bool is_accident_ready() const { return m_is_accident_ready; }
    void set_is_accident_ready(const bool& accident_ready) { m_is_accident_ready = accident_ready; }
    [[nodiscard]] std::string insurance_name() const { return m_insurance_name; }
    void set_insurance_name(const std::string& insurance_name) { m_insurance_name = insurance_name; }
    [[nodiscard]] Utils::Date last_insurance_date() const { return m_last_insurance_date; }
    void set_last_insurance_date(const Utils::Date& date) { m_last_insurance_date = date; }

    void print_van_info() const;

    double calc_rental_fee(const int& days, const int& cat, const std::string& type_rentable) override;

private:
    std::string m_brand;
    std::string m_model;
    std::string m_plate_number;
    int m_hp {};
    int m_year {};
    int m_doors {};
    bool m_is_rented {};
    int m_cat;
    bool m_has_gps;
    Utils::Date m_last_ITV;
    bool m_is_accident_ready;
    std::string m_insurance_name;
    Utils::Date m_last_insurance_date;
};

}
