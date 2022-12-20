#pragma once

#include <vector>
#include <memory>

#include <wx/wx.h>
#include <wx/gbsizer.h>

#include "audio/openal/source.h"
#include "audio/openal/buffer.h"

class MainWindow : public wxFrame {
public:
    MainWindow();
    ~MainWindow();
private:
    void on_open(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);

    void on_play_pause(wxCommandEvent& event);
    void on_stop(wxCommandEvent& event);

    std::shared_ptr<al::Buffer> load_song(const wxString& file_path);

    wxDECLARE_EVENT_TABLE();

    wxGridBagSizer* sizer = nullptr;
    wxButton* btn_play_pause = nullptr;
    wxButton* btn_stop = nullptr;

    std::unique_ptr<al::Source> source;
    bool started = false;

    std::shared_ptr<al::Buffer> active_buffer;
    std::vector<std::shared_ptr<al::Buffer>> buffers;
};
