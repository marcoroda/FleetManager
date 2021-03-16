#include <iostream>
#include "utils.h"
#include "Van.h"

void menu(void)
{
	std::cout << "\n\n";
	std::cout << " *******************************" << std::endl;
	std::cout << " ** MarbeVan Managment System **" << std::endl;
	std::cout << " *******************************" << std::endl;

	std::cout << "1 - Add Van to the system;" << std::endl;
	std::cout << "2 - Delete Van from the system;" << std::endl;

	char sel;
	std::cin >> sel;

	switch (sel)
	{
	case '1':
		std::cout << "** Add Van **";
		break;
	case '2':
		std::cout << "** Delete Van **";
		break;
	case '3':
		std::cout << "** Show Van Info **";
		break;
		// you can have any number of case statements.
	default: //Optional
		std::cout << "n.a.";
	}
}