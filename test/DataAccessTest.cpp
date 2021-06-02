#include <FleetManager/DataAccess.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(DataAccess, DBConnection)
{
    mongocxx::instance instance {}; // This should be done only once.
    std::string uri_db { "mongodb://localhost:27017" };
    spdlog::info(fmt::format("Trying to connect to DB at {}", uri_db));
    mongocxx::uri uri(uri_db);
    spdlog::info(fmt::format("Successfully connected to DB at {}", uri_db));
    mongocxx::client client(uri);
    mongocxx::database db = client["mydb"];

    auto data = Data::DataAccess { db, "my_vans" };
    data.list_all();

    EXPECT_EQ(1, 1);
}
