#include <memory>

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/gbsizer.h>
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
    EVT_BUTTON(40, MainWindow::on_stop)
wxEND_EVENT_TABLE()

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "tiny-music-player") {
    setup_menubar();

    CreateStatusBar();
    SetStatusText("Welcome to tiny-music-player!");

    setup_widgets();

    source = std::make_unique<al::Source>();
}

MainWindow::~MainWindow() {
    source->stop();
}

void MainWindow::setup_menubar() {
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
}

void MainWindow::setup_widgets() {
    wxPanel* pnl_songs = new wxPanel(this);
    wxPanel* pnl_current_song = new wxPanel(this);
    wxPanel* pnl_slider_buttons = new wxPanel(this);

    btn_play_pause = new wxButton(pnl_slider_buttons, 30, "Play/Pause");
    btn_stop = new wxButton(pnl_slider_buttons, 40, "Stop");
    sld_track = new wxSlider(pnl_slider_buttons, 50, 10, 0, 100);

    // TODO temporary
    pnl_songs->SetBackgroundColour({ 255, 0, 0 });
    pnl_current_song->SetBackgroundColour({ 0, 255, 0 });
    pnl_slider_buttons->SetBackgroundColour({ 0, 0, 255 });

    // pnl_songs->SetSize({ 100, 100 });
    // pnl_current_song->SetSize({ 100, 100 });

    wxGridBagSizer* szr_main = new wxGridBagSizer;
    szr_main->Add(pnl_songs, { 0, 0 }, wxDefaultSpan, wxEXPAND | wxALL);
    szr_main->Add(pnl_current_song, { 1, 0 }, wxDefaultSpan, wxEXPAND | wxALL);
    szr_main->Add(pnl_slider_buttons, { 0, 1 }, { 2, 1 }, wxEXPAND | wxALL);

    wxBoxSizer* szr_buttons = new wxBoxSizer(wxHORIZONTAL);
    szr_buttons->Add(btn_play_pause);
    szr_buttons->Add(btn_stop);

    wxBoxSizer* szr_slider_buttons = new wxBoxSizer(wxVERTICAL);
    szr_slider_buttons->Add(szr_buttons);
    szr_slider_buttons->Add(sld_track);

    pnl_slider_buttons->SetSizer(szr_slider_buttons);
    SetSizer(szr_main);
}

void MainWindow::on_open(wxCommandEvent& event) {
    wxFileDialog dialog = wxFileDialog {this};
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

void MainWindow::on_stop(wxCommandEvent& event) {
    started = false;

    source->stop();
    spdlog::debug("Stopped song");
}

std::shared_ptr<al::Buffer> MainWindow::load_song(const wxString& file_path) {
    auto data = std::make_shared<SoundData>(file_path.data().AsChar());

    return std::make_shared<al::Buffer>(data);
}
