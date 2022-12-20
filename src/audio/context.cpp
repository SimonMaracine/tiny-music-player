#include <stdlib.h>

#include <AL/alc.h>
#include <spdlog/spdlog.h>

#include "audio/context.h"
#include "audio/openal/listener.h"

static ALCdevice* _global_device = nullptr;
static ALCcontext* _global_context = nullptr;

#if 0
static void maybe_check_errors(ALCdevice* device) {
#ifdef PLATFORM_GAME_DEBUG
    const ALCenum error = alcGetError(device);

    if (error != ALC_NO_ERROR) {
        spdlog::critical("OpenAL Context Debug Error: {}", error);
        exit(1);
    }
#endif
}
#endif

OpenAlContext::OpenAlContext() {
    // Choose the default device
    device = alcOpenDevice(nullptr);

    if (device == nullptr) {
        spdlog::critical("Could not open an AL device, exiting...");
        exit(1);
    }

    context = alcCreateContext(device, nullptr);  // TODO maybe pass some context attributes

    if (context == nullptr) {
        spdlog::critical("Could not create an AL context, exiting...");
        exit(1);
    }

    if (alcMakeContextCurrent(context) == ALC_FALSE) {
        alcDestroyContext(context);
        alcCloseDevice(device);

        spdlog::critical("Could not make AL context current, exiting...");
        exit(1);
    }

    _global_device = device;
    _global_context = context;

    listener.set_distance_model(al::DistanceModel::InverseClamped);

    spdlog::info("Created OpenAL device and context");
}

OpenAlContext::~OpenAlContext() {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);

    _global_device = nullptr;
    _global_context = nullptr;

    spdlog::info("Destroyed OpenAL context and device");
}

// TODO not used
void destroy_openal_context() {
    alcMakeContextCurrent(nullptr);

    if (_global_context != nullptr) {
        alcDestroyContext(_global_context);

        if (_global_device != nullptr) {
            alcCloseDevice(_global_device);
        }
    }
}
