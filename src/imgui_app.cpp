#include <FleetManager/DataAccess.h>
#include <FleetManager/Van.h>
#include <FleetManager/imgui_app.h>
#include <array>
#include <imgui.h>
#include <iostream>
#include <mongocxx/cursor.hpp>
#include <fmt/format.h>

namespace GUI {

Rentable::Van show_add_van()
{
    static std::array<char, 15> brand { "Opel" };
    static std::array<char, 15> model { "Zafira" };
    static int year { 2020 };
    static std::array<char, 20> plate_number { "4148 JXX" };
    static int hp { 110 };
    static int nbr_doors { 5 };

    ImGui::InputText("Insert Brand", brand.data(), brand.size());
    ImGui::InputText("Insert Model", model.data(), model.size());
    ImGui::InputInt("Insert Year of Manufacture", &year, 1, 100);
    ImGui::InputText("Insert Plate Number", plate_number.data(), plate_number.size());
    ImGui::InputInt("Insert HP", &hp, 1, 1000);
    ImGui::InputInt("Insert Number of Doors", &nbr_doors, 1, 100);
    return Rentable::Van { brand.data(), model.data(), year, plate_number.data(), hp, nbr_doors };
}

void show_all_vans(const Data::DataAccess& data, const std::string& prefix)
{
    std::string s = fmt::format("{} Current Added Vans to DB.", prefix);
    ImGui::Begin(s.data());
    mongocxx::cursor cursor = data.collection().find({});
    static bool selected = false;
    for (auto& doc : cursor) {
        std::string current_doc = bsoncxx::to_json(*cursor.begin());
        ImGui::Selectable(current_doc.data(), &selected);
    }
    ImGui::End();
}

}