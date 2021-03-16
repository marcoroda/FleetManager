#include <iostream>
#include "Van.h"
#include "utils.h"
#include <string>


int main(void)
{
	/* Meunu */
	std::vector<van::Van> vans_db = van::loadVansFromFile(van::file_vans_bd, ',');
	van::printVansFromDB(vans_db);

	menu();


	return 1;
}

//void parseVanObject(std::vector<std::string>& obj_data)
//{
//	Van(obj_data.at(0), obj_data.at(1), obj_data.at(2));
//}


//}
//
