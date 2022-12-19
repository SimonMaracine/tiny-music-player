#pragma once

struct vec3 {
    vec3() = default;
    vec3(float x, float y, float z)
        : x(x), y(y), z(z) {}
    vec3(float value)
        : x(value), y(value), z(value) {}

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};
