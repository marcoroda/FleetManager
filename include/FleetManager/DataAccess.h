#pragma once

#include <FleetManager/Van.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <string>

namespace Data {

class DataAccess {
public:
    DataAccess(const mongocxx::database& db, std::string collection_name)
        : m_db(db)
        , m_collection_name(std::move(collection_name))
    {
        m_collection = db[m_collection_name];
    }

    enum class DBOp {
        OK,
        NOK,
    };

    [[nodiscard]] mongocxx::collection collection() const { return m_collection; }
    [[nodiscard]] DBOp add_van(const Rentable::Van& van);
    [[nodiscard]] DBOp delete_van(const Rentable::Van& van);
    [[nodiscard]] bool exists_van(const Rentable::Van& van);
    void list_all();

private:
    mongocxx::database m_db;
    mongocxx::collection m_collection;
    std::string m_collection_name;
};
}
