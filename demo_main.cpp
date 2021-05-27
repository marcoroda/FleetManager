#include <FleetManager/Van.h>
#include <FleetManager/app.h>
#include <FleetManager/DataAccess.h>

int main()
{
    auto my_van = Rentable::Van { "Opel", "Zafira", 2012, "4148 JXX", 120, 5 };

    mongocxx::instance instance {}; // This should be done only once.
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["mydb"];

    auto data_access = Data::DataAccess { db, "vans" };
    data_access.add_van(my_van);
    data_access.delete_van(my_van);

    return EXIT_SUCCESS;
}