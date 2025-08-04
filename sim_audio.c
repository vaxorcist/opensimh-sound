/* sim_video.c: Bitmap video output

   Copyright (c) 2011-2013, Matt Burke

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
   THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
   IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

   Except as contained in this notice, the name of the author shall not be
   used in advertising or otherwise to promote the sale, use or other dealings
   in this Software without prior written authorization from the author.

   08-Nov-2013  MB      Added globals for current mouse status
   11-Jun-2013  MB      First version
*/

#if defined(HAVE_LIBPNG) && defined(USE_SIM_VIDEO) && defined(HAVE_LIBSDL)
#include <png.h>
#endif
#include "sim_video.h"
#include "scp.h"

#include <SDL_audio.h>
#include <math.h>

const int SOUND_AMPLITUDE = 20000;
const int SOUND_SAMPLE_FREQUENCY = 11025;
static int16 *vid_sound_data;
static int vid_sound_offset;
static int vid_sound_duration;
static int vid_sound_samples;

/* ===================================================================== */
static void vid_audio_callback(void *ctx, Uint8 *stream, int length)
/* ===================================================================== */
{
    int i, sum, remnant = ((vid_sound_samples - vid_sound_offset) * sizeof (*vid_sound_data));

    if (length > remnant) {
        memset (stream + remnant, 0, length - remnant);
        length = remnant;
        if (remnant == 0) {
            SDL_PauseAudio(1);
            return;
            }
        }
    memcpy (stream, &vid_sound_data[vid_sound_offset], length);
    for (i=sum=0; i<length; i++)
        sum += stream[i];
    vid_sound_offset += length / sizeof(*vid_sound_data);
}

/* ===================================================================== */
static void vid_sound_setup (int duration_ms, int tone_frequency)
/* ===================================================================== */
{
    if (!vid_sound_data) {
        int i;
        SDL_AudioSpec desiredSpec;

        SDL_InitSubSystem (SDL_INIT_AUDIO);
        memset (&desiredSpec, 0, sizeof(desiredSpec));
        desiredSpec.freq = SOUND_SAMPLE_FREQUENCY;
        desiredSpec.format = AUDIO_S16SYS;
        desiredSpec.channels = 1;
        desiredSpec.samples = 2048;
        desiredSpec.callback = vid_audio_callback;

        SDL_OpenAudio(&desiredSpec, NULL);

        vid_sound_samples = (int)((SOUND_SAMPLE_FREQUENCY * duration_ms) / 1000.0);
        vid_sound_duration = duration_ms;
        vid_sound_data = (int16 *)malloc (sizeof(*vid_sound_data) * vid_sound_samples);
        for (i=0; i<vid_sound_samples; i++)
            vid_sound_data[i] = (int16)(SOUND_AMPLITUDE * sin(((double)(i * M_PI * tone_frequency)) / SOUND_SAMPLE_FREQUENCY));
        }
}

/* ===================================================================== */
static void vid_sound_cleanup (void)
/* ===================================================================== */
{
    SDL_CloseAudio();
    free (vid_sound_data);
    vid_sound_data = NULL;
    SDL_QuitSubSystem (SDL_INIT_AUDIO);
}

/* ===================================================================== */
void vid_sound_event (void)
/* ===================================================================== */
{
    vid_sound_offset = 0;                /* reset to beginning of sample set */
    SDL_PauseAudio (0);                 /* Play sound */
}

/* ===================================================================== */
void vid_sound (void)
/* ===================================================================== */
{
    SDL_Event user_event;

    user_event.type = SDL_USEREVENT;
    user_event.user.code = 10;
    user_event.user.data1 = NULL;
    user_event.user.data2 = NULL;
    #if defined (SDL_MAIN_AVAILABLE)
    while (SDL_PushEvent (&user_event) < 0)
        sim_os_ms_sleep (10);
    #else
    vid_sound_event ();
    #endif
    SDL_Delay (vid_sound_duration + 100);/* Wait for sound to finish */
}

