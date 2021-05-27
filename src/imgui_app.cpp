#include <FleetManager/Van.h>
#include <FleetManager/imgui_app.h>
#include <array>
#include <imgui.h>

namespace GUI {

void ShowExampleMenuFile()
{
    ImGui::MenuItem("(dummy menu)", nullptr, false, false);
    if (ImGui::MenuItem("New")) { }
    if (ImGui::MenuItem("Open", "Ctrl+O")) { }
    if (ImGui::BeginMenu("Open Recent")) {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More..")) {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse..")) {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) { }
    if (ImGui::MenuItem("Save As..")) { }
    ImGui::Separator();
    if (ImGui::BeginMenu("Options")) {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        static bool b = true;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::Checkbox("Check", &b);
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Colors")) {
        float sz = ImGui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++) {
            const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
            ImGui::Dummy(ImVec2(sz, sz));
            ImGui::SameLine();
            ImGui::MenuItem(name);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (ImGui::MenuItem("Checked", nullptr, true)) { }
    if (ImGui::MenuItem("Quit", "Alt+F4")) { }
}
Rentable::Van show_add_van()
{
    ImGui::Begin("Add to Database");
    static std::array<char, 15> brand { "Brand" };
    static std::array<char, 15> model { "Model" };
    static int year = 2018;
    static std::array<char, 20> plate_number { "4248 JXX" };
    static int hp = 2018;
    static int nbr_doors = 2018;

    ImGui::InputText("Insert Brand", brand.data(), brand.size());
    ImGui::InputText("Insert Model", model.data(), model.size());
    ImGui::InputInt("Insert Year of Manufacture", &year, 1, 100);
    ImGui::InputText("Insert Plate Number", plate_number.data(), plate_number.size());
    ImGui::InputInt("Insert HP", &hp, 1, 1000);
    ImGui::InputInt("Insert Number of Doors", &nbr_doors, 1, 100);

    return Rentable::Van{brand.data(), model.data(), year, plate_number.data(), hp, nbr_doors};
}

}