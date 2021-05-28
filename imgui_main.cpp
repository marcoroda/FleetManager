#include <FleetManager/DataAccess.h>
#include <FleetManager/imgui_app.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>

int main()
{
    mongocxx::instance instance {}; // This should be done only once.
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);
    mongocxx::database db = client["mydb"];

    sf::RenderWindow window(sf::VideoMode(GUI::WIDTH, GUI::HEIGHT), "FleetManager Application");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window, false);

    static bool btn_st_add_van { false };

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
            if (data_access.add_van(added_van) != Data::DataAccess::DBOp::OK) {
                std::cout << "Van already exist in the Database! \n";
            }
            data_access.list_all();
        }
        ImGui::End();

        ImGui::Begin("My Vans");
        //        auto data_access = Data::DataAccess {db, "my_vans"};
        //        data_access.list_all();
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return EXIT_SUCCESS;
}