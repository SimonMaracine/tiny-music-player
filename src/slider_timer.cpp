#include "slider_timer.h"

static int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void SliderTimer::Notify() {
    const int value = map(source->sample_offset(), 0, song->samples, sld_track->GetMin(), sld_track->GetMax());
    sld_track->SetValue(value);
}
