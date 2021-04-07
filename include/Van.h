#pragma once
#include <string>
#include <utility>
#include <vector>

namespace van {
    const uint32_t nbr_fields_p_van = 3;
    const std::string file_vans_bd = "../db/vans_db.csv";

    class Van {
      public:
        Van() = default;
        Van(std::string brand, int year, std::string plate_number);
        std::string getBrand() { return brand; }
        void setBrand(std::string brand_) { this->brand = std::move(brand_); }
        int getYear() const { return year; }
        void setYear(int year_) { this->year = year_; }
        std::string getPlateNumber() const { return plate_number; }
        void setPlateNumber(std::string plate_) { this->plate_number = std::move(plate_); }
        void printVanInfo();

      private:
        std::string brand;
        int year;
        std::string plate_number;
        bool is_rented{false};
    };

    std::vector<Van> loadVansFromFile(const std::string& file_name, char delimiter);
    void printVansFromDB(std::vector<Van> vans_db);
    std::vector<Van> createVanDb(const std::vector<std::string>& raw_row);
} // namespace van
