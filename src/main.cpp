#include "main.h"
#include "app.h"

IMPLEMENT_APP(FleetManagerApp)

bool FleetManagerApp::OnInit() {
    auto* main_app = new GUI::App(wxT("FleetManager - MarbeVan"), GUI::xx_size_main_window, GUI::yy_size_main_window);
    main_app->SetMenuBar(main_app->get_menu_bar());
    main_app->Show(true);
    SetTopWindow(main_app);

    return true;
}