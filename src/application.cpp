#include <memory>

#include <wx/wx.h>
#include <spdlog/spdlog.h>

#include "audio/context.h"
#include "application.h"
#include "main_window.h"

wxIMPLEMENT_APP(Application);

Application::Application() {

}

Application::~Application() {

}

bool Application::OnInit() {
    openal_context = std::make_unique<OpenAlContext>();
    openal_context->get_listener().set_distance_model(al::DistanceModel::None);

    spdlog::set_level(spdlog::level::debug);

    window = new MainWindow;
    window->Show();

    return true;
}
