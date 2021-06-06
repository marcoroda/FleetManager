#pragma once
#include <FleetManager/Van.h>
#include <SFML/Graphics/Color.hpp>

namespace GUI {

constexpr unsigned int WIDTH = 1800;
constexpr unsigned int HEIGHT = 700;

Rentable::Van show_add_van();
void show_all_vans(const Data::DataAccess& data, const std::string& suffix);
void add_rentable_to_db(const mongocxx::database& db);
void rent(const mongocxx::database& db);
void show_rent_form(const std::string& item_to_rent);
}