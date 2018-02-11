/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <kos.h> 
#include <stdio.h> 
#include "quakedef.h" 

static dma_t the_shm; 
static int snd_inited; 

extern int desired_speed; 
extern int desired_bits; 

static void paint_audio(void *unused, unsigned char *stream, int len) 
{ 
   if ( shm ) { 
      shm->buffer = stream; 
      shm->samplepos += len/(shm->samplebits/8)/2; 
      // Check for samplepos overflow? 
      S_PaintChannels (shm->samplepos); 
   } 
} 
//snd_stream_callback_t
static void sound_cb(/*snd_stream_hnd_t hnd*/int something, int len, unsigned char *stream)
{ 
   paint_audio(NULL, stream,len); 
} 

qboolean SNDDMA_Init(void) 
{ 
   static snd_stream_hnd_t stream_hnd = -1; 
   // Manoel Kasimier - begin 
   flashrom_syscfg_t   flashcfg; 
   flashrom_get_syscfg (&flashcfg); 
   Cvar_SetValue("snd_stereo", flashcfg.audio); 
   // Manoel Kasimier - end 

   snd_inited = 0; 

   // Manoel Kasimier - begin 
   if (!desired_speed) 
   { 
      Con_Printf("Audio disabled\n"); 
      return 0; 
   } 
   // Manoel Kasimier - end 

   if ( snd_stream_init() < 0 ) 
   { 
      Con_Printf("Couldn't initialize KOS Audio\n"); 
      return 0; 
   } 
   // Manoel Kasimier - end 

   stream_hnd = snd_stream_alloc(NULL, SND_STREAM_BUFFER_MAX); 
   if ( stream_hnd == -1 ) { 
           Con_Printf("Couldn't open KOS audio.\n"); 
      return 0; 
   } 

   stream_hnd = snd_stream_reinit(stream_hnd, (void *)sound_cb); 

   snd_stream_start(stream_hnd, desired_speed, 1); 
    
   /* Fill the audio DMA information block */ 
   shm = &the_shm; 
   shm->splitbuffer = 0; 
   shm->samplebits = (/*AUDIO_S16LSB*/ 0x8010 & 0xFF); 
   shm->speed = desired_speed; 
   shm->channels = 2; 
   shm->samples = 512*shm->channels; 
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
      snd_stream_shutdown(); 
      snd_inited = 0; 
   } 
} 

void SNDDMA_Submit(void) 
{ 
} 
