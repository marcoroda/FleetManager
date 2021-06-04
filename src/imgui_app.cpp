#include <FleetManager/DataAccess.h>
#include <FleetManager/Date.h>
#include <FleetManager/Utils.h>
#include <FleetManager/Van.h>
#include <FleetManager/imgui_app.h>
#include <array>
#include <fmt/format.h>
#include <imgui.h>
#include <iostream>
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

    ImGui::PopItemWidth();

    return Rentable::Van {
        brand_input.data(),
        model_input.data(),
        year_input,
        plate_number_input.data(),
        hp_input,
        nbr_doors_input,
        false,
        std::stoi(cat),
        (gps == "True"),
        last_ITV_date,
        (accident_ready == "True"),
        "AXA",
        last_insurance_date,
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

void rent(const mongocxx::database& db)
{
    static bool btn_st_rent_van { false };
    static bool show_current_rented_vans { false };

    auto data_access = Data::DataAccess { db, "my_vans" };
    std::vector<std::string> available_vans_plate_number = data_access.get_available_for_renting();

    static std::string current_item { "1234 XYZ" };

    ImGui::PushItemWidth(200);
    if (ImGui::BeginCombo("Currently Available Vans", current_item.data())) {
        for (auto& item : available_vans_plate_number) {
            bool is_selected = (current_item == item); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(item.data(), is_selected))
                current_item = item;
        }
        ImGui::EndCombo();
    }

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    for (int nbr_spacing = 0; nbr_spacing < 3; ++nbr_spacing)
        ImGui::Spacing();
    btn_st_rent_van = ImGui::Button("Rent Van");
    ImGui::PopFont();
    if (btn_st_rent_van) {
        spdlog::info(fmt::format("Selected Item is: {}", current_item));
    }

    ImGui::Checkbox("Show currently rented vans", &show_current_rented_vans);
    if (show_current_rented_vans) {
        auto data = Data::DataAccess { db, "rented_vans" };
        GUI::show_all_vans(data, "collection: rented_vans");
    }

    ImGui::PopItemWidth();
}

}