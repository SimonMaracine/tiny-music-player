#pragma once

#include <AL/al.h>

#include "other/vec3.h"

namespace al {
    enum class DistanceModel {
        None = AL_NONE,
        Exponent = AL_EXPONENT_DISTANCE,
        Inverse = AL_INVERSE_DISTANCE,
        Linear = AL_LINEAR_DISTANCE,
        ExponentClamped = AL_EXPONENT_DISTANCE_CLAMPED,
        InverseClamped = AL_INVERSE_DISTANCE_CLAMPED,
        LinearClamped = AL_LINEAR_DISTANCE_CLAMPED
    };

    class Listener {
    public:
        Listener() = default;
        ~Listener() = default;

        Listener(const Listener&) = delete;
        Listener& operator=(const Listener&) = delete;
        Listener(Listener&&) = delete;
        Listener& operator=(Listener&&) = delete;

        float get_gain() const { return gain; }
        const vec3& get_position() const { return position; }
        const vec3& get_velocity() const { return velocity; }
        const vec3& get_look_at() const { return look_at; }
        const vec3& get_up() const { return up; }
        DistanceModel get_distance_model() const { return distance_model; }

        void set_gain(float gain);
        void set_position(const vec3& position);
        void set_velocity(const vec3& velocity);
        void set_look_at_and_up(const vec3& look_at, const vec3& up);
        void set_distance_model(DistanceModel distance_model);
    private:
        float gain = 1.0f;
        vec3 position;
        vec3 velocity;
        vec3 look_at = vec3(0.0f, 0.0f, -1.0f);
        vec3 up = vec3(0.0f, 1.0f, 0.0f);
        DistanceModel distance_model = DistanceModel::InverseClamped;
    };
}
