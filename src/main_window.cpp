#include <wx/wx.h>

#include "main_window.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_OPEN,   MainWindow::on_open)
    EVT_MENU(wxID_EXIT,  MainWindow::on_exit)
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
}

MainWindow::~MainWindow() {

}

void MainWindow::on_open(wxCommandEvent& event) {
    wxFileDialog* dialog = new wxFileDialog(this);
    dialog->Show();
}

void MainWindow::on_exit(wxCommandEvent& event) {
    wxExit();
}

void MainWindow::on_about(wxCommandEvent& event) {
    wxMessageBox("tiny-music-player, a music player.", "About", wxOK | wxICON_INFORMATION);
}

void MainWindow::on_play_pause(wxCommandEvent& event) {

}
