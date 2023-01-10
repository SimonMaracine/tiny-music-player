#include <memory>
#include <string>

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/gbsizer.h>
#include <spdlog/spdlog.h>

#include "audio/openal/source.h"
#include "audio/openal/buffer.h"
#include "audio/sound_data.h"
#include "main_window.h"
#include "song.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_OPEN, MainWindow::on_open)
    EVT_MENU(wxID_EXIT, MainWindow::on_exit)
    EVT_MENU(wxID_ABOUT, MainWindow::on_about)
    EVT_MENU(20, MainWindow::on_playlist_clear)
    EVT_BUTTON(30, MainWindow::on_play_pause)
    EVT_BUTTON(40, MainWindow::on_stop)
    EVT_LISTBOX(800, MainWindow::on_list_box_select)
wxEND_EVENT_TABLE()

static const char* DEFAULT_STATUS_TEXT = "Welcome to tiny-music-player!";
static constexpr int SONG_NAME_FONT_SIZE = 16;
static constexpr int SONGS_LIST_FONT_SIZE = 12;
static constexpr int SLIDER_SIZE = 1000;
static const wxSize WINDOW_MINIMUM_SIZE = { 640, 480 };  // Cannot be constexpr :P

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "tiny-music-player") {
    setup_menubar();

    CreateStatusBar();
    SetStatusText(DEFAULT_STATUS_TEXT);

    setup_widgets();
    SetMinSize(WINDOW_MINIMUM_SIZE);
    Center();

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
    men_playlist->Append(20, "Clear", "Create a new playlist");

    wxMenu* men_help = new wxMenu;
    men_help->Append(wxID_ABOUT, "About", "See information about the program");

    wxMenuBar* menu_bar = new wxMenuBar;
    menu_bar->Append(men_file, "File");
    menu_bar->Append(men_playlist, "Playlist");
    menu_bar->Append(men_help, "Help");

    SetMenuBar(menu_bar);
}

void MainWindow::setup_widgets() {
    const wxFont song_name_font = wxFont(SONG_NAME_FONT_SIZE, wxDEFAULT, wxNORMAL, wxBOLD, false, "");
    const wxFont songs_list_font = wxFont(SONGS_LIST_FONT_SIZE, wxDEFAULT, wxNORMAL, wxNORMAL, false, "");

    wxPanel* pnl_main = new wxPanel(this);
    wxPanel* pnl_current_song = new wxPanel(pnl_main);
    wxPanel* pnl_slider_buttons = new wxPanel(pnl_main);

    btn_play_pause = new wxButton(pnl_slider_buttons, 30, "Play/Pause");
    btn_stop = new wxButton(pnl_slider_buttons, 40, "Stop");
    sld_track = new wxSlider(pnl_slider_buttons, 50, 0, 0, SLIDER_SIZE);

    lst_songs = new wxListBox(pnl_main, 800);
    lst_songs->SetFont(songs_list_font);
    active_song_name = new wxStaticText(pnl_current_song, wxID_ANY, "No selected song");
    active_song_name->SetFont(song_name_font);

    szr_main = new wxGridBagSizer;
    szr_main->Add(lst_songs, { 0, 0 }, wxDefaultSpan, wxEXPAND);
    szr_main->Add(pnl_current_song, { 0, 1 }, wxDefaultSpan, wxEXPAND);
    szr_main->Add(pnl_slider_buttons, { 1, 0 }, { 1, 2 }, wxEXPAND);

    szr_main->AddGrowableRow(0);
    szr_main->AddGrowableCol(0);
    szr_main->AddGrowableCol(1);

    wxBoxSizer* szr_buttons = new wxBoxSizer(wxHORIZONTAL);
    szr_buttons->Add(btn_play_pause);
    szr_buttons->AddSpacer(15);
    szr_buttons->Add(btn_stop);

    wxBoxSizer* szr_slider_buttons = new wxBoxSizer(wxVERTICAL);
    szr_slider_buttons->Add(szr_buttons, wxSizerFlags().CenterHorizontal());
    szr_slider_buttons->Add(sld_track, wxSizerFlags().Expand().Align(wxALL));

    wxBoxSizer* szr_current_song = new wxBoxSizer(wxVERTICAL);
    szr_current_song->AddSpacer(20);
    szr_current_song->Add(active_song_name, wxSizerFlags().CenterHorizontal());

    pnl_slider_buttons->SetSizer(szr_slider_buttons);
    pnl_current_song->SetSizer(szr_current_song);
    pnl_main->SetSizer(szr_main);
}

