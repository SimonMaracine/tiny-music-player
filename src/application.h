#pragma once

#include <memory>

#include <wx/wx.h>

#include "audio/context.h"
#include "main_window.h"

class Application : public wxApp {
public:
    Application();
    virtual ~Application();

    virtual bool OnInit() override;
private:
    MainWindow* window = nullptr;
    std::unique_ptr<OpenAlContext> openal_context;
};
