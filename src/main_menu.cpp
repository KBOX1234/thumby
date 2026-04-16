#include "main_menu.hpp"
#include "state_manager.hpp"
#include <wx/wx.h>

main_frame::main_frame() : wxFrame(nullptr, wxID_ANY, "Thumbertha - USB thumb drive utility"){
    main_sizer = new wxBoxSizer(wxVERTICAL);

    // Create widgets
    file_select = new wxStaticText(this, wxID_ANY, "No Selected File");
    file_select_button = new wxButton(this, wxID_ANY, "Load ISO");

    // Create a horizontal row
    wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);

    // Left: label
    row->Add(file_select, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

    // Middle: flexible space
    row->AddStretchSpacer(1);

    // Right: button
    row->Add(file_select_button, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

    // Add row to main layout
    main_sizer->Add(row, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

    SetSizer(main_sizer);

    // Bind event
    file_select_button->Bind(wxEVT_BUTTON, &main_frame::select_file, this);
}

void main_frame::select_file(wxCommandEvent& event){
    wxFileDialog openFileDialog(
        this,
        "Open a file",
        "",
        "",
        "*.iso|*.iso",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (openFileDialog.ShowModal() == wxID_OK){
        wxString path = openFileDialog.GetPath();
        state_master.set_file(path.ToStdString());

        file_select->SetLabel(path);
    }
}

bool thumbertha::OnInit(){
    main_frame* frame = new main_frame();
    frame->Show(true);
    return true;
}
