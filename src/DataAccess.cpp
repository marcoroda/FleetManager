#include <FleetManager/DataAccess.h>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>
#include <mongocxx/client.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

namespace Data {

DataAccess::DBOp DataAccess::add_van(const Rentable::Van& van)
{
    std::cout << "Adding Van to DB: " << m_db.name() << " and Collection: " << m_collection_name << "\n";

    auto builder = document {};
    bsoncxx::document::value doc_value = builder
        << "Brand"
        << van.brand()
        << "Model"
        << van.model()
        << "PlateNumber"
        << van.plate_number()
        << "HP"
        << van.hp()
        << "Year"
        << van.year()
        << "NumberDoors"
        << van.doors()
        << bsoncxx::builder::stream::finalize;

    bsoncxx::stdx::optional<mongocxx::result::insert_one> result = m_collection.insert_one(doc_value.view());

    mongocxx::cursor cursor = m_collection.find({});
    for (auto& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << "\n";
    }
    return DataAccess::DBOp::OK;
}

DataAccess::DBOp DataAccess::delete_van(const Rentable::Van& van)
{
    std::cout << "Deleting Van from DB: " << m_db.name() << " and Collection: " << m_collection_name << "\n";
    return DataAccess::DBOp::OK;
}

}
