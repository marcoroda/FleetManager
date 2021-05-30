#include <FleetManager/DataAccess.h>
#include <FleetManager/imgui_app.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <fmt/format.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_internal.h>
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

    static bool my_tool_active { false };
    static bool btn_st_add_van { false };
    static bool show_current_vans { false };

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

        ImGui::Checkbox("Check to Show Current Vans in Database", &show_current_vans);
        if (show_current_vans) {
            auto data = Data::DataAccess { db, "my_vans" };
            GUI::show_all_vans(data, "");
        }

        ImGui::End();

        //        ImGui::Begin("Vans in Database");
        //        static bool test_window = false;
        //        ImGui::Checkbox("Hovered/Active tests after Begin() for title bar testing", &test_window);
        //        if (test_window) {
        //            ImGui::Begin("Title bar Hovered/Active tests", &test_window);
        //            if (ImGui::BeginPopupContextItem()) // <-- This is using IsItemHovered()
        //            {
        //                if (ImGui::MenuItem("Close")) {
        //                    test_window = false;
        //                }
        //                ImGui::EndPopup();
        //            }
        //            GUI::show_all_vans(data, "my");
        //            ImGui::End();
        //        }
        //        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return EXIT_SUCCESS;
}