#include <string>
#include <string_view>
#include <assert.h>
#include <stdlib.h>

#include <stb_vorbis.h>
#include <spdlog/spdlog.h>

#include "audio/sound_data.h"

static void check_bits_per_sample(size_t bits_per_sample, std::string_view file_path) {
    if (bits_per_sample == 8) {
        spdlog::warn("bits_per_sample = 8 for sound file `{}`", file_path);
    }
}

SoundData::SoundData(std::string_view file_path)
    : file_path(file_path) {
    spdlog::debug("Loading sound data `{}`...", file_path);

    samples = stb_vorbis_decode_filename(file_path.data(), &channels, &sample_rate, &data);

    if (data == nullptr) {
        spdlog::critical("Could not load sound data `{}`, exiting...", file_path);
        exit(1);
    }

    size = compute_size();
    bits_per_sample = compute_bits_per_sample();

    check_bits_per_sample(bits_per_sample, file_path);
}

SoundData::~SoundData() {
    assert(data != nullptr);

    free(data);

    spdlog::debug("Freed sound data `{}`", file_path);
}

size_t SoundData::compute_size() {
    return samples * channels * sizeof(short);
}

size_t SoundData::compute_bits_per_sample() {
    return (8 * size) / (samples * channels);
}
