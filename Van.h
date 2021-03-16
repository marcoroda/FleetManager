#pragma once
#include <string>
#include <vector>

namespace van
{
	const uint32_t nbr_fields_p_van = 3;
	const std::string file_vans_bd = "vans_db.csv";
	
	class Van
	{
	public:
		Van();
		Van(std::string, int year, std::string);
		std::string getBrand(void) { return brand; }
		void setBrand(std::string brand) { this->brand = brand; }
		int getYear(void) { return year; }
		void setYear(int year) { this->year = year; }
		std::string getPlateNumber(void) { return plate_number; }
		void setPlateNumber(std::string plate) { this->plate_number = plate; }
		void printVanInfo(void);

	private:
		std::string brand;
		int year;
		std::string plate_number;
	};

	std::vector<Van> loadVansFromFile(std::string file_name, const char delimiter);
	void printVansFromDB(std::vector<Van> vans_db);
	std::vector<Van> createVanDb(const std::vector<std::string>& raw_row);
}
