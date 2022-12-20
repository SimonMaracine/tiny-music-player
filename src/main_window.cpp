#include <memory>

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <spdlog/spdlog.h>

#include "audio/openal/source.h"
#include "audio/openal/buffer.h"
#include "audio/sound_data.h"
#include "main_window.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_OPEN, MainWindow::on_open)
    EVT_MENU(wxID_EXIT, MainWindow::on_exit)
    EVT_MENU(wxID_ABOUT, MainWindow::on_about)
    EVT_BUTTON(30, MainWindow::on_play_pause)
wxEND_EVENT_TABLE()

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "tiny-music-player") {
    wxMenu* men_file = new wxMenu;
    men_file->Append(wxID_OPEN, "Open", "Open a new song to play");
    men_file->Append(wxID_EXIT, "Exit", "Exit the program");

    wxMenu* men_playlist = new wxMenu;
    men_playlist->Append(20, "New", "Create a new playlist");

    wxMenu* men_help = new wxMenu;
    men_help->Append(wxID_ABOUT, "About", "See information about the program");

    wxMenuBar* menu_bar = new wxMenuBar;
    menu_bar->Append(men_file, "File");
    menu_bar->Append(men_playlist, "Playlist");
    menu_bar->Append(men_help, "Help");

    SetMenuBar(menu_bar);

    CreateStatusBar();
    SetStatusText("Welcome to tiny-music-player!");

    sizer = new wxFlexGridSizer(2);

    btn_play_pause = new wxButton(this, 30, "Play/Pause");

    sizer->Add(btn_play_pause);

    source = std::make_unique<al::Source>();
}

MainWindow::~MainWindow() {
    source->stop();
}

void MainWindow::on_open(wxCommandEvent& event) {
    wxFileDialog dialog = wxFileDialog(this);
    int result = dialog.ShowModal();

    if (result == wxID_CANCEL) {
        return;
    }

    auto buffer = load_song(dialog.GetPath());

    active_buffer = buffer;
    buffers.push_back(buffer);

    spdlog::info("Loaded song `{}`", dialog.GetPath().data().AsChar());
}

void MainWindow::on_exit(wxCommandEvent& event) {
    wxExit();
}

void MainWindow::on_about(wxCommandEvent& event) {
    wxMessageBox("tiny-music-player, a music player.", "About", wxOK | wxICON_INFORMATION);
}

void MainWindow::on_play_pause(wxCommandEvent& event) {
    if (active_buffer == nullptr) {
        return;
    }

    if (!started) {
        started = true;

        source->play(active_buffer.get());
        spdlog::debug("Playing song");

        return;
    }

    if (source->is_playing()) {
        source->pause();
        spdlog::debug("Paused song");
    } else {
        source->continue_();
        spdlog::debug("Continuing song");
    }
}

std::shared_ptr<al::Buffer> MainWindow::load_song(const wxString& file_path) {
    auto data = std::make_shared<SoundData>(file_path.data().AsChar());

    return std::make_shared<al::Buffer>(data);
}
