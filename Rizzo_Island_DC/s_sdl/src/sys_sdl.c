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
// dc_sys.h -- System driver for SDL

#include "quakedef.h"

// Now using new, improved SDL
#include <SDL.h>

// BlackAura (08-12-2002) - Added definition of isDedicated
qboolean	isDedicated = false;
/*
===============================================================================

FILE IO

===============================================================================
*/

#define MAX_HANDLES             64 // edited
FILE    *sys_handles[MAX_HANDLES];

int             findhandle (void)
{
	int             i;
	
	for (i=1 ; i<MAX_HANDLES ; i++)
		if (!sys_handles[i])
			return i;
	Sys_Error ("out of handles");
	return -1;
}

/*
================
filelength
================
*/
int filelength (FILE *f)
{
	int             pos;
	int             end;

	pos = ftell (f);
	fseek (f, 0, SEEK_END);
	end = ftell (f);
	fseek (f, pos, SEEK_SET);

	return end;
}

int Sys_FileOpenRead (char *path, int *hndl)
{
	FILE    *f;
	int             i;
	
	i = findhandle ();

	f = fopen(path, "rb");
	if (!f)
	{
		*hndl = -1;
		return -1;
	}
	sys_handles[i] = f;
	*hndl = i;
	
	return filelength(f);
}

int Sys_FileOpenWrite (char *path)
{
	FILE    *f;
	int             i;
	
	i = findhandle ();

	f = fopen(path, "wb");
	if (!f)
		Sys_Error ("Error opening %s", path);
	sys_handles[i] = f;
	
	return i;
}

void Sys_FileClose (int handle)
{
	fclose (sys_handles[handle]);
	sys_handles[handle] = NULL;
}

void Sys_FileSeek (int handle, int position)
{
	fseek (sys_handles[handle], position, SEEK_SET);
}

int Sys_FileRead (int handle, void *dest, int count)
{
	return fread (dest, 1, count, sys_handles[handle]);
}

int Sys_FileWrite (int handle, void *data, int count)
{
	return fwrite (data, 1, count, sys_handles[handle]);
}

int     Sys_FileTime (char *path)
{
	FILE    *f;
	
	f = fopen(path, "rb");
	if (f)
	{
		fclose(f);
		return 1;
	}
	
	return -1;
}

void Sys_mkdir (char *path)
{
}


/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_MakeCodeWriteable (unsigned long startaddr, unsigned long length)
{
}


void Sys_Error (char *error, ...)
{
	va_list         argptr;

	printf ("Sys_Error: ");   
	va_start (argptr,error);
	vprintf (error,argptr);
	va_end (argptr);
	printf ("\n");

	exit (1);
}

void Sys_Printf (char *fmt, ...)
{
	va_list         argptr;
	
	va_start (argptr,fmt);
	vprintf (fmt,argptr);
	va_end (argptr);
}

void Sys_Quit (void)
{
	Host_Shutdown();
	exit (0);
}

double Sys_FloatTime (void)
{
	unsigned int msec;
	double time;
	msec = SDL_GetTicks();
	time = msec / 1000.0;
	return time;
}

char *Sys_ConsoleInput (void)
{
	return NULL;
}

void Sys_Sleep (void)
{
}

void Sys_HighFPPrecision (void)
{
}

void Sys_LowFPPrecision (void)
{
}

//=============================================================================

/*#define	SDL_INIT_TIMER		0x00000001
#define SDL_INIT_AUDIO		0x00000010
#define SDL_INIT_VIDEO		0x00000020
#define SDL_INIT_CDROM		0x00000100
#define SDL_INIT_JOYSTICK	0x00000200
#define SDL_INIT_NOPARACHUTE	0x00100000	/* Don't catch fatal signals 
#define SDL_INIT_EVENTTHREAD	0x01000000	/* Not supported on all OS's 
#define SDL_INIT_EVERYTHING	0x0000FFFF
*/

#define SDL_INIT_QUAKE	(SDL_INIT_EVERYTHING)

int main(int argc, char *argv[])
{
	static quakeparms_t    parms;
	double time, oldtime, newtime;
	char *pathptr;

	// Initialise SDL
	SDL_Init(SDL_INIT_QUAKE);
	atexit(SDL_Quit);

	// Change to the executable directory (because I'm lazy)
	pathptr = COM_SkipPath(argv[0]);
	pathptr--;
	*pathptr = '\0';
	chdir(argv[0]);
	*pathptr = '/';

	parms.memsize = 10*1024*1024;
	parms.membase = malloc (parms.memsize);
	parms.basedir = ".";

	COM_InitArgv (argc, argv);

	parms.argc = com_argc;
	parms.argv = com_argv;

	printf ("Host_Init\n");
	Host_Init (&parms);

	// BlackAura (08-12-2002) - Keep running frames
	oldtime = Sys_FloatTime() - 0.1;
	while (1)
	{
		// BlackAura (08-12-2002) - Calculate time
		newtime = Sys_FloatTime();
		time = newtime - oldtime;

		// BlackAura (08-12-2002) - Lock time
		if (time > sys_ticrate.value*2)
			oldtime = newtime;
		else
			oldtime += time;

		// BlackAura (08-12-2002) - Run a frame
		Host_Frame(time);
	}
}


