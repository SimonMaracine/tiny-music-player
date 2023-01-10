#include <string>
#include <iostream>
#include <stdio.h>

#include <AL/al.h>
#include <spdlog/spdlog.h>

#include "audio/openal/info_and_debug.h"

namespace al {
    void maybe_check_errors() {
#ifndef NDEBUG
        const ALenum error = alGetError();

        if (error != AL_NO_ERROR) {
            std::cout << "OpenAL Debug Error:" << std::endl;

            switch(error) {
                case AL_INVALID_NAME:
                    spdlog::critical("({}) AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function", error);
                    break;
                case AL_INVALID_ENUM:
                    spdlog::critical("({}) AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function", error);
                    break;
                case AL_INVALID_VALUE:
                    spdlog::critical("({}) AL_INVALID_VALUE: an invalid value was passed to an OpenAL function", error);
                    break;
                case AL_INVALID_OPERATION:
                    spdlog::critical("({}) AL_INVALID_OPERATION: the requested operation is not valid", error);
                    break;
                case AL_OUT_OF_MEMORY:
                    spdlog::critical("({}) AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory", error);
                    break;
                default:
                    spdlog::critical("({}) Unknown AL error", error);
            }

            exit(1);
        }
#endif
    }

    std::string get_info() {
        std::string output;

        output.append("\n*** OpenAL Version ***\n");

        constexpr size_t LENGTH = 256;  // 256 should be enough

        char line[LENGTH];
        snprintf(line, LENGTH, "OpenAL version: %s\n", alGetString(AL_VERSION));
        output.append(line);

        return output;
    }

    const char* get_version() {
        return alGetString(AL_VERSION);
    }
}
