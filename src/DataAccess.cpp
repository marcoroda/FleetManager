#include <FleetManager/DataAccess.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <fmt/format.h>
#include <iostream>
#include <mongocxx/client.hpp>
#include <spdlog/spdlog.h>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

namespace Data {

DataAccess::DBOp DataAccess::add(const Rentable::Van& van)
{
    spdlog::info(fmt::format("Adding Van to DB: {} and Collection: {}", m_db.name().to_string(), m_collection_name));
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
        << "IsRented"
        << van.is_rented()
        << "Cat"
        << van.cat()
        << "GPS"
        << van.has_gps()
        << "LastITV"
        //        << van.last_ITV()
        << "2021-04-12"
        << "IsAccidentReady"
        << van.is_accident_ready()
        << "InsuranceName"
        << van.insurance_name()
        << "LastInsurance"
        //        << van.last_insurance_date()
        << "2021-04-12"
        << bsoncxx::builder::stream::finalize;

    if (!exists(van)) {
        bsoncxx::stdx::optional<mongocxx::result::insert_one> result = m_collection.insert_one(doc_value.view());
    } else {
        return DataAccess::DBOp::EXISTS;
    }

    return DataAccess::DBOp::OK;
}

DataAccess::DBOp DataAccess::remove(const Rentable::Van& van)
{
    std::cout << "Deleting Van from DB: " << m_db.name() << " and Collection: " << m_collection_name << "\n";
    return DataAccess::DBOp::OK;
}

bool DataAccess::exists(const Rentable::Van& van)
{
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = m_collection.find_one(document {}
        << "PlateNumber" << van.plate_number()
        << finalize);

    if (maybe_result) {
        return true;
    }
    return false;
}

void DataAccess::list_all()
{
    mongocxx::cursor cursor = m_collection.find({});
    for (auto& doc : cursor) {
        spdlog::info(bsoncxx::to_json(doc));
    }
}
std::vector<std::string> DataAccess::get_available_for_renting()
{
    std::vector<std::string> my_available_vans {};

    auto filter_available_vans = document {} << "IsRented"
                                             << false
                                             << finalize;
    auto cursor = m_collection.find(filter_available_vans.view());
    for (auto&& doc : cursor) {
        bsoncxx::stdx::string_view view = doc["PlateNumber"].get_utf8().value;
        std::string plate_number = view.to_string();
        my_available_vans.emplace_back(plate_number);
    }

    return my_available_vans;
}

}
