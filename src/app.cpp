#include "app.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace GUI {
    App::App(const wxString& title, int xx, int yy) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition,
                                                              wxSize(xx, yy)) {

        auto* panel = new wxPanel(this, wxID_ANY);
        //        auto* button = new wxButton(panel, wxID_EXIT, wxT("Quit"),
        //                                    wxPoint(xx_button, yy_button));
        //        Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED,
        //                wxCommandEventHandler(App::OnQuit));
        //        button->SetFocus();

        m_menu_bar = new wxMenuBar();
        // File Menu
        m_file_menu = new wxMenu();
        m_file_menu->Append(wxID_EXIT, _T("&Quit"));
        m_menu_bar->Append(m_file_menu, _T("&File"));
        // About menu
        m_help_menu = new wxMenu();
        m_help_menu->Append(wxID_ABOUT, _T("&About"));
        m_menu_bar->Append(m_help_menu, _T("&Help"));

        //        SetMenuBar(m_menu_bar);
        Center();
    }

    BEGIN_EVENT_TABLE(App, wxFrame)
    EVT_MENU(wxID_EXIT, App::OnMenuFileQuit)
    EVT_MENU(wxID_ABOUT, App::OnMenuHelpAbout)
    END_EVENT_TABLE()

    void App::OnMenuFileQuit(wxCommandEvent& event) {
        Close(false);
    }
    void App::OnMenuHelpAbout(wxCommandEvent& event) {
        wxInitAllImageHandlers();
        auto* help_window = new HelpWindow(this, wxT("FleetManager - About"), GUI::xx_size_help_window, GUI::yy_size_help_window);
        help_window->SetSizeHints(GUI::xx_size_help_window, GUI::yy_size_help_window, GUI::xx_size_help_window, GUI::yy_size_help_window);
        auto* help_logo_image = new wxImagePanel(help_window, wxT("../pics/logo.png"), wxBITMAP_TYPE_PNG);
        auto* help_text = new wxStaticText(help_window, wxID_ANY, wxT("Fleet Manager App : MarbeVan S.A."), wxDefaultPosition, wxDefaultSize, 0, wxT("help_message"));

        auto* vbox = new wxBoxSizer(wxVERTICAL);

        vbox->Add(help_logo_image, 1, wxEXPAND | wxLEFT | wxUP, 28);
        vbox->Add(help_text, 1, wxLEFT | wxUP, 28);

        help_window->SetSizer(vbox);
        help_window->Show(true);
    }

    HelpWindow::HelpWindow(wxWindow* parent, const wxString& title, int xx, int yy) : wxFrame(parent, wxID_ANY, title, wxDefaultPosition,
                                                                                              wxSize(xx, yy)) { Center(); }

    BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
    EVT_PAINT(wxImagePanel::paintEvent)
    END_EVENT_TABLE()

    wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) : wxPanel(parent) {
        // load the file... ideally add a check to see if loading was successful
        image.LoadFile(file, format);
    }


    void wxImagePanel::paintEvent(wxPaintEvent& evt) {
        // depending on your system you may need to look at double-buffered dcs
        wxPaintDC dc(this);
        render(dc);
    }

    void wxImagePanel::render(wxDC& dc) {
        dc.DrawBitmap(image, 0, 0, false);
    }

} // namespace GUI
