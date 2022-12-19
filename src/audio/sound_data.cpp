#include <string>
#include <string_view>

#include <stb_vorbis.h>

#include "audio/sound_data.h"
#include "other/logging.h"

// TODO remained here
static void check_bits_per_sample(size_t bits_per_sample, std::string_view file_path) {
    if (bits_per_sample == 8) {
        // LOG_WARNING("bits_per_sample = 8 for sound file `{}`", file_path);
    }
}

SoundData::SoundData(std::string_view file_path)
    : file_path(file_path) {
    // DEB_DEBUG("Loading sound data `{}`...", file_path);

    samples = stb_vorbis_decode_filename(file_path.data(), &channels, &sample_rate, &data);

    if (data == nullptr) {
        // REL_CRITICAL("Could not load sound data `{}`, exiting...", file_path);
        // game_exit::exit_critical();
    }

    size = compute_size();
    bits_per_sample = compute_bits_per_sample();

    check_bits_per_sample(bits_per_sample, file_path);
}

SoundData::~SoundData() {
    // ASSERT(data != nullptr, "No data");

    free(data);

    // DEB_DEBUG("Freed sound data `{}`", file_path);
}

size_t SoundData::compute_size() {
    return samples * channels * sizeof(short);
}

size_t SoundData::compute_bits_per_sample() {
    return (8 * size) / (samples * channels);
}
