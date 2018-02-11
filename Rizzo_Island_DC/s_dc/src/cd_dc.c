/*
Copyright (C) 2005-2006 Manoel Kasimier ( manoelkasimier@yahoo.com.br )

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
#include "quakedef.h"

extern int DC_CheckDrive();
extern cvar_t snd_stereo, snd_swapstereo;
cvar_t cd_enabled = {"cd_enabled", "1", true};
static qboolean inited=false, cdstereo=true, cdswapstereo=false;
static float cdvolume = 0.0;

void CDAudio_Play(byte track, qboolean looping)
{
	int trnum = track;

	if(!inited || !cdValid || !cd_enabled.value)
		return;

	// Ignore if playing the same track
	if (playing && trnum == playTrack)
		return;

	// Ignore invalid tracks
	if (track < 1 || track > 99)
		return;
	if (!audioTrack[track])
		return;

	// Stop old track
	CDAudio_Stop();

	// Play new track
	cdrom_cdda_play(trnum, trnum, looping*15, CDDA_TRACKS);
	playLooping = looping;
	playTrack = trnum;
	playing = true;

	// Make sure the output is correct
	spu_cdda_volume((int)ceil(cdvolume*15.0), (int)ceil(cdvolume*15.0));
	if (!cdstereo)
		spu_cdda_pan(16, 16);
	else if (cdswapstereo)
		spu_cdda_pan(31, 0);
	else
		spu_cdda_pan(0, 31);
}

void CDAudio_Pause(void)
{
	if(!inited || !cdValid || !cd_enabled.value)
		return;
	if (DC_CheckDrive() == CD_STATUS_PLAYING)
		cdrom_cdda_pause();
	// Ignore if stopped or already paused
	if (!playing)
		return;
	playing = false;
	wasPlaying = true;
}

void CDAudio_Stop(void)
{
	if(!inited || !cdValid || !cd_enabled.value)
		return;
	CDAudio_Pause(); // Just pause, there's no need to spin down
	wasPlaying = false; // Fake stop
}

void CDAudio_Resume(void)
{
	// Ignore if stopped or already playing
	if(!inited || !cdValid || !cd_enabled.value || !wasPlaying || playing)
		return;
	if (DC_CheckDrive() == CD_STATUS_PAUSED)
		cdrom_cdda_resume();
	playing = true;
}

void CDAudio_Update(void)
{
	if(!inited || !cdValid || !cd_enabled.value)
		return;

	// Update volume
	if (cdvolume != bgmvolume.value)
	{
		cdvolume = bgmvolume.value;
		spu_cdda_volume((int)ceil(cdvolume*15.0), (int)ceil(cdvolume*15.0));
	}
	// Update stereo stuff
	if ((cdstereo != (qboolean)snd_stereo.value) || (cdswapstereo != (qboolean)snd_swapstereo.value))
	{
		cdstereo = (qboolean)snd_stereo.value;
		cdswapstereo = (qboolean)snd_swapstereo.value;
		if (!cdstereo)
			spu_cdda_pan(16, 16);
		else if (cdswapstereo)
			spu_cdda_pan(31, 0);
		else
			spu_cdda_pan(0, 31);
	}

	if (playing)
		if (DC_CheckDrive() == CD_STATUS_PAUSED)
			// the GD-ROM drive probably stopped to load something, so let's restart the music
			cdrom_cdda_play((int)playTrack, (int)playTrack, playLooping*15, CDDA_TRACKS);
}

static void CD_help (void)
{
	Con_Printf("CD parameters:\n on\n off\n play <track>\n loop <track>\n pause\n resume\n stop\n info\n");
}

static void CD_f (void)
{
	int		i;
	char	*command;
	command = Cmd_Argv(1);

	if (Cmd_Argc() < 2)
	{
		CD_help();
		return;
	}

	if (!Q_strcasecmp(command, "off"))
	{
		CDAudio_Stop();
		Cvar_SetValue("cd_enabled", 0);
	}
	else if (!Q_strcasecmp(command, "on"))		Cvar_SetValue("cd_enabled", 1);
	else if (!Q_strcasecmp(command, "play"))	Cmd_Argc() == 3 ? CDAudio_Play((byte)Q_atoi(Cmd_Argv (2)), false) : CD_help();
	else if (!Q_strcasecmp(command, "loop"))	Cmd_Argc() == 3 ? CDAudio_Play((byte)Q_atoi(Cmd_Argv (2)), true) : CD_help();
	else if (!Q_strcasecmp(command, "pause"))	CDAudio_Pause();
	else if (!Q_strcasecmp(command, "resume"))	CDAudio_Resume();
	else if (!Q_strcasecmp(command, "stop"))	CDAudio_Stop();
	else if (!Q_strcasecmp(command, "info"))
	{
		Con_Printf("%u tracks\n", maxTrack);
		if (!cdValid)
			Con_Printf("No audio tracks on disc\n");
		else // List valid tracks
		{
			Con_Printf("Audio tracks on disc:\n");
			for (i=1; i<=99; i++)
				if (audioTrack[i])
				{
					if (!audioTrack[i+1]) // end of sequence, or isolated track
						Con_Printf(" %2i\n", i);
					else if (!audioTrack[i-1]) // beginning of sequence
						Con_Printf(" %2i -", i);
				}
			if (cd_enabled.value)
			{
				if (DC_CheckDrive() == CD_STATUS_PLAYING)
					Con_Printf("Currently %s track %u\n", playLooping ? "looping" : "playing", playTrack);
				else if (!playing && wasPlaying)
					Con_Printf("Paused %s track %u\n", playLooping ? "looping" : "playing", playTrack);
				else if (playing)
					Con_Printf("Failed to %s track %u\n", playLooping ? "loop" : "play", playTrack);
			}
		}

		if (!cd_enabled.value)
			Con_Printf("CD audio is off\n");
		Con_Printf("Volume is %.1f\n", cdvolume);
		return;
	}
	else // invalid parameter
		CD_help();
}

void cd_cvar_check(void)
{
	if (!cd_enabled.value)
	{
		cd_enabled.value = 1;
		if (playing)
			CDAudio_Stop();
		cd_enabled.value = 0;
	}
}

int CDAudio_Init(void)
{
	int track, first, last;
	CDROM_TOC toc;

	Cvar_RegisterVariableWithCallback(&cd_enabled, cd_cvar_check);
	Cmd_AddCommand ("cd", CD_f);

	cdValid = false;
	maxTrack = 0;
	for (track=0; track<=100; track++)
		audioTrack[track] = false;
	if (!cdrom_read_toc(&toc, 0))
	{
		first = TOC_TRACK(toc.first);
		last = TOC_TRACK(toc.last);
		if (first < 1 || last > 99 || first > last)
			return -1;
		maxTrack = last;
		for (track=first; track<=maxTrack; track++)
		{
			if (TOC_CTRL(toc.entry[track-1]) == 0) // is an audio track
			{
				audioTrack[track] = true;
				cdValid = true;
			}
		}
	}
	if (cdValid == false)
		return -1;

	inited = true;
	CDAudio_Update();
	return 0;
}

void CDAudio_Shutdown(void)
{
	inited = false;
	cdrom_spin_down();
}
