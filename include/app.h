#include <wx/wx.h>

namespace GUI {
    constexpr int xx_size_main_window = 1300;
    constexpr int yy_size_main_window = 800;
    constexpr int xx_size_help_window = 300;
    constexpr int yy_size_help_window = 300;

    class App : public wxFrame {
      public:
        App(const wxString& title, int xx, int yy);
        //        void OnQuit(wxCommandEvent& event);
        void OnMenuFileQuit(wxCommandEvent& event);
        void OnMenuHelpAbout(wxCommandEvent& event);
        wxMenuBar* get_menu_bar() {
            return m_menu_bar;
        }

      protected:
        DECLARE_EVENT_TABLE()

      private:
        wxMenuBar* m_menu_bar;
        wxMenu* m_file_menu;
        wxMenu* m_help_menu;
    };

    class HelpWindow : public wxFrame {
      public:
        HelpWindow(wxWindow* parent, const wxString& title, int xx, int yy);
    };

    class wxImagePanel : public wxPanel {
        wxBitmap image;

      public:
        wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);

        void paintEvent(wxPaintEvent& evt);

        void render(wxDC& dc);
      protected:
        DECLARE_EVENT_TABLE()
    };

} // namespace GUI
