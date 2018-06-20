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
// dc_sys.c -- Dreamcast/KOS System Driver

#include "quakedef.h"
#include "errno.h"

#include <arch/timer.h>
#include <arch/arch.h>
#include <dc/cdrom.h>
#include <kos.h>

// BlackAura (08-12-2002) - Added definition of isDedicated
qboolean	isDedicated = false;

/*
===============================================================================

FILE IO

===============================================================================
*/

#define MAX_HANDLES             64
FILE    *sys_handles[MAX_HANDLES];

int findhandle (void)
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


static void drawtext(int x, int y, char *string)
{
	int offset = ((y*640)+x);
	bfont_draw_str(vram_s + offset, 640, 1, string);
}

void Sys_Error (char *error, ...)
{
	va_list         argptr;
	char			strbuffer[2048];

	printf ("Sys_Error: ");   
	va_start (argptr, error);
	vsprintf (strbuffer, error, argptr);
	va_end (argptr);
	printf ("%s\n", strbuffer);

	/* Reset video mode, clear screen */
	vid_set_mode(DM_640x480, PM_RGB565);
	vid_empty();

	/* Display the error message on screen */
	drawtext(32, 64, "Rizzo Island - Fatal Error");
	drawtext(32, 96, strbuffer);

	/* Hang - infinite loop */
	while(1);

	exit (1);
}

void Sys_Printf (char *fmt, ...)
{
	va_list         argptr;
	char			strbuffer[2048];

	va_start (argptr, fmt);
	vsprintf (strbuffer, fmt, argptr);
	printf (strbuffer);
	va_end (argptr);
}

//int main (); // Manoel Kasimier
//extern unsigned char *hunk_base; // Manoel Kasimier
void Sys_Quit (void)
{
	// BlackAura (08-12-2002) - Reboot to main system menu
	Host_Shutdown();

//	free(hunk_base); // Manoel Kasimier
//	main (); // Manoel Kasimier

	arch_menu();
}

double Sys_FloatTime (void)
{
	uint32 sec, msec;
	double time;
	timer_ms_gettime(&sec, &msec);
	time = sec + (msec / 1000.0);
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

// BlackAura (09-12-2002) - Reboot if disc lid is open
int DC_CheckDrive()
{
	int disc_status, disc_type;

	// Get drive and disc status
	cdrom_get_status(&disc_status, &disc_type);

	return disc_status;
}

char dc_basedir[16] = "/cd/rizzo"; // Manoel Kasimier

static void assert_hnd(const char * file, int line, const char * expr, const char * msg, const char * func)
{
	char strbuffer[1024];

	/* Reset video mode, clear screen */
	vid_set_mode(DM_640x480, PM_RGB565);
	vid_empty();

	/* Display the error message on screen */
	drawtext(32, 64, "Rizzo Island - Assertion failure");

	sprintf(strbuffer, " Location: %s, line %d (%s)", file, line, func);
	drawtext(32, 96, strbuffer);

	sprintf(strbuffer, "Assertion: %s", expr);
	drawtext(32, 128, strbuffer);

	sprintf(strbuffer, "  Message: %s", msg);
	drawtext(32, 160, strbuffer);

	/* Hang - infinite loop */
	while(1);
}

#include <assert.h>

//static int this_argc = 1;
//static char *this_argv[] = {"quake"};

// BlackAura (24-12-2002) - Modlist interface
extern int	modmenu_argc;
extern char	*modmenu_argv[MAX_NUM_ARGVS]; // Manoel Kasimier - edited
extern void ModMenu(const char *basedir);

int mem_size = 10*1024*1024; // Manoel Kasimier

int main ()
{
	static quakeparms_t    parms;
	double time, oldtime, newtime;

	// BlackAura (24-12-2002) - Jump to the ModMenu
	ModMenu(dc_basedir);

	// Set up assertion handler
	assert_set_handler(assert_hnd);

	// BlackAura (08-12-2002) - Allocate heap
	vid_clear(0, 0, 0);
	parms.memsize = mem_size; // Manoel Kasimier - edited
	parms.membase = malloc(parms.memsize);
	vid_clear(0, 0, 0);

	// BlackAura (08-12-2002) - Set base directory
	parms.basedir = dc_basedir;

	// BlackAura (24-12-2002) - Initialise arguments from modlist
	COM_InitArgv (modmenu_argc, modmenu_argv);
//	COM_InitArgv (this_argc, this_argv);
	parms.argc = com_argc;
	parms.argv = com_argv;

	// BlackAura (08-12-2002) - Init the host
	printf ("Host_Init\n");
	Host_Init (&parms);

	// BlackAura (08-12-2002) - Keep running frames
	oldtime = Sys_FloatTime() - 0.1;
	while(1)
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

		Vibration_Update (); // Manoel Kasimier

		// BlackAura (09-12-2002) - Check if the CD drive has been opened
		if(DC_CheckDrive() == CD_STATUS_OPEN) // Drive is open - return to BIOS menu
			Sys_Quit();
	}

	return 0;
}


