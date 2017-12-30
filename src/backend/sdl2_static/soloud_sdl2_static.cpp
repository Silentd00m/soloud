/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/
#include <stdlib.h>

#include "soloud.h"

#if !defined(WITH_SDL2_STATIC)

namespace SoLoud
{
result sdl2static_init(SoLoud::Soloud* aSoloud, int const& aDevice,
                       unsigned int aFlags, unsigned int aSamplerate,
                       unsigned int aBuffer)
{
    return NOT_IMPLEMENTED;
}
}

#else

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include <math.h>

namespace SoLoud
{
static SDL_AudioSpec gActiveAudioSpec;

void soloud_sdl2static_audiomixer(void* userdata, Uint8* stream, int len)
{
    short*          buf    = (short*)stream;
    SoLoud::Soloud* soloud = (SoLoud::Soloud*)userdata;
    if(gActiveAudioSpec.format == AUDIO_F32) {
        int samples = len / (gActiveAudioSpec.channels * sizeof(float));
        soloud->mix((float*)buf, samples);
    } else // assume s16 if not float
    {
        int samples = len / (gActiveAudioSpec.channels * sizeof(short));
        soloud->mixSigned16(buf, samples);
    }
}

static void soloud_sdl2static_deinit(SoLoud::Soloud* aSoloud)
{
    SDL_CloseAudioDevice(aSoloud->mDeviceHandle);
}

result sdl2static_init(SoLoud::Soloud* aSoloud, int const& aDevice,
                       unsigned int aFlags, unsigned int aSamplerate,
                       unsigned int aBuffer, unsigned int aChannels)
{
    SDL_AudioSpec as;
    as.freq     = aSamplerate;
    as.format   = AUDIO_F32;
    as.channels = aChannels;
    as.samples  = aBuffer;
    as.callback = soloud_sdl2static_audiomixer;
    as.userdata = (void*)aSoloud;

    // if(SDL_OpenAudio(&as, &gActiveAudioSpec) < 0) {
    //     as.format = AUDIO_S16;
    //     if(SDL_OpenAudio(&as, &gActiveAudioSpec) < 0 ||
    //        gActiveAudioSpec.format != AUDIO_S16) {
    //         return UNKNOWN_ERROR;
    //     }
    // }

    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &as, &gActiveAudioSpec,
                                                SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    aSoloud->postinit(gActiveAudioSpec.freq, gActiveAudioSpec.samples, aFlags,
                      gActiveAudioSpec.channels);

    aSoloud->mBackendCleanupFunc = soloud_sdl2static_deinit;

    SDL_PauseAudio(0);
    aSoloud->mBackendString = "SDL2 (static)";
    return 0;
}
};
#endif