void MainWindow::on_open(wxCommandEvent& event) {
    wxFileDialog dialog = wxFileDialog {this};
    const int result = dialog.ShowModal();

    if (result == wxID_CANCEL) {
        return;
    }

    auto song = load_song(dialog.GetPath());
    spdlog::info("Loaded song `{}`", dialog.GetPath().data().AsChar());

    add_song_to_playlist(song);
}

void MainWindow::on_exit(wxCommandEvent& event) {
    wxExit();
}

void MainWindow::on_about(wxCommandEvent& event) {
    wxMessageBox(
        "tiny-music-player, a music player.\nMade with tiny experience.\nBy Simon.",
        "About",
        wxOK | wxICON_INFORMATION
    );
}

void MainWindow::on_playlist_clear(wxCommandEvent& event) {
    lst_songs->Clear();
    songs.clear();
    active_song = nullptr;
    active_song_index = -1;
    playing_song_index = -1;
    started = false;
    source->stop();
    timer.reset();
    sld_track->SetValue(0);
    SetStatusText(DEFAULT_STATUS_TEXT);

    spdlog::debug("Cleared playlist and reset everything");
}

void MainWindow::on_play_pause(wxCommandEvent& event) {
    if (active_song == nullptr) {
        return;
    }

    if (!started && playing_song_index != active_song_index) {
        started = true;

        source->play(active_song->buffer.get());
        playing_song_index = active_song_index;

        timer = std::make_unique<SliderTimer>(sld_track, active_song, source.get());
        timer->Start(200);

        SetStatusText("Playing song");

        spdlog::debug("Playing song");
        return;
    }

    if (source->is_playing()) {
        source->pause();
        SetStatusText("Paused song");

        spdlog::debug("Paused song");
    } else {
        source->continue_();
        SetStatusText("Playing song");

        spdlog::debug("Continuing song");
    }
}

void MainWindow::on_stop(wxCommandEvent& event) {
    if (active_song == nullptr) {
        return;
    }

    started = false;

    source->stop();
    playing_song_index = -1;

    if (timer != nullptr) {
        timer->Stop();
    }

    sld_track->SetValue(0);

    SetStatusText(DEFAULT_STATUS_TEXT);

    spdlog::debug("Stopped song");
}

void MainWindow::on_list_box_select(wxCommandEvent& event) {
    const int selection = lst_songs->GetSelection();
    select_song(songs.at(selection));
    active_song_index = selection;

    szr_main->Layout();
}

std::shared_ptr<Song> MainWindow::load_song(const wxString& file_path) {
    auto data = std::make_shared<SoundData>(file_path.data().AsChar());

    auto song = std::make_shared<Song>();
    song->buffer = std::make_shared<al::Buffer>(data);
    song->samples = data->get_samples();
    song->name = file_path;

    return song;
}

void MainWindow::add_song_to_playlist(std::shared_ptr<Song> song) {
    lst_songs->AppendString(get_name(song->name));
    songs.push_back(song);

    szr_main->Layout();

    spdlog::debug("Added a new song to the playlist");
}

void MainWindow::select_song(std::shared_ptr<Song> song) {
    active_song_name->SetLabelText(get_name(song->name));
    active_song = song;

    started = false;
}

std::string MainWindow::get_name(const std::string& file_path) {
    const size_t slash = file_path.find_last_of("/\\");
    return file_path.substr(slash + 1);
}
