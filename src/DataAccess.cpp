#include <FleetManager/DataAccess.h>
#include <FleetManager/Utils.h>
#include <algorithm>
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
        << "FuelType"
        << van.fuel_type()
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
    std::string plate_number = van.plate_number();
    std::string::iterator end_pos = std::remove(plate_number.begin(), plate_number.end(), ' ');
    plate_number.erase(end_pos, plate_number.end());

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = m_collection.find_one(document {}
        << "PlateNumber" << plate_number
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

Rentable::Van DataAccess::get_van_in_db_by_plate_number(const std::string& item_to_rent)
{
    auto filter_vans = document {} << "PlateNumber"
                                   << item_to_rent
                                   << finalize;

    bsoncxx::stdx::string_view view {};
    auto returned_document = m_collection.find_one(filter_vans.view());
    if (returned_document.has_value()) {
        view = returned_document->view()["Brand"].get_utf8().value;
        std::string brand = view.to_string();

        view = returned_document->view()["Model"].get_utf8().value;
        std::string model = view.to_string();

        int year = returned_document->view()["Year"].get_int32().value;

        view = returned_document->view()["PlateNumber"].get_utf8().value;
        std::string plate_number = view.to_string();

        int hp = returned_document->view()["HP"].get_int32().value;
        int nbr_doors = returned_document->view()["NumberDoors"].get_int32().value;
        bool is_rented = returned_document->view()["IsRented"].get_bool().value;
        int cat = returned_document->view()["Cat"].get_int32().value;
        bool has_gps = returned_document->view()["GPS"].get_bool().value;

        view = returned_document->view()["LastITV"].get_utf8().value;
        std::string last_itv = view.to_string();

        bool is_accident_ready = returned_document->view()["IsAccidentReady"].get_bool().value;

        view = returned_document->view()["InsuranceName"].get_utf8().value;
        std::string insurance_name = view.to_string();

        view = returned_document->view()["LastInsurance"].get_utf8().value;
        std::string last_insurance_date = view.to_string();

        view = returned_document->view()["FuelType"].get_utf8().value;
        std::string fuel_type = view.to_string();

        return Rentable::Van { brand, model, year, plate_number, hp, nbr_doors, is_rented, cat, has_gps, Utils::Date { Utils::split(last_itv, '-') },
            is_accident_ready, insurance_name, Utils::Date { Utils::split(last_insurance_date, '-') }, fuel_type };
    }

    return Rentable::Van {};
}

Rentable::RentTransaction DataAccess::get_rented_in_db_by_plate_number(const std::string& item_to_checkout)
{
    auto filter_vans = document {} << "PlateNumber"
                                   << item_to_checkout
                                   << finalize;

    bsoncxx::stdx::string_view view {};
    auto returned_document = m_collection.find_one(filter_vans.view());

    if (returned_document.has_value()) {
        view = returned_document->view()["PlateNumber"].get_utf8().value;
        std::string plate_number = view.to_string();

        double days = returned_document->view()["Days"].get_double().value;

        view = returned_document->view()["ClientName"].get_utf8().value;
        std::string client_name = view.to_string();

        view = returned_document->view()["ClientDNI"].get_utf8().value;
        std::string client_DNI = view.to_string();

        view = returned_document->view()["TimeDate"].get_utf8().value;
        std::string time_date = view.to_string();

        view = returned_document->view()["TimeHour"].get_utf8().value;
        std::string time_hour = view.to_string();

        bool is_check_in = returned_document->view()["IsCheckIn"].get_bool().value;

        return Rentable::RentTransaction { plate_number, days, client_name, client_DNI, time_date, time_hour, is_check_in };
    }

    return Rentable::RentTransaction {};
}

std::vector<std::string> DataAccess::get_currently_rented()
{
    std::vector<std::string> currently_rented {};
    auto filter_vans_rented = document {} << "IsRented"
                                          << true
                                          << finalize;

    auto cursor = m_collection.find(filter_vans_rented.view());

    for (auto&& doc : cursor) {
        bsoncxx::stdx::string_view view = doc["PlateNumber"].get_utf8().value;
        std::string plate_number = view.to_string();
        currently_rented.emplace_back(plate_number);
    }

    return currently_rented;
}

DataAccess::DBOp DataAccess::add_rent_transaction_entry(const Rentable::RentTransaction& rent_transaction, bool is_check_in)
{
    spdlog::info(fmt::format("Adding Rent Transaction to DB: {} and Collection: {}", m_db.name().to_string(), m_collection_name));

    if (rent_transaction.client_name().empty() || rent_transaction.client_DNI().empty() || rent_transaction.days() == 0.0) {
        return DataAccess::DBOp::NOK;
    }

    bsoncxx::document::value doc_value = document {}
        << "PlateNumber"
        << rent_transaction.plate_number()
        << "TimeDate"
        << rent_transaction.time_date()
        << "TimeHour"
        << rent_transaction.time_hour()
        << "Days"
        << rent_transaction.days()
        << "ClientName"
        << rent_transaction.client_name()
        << "ClientDNI"
        << rent_transaction.client_DNI()
        << "IsCheckIn"
        << rent_transaction.is_check_in()
        << bsoncxx::builder::stream::finalize;

    bsoncxx::stdx::optional<mongocxx::result::insert_one> insert_result = m_collection.insert_one(doc_value.view());

    mongocxx::collection my_vans_collection = db()["my_vans"];
    bsoncxx::stdx::optional<mongocxx::result::update> result_update = my_vans_collection.update_one(document {} << "PlateNumber" << rent_transaction.plate_number() << bsoncxx::builder::stream::finalize,
        document {} << "$set" << open_document << "IsRented" << is_check_in << close_document << finalize);

    if (result_update) {
        spdlog::info("Number of modified docs: {}", result_update->modified_count());
        return DataAccess::DBOp::OK;
    }

    else
        return DataAccess::DBOp::NOK;
}

}
