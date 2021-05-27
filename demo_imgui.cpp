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

    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

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
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        ImGui::Begin("Menu");
        if (ImGui::BeginMenu("Menu")) {
//            GUI::ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        static int output_dest = 0;
        static bool output_only_modified = true;

        if (ImGui::Button("Export")) {
            if (output_dest == 0)
                ImGui::LogToClipboard();
            else
                ImGui::LogToTTY();
            ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;"
                           "\n");
            ImGui::LogFinish();
        }

        ImGui::PushItemWidth(120);
        ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0");
        ImGui::PopItemWidth();
        ImGui::Checkbox("Only Modified Colors", &output_only_modified);
        ImGui::End();

        ImGui::Begin("Add to Database");
        static char brand[20] = "Opel";
        static char model[20] = "Model";
        static int year = 2018;
        static char plate[20] = "4248 JXX";
        static int hp = 2018;
        static int nbr_doors = 2018;
        ImGui::InputText("Insert Brand", brand, IM_ARRAYSIZE(brand));
        ImGui::InputText("Insert Model", model, IM_ARRAYSIZE(model));
        ImGui::InputInt("Insert Year of Manufacture", &year, 1, 100);
        ImGui::InputText("Insert Plate Number", plate, IM_ARRAYSIZE(brand));
        ImGui::InputInt("Insert HP", &hp, 1, 1000);
        ImGui::InputInt("Insert Number of Doors", &nbr_doors, 1, 100);

        if (ImGui::Button("Add Van")) {
            auto my_van = Rentable::Van { "Opel", "Zafira", 2012, "4148 JXX", 120, 5 };
            auto data_access = Data::DataAccess { db, "vans" };
            data_access.add_van(my_van);
        }

        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}