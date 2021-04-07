#include "utils.h"
#include "Van.h"
#include <iostream>

namespace utils {
    void clear_console() {
        std::cout << "\x1B[2J\x1B[H";
    }
    void menu(void) {
        utils::clear_console();
        std::cout << "\n\n";
        std::cout << " *******************************" << std::endl;
        std::cout << " ** MarbeVan Managment System **" << std::endl;
        std::cout << " *******************************" << std::endl;

        std::cout << "1 - Add Van to the system;" << std::endl;
        std::cout << "2 - Delete Van from the system;" << std::endl;
        std::cout << "3 - Show Van info;" << std::endl;

        char sel{'0'};

        do {
            std::cin >> sel;
            switch(sel) {
                case '1':
                    std::cout << "** List Vans in database **";
                    // Van::list_vans();
                    break;
                case '2':
                    std::cout << "** Delete Van **";
                    break;
                case '3':
                    std::cout << "** Show Van Info **";
                    break;
                default:
                    std::cout << "** Not Applicable... **";
                    utils::clear_console();
                    utils::menu();
                    break;
            }
        } while(sel != '1' || '2' || '3');
    }
} // namespace utils
