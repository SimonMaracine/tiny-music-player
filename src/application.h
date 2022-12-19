#pragma once

#include <wx/wx.h>

#include "main_window.h"

class Application : public wxApp {
public:
    Application();
    virtual ~Application();

    virtual bool OnInit() override;
private:
    MainWindow* window = nullptr;
};
