#include "spdlog/spdlog.h"
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

int main()
{
    mongocxx::instance instance {}; // This should be done only once.
    std::string uri_db { "mongodb://localhost:27017" };
    spdlog::info(fmt::format("Trying to connect to DB at {}", uri_db));
    mongocxx::uri uri(uri_db);
    spdlog::info(fmt::format("Successfully connected to DB at {}", uri_db));
    mongocxx::client client(uri);
    mongocxx::database db = client["mydb"];

    sf::RenderWindow window(sf::VideoMode(GUI::WIDTH, GUI::HEIGHT), "FleetManager Application");
    window.setFramerateLimit(30);

    ImGui::SFML::Init(window, false);

    sf::Color bg_color;
    float color[3] = { 0.22f, 0.32f, 0.63f };
    bg_color.r = static_cast<sf::Uint8>(color[0] * 255.f);
    bg_color.g = static_cast<sf::Uint8>(color[1] * 255.f);
    bg_color.b = static_cast<sf::Uint8>(color[2] * 255.f);

    const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
    static bool toggles[] = { true, false, false, false, false };

    // Load Fonts
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("../../res/Ubuntu-R.ttf", 21.f); // regular
    io.Fonts->AddFontFromFileTTF("../../res/Ubuntu-B.ttf", 22.f); // bold
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

//        ImGui::Begin("Set Background Color");
//        if (ImGui::ColorEdit3("Background color", color)) {
//            bg_color.r = static_cast<sf::Uint8>(color[0] * 255.f);
//            bg_color.g = static_cast<sf::Uint8>(color[1] * 255.f);
//            bg_color.b = static_cast<sf::Uint8>(color[2] * 255.f);
//        }
//        ImGui::End();

        ImGui::Begin("Add Rentable to Database");
        GUI::add_rentable_to_db(db);
        ImGui::End();

        ImGui::Begin("Rent Check-In");
        GUI::rent_check_in(db);
        ImGui::End();

        ImGui::Begin("Rent Check-Out");
        GUI::rent_check_out(db);
        ImGui::End();


        //        ImGui::Begin("test2");
        //        if (ImGui::TreeNode("Modals")) {
        //            ImGui::TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside the window.");
        //
        //            if (ImGui::Button("Delete.."))
        //                ImGui::OpenPopup("Delete?");
        //            if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        //                ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
        //                ImGui::Separator();
        //
        //                //static int dummy_i = 0;
        //                //ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");
        //
        //                static bool dont_ask_me_next_time = false;
        //                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        //                ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        //                ImGui::PopStyleVar();
        //
        //                if (ImGui::Button("OK", ImVec2(120, 0))) {
        //                    ImGui::CloseCurrentPopup();
        //                }
        //                ImGui::SetItemDefaultFocus();
        //                ImGui::SameLine();
        //                if (ImGui::Button("Cancel", ImVec2(120, 0))) {
        //                    ImGui::CloseCurrentPopup();
        //                }
        //                ImGui::EndPopup();
        //            }
        //
        //            if (ImGui::Button("Stacked modals.."))
        //                ImGui::OpenPopup("Stacked 1");
        //            if (ImGui::BeginPopupModal("Stacked 1")) {
        //                ImGui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it.");
        //                static int item = 1;
        //                ImGui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
        //                static float color[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
        //                ImGui::ColorEdit4("color", color); // This is to test behavior of stacked regular popups over a modal
        //
        //                if (ImGui::Button("Add another modal.."))
        //                    ImGui::OpenPopup("Stacked 2");
        //                if (ImGui::BeginPopupModal("Stacked 2")) {
        //                    ImGui::Text("Hello from Stacked The Second!");
        //                    if (ImGui::Button("Close"))
        //                        ImGui::CloseCurrentPopup();
        //                    ImGui::EndPopup();
        //                }
        //
        //                if (ImGui::Button("Close"))
        //                    ImGui::CloseCurrentPopup();
        //                ImGui::EndPopup();
        //            }
        //
        //            ImGui::TreePop();
        //        }
        //
        //        ImGui::End();
        //        ImGui::Begin("Menu");
        //        // Showing a menu with toggles
        //        if (ImGui::Button("Toggle.."))
        //            ImGui::OpenPopup("my_toggle_popup");
        //        if (ImGui::BeginPopup("my_toggle_popup"))
        //        {
        //            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
        //                ImGui::MenuItem(names[i], "", &toggles[i]);
        //            if (ImGui::BeginMenu("Sub-menu"))
        //            {
        //                ImGui::MenuItem("Click me");
        //                ImGui::EndMenu();
        //            }
        //
        //            ImGui::Separator();
        //            ImGui::Text("Tooltip here");
        //            if (ImGui::IsItemHovered())
        //                ImGui::SetTooltip("I am a tooltip over a popup");
        //
        //            if (ImGui::Button("Stacked Popup"))
        //                ImGui::OpenPopup("another popup");
        //            if (ImGui::BeginPopup("another popup"))
        //            {
        //                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
        //                    ImGui::MenuItem(names[i], "", &toggles[i]);
        //                if (ImGui::BeginMenu("Sub-menu"))
        //                {
        //                    ImGui::MenuItem("Click me");
        //                    ImGui::EndMenu();
        //                }
        //                ImGui::EndPopup();
        //            }
        //            ImGui::EndPopup();
        //        }
        //        ImGui::End();

        //        ImGui::Begin("Test");
        //
        //        if (ImGui::TreeNode("Context menus"))
        //        {
        //            // BeginPopupContextItem() is a helper to provide common/simple popup behavior of essentially doing:
        //            //    if (IsItemHovered() && IsMouseReleased(0))
        //            //       OpenPopup(id);
        //            //    return BeginPopup(id);
        //            // For more advanced uses you may want to replicate and cuztomize this code. This the comments inside BeginPopupContextItem() implementation.
        //            static float value = 0.5f;
        //            ImGui::Text("Value = %.3f (<-- right-click here)", value);
        //            if (ImGui::BeginPopupContextItem("item context menu"))
        //            {
        //                if (ImGui::Selectable("Set to zero")) value = 0.0f;
        //                if (ImGui::Selectable("Set to PI")) value = 3.1415f;
        //                ImGui::PushItemWidth(-1);
        //                ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
        //                ImGui::PopItemWidth();
        //                ImGui::EndPopup();
        //            }
        //
        //            // We can also use OpenPopupOnItemClick() which is the same as BeginPopupContextItem() but without the Begin call.
        //            // So here we will make it that clicking on the text field with the right mouse button (1) will toggle the visibility of the popup above.
        //            ImGui::Text("(You can also right-click me to the same popup as above.)");
        //            ImGui::OpenPopupOnItemClick("item context menu", 1);
        //
        //            // When used after an item that has an ID (here the Button), we can skip providing an ID to BeginPopupContextItem().
        //            // BeginPopupContextItem() will use the last item ID as the popup ID.
        //            // In addition here, we want to include your editable label inside the button label. We use the ### operator to override the ID (read FAQ about ID for details)
        //            static char name[32] = "Label1";
        //            char buf[64]; sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
        //            ImGui::Button(buf);
        //            if (ImGui::BeginPopupContextItem())
        //            {
        //                ImGui::Text("Edit name:");
        //                ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
        //                if (ImGui::Button("Close"))
        //                    ImGui::CloseCurrentPopup();
        //                ImGui::EndPopup();
        //            }
        //            ImGui::SameLine(); ImGui::Text("(<-- right-click here)");
        //
        //            ImGui::TreePop();
        //        }
        //        ImGui::End();
        //
        //        ImGui::Begin("Test2");
        //        // Create multiple items in a same cell before switching to next column
        //        if (ImGui::TreeNode("Mixed items"))
        //        {
        //            ImGui::Columns(3, "mixed");
        //            ImGui::Separator();
        //
        //            ImGui::Text("Hello");
        //            ImGui::Button("Banana");
        //            ImGui::NextColumn();
        //
        //            ImGui::Text("ImGui");
        //            ImGui::Button("Apple");
        //            static float foo = 1.0f;
        //            ImGui::InputFloat("red", &foo, 0.05f, 0, "%.3f");
        //            ImGui::Text("An extra line here.");
        //            ImGui::NextColumn();
        //
        //            ImGui::Text("Sailor");
        //            ImGui::Button("Corniflower");
        //            static float bar = 1.0f;
        //            ImGui::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
        //            ImGui::NextColumn();
        //
        //            if (ImGui::CollapsingHeader("Category A")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
        //            if (ImGui::CollapsingHeader("Category B")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
        //            if (ImGui::CollapsingHeader("Category C")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
        //            ImGui::Columns(1);
        //            ImGui::Separator();
        //            ImGui::TreePop();
        //        }
        //        ImGui::End();

        window.clear(bg_color);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return EXIT_SUCCESS;
}