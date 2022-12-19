#include <wx/wx.h>

#include "application.h"
#include "main_window.h"

wxIMPLEMENT_APP(Application);

Application::Application() {

}

Application::~Application() {

}

bool Application::OnInit() {
    window = new MainWindow();
    window->Show();

    return true;
}
