#include <FleetManager/app.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace GUI {

App::App(const wxString& title, int xx, int yy)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(xx, yy))
{
    m_menu_bar = new wxMenuBar();

    m_file_menu = new wxMenu();
    m_file_menu->Append(wxID_EXIT, _T("&Quit"));
    m_menu_bar->Append(m_file_menu, _T("&File"));

    m_view_menu = new wxMenu();
    m_view_menu->Append(wxID_VIEW_LIST, _T("&View"));
    m_menu_bar->Append(m_view_menu, _T("&View"));

    m_help_menu = new wxMenu();
    m_help_menu->Append(wxID_ABOUT, _T("&About"));
    m_menu_bar->Append(m_help_menu, _T("&Help"));

    Center();
}

BEGIN_EVENT_TABLE(App, wxFrame)
EVT_MENU(wxID_EXIT, App::OnMenuFileQuit)
EVT_MENU(wxID_ABOUT, App::OnMenuHelpAbout)
END_EVENT_TABLE()

void App::OnMenuFileQuit(wxCommandEvent& event)
{
    Close(false);
}
void App::OnMenuHelpAbout(wxCommandEvent& event)
{
    wxInitAllImageHandlers();
    auto* help_window = new HelpWindow(this, wxT("FleetManager - About"), GUI::xx_size_help_window, GUI::yy_size_help_window);
    auto* help_text = new wxStaticText(help_window, wxID_ANY, wxT("Fleet Manager App : MarbeVan S.A."), wxDefaultPosition, wxDefaultSize, 0, wxT("help_message"));

    auto* vbox = new wxBoxSizer(wxVERTICAL);

    vbox->Add(help_text, 1, wxLEFT | wxUP, 28);
    help_window->SetSizer(vbox);
    help_window->Show(true);
}

HelpWindow::HelpWindow(wxWindow* parent, const wxString& title, int xx, int yy)
    : wxFrame(parent, wxID_ANY, title, wxDefaultPosition,
        wxSize(xx, yy))
{
    Center();
}

} // namespace GUI
