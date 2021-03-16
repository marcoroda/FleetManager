#include "Van.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define _DEBUG_

namespace van
{
	Van::Van() { }
	Van::Van(std::string brand, int year, std::string plate_number) : brand(brand), year(year), plate_number(plate_number)
	{
		//std::cout << "Custom Ctor used!!" << std::endl;
	}

	void Van::printVanInfo()
	{
		std::cout << "** Brand: " << getBrand() << ", Year: " << getYear() << ", Plate Number: " << getPlateNumber();
		std::cout << "\n";
	}

	// D:\Projs_Work_Nerd_Stuff\DumpMe\CMakeMarbeVan\src\vans_database.csv
	std::vector<Van> loadVansFromFile(std::string file_name, const char delimiter)
	{
		std::cout << "\n\nLoading Vans from "<< file_name << "... " << std::endl;

		std::ifstream my_file(file_name);
		if (!my_file.is_open())
			std::cout << " Failed to open" << std::endl;

		// Parsed line bu line into a vector of strings
		std::string line; // tmp string var
		std::vector<std::string> raw_row;

		while (getline(my_file, line))
		{
			std::istringstream iss{ line }; // string stream from line
			std::string elem;               // current elem (in between delimiter)
			while (getline(iss, elem, delimiter)) {
				raw_row.push_back(elem); // add the token to the 
			}
		}

		// TODO: Replace by a lambda expression!!
		auto nbr_vans = raw_row.size() / van::nbr_fields_p_van;
		std::cout << nbr_vans <<  " loaded vans from .csv file." << std::endl;
		// Create vector <Van> with loaded Vans
		std::vector<Van> vans_db = createVanDb(raw_row);
		std::cout << "Loading completed ..." << std::endl;

		return vans_db;
	}

	void printVansFromDB(std::vector<Van> vans_db)
	{
		std::cout << "Priting Vans in DB: " << std::endl;
		for (auto& van : vans_db)
		{
			van.printVanInfo();
		}
	}

	std::vector<Van> createVanDb(const std::vector<std::string>& raw_row)
	{
		std::vector<Van> van_db;
		auto num_vans_in_db = raw_row.size() / van::nbr_fields_p_van;
		van_db.reserve(num_vans_in_db);

		for (int i = 0; i < raw_row.size(); i = i + van::nbr_fields_p_van)
		{
			std::string brand = raw_row.at(i);
			int year = std::stoi(raw_row.at(i+1));
			std::string plate_number = raw_row.at(i+2);
			van_db.push_back(Van(brand, year, plate_number));
		}

		return van_db;
	}
}

