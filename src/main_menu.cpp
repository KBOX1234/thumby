#include "main_menu.hpp"
#include "state_manager.hpp"
#include "flasher.hpp"
#include <string>
#include <vector>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/wx.h>
#include <thread>

main_frame::main_frame() : wxFrame(nullptr, wxID_ANY, "Thumbertha - USB thumb drive utility"){
    main_sizer = new wxBoxSizer(wxVERTICAL);

    // Create widgets
    file_select = new wxStaticText(this, wxID_ANY, "No Selected File");
    file_select_button = new wxButton(this, wxID_ANY, "Load ISO");
    burn = new wxButton(this, wxID_ANY, "Burn ISO");
    device_list = new wxComboBox(this, wxID_ANY,"", wxDefaultPosition, wxDefaultSize);


    // Create a horizontal row
    wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);

    // Left: label
    row->Add(file_select, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

    // Middle: flexible space
    row->AddStretchSpacer(1);

    // Right: button
    row->Add(file_select_button, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

    row->Add(burn, 0, wxALIGN_CENTER_VERTICAL, 5);

    row->Add(device_list, 1, wxEXPAND | wxALL, 10);


    // Add row to main layout
    main_sizer->Add(row, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

    SetSizer(main_sizer);

    // Bind event
    file_select_button->Bind(wxEVT_BUTTON, &main_frame::select_file, this);
    burn->Bind(wxEVT_BUTTON, &main_frame::burn_bt, this);

    refresh_device_dropdown();

    flash_master.SetProgressCallback([this](int percent) {
        wxTheApp->CallAfter([=]() {
            progressBar->Update(percent);
        });
    });
}

void main_frame::refresh_device_dropdown(){
    

    device_list->Clear();
    std::vector<std::string> devices = flash_master.get_devices();

    for(size_t i = 0; i < devices.size(); i++){
        device_list->Append(devices[i]);
    }
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

void main_frame::burn_bt(wxCommandEvent& event){
    wxString dev = device_list->GetValue();

    wxMessageDialog dialog(
        this,
        "WARNING! You are about to erase this device: " + dev + "\nAre you shure?",
        "Warning",
        wxYES_NO | wxNO_DEFAULT | wxICON_WARNING
    );

    int result = dialog.ShowModal();
    progressBar = new wxProgressDialog("Flashing ISO", "Writing to USB...", 100, this, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
    if (result == wxID_YES) {
        std::thread([this, dev]() {

            int status = flash_master.flash_device(dev.ToStdString(), state_master.get_file_path());

            wxTheApp->CallAfter([this, status]() {
                if(status == FLASH_ERROR_NON) wxMessageBox("Flashing complete!");
                else{
                    wxMessageBox("Flashing failed with code: " + std::to_string(status) + "\nMabey try running as Admin?");
                }
            });

        }).detach();

    } 
    else {
        wxLogMessage("Cancled");
    }
}
