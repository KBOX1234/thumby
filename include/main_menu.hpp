#pragma once

#include <wx/gauge.h>
#include <wx/progdlg.h>
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
        wxComboBox* device_list;
        wxProgressDialog* progressBar;

    public:
        main_frame();

        void refresh_device_dropdown();

        void select_file(wxCommandEvent& event);

        void burn_bt(wxCommandEvent& event);
};
