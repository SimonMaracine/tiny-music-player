#include <AL/alc.h>

#include "audio/context.h"
#include "audio/openal/listener.h"

static ALCdevice* _global_device = nullptr;
static ALCcontext* _global_context = nullptr;

#if 0
static void maybe_check_errors(ALCdevice* device) {
#ifdef PLATFORM_GAME_DEBUG
    const ALCenum error = alcGetError(device);

    if (error != ALC_NO_ERROR) {
        // REL_CRITICAL("OpenAL Context Debug Error: {}", error);
        // game_exit::exit_critical();
    }
#endif
}
#endif

OpenAlContext::OpenAlContext() {
    // Choose the default device
    device = alcOpenDevice(nullptr);

    if (device == nullptr) {
        // REL_CRITICAL("Could not open an AL device, exiting...");
        // game_exit::exit_critical();
    }

    context = alcCreateContext(device, nullptr);  // TODO maybe pass some context attributes

    if (context == nullptr) {
        // REL_CRITICAL("Could not create an AL context, exiting...");
        // game_exit::exit_critical();
    }

    if (alcMakeContextCurrent(context) == ALC_FALSE) {
        alcDestroyContext(context);
        alcCloseDevice(device);

        // REL_CRITICAL("Could not make AL context current, exiting...");
        // game_exit::exit_critical();
    }

    _global_device = device;
    _global_context = context;

    listener.set_distance_model(al::DistanceModel::InverseClamped);

    // DEB_INFO("Created OpenAL device and context");
}

OpenAlContext::~OpenAlContext() {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);

    _global_device = nullptr;
    _global_context = nullptr;

    // DEB_INFO("Destroyed OpenAL context and device");
}

void destroy_openal_context() {
    alcMakeContextCurrent(nullptr);

    if (_global_context != nullptr) {
        alcDestroyContext(_global_context);

        if (_global_device != nullptr) {
            alcCloseDevice(_global_device);
        }
    }
}
