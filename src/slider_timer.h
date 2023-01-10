#pragma once

#include <memory>

#include <wx/wx.h>

#include "audio/openal/source.h"
#include "song.h"

class SliderTimer : public wxTimer {
public:
    SliderTimer() = default;
    SliderTimer(wxSlider* sld_track, std::shared_ptr<Song> song, al::Source* source)
        : wxTimer(), sld_track(sld_track), song(song), source(source) {}
    virtual ~SliderTimer() = default;

    SliderTimer(const SliderTimer&) = delete;
    SliderTimer& operator=(const SliderTimer&) = delete;
    SliderTimer(SliderTimer&&) = delete;
    SliderTimer& operator=(SliderTimer&&) = delete;

    virtual void Notify() override;
private:
    wxSlider* sld_track = nullptr;
    std::shared_ptr<Song> song;
    al::Source* source = nullptr;
};
