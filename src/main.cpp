#include <wx/wx.h>


class App : public wxApp {
public:
    bool OnInit() override {
        wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "Test");
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(App);
