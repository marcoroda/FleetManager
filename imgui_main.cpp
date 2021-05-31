#include <FleetManager/DataAccess.h>
#include <FleetManager/imgui_app.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include "spdlog/spdlog.h"
#include <fmt/format.h>

int main()
{
    mongocxx::instance instance {}; // This should be done only once.
    std::string uri_db{"mongodb://localhost:27017"};
    spdlog::info(fmt::format("Trying to connect to DB at {}", uri_db));
    mongocxx::uri uri(uri_db);
    spdlog::info(fmt::format("Successfully connected to DB at {}", uri_db));
    mongocxx::client client(uri);
    mongocxx::database db = client["mydb"];

    sf::RenderWindow window(sf::VideoMode(GUI::WIDTH, GUI::HEIGHT), "FleetManager Application");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window, false);

    sf::Color bg_color;
    float color[3] = { 0.22f, 0.22f, 0.13f };
    bg_color.r = static_cast<sf::Uint8>(color[0] * 255.f);
    bg_color.g = static_cast<sf::Uint8>(color[1] * 255.f);
    bg_color.b = static_cast<sf::Uint8>(color[2] * 255.f);

    static bool my_tool_active { false };
    static bool btn_st_add_van { false };
    static bool show_current_vans { false };
    static bool show_current_rented_vans { false };

    // Load Fonts
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("../../resources/Ubuntu-R.ttf", 19.f); // regular
    io.Fonts->AddFontFromFileTTF("../../resources/Ubuntu-B.ttf", 21.f); // bold
    ImGui::SFML::UpdateFontTexture();

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event {};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Set Background Color");
        if (ImGui::ColorEdit3("Background color", color)) {
            bg_color.r = static_cast<sf::Uint8>(color[0] * 255.f);
            bg_color.g = static_cast<sf::Uint8>(color[1] * 255.f);
            bg_color.b = static_cast<sf::Uint8>(color[2] * 255.f);
        }
        ImGui::End();

        ImGui::Begin("Add to Database");
        auto added_van = GUI::show_add_van();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        for (int nbr_spacing = 0; nbr_spacing < 3; ++nbr_spacing)
            ImGui::Spacing();
        btn_st_add_van = ImGui::Button("Add Van");
        ImGui::PopFont();
        if (btn_st_add_van) {
            added_van.print_van_info();
            auto data_access = Data::DataAccess { db, "my_vans" };
            if (data_access.add_van(added_van) != Data::DataAccess::DBOp::OK)
                spdlog::info("Van already exist in the Database!");

            data_access.list_all();
        }

        ImGui::Checkbox("Check to Show Current Vans in Database", &show_current_vans);
        if (show_current_vans) {
            auto data = Data::DataAccess { db, "my_vans" };
            GUI::show_all_vans(data, "collection: my_vans");
        }
        ImGui::End();

//        ImGui::Begin("Rent Van");
//        char *array = {"One, good, thing, about, music"};
//        int item;
//        ImGui::Combo("Vans", &item, array);

//        ImGui::Checkbox("Check to Show Current rented Vans", &show_current_rented_vans);
//        if (show_current_rented_vans) {
//            auto data = Data::DataAccess { db, "rented_vans" };
//            GUI::show_all_vans(data, "collection: rented_vans");
//        }
//        ImGui::End();

        window.clear(bg_color);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return EXIT_SUCCESS;
}