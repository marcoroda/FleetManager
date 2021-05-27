#include <FleetManager/DataAccess.h>
#include <FleetManager/imgui_app.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

int main()
{
    mongocxx::instance instance {}; // This should be done only once.
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);
    mongocxx::database db = client["mydb"];

    sf::RenderWindow window(sf::VideoMode(GUI::WIDTH, GUI::HEIGHT), "FleetManager Application");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window, false);

    // Load Fonts
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("../../resources/Ubuntu-R.ttf", 19.f);
    io.Fonts->AddFontFromFileTTF("../../resources/Ubuntu-B.ttf", 21.f);
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

        ImGui::Begin("Hello, world!");
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImGui::Button("Look at this pretty button");
        ImGui::PopFont();
        ImGui::End();

        auto added_van = GUI::show_add_van();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        if (ImGui::Button("Add Van")) {
            added_van.print_van_info();
            auto data_access = Data::DataAccess { db, "my_vans" };
            if (data_access.add_van(added_van) != Data::DataAccess::DBOp::OK)
                return EXIT_FAILURE;
        }
        ImGui::PopFont();
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return EXIT_SUCCESS;
}