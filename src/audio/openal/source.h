#pragma once

#include <AL/al.h>

#include "audio/openal/buffer.h"
#include "other/vec3.h"

namespace al {
    class Source {
    public:
        Source();
        ~Source();

        Source(const Source&) = delete;
        Source& operator=(const Source&) = delete;
        Source(Source&&) = delete;
        Source& operator=(Source&&) = delete;

        void play(Buffer* buffer);
        void stop();
        void pause();
        void continue_();

        float get_gain() { return gain; }
        float get_pitch() { return pitch; }
        const vec3& get_position() { return position; }
        const vec3& get_velocity() { return velocity; }
        const vec3& get_direction() { return direction; }
        bool get_looping() { return looping; }
        float get_rolloff_factor() { return rolloff_factor; }
        float get_reference_distance() { return reference_distance; }
        float get_max_distance() { return max_distance; }

        bool is_playing();

        void set_gain(float gain);
        void set_pitch(float pitch);
        void set_position(const vec3& position);
        void set_velocity(const vec3& velocity);
        void set_direction(const vec3& direction);
        void set_looping(bool looping);
        void set_rolloff_factor(float rolloff_factor);
        void set_reference_distance(float reference_distance);
        void set_max_distance(float max_distance);
    private:
        ALuint source = 0;
        ALuint attached_buffer = 0;  // The buffer that is currently attached to this source

        float gain = 1.0f;
        float pitch = 1.0f;
        vec3 position;
        vec3 velocity;
        vec3 direction;
        bool looping = false;

        float rolloff_factor = 1.0f;  // How fast the gain loses energy with distance
        float reference_distance = 8.0f;  // The distance at which gain is 1.0
        float max_distance = 22.0f;  // The distance at which the gain is 0.0 (linear) or at which the gain doesn't decrease (clamped)
    };
}
