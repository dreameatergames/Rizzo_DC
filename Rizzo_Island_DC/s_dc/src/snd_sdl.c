
#include <kos.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_byteorder.h>
#include "quakedef.h"

static dma_t the_shm;
static int snd_inited;

extern int desired_speed;
extern int desired_bits;

static void paint_audio(void *unused, Uint8 *stream, int len)
{
	if ( shm ) {
		shm->buffer = stream;
		shm->samplepos += len/(shm->samplebits/8)/2;
		// Check for samplepos overflow?
		S_PaintChannels (shm->samplepos);
	}
}

qboolean SNDDMA_Init(void)
{
	SDL_AudioSpec desired, obtained;
	// Manoel Kasimier - begin
	flashrom_syscfg_t	flashcfg;
	flashrom_get_syscfg (&flashcfg);
	Cvar_SetValue("snd_stereo", flashcfg.audio);
	// Manoel Kasimier - end

//	desired_speed = 11025;//22050; // Manoel Kasimier - removed

	snd_inited = 0;

	// Manoel Kasimier - begin
	if (!desired_speed)
	{
		Con_Printf("Audio disabled\n");
		return 0;
	}
	// Manoel Kasimier - end

	/* Set up the desired format */
	desired.freq = desired_speed;
	switch (desired_bits) {
		case 8:
			desired.format = AUDIO_U8;
			break;
		case 16:
			if ( SDL_BYTEORDER == SDL_BIG_ENDIAN )
				desired.format = AUDIO_S16MSB;
			else
				desired.format = AUDIO_S16LSB;
			break;
		default:
        		Con_Printf("Unknown number of audio bits: %d\n",
								desired_bits);
			return 0;
	}
	desired.channels = 2;
#ifdef WIN32
	desired.samples = 2048;
#else
// Manoel Kasimier - begin
#ifdef _arch_dreamcast
	desired.samples = 512;//1024;
#else
// Manoel Kasimier - end
	desired.samples = 1024;
#endif // Manoel Kasimier
#endif
	desired.callback = paint_audio;
	// Manoel Kasimier - begin
	desired.userdata = NULL;
	if ( SDL_Init(0/*SDL_INIT_AUDIO*/) < 0 )
	{
		Con_Printf("Couldn't initialize SDL\n");
		return 0;
	}
	// Manoel Kasimier - end

	/* Open the audio device */
	if ( SDL_OpenAudio(&desired, &obtained) < 0 ) {
        	Con_Printf("Couldn't open SDL audio: %s\n", SDL_GetError());
		return 0;
	}

	/* Make sure we can support the audio format */
	switch (obtained.format) {
		case AUDIO_U8:
			/* Supported */
			break;
		case AUDIO_S16LSB:
		case AUDIO_S16MSB:
			if ( ((obtained.format == AUDIO_S16LSB) &&
			     (SDL_BYTEORDER == SDL_LIL_ENDIAN)) ||
			     ((obtained.format == AUDIO_S16MSB) &&
			     (SDL_BYTEORDER == SDL_BIG_ENDIAN)) ) {
				/* Supported */
				break;
			}
			/* Unsupported, fall through */;
		default:
			/* Not supported -- force SDL to do our bidding */
			SDL_CloseAudio();
			if ( SDL_OpenAudio(&desired, NULL) < 0 ) {
        			Con_Printf("Couldn't open SDL audio: %s\n",
							SDL_GetError());
				return 0;
			}
			memcpy(&obtained, &desired, sizeof(desired));
			break;
	}
	SDL_PauseAudio(0);

	/* Fill the audio DMA information block */
	shm = &the_shm;
	shm->splitbuffer = 0;
	shm->samplebits = (obtained.format & 0xFF);
	shm->speed = obtained.freq;
	shm->channels = obtained.channels;
	shm->samples = obtained.samples*shm->channels;
	shm->samplepos = 0;
	shm->submission_chunk = 1;
	shm->buffer = NULL;

	snd_inited = 1;
	return 1;
}

int SNDDMA_GetDMAPos(void)
{
	return shm->samplepos;
}

void SNDDMA_Shutdown(void)
{
	if (snd_inited)
	{
		SDL_CloseAudio();
		snd_inited = 0;
	}
}

void SNDDMA_Submit(void)
{
}
