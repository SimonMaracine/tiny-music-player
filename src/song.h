#pragma once

#include <memory>
#include <string>

#include "audio/openal/buffer.h"

struct Song {
    std::shared_ptr<al::Buffer> buffer;
    int samples = 0;
    std::string name;
};
