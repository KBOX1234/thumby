#pragma once

#include <wx/wx.h>

class thumbertha : public wxApp {
    public:
        bool OnInit() override;
};

class main_frame : public wxFrame {
    private:
        wxBoxSizer* main_sizer;
        wxStaticText* file_select;
        wxButton* file_select_button;
        wxButton* burn;

    public:
        main_frame();

        void select_file(wxCommandEvent& event);
};
