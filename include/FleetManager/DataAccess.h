#pragma once

#include "RentTransaction.h"
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
        EXISTS,
    };

    [[nodiscard]] mongocxx::collection collection() const { return m_collection; }
    [[nodiscard]] mongocxx::database db() const { return m_db; };
    [[nodiscard]] DBOp add(const Rentable::Van& van);
    [[nodiscard]] DBOp remove(const Rentable::Van& van);
    [[nodiscard]] bool exists(const Rentable::Van& van);
    [[nodiscard]] std::vector<std::string> get_available_for_renting();
    [[nodiscard]] Rentable::Van get_by_plate_number(const std::string& item_to_rent);
    void list_all();
    [[nodiscard]] DBOp add_rent_transaction_entry(const Rentable::RentTransaction& rent_transaction);

private:
    mongocxx::database m_db;
    mongocxx::collection m_collection;
    std::string m_collection_name;
};
}
