#include <assert.h>

#include <AL/al.h>

#include "audio/openal/listener.h"
#include "audio/openal/info_and_debug.h"
#include "other/vec3.h"

namespace al {
    void Listener::set_gain(float gain) {
        assert(gain >= 0.0f);

        alListenerf(AL_GAIN, gain);

        maybe_check_errors();

        this->gain = gain;
    }

    void Listener::set_position(const vec3& position) {
        alListener3f(AL_POSITION, position.x, position.y, position.z);

        maybe_check_errors();

        this->position = position;
    }

    void Listener::set_velocity(const vec3& velocity) {
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);

        maybe_check_errors();

        this->velocity = velocity;
    }

    void Listener::set_look_at_and_up(const vec3& look_at, const vec3& up) {
        const float orientation[6] = {
            look_at.x, look_at.y, look_at.z,
            up.x, up.y, up.z
        };

        alListenerfv(AL_ORIENTATION, orientation);

        maybe_check_errors();

        this->look_at = look_at;
        this->up = up;
    }

    void Listener::set_distance_model(DistanceModel distance_model) {
        alDistanceModel(static_cast<ALenum>(distance_model));

        maybe_check_errors();

        this->distance_model = distance_model;
    }
}
