#pragma once

#include <vector>
#include <memory>
#include <string>

#include <wx/wx.h>
#include <wx/gbsizer.h>

#include "audio/openal/source.h"
#include "slider_timer.h"
#include "song.h"

class MainWindow : public wxFrame {
public:
    MainWindow();
    ~MainWindow();
private:
    void setup_menubar();
    void setup_widgets();

    void on_open(wxCommandEvent&);
    void on_exit(wxCommandEvent&);
    void on_about(wxCommandEvent&);
    void on_playlist_clear(wxCommandEvent&);

    void on_play_pause(wxCommandEvent&);
    void on_stop(wxCommandEvent&);

    void on_list_box_select(wxCommandEvent&);

    std::shared_ptr<Song> load_song(const wxString& file_path);
    void add_song_to_playlist(std::shared_ptr<Song> song);
    void select_song(std::shared_ptr<Song> song);
    static std::string get_name(const std::string& file_path);

    wxDECLARE_EVENT_TABLE();

    wxGridBagSizer* szr_main = nullptr;

    wxButton* btn_play_pause = nullptr;
    wxButton* btn_stop = nullptr;
    wxSlider* sld_track = nullptr;

    wxListBox* lst_songs = nullptr;
    wxStaticText* active_song_name = nullptr;

    std::unique_ptr<SliderTimer> timer;

    std::unique_ptr<al::Source> source;
    bool started = false;

    std::shared_ptr<Song> active_song = nullptr;
    std::vector<std::shared_ptr<Song>> songs;
    int active_song_index = -1;
    int playing_song_index = -1;
};
