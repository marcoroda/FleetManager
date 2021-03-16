#include "Van.h"
#include "utils.h"
#include <cstdlib>
#include <iostream>
#include <string>

int main(void) {
    std::vector<van::Van> vans_db = van::loadVansFromFile(van::file_vans_bd, ',');
    van::printVansFromDB(vans_db);

    utils::menu();

    return EXIT_SUCCESS;
}