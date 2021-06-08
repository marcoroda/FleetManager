#include <FleetManager/DataAccess.h>
#include <FleetManager/Date.h>
#include <FleetManager/RentTransaction.h>
#include <FleetManager/Utils.h>
#include <FleetManager/Van.h>
#include <FleetManager/imgui_app.h>
#include <FleetManager/wx_app.h>
#include <array>
#include <chrono>
#include <fmt/format.h>
#include <imgui.h>
#include <mongocxx/cursor.hpp>
#include <spdlog/spdlog.h>

namespace GUI {

Rentable::Van show_add_van()
{
    static std::array<char, 15> brand_input { "Opel" };
    static std::array<char, 15> model_input { "Zafira" };
    static int year_input { 2020 };
    static std::array<char, 20> plate_number_input { "4148 JXX" };
    static int hp_input { 110 };
    static int nbr_doors_input { 5 };
    static bool is_rented_input { false };
    static std::vector<std::string> cat_input { "1", "2", "3" };
    static std::vector<std::string> has_gps_input { "True", "False" };
    static std::array<char, 11> last_ITV_date_input { "2020-04-12" };
    static std::vector<std::string> is_accident_ready_input { "True", "False" };
    static std::array<char, 11> last_insurance_date_input { "2020-04-12" };
    static std::vector<std::string> fuel_type_input { "Diesel", "Gasoline", "Electric" };

    ImGui::PushItemWidth(300);
    ImGui::InputText("Insert Brand", brand_input.data(), brand_input.size());
    ImGui::InputText("Insert Model", model_input.data(), model_input.size());
    ImGui::InputInt("Insert Year of Manufacture", &year_input, 1, 100);
    ImGui::InputText("Insert Plate Number", plate_number_input.data(), plate_number_input.size());
    ImGui::InputInt("Insert HP", &hp_input, 1, 1000);
    ImGui::InputInt("Insert Number of Doors", &nbr_doors_input, 1, 100);

    static std::string cat = cat_input.at(0);
    if (ImGui::BeginCombo("Van Category", cat.data())) {
        for (auto& item : cat_input) {
            bool is_selected = (cat == item); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(item.data(), is_selected))
                cat = item;
        }
        ImGui::EndCombo();
    }

    static std::string gps = has_gps_input.at(1);
    if (ImGui::BeginCombo("Has GPS?", gps.data())) {
        for (auto& item : has_gps_input) {
            bool is_selected = (gps == item); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(item.data(), is_selected))
                gps = item;
        }
        ImGui::EndCombo();
    }

    ImGui::InputText("Insert Date of last ITV (format: 1990-04-12)", last_ITV_date_input.data(), last_ITV_date_input.size());
    std::string last_ITV_date_str { std::begin(last_ITV_date_input), std::end(last_ITV_date_input) };
    Utils::Date last_ITV_date {};
    if (last_ITV_date_str.size() == 10) {
        std::vector<int> input_date = Utils::split(last_ITV_date_str, '-');
        last_ITV_date = Utils::Date { input_date };
    }

    static std::string accident_ready = is_accident_ready_input.at(1);
    if (ImGui::BeginCombo("Is Accident Ready?", accident_ready.data())) {
        for (auto& item : is_accident_ready_input) {
            bool is_selected = (accident_ready == item); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(item.data(), is_selected))
                accident_ready = item;
        }
        ImGui::EndCombo();
    }

    ImGui::InputText("Insert Date of last Insurance (format: 1990-04-12)", last_insurance_date_input.data(), last_insurance_date_input.size());
    std::string last_insurance_date_str { std::begin(last_insurance_date_input), std::end(last_insurance_date_input) };
    Utils::Date last_insurance_date {};
    if (last_insurance_date_str.size() == 10) {
        std::vector<int> input_date = Utils::split(last_insurance_date_str, '-');
        last_insurance_date = Utils::Date { input_date };
    }

    static std::string fuel_type = fuel_type_input.at(1);
    if (ImGui::BeginCombo("Insert Fuel Type", fuel_type.data())) {
        for (auto& item : fuel_type_input) {
            bool is_selected = (fuel_type == item); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(item.data(), is_selected))
                fuel_type = item;
        }
        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();

    std::string plate_number { plate_number_input.data() };
    std::string::iterator end_pos = std::remove(plate_number.begin(), plate_number.end(), ' ');
    plate_number.erase(end_pos, plate_number.end());

    return Rentable::Van {
        brand_input.data(),
        model_input.data(),
        year_input,
        plate_number,
        hp_input,
        nbr_doors_input,
        false,
        std::stoi(cat),
        (gps == "True"),
        last_ITV_date,
        (accident_ready == "True"),
        "AXA",
        last_insurance_date,
        fuel_type,
    };
}

