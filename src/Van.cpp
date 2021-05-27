#include <FleetManager/Van.h>
#include <iostream>

namespace Rentable {

void printVanInfo(const Van& my_van)
{
    std::cout << "Van Info: \n";
    std::cout << "plate number: " << my_van.plate_number();
    std::cout << "model: " << my_van.model();
    std::cout << "year: " << my_van.year();
}

double Van::calc_rental_fee(const int& days, const int& cat, const std::string& type_rentable)
{
    return 1.2;
}

} // namespace Van
