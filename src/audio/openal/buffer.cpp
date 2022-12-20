#include <memory>
#include <stdlib.h>

#include <AL/al.h>
#include <spdlog/spdlog.h>

#include "audio/openal/buffer.h"
#include "audio/openal/info_and_debug.h"

static ALenum get_format(int channels, size_t bps) {
    ALenum format = 0;

    if (channels == 1 && bps == 8) {
        format = AL_FORMAT_MONO8;
    } else if (channels == 1 && bps == 16) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2 && bps == 8) {
        format = AL_FORMAT_STEREO8;
    } else if (channels == 2 && bps == 16) {
        format = AL_FORMAT_STEREO16;
    } else {
        spdlog::critical("Unknown format: channels = `{}`, bps = `{}`, exiting...", channels, bps);
        exit(1);
    }

    return format;
}

namespace al {
    Buffer::Buffer(const void* data, size_t size, int channels, size_t bps, int frequency) {
        alGenBuffers(1, &buffer);
        alBufferData(buffer, get_format(channels, bps), data, size, frequency);

        maybe_check_errors();

        spdlog::debug("Created AL buffer {}", buffer);
    }

    Buffer::Buffer(std::shared_ptr<SoundData> data) {
        alGenBuffers(1, &buffer);
        alBufferData(
            buffer,
            get_format(data->get_channels(), data->get_bps()),
            data->get_data(),
            data->get_size(),
            data->get_frequency()
        );

        maybe_check_errors();

        spdlog::debug("Created AL buffer {}", buffer);
    }

    Buffer::~Buffer() {
        for (ALuint source : sources_attached) {
            alSourceStop(source);
            alSourcei(source, AL_BUFFER, 0);

            maybe_check_errors();
        }

        alDeleteBuffers(1, &buffer);

        maybe_check_errors();

        spdlog::debug("Deleted AL buffer {}", buffer);
    }
}
