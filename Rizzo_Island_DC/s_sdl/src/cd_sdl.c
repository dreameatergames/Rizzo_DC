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
#include "quakedef.h"
#include <SDL.h>
static qboolean cdValid = true;
static qboolean initialized = true;
static qboolean enabled = true;
static qboolean playLooping = false;
static SDL_CD *cd_id;
static float cdvolume = 1.0;

static void CD_f();

static void CDAudio_Eject()
{
	if(!cd_id || !enabled) return;

	if(SDL_CDEject(cd_id))
		Con_DPrintf("Unable to eject CD-ROM tray.\n");
}

void CDAudio_Play(byte track, qboolean looping)
{
	CDstatus cd_stat;
	if(!cd_id || !enabled) return;
	
	if(!cdValid)
	{
		if(!CD_INDRIVE(cd_stat=SDL_CDStatus(cd_id)) ||(!cd_id->numtracks)) return;
		cdValid = true;
	}

	if((track < 1) || (track >= cd_id->numtracks))
	{
		Con_DPrintf("CDAudio: Bad track number: %d\n",track);
		return;
	}
	track--; /* Convert track from person to SDL value */
	if(cd_stat == CD_PLAYING)
	{
		if(cd_id->cur_track == track) return;
		CDAudio_Stop();
	}

	if(SDL_CDPlay(cd_id,cd_id->track[track].offset,
			  cd_id->track[track].length))
	{
		Con_DPrintf("CDAudio_Play: Unable to play track: %d\n",track+1);
		return;
	}
	playLooping = looping;
}


void CDAudio_Stop()
{
	int cdstate;
	if(!cd_id || !enabled) return;
	cdstate = SDL_CDStatus(cd_id);
	if((cdstate != CD_PLAYING) && (cdstate != CD_PAUSED)) return;

	if(SDL_CDStop(cd_id))
		Con_DPrintf("CDAudio_Stop: Failed to stop track.\n");
}

void CDAudio_Pause()
{
	if(!cd_id || !enabled) return;
	if(SDL_CDStatus(cd_id) != CD_PLAYING) return;

	if(SDL_CDPause(cd_id))
		Con_DPrintf("CDAudio_Pause: Failed to pause track.\n");
}


void CDAudio_Resume()
{
	if(!cd_id || !enabled) return;
	if(SDL_CDStatus(cd_id) != CD_PAUSED) return;

	if(SDL_CDResume(cd_id))
		Con_DPrintf("CDAudio_Resume: Failed tp resume track.\n");
}

void CDAudio_Update()
{
	if(!cd_id || !enabled) return;
	if(bgmvolume.value != cdvolume)
	{
		if(cdvolume)
		{
			Cvar_SetValue("bgmvolume",0.0);
			CDAudio_Pause();
		}
		else
		{
			Cvar_SetValue("bgmvolume",1.0);
			CDAudio_Resume();
		}
		cdvolume = bgmvolume.value;
		return;
	}
	if(playLooping && (SDL_CDStatus(cd_id) != CD_PLAYING)
		 && (SDL_CDStatus(cd_id) != CD_PAUSED))
		CDAudio_Play(cd_id->cur_track+1,true);
}

int CDAudio_Init()
{
	if((cls.state == ca_dedicated) || COM_CheckParm("-nocdaudio"))
		return -1;
		
	cd_id = SDL_CDOpen(0);
	if(!cd_id)
	{
		Con_Printf("CDAudio_Init: Unable to open default CD-ROM drive: %s\n",
			SDL_GetError());
		return -1;
	}
	
	initialized = true;
	enabled = true;
	cdValid = true;
	
	if(!CD_INDRIVE(SDL_CDStatus(cd_id)))
	{
		Con_Printf("CDAudio_Init: No CD in drive.\n");
		cdValid = false;
	}
	if(!cd_id->numtracks)
	{
		Con_Printf("CDAudio_Init: CD contains no audio tracks.\n");
		cdValid = false;
	}
	Cmd_AddCommand("cd",CD_f);
	Con_Printf("CD Audio Initialized.\n");
	return 0;
}


void CDAudio_Shutdown()
{
	if(!cd_id) return;
	CDAudio_Stop();
	SDL_CDClose(cd_id);
	cd_id = NULL;
}

static void CD_f()
{
	char *command;
	int cdstate;
	if(Cmd_Argc() < 2) return;

	command = Cmd_Argv(1);
	if(!Q_strcasecmp(command,"on"))
	{
		enabled = true;
	}
	if(!Q_strcasecmp(command,"off"))
	{
		if(!cd_id) return;
		cdstate = SDL_CDStatus(cd_id);
		if((cdstate == CD_PLAYING) || (cdstate == CD_PAUSED))
			CDAudio_Stop();
		enabled = false;
		return;
	}
	if(!Q_strcasecmp(command,"play"))
	{
		CDAudio_Play(Q_atoi(Cmd_Argv(2)),false);
		return;
	}
	if(!Q_strcasecmp(command,"loop"))
	{
		CDAudio_Play(Q_atoi(Cmd_Argv(2)),true);
		return;
	}
	if(!Q_strcasecmp(command,"stop"))
	{
		CDAudio_Stop();
		return;
	}
	if(!Q_strcasecmp(command,"pause"))
	{
		CDAudio_Pause();
		return;
	}
	if(!Q_strcasecmp(command,"resume"))
	{
		CDAudio_Resume();
		return;
	}
	if(!Q_strcasecmp(command,"eject"))
	{
		CDAudio_Eject();
		return;
	}
	if(!Q_strcasecmp(command,"info"))
	{
		if(!cd_id) return;
		cdstate = SDL_CDStatus(cd_id);
		Con_Printf("%d tracks\n",cd_id->numtracks);
		if(cdstate == CD_PLAYING)
			Con_Printf("Currently %s track %d\n",
				playLooping ? "looping" : "playing",
				cd_id->cur_track+1);
		else
		if(cdstate == CD_PAUSED)
			Con_Printf("Paused %s track %d\n",
				playLooping ? "looping" : "playing",
				cd_id->cur_track+1);
		return;
	}
}