void show_all_vans(const Data::DataAccess& data, const std::string& suffix)
{
    std::string s = fmt::format("Current Added Vans to DB {}", suffix);
    ImGui::Begin(s.data());
    mongocxx::cursor cursor = data.collection().find({});
    static bool selected = false;
    for (auto& doc : cursor) {
        std::string current_doc = bsoncxx::to_json(*cursor.begin());
        ImGui::Selectable(current_doc.data(), &selected);
    }
    ImGui::End();
}

void add_rentable_to_db(const mongocxx::database& db)
{

    static bool btn_st_add_van { false };
    static bool show_current_vans { false };

    auto inserted_van = GUI::show_add_van();
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    for (int nbr_spacing = 0; nbr_spacing < 3; ++nbr_spacing)
        ImGui::Spacing();
    btn_st_add_van = ImGui::Button("Add Van");
    ImGui::PopFont();
    if (btn_st_add_van) {
        inserted_van.print_van_info();
        auto data_access = Data::DataAccess { db, "my_vans" };
        if (data_access.add(inserted_van) != Data::DataAccess::DBOp::OK) {
            spdlog::info("Van already exist in the Database!");
            ImGui::OpenPopup("Van already exists!");
        } else {
            spdlog::info("Van added successfully to Database!");
            ImGui::OpenPopup("Van added successfully!");
        }
    }
    if (ImGui::BeginPopupModal("Van already exists!", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("The inserted plate number already exists in the Database");
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(500, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Van added successfully!", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(500, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::Checkbox("Show vans in database", &show_current_vans);
    if (show_current_vans) {
        auto data = Data::DataAccess { db, "my_vans" };
        GUI::show_all_vans(data, "collection: my_vans");
    }
}

void rent_check_in(const mongocxx::database& db)
{
    static bool btn_st_rent_van { false };
    static bool show_current_rented_vans { false };

    auto data_access = Data::DataAccess { db, "my_vans" };
    std::vector<std::string> available_vans_plate_number = data_access.get_available_for_renting();

    static std::string item_to_rent {};
    if (available_vans_plate_number.empty())
        return;
    else
        item_to_rent = available_vans_plate_number.at(0);

    ImGui::PushItemWidth(200);
    if (ImGui::BeginCombo("Currently Available Vans", item_to_rent.data())) {
        for (auto& item : available_vans_plate_number) {
            bool is_selected = (item_to_rent == item); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(item.data(), is_selected))
                item_to_rent = item;
        }
        ImGui::EndCombo();
    }

    static double days_input { 0 };
    static std::array<char, 50> client_name_input { "" };
    static std::array<char, 50> client_DNI_input { "" };
    static float deposit_input { 0 };

    auto data_access_obj = Data::DataAccess { db, "my_vans" };
    auto van_to_rent_from_db = data_access_obj.get_van_in_db_by_plate_number(item_to_rent);
    GUI::spacing_vertical(3);
    if (!item_to_rent.empty()) {
        ImGui::Text("%s", fmt::format("Brand: {}", van_to_rent_from_db.brand()).data());
        ImGui::Text("%s", fmt::format("Model: {}", van_to_rent_from_db.model()).data());
        ImGui::Text("%s", fmt::format("Number of Doors: {} ", van_to_rent_from_db.doors()).data());
        ImGui::Text("%s", fmt::format("Cat: {} ", van_to_rent_from_db.cat()).data());
        ImGui::InputDouble("Insert Number of Days", &days_input, 1, 100, "%.1f");

        GUI::spacing_vertical(2);

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImGui::Text("%s", fmt::format("Calculated Fee: {} Euros", van_to_rent_from_db.calc_rental_fee(days_input, van_to_rent_from_db.cat())).data());
        ImGui::PopFont();

        GUI::spacing_vertical(2);
        ImGui::PushItemWidth(350);
        ImGui::InputText("Insert Client Name", client_name_input.data(), client_name_input.size());
        ImGui::InputText("Insert Client DNI", client_DNI_input.data(), client_DNI_input.size());
        ImGui::PopItemWidth();

        for (int nbr_spacing = 0; nbr_spacing < 3; ++nbr_spacing)
            ImGui::Spacing();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        btn_st_rent_van = ImGui::Button("Rent Van");
        ImGui::PopFont();
    }

    if (btn_st_rent_van) {
        spdlog::info(fmt::format("Selected Item is: {}", item_to_rent));
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        char char_date[100];
        char char_hour[100];
        std::strftime(char_date, 100, "%d/%m/%Y", std::localtime(&in_time_t));
        std::strftime(char_hour, 100, "%T", std::localtime(&in_time_t));

        auto rent_transaction = Rentable::RentTransaction { van_to_rent_from_db.plate_number(), days_input, client_name_input.data(), client_DNI_input.data(), char_date, char_hour, true };
        auto rent_transaction_data = Data::DataAccess { db, "rent_transactions" };
        auto result = rent_transaction_data.add_rent_transaction_entry(rent_transaction, true);
        if (result == Data::DataAccess::DBOp::OK) {
            spdlog::info(fmt::format("Rent Transaction added to DB"));
            ImGui::OpenPopup("Van Transaction Added Successfully!");
        } else {
            ImGui::OpenPopup("Van Transaction Failed!");
            spdlog::info(fmt::format("Missing added information on mandatory fields!!"));
        }
    }

    if (ImGui::BeginPopupModal("Van Transaction Added Successfully!", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(500, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Van Transaction Failed!", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Separator();
        ImGui::Text("Missing added information on mandatory fields!!");
        if (ImGui::Button("OK", ImVec2(500, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();
}

void rent_check_out(const mongocxx::database& db)
{
    static bool btn_st_check_out_van { false };
    static bool show_current_rented_vans { false };
    static bool show_check_out_client_info { false};

    auto data_access = Data::DataAccess { db, "my_vans" };
    std::vector<std::string> currently_rented_vans_plate_number = data_access.get_currently_rented();

    static std::string item_to_checkout {};
    if (currently_rented_vans_plate_number.empty()) {
        show_check_out_client_info = false;
        return;
    }
    else {
        item_to_checkout = currently_rented_vans_plate_number.at(0);
        show_check_out_client_info = true;
    }

    ImGui::PushItemWidth(200);
    if (ImGui::BeginCombo("Currently Rented Vans", item_to_checkout.data())) {
        for (auto& item : currently_rented_vans_plate_number) {
            bool is_selected = (item_to_checkout == item); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(item.data(), is_selected))
                item_to_checkout = item;
        }
        ImGui::EndCombo();
    }

    auto data_access_obj = Data::DataAccess { db, "rent_transactions" };
    auto van_to_check_out_from_db = data_access_obj.get_rented_in_db_by_plate_number(item_to_checkout);
    GUI::spacing_vertical(3);

    if (show_check_out_client_info) {
        ImGui::Text("%s", fmt::format("Client Name: {}", van_to_check_out_from_db.client_name()).data());
        ImGui::Text("%s", fmt::format("Client DNI: {}", van_to_check_out_from_db.client_DNI()).data());
        ImGui::Text("%s", fmt::format("Days Rented {}", van_to_check_out_from_db.days()).data());

        for (int nbr_spacing = 0; nbr_spacing < 3; ++nbr_spacing)
            ImGui::Spacing();

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        btn_st_check_out_van = ImGui::Button("Check Out");
        ImGui::PopFont();
    }

    if (btn_st_check_out_van) {
        spdlog::info(fmt::format("Selected Item to checout is: {}", item_to_checkout));
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        char char_date[100];
        char char_hour[100];
        std::strftime(char_date, 100, "%d/%m/%Y", std::localtime(&in_time_t));
        std::strftime(char_hour, 100, "%T", std::localtime(&in_time_t));

        auto rent_transaction = Rentable::RentTransaction { van_to_check_out_from_db.plate_number(), van_to_check_out_from_db.days(), van_to_check_out_from_db.client_name(), van_to_check_out_from_db.client_DNI(), char_date, char_hour, false };
        auto rent_transaction_data = Data::DataAccess { db, "rent_transactions" };
        auto result = rent_transaction_data.add_rent_transaction_entry(rent_transaction, false);

        if (result == Data::DataAccess::DBOp::OK) {
            spdlog::info(fmt::format("Rent Transaction check-out added to DB"));
            ImGui::OpenPopup("Van Checked Out Successfully!");
        } else {
            spdlog::info(fmt::format("Rent Transaction check-out failed"));
            ImGui::OpenPopup("Van Check Out Failed!");
        }
    }

    if (ImGui::BeginPopupModal("Van Checked Out Successfully!", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(500, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Van Check Out Failed!", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(500, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

}

void spacing_vertical(const int& nbr_spaces)
{
    for (int spacing = 0; spacing < nbr_spaces; ++spacing)
        ImGui::Spacing();
}
}