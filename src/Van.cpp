#include <FleetManager/Van.h>
#include <iostream>

namespace Rentable {

void Van::print_van_info() const
{
    std::cout << "Van Info: \n";
    std::cout << "Brand: " << m_brand << ", ";
    std::cout << "Model: " << m_model << ", ";
    std::cout << "Plate Number: " << m_plate_number << ", ";
    std::cout << "Year: " << m_year << ", ";
    std::cout << "HP: " << m_hp << ", ";
    std::cout << "Doors: " << m_doors << ", ";
    std::cout << "Is Rented: " << m_is_rented << "\n";
}

double Van::calc_rental_fee(const double& days, const int& cat)
{
    return 60 * days * float(cat);
}

} // namespace Van
