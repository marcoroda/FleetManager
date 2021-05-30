#pragma once
#include <FleetManager/Van.h>

namespace GUI {

constexpr unsigned int WIDTH = 1500;
constexpr unsigned int HEIGHT = 800;

Rentable::Van show_add_van();
void show_all_vans(const Data::DataAccess &data, const std::string& suffix);
}