#include <assert.h>

#include <AL/al.h>
#include <spdlog/spdlog.h>

#include "audio/openal/source.h"
#include "audio/openal/info_and_debug.h"
#include "other/vec3.h"

namespace al {
    Source::Source() {
        alGenSources(1, &source);
        alSourcef(source, AL_ROLLOFF_FACTOR, rolloff_factor);
        alSourcef(source, AL_REFERENCE_DISTANCE, reference_distance);
        alSourcef(source, AL_MAX_DISTANCE, max_distance);

        maybe_check_errors();

        spdlog::debug("Created AL source {}", source);
    }

    Source::~Source() {
        stop();

        alDeleteSources(1, &source);

        maybe_check_errors();

        spdlog::debug("Deleted AL source {}", source);
    }

    void Source::play(Buffer* buffer) {
        stop();

        if (buffer->buffer != attached_buffer) {
            attached_buffer = buffer->buffer;
            alSourcei(source, AL_BUFFER, attached_buffer);

            maybe_check_errors();

            buffer->sources_attached.insert(source);
        }

        alSourcePlay(source);

        maybe_check_errors();
    }

    void Source::stop() {
        alSourceStop(source);

        maybe_check_errors();
    }

    void Source::pause() {
        alSourcePause(source);

        maybe_check_errors();
    }

    void Source::continue_() {
        alSourcePlay(source);

        maybe_check_errors();
    }

    bool Source::is_playing() {
        int state = 0;
        alGetSourcei(source, AL_SOURCE_STATE, &state);

        maybe_check_errors();

        return state == AL_PLAYING;
    }

    void Source::set_gain(float gain) {
        assert(gain >= 0.0f);

        alSourcef(source, AL_GAIN, gain);

        maybe_check_errors();

        this->gain = gain;
    }

    void Source::set_pitch(float pitch) {
        assert(pitch >= 0.0f);

        alSourcef(source, AL_PITCH, pitch);

        maybe_check_errors();

        this->pitch = pitch;
    }

    void Source::set_position(const vec3& position) {
        alSource3f(source, AL_POSITION, position.x, position.y, position.z);

        maybe_check_errors();

        this->position = position;
    }

    void Source::set_velocity(const vec3& velocity) {
        alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);

        maybe_check_errors();

        this->velocity = velocity;
    }

    void Source::set_direction(const vec3& direction) {
        alSource3f(source, AL_DIRECTION, direction.x, direction.y, direction.z);

        maybe_check_errors();

        this->direction = direction;
    }

    void Source::set_looping(bool looping) {
        alSourcei(source, AL_LOOPING, static_cast<ALint>(looping));

        maybe_check_errors();

        this->looping = looping;
    }

    void Source::set_rolloff_factor(float rolloff_factor) {
        alSourcef(source, AL_ROLLOFF_FACTOR, rolloff_factor);

        maybe_check_errors();

        this->rolloff_factor = rolloff_factor;
    }

    void Source::set_reference_distance(float reference_distance) {
        alSourcef(source, AL_REFERENCE_DISTANCE, reference_distance);

        maybe_check_errors();

        this->reference_distance = reference_distance;
    }

    void Source::set_max_distance(float max_distance) {
        alSourcef(source, AL_MAX_DISTANCE, max_distance);

        maybe_check_errors();

        this->max_distance = max_distance;
    }
}
