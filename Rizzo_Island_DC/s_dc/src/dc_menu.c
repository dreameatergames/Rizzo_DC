/*
Copyright (C) 2002-2003 BlackAura (obsidianglow@HotPOP.com)
Copyright (C) 1996-2003 Id Software, Inc.

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

// ===========================================
// Quake Dreamcast Modmenu
// By BlackAura
// ===========================================

#include <kos.h>

// Manoel Kasimier - begin
extern int dcvidmode;
char *vidnames[] =
{
	"320x200    ", // to make BA happy =)
	"320x240    ",
	"320x480    ",
	"640x240    ",
	"640x400    ",
	"640x480    ",
	"768x480    ", // TV only / PAL / NTSC widescreen?
	"768x576 PAL",
	"800x608 VGA",
};
extern int DC_CheckDrive();
int		prevcdstate, cdstate;

extern int mem_size;
extern int desired_speed;
extern char dc_basedir[16];
// Manoel Kasimier - end

int		modmenu_argc = 0;
char	*modmenu_argv[MAX_NUM_ARGVS]; // Manoel Kasimier - edited

typedef struct modentry_s
{
	char	fullpath[256];
	char	game_name[256];
	char	game_dir[256];
	char	game_args[256];
	char	game_text[1024];
	int		sound, video, heapsize; // Manoel Kasimier
} modentry_t;

#define MAX_MODLIST		100
#define MODLIST_SIZE	(sizeof(modentry_t) * MAX_MODLIST)
static modentry_t		*modlist = NULL;
static int				modlist_c = 0;

static void ModList_Clear()
{
	int i;
	memset(modlist, 0, MODLIST_SIZE);
	modlist_c = 0;
	for (i=0; i<MAX_MODLIST; i++) // Manoel Kasimier
		modlist[i].sound = 11025; // Manoel Kasimier
}

static void ModList_Free()
{
	ModList_Clear();
	free(modlist);
}

static void ModList_Alloc()
{
	modlist = (modentry_t *)malloc(MODLIST_SIZE);
	ModList_Clear();
}

// Manoel Kasimier - begin
static void ModList_LoadIniFile(int c)
{
	char		str[1024], *s;
	int			i, r;
	FILE		*f;
	// Open .ini file
	if ((f = fopen (va("%s/nxmakaqu.ini", modlist[c].fullpath), "r")))
	{
		while (!feof(f))
		{
			for (i=0 ; true ; i++)
			{
				r = fgetc (f);
				if (r == EOF || !r || r == '\n')
					break;
				if (i >= sizeof(str)-1) // end of string
					continue;
				if (i > 0)
					if (str[i-1] == '/' && r == '/') // start of comment
						str[i-1] = 0;
				str[i] = r;
			}
			str[i] = 0;

			for (s=str; (*s && *s <= ' '); s++); // skip whitespace
			s = COM_Parse (s);
			if (!s || !*s)
				continue;
			if (!stricmp(com_token, "name"))
			{
				if ((s = COM_Parse (s)))
					strcpy (modlist[c].game_name, com_token);
			}
			else if (!stricmp(com_token, "description"))
			{
				if ((s = COM_Parse (s)))
					strcpy (modlist[c].game_text, com_token);
			}
			else if (!stricmp(com_token, "sound"))
			{
				if ((s = COM_Parse (s)))
					modlist[c].sound = Q_atoi(com_token);
			}
			else if (!stricmp(com_token, "video"))
			{
				if ((s = COM_Parse (s)))
					modlist[c].video = Q_atoi(com_token);
			}
			else if (!stricmp(com_token, "heapsize"))
			{
				if ((s = COM_Parse (s)))
					modlist[c].heapsize = Q_atoi(com_token)*1024;
			}
			else if (!stricmp(com_token, "commandline"))
			{
				if ((s = COM_Parse (s)))
				{
					if (modlist[c].game_args[0])
						strcat (modlist[c].game_args, va(" %s", com_token));
					else
						strcpy (modlist[c].game_args, com_token);
				}
			}
		}
		fclose (f);
	}
}
// Manoel Kasimier - end

static void ModList_ReScan(const char *base)
{
	// Vars for directory scanning
	int			dir;
	dirent_t	*ent;

	// Flush the modlist
	if(modlist)
		ModList_Clear();
	else
		ModList_Alloc();

	// Open directory for listing
	dir = fs_open(base, O_RDONLY | O_DIR);
	if(dir == -1) // Manoel Kasimier - edited
		goto id1; // Manoel Kasimier

	// Scan for id1 directory
	while((ent = fs_readdir(dir)) != NULL)
	{
		// Skip files, parent directories
		if( (!(ent->size < 0)) || (ent->name[0] == '.') )
			continue;

		// Skip if not id1 directory
		if(stricmp(ent->name, "id1"))
			continue;

		strncpy(modlist[modlist_c].game_dir, "id1", 256);
		strncpy(modlist[modlist_c].fullpath, base, 256);
		strcat(modlist[modlist_c].fullpath, "/");
		strcat(modlist[modlist_c].fullpath, "id1");
		strncpy(modlist[modlist_c].game_name, "Quake", 256);
		strncpy(modlist[modlist_c].game_text, "Quake by id Software", 256);
		ModList_LoadIniFile(modlist_c); // Manoel Kasimier
		modlist_c++;
	}
	fs_close(dir);
	// Manoel Kasimier - added /ID1 support - begin
id1:
	if (!modlist_c) // if /cd/quake/id1 was not found, look for /cd/id1
	{
		dir = fs_open("/cd", O_RDONLY | O_DIR);
		if(dir == -1) // Manoel Kasimier - edited
			goto mods; // Manoel Kasimier

		// Scan for id1 directory
		while((ent = fs_readdir(dir)) != NULL)
		{
			// Skip files, parent directories
			if( (!(ent->size < 0)) || (ent->name[0] == '.') )
				continue;

			// Skip if not id1 directory
			if(stricmp(ent->name, "id1"))
				continue;

			strncpy(modlist[modlist_c].game_dir, "id1", 256);
			strcpy(modlist[modlist_c].fullpath, "/cd/ID1");
			strncpy(modlist[modlist_c].game_name, "Quake", 256);
			strncpy(modlist[modlist_c].game_text, "Quake PC CD-ROM by id Software", 256);
		//	strncpy(modlist[modlist_c].game_args, "-basedir /cd", 256);
			ModList_LoadIniFile(modlist_c); // Manoel Kasimier
			modlist_c++;
		}
		fs_close(dir);
	}
mods:
	// Manoel Kasimier - added /ID1 support - end
	dir = fs_open(base, O_RDONLY | O_DIR);
	if(dir == -1) // Manoel Kasimier
		return; // Manoel Kasimier

	// Manoel Kasimier - added mission packs - begin
	// Look for the mission pack 1 first, and then for the mission pack 2, before adding the other games.

	// Scan for hipnotic directory
	while((ent = fs_readdir(dir)) != NULL)
	{
		// Skip files, parent directories
		if( (!(ent->size < 0)) || (ent->name[0] == '.') )
			continue;

		// Skip if not hipnotic directory
		if(stricmp(ent->name, "hipnotic"))
			continue;

		// Mission Pack 2: Scourge of Armagon (hipnotic)
		strncpy(modlist[modlist_c].game_dir, "hipnotic", 256);
		strncpy(modlist[modlist_c].fullpath, "/cd/QUAKE/HIPNOTIC", 256);
		strncpy(modlist[modlist_c].game_name, "Scourge of Armagon", 256);
		strncpy(modlist[modlist_c].game_text, "Quake Mission Pack 1", 256);
		// The mission packs uses their own arguments instead of -game
		strncpy(modlist[modlist_c].game_args, "-hipnotic", 256);
		ModList_LoadIniFile(modlist_c);
		modlist_c++;
	}
	fs_close(dir);
	dir = fs_open(base, O_RDONLY | O_DIR);
	// Scan for rogue directory
	while((ent = fs_readdir(dir)) != NULL)
	{
		// Skip files, parent directories
		if( (!(ent->size < 0)) || (ent->name[0] == '.') )
			continue;

		// Skip if not rogue directory
		if(stricmp(ent->name, "rogue"))
			continue;

		// Mission Pack 1: Dissolution of Eternity (rogue)
		strncpy(modlist[modlist_c].game_dir, "rogue", 256);
		strncpy(modlist[modlist_c].fullpath, "/cd/QUAKE/ROGUE", 256);
		strncpy(modlist[modlist_c].game_name, "Dissolution of Eternity", 256);
		strncpy(modlist[modlist_c].game_text, "Quake Mission Pack 2", 256);
		// The mission packs uses their own arguments instead of -game
		strncpy(modlist[modlist_c].game_args, "-rogue", 256);
		ModList_LoadIniFile(modlist_c);
		modlist_c++;
	}
	fs_close(dir);
	dir = fs_open(base, O_RDONLY | O_DIR);
	// Manoel Kasimier - added mission packs - end

	// Scan for other directories
	while((ent = fs_readdir(dir)) != NULL)
	{
		// Skip files, parent directories
		if( (!(ent->size < 0)) || (ent->name[0] == '.') )
			continue;

		// Skip id1 directory
		if(!stricmp(ent->name, "id1"))
			continue;
		// Manoel Kasimier - added mission packs - begin
		if(!stricmp(ent->name, "rogue"))
			continue;
		if(!stricmp(ent->name, "hipnotic"))
			continue;
		// Manoel Kasimier - added mission packs - end

		// Copy directory into modlist
		strncpy(modlist[modlist_c].game_dir, ent->name, 256);

		// Work out full pathname
		strncpy(modlist[modlist_c].fullpath, base, 256);
		strcat(modlist[modlist_c].fullpath, "/");
		strcat(modlist[modlist_c].fullpath, ent->name);

		// Fill out game name and description
		strncpy(modlist[modlist_c].game_name, ent->name, 256);
		strncpy(modlist[modlist_c].game_text, ent->name, 256);

		// Fill out argument list
		strncpy(modlist[modlist_c].game_args, "-game ", 256);
		strcat(modlist[modlist_c].game_args, ent->name);

		ModList_LoadIniFile(modlist_c); // Manoel Kasimier

		// Add to count
		modlist_c++;
	}
	fs_close(dir);
}

// Set up arguments from a mod
// Adapted from nxDoom
static void ModMenu_Select(int i)
{
	char commandLine[1024];
	int		n, c = 0, slcl;

	// Rebuild the full command line
	if(modlist[i].game_args[0]) // Manoel Kasimier - edited
		sprintf(commandLine, " %s", modlist[i].game_args); // Manoel Kasimier - edited
	else
		commandLine[0] = 0; // Manoel Kasimier

	// Count the arguments
	modmenu_argc = 1;
	for(n = 0; n < strlen(commandLine); n++)
	{
		if (commandLine[n] == ' ')
			modmenu_argc++;
	}

	// Set up modmenu_argv
//	modmenu_argv = (char **)malloc(sizeof(char **) * modmenu_argc); // Manoel Kasimier - removed
	modmenu_argv[0] = commandLine;
	c = 1;
	slcl = strlen(commandLine);
	for(n = 0; n < slcl; n++)
	{
		if(commandLine[n] <= ' ') // Manoel Kasimier - edited
		{
			// Manoel Kasimier - begin
			do
				commandLine[n++] = 0;
			while (n < slcl && commandLine[n] <= ' ');
			if (n < slcl)
				modmenu_argv[c++] = commandLine + n;
			// Manoel Kasimier - end
		}
	}
	// Manoel Kasimier - begin
	if (modlist[i].video > 0 && modlist[i].video < 10) // 1 to 9
		dcvidmode = modlist[i].video - 1;

	desired_speed = modlist[i].sound;
	if (dcvidmode > 3)
	{
		if (desired_speed) // don't change it if it's off
			desired_speed = 11025;
		mem_size = 8*1024*1024;
	}
	if (modlist[i].heapsize)
		mem_size = modlist[i].heapsize;
	if (mem_size < 8*1024*1024)
		mem_size = 8*1024*1024;
	if (!Q_strncasecmp(modlist[i].fullpath, "/cd/ID1", 7))
		Q_strcpy(dc_basedir, "/cd");
	// Manoel Kasimier - end
}

static int		md_top = 0;
static int		md_bottom = 11;
static int		md_select = 0;
static int		md_last = -1;
static short	*bbuffer;

static void ModMenu_DrawText(int x, int y, char *string)
{
	bfont_draw_str(bbuffer + ((y + 2) * 24 * 640) + (x * 12), 640, 0, string); // edited
//	bfont_draw_str(bbuffer + ((y + 1) * 24 * 640) + (x * 12), 640, 0, string);
}
// Manoel Kasimier - begin
static void ModMenu_DrawText_Center(int x, int y, char *string)
{
	bfont_draw_str(bbuffer + ((y+2) * 24 * 640) + (x*12)+(320-strlen(string)*6), 640, 0, string);
}
// Manoel Kasimier - end

static void MMD_CLS()
{
	int x, y, ofs;
	for(y = 0; y < 480; y++)
	{
		ofs = (640 * y);
		for(x = 0; x < 640; x++)
			bbuffer[ofs + x] = 0;
	}
}

// Manoel Kasimier - begin
#define NOGAMES 99
static void ModMenu_DrawWarning(int i)
{
	if(cdstate != prevcdstate)
	{
		// Clear the buffer
		MMD_CLS();
		if (i == NOGAMES)
		{
			ModMenu_DrawText_Center(0, 7, "No games found");
			ModMenu_DrawText_Center(0, 8, "Swap the disc");
		}
		if (i == CD_STATUS_NO_DISC)
		{
			ModMenu_DrawText_Center(0, 7, "Drive empty");
			ModMenu_DrawText_Center(0, 8, "Insert a disc");
		}
		if (i == CD_STATUS_OPEN)
			ModMenu_DrawText_Center(0, 8, "Drive open");
		if (i == CD_STATUS_BUSY)
			ModMenu_DrawText_Center(0, 8, "Reading disc...");
		else
		{
			if(modlist)
				ModList_Clear();
			modlist_c = 0;
		}
		// Blit the buffer
		memcpy(vram_s, bbuffer, 640 * 480 * 2);
	}
}
// Manoel Kasimier - end

static void ModMenu_Draw()
{
	int i;

	if(md_last == md_select)
		return;

	// Clear the buffer
	MMD_CLS();

	// Draw the list using the BIOS font
//	ModMenu_DrawText(0, 0, "nxQuake ModMenu by BlackAura");
	ModMenu_DrawText_Center(0/*18*/, 0, "Select a game");
	for(i = md_top; i < md_bottom; i++)
	{
		if(i < modlist_c)
		{
			char tstring[256];
			sprintf(tstring, "%2i  %s", i, modlist[i].game_name);
			ModMenu_DrawText(3, (i - md_top) + 2, tstring); // Manoel Kasimier - edited
		}
	}
	ModMenu_DrawText(1, (md_select - md_top) + 2, ">"); // Manoel Kasimier - edited

	if(stricmp(modlist[md_select].game_name, modlist[md_select].game_text)) // Manoel Kasimier - show description
		ModMenu_DrawText_Center(0, 14, modlist[md_select].game_text); // Manoel Kasimier - show description

	ModMenu_DrawText_Center(0, 16, va("Video mode: < %s >", vidnames[modlist[md_select].video ? (modlist[md_select].video-1) : dcvidmode]));

	// Blit the buffer
	memcpy(vram_s, bbuffer, 640 * 480 * 2);

	// Store last selected, avoid continuous redraws
	md_last = md_select;
}

// Manoel Kasimier - begin
static void ModMenu_Scroll()
{
	if (md_select >= md_bottom)
	{
		md_bottom = md_select + 1;
		md_top = md_select - 10;
	}
	if (md_select < md_top)
	{
		md_top = md_select;
		md_bottom = md_select + 11;
	}
}
// Manoel Kasimier - end

void ModMenu(const char *basedir)
{
	int	finished = 0;
	int button = 0; // Manoel Kasimier

	// Controller garbage
	maple_device_t	*dev;
	cont_state_t	*state;
	state = NULL; // compiler warning fix

	// Scan the modlist
	ModList_ReScan(basedir);

	// Exit if there's only 1 game and the user isn't holding the X button
	if((dev = maple_enum_type(0, MAPLE_FUNC_CONTROLLER)))
		if ((state = (cont_state_t *)maple_dev_status(dev)))
			if(state->buttons & CONT_X)
				button = CONT_X;
	if(modlist_c == 1 && !button) // Manoel Kasimier - edited
//	if(modlist_c <= 1)
	{
		ModMenu_Select(0);
		ModList_Free();
		return;
	}

	// Init KOS stuff
	vid_set_mode(DM_640x480, PM_RGB565);
	bbuffer = (unsigned short *)malloc(640 * 480 * 2);
	
	while(!finished)
	{
		button = 0; // Manoel Kasimier
		// Handle the controller
		dev = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
		if(dev)
		{
			state = (cont_state_t *)maple_dev_status(dev);
			if(state)
			{
				// Manoel Kasimier - begin
				if(modlist_c)
				{
				if(state->buttons & CONT_DPAD_LEFT)
				{
					button = CONT_DPAD_LEFT;
					dcvidmode--;
					if(dcvidmode < 0)
						dcvidmode = 7;//8
					md_last = -1;
				}
				else if(state->buttons & CONT_DPAD_RIGHT)
				{
					button = CONT_DPAD_RIGHT;
					dcvidmode++;
					if(dcvidmode > 7)//8
						dcvidmode = 0;
					md_last = -1;
				}
				// Manoel Kasimier - end
				if(state->buttons & CONT_DPAD_DOWN)
				{
					if(md_select < modlist_c - 1)
						md_select++;
					// Manoel Kasimier - begin
					else
						md_select = 0;
					ModMenu_Scroll();
					button = CONT_DPAD_DOWN;
					// Manoel Kasimier - end
				}
				else if(state->buttons & CONT_DPAD_UP)
				{
					if(md_select > 0)
						md_select--;
					// Manoel Kasimier - begin
					else
						md_select = modlist_c - 1;
					ModMenu_Scroll();
					button = CONT_DPAD_UP;
					// Manoel Kasimier - end
				}
				else if(state->buttons & CONT_A || state->buttons & CONT_START)
				{
					ModMenu_Select(md_select);
					finished = 1;
				}
				} // Manoel Kasimier
			}
		}
		// Manoel Kasimier - begin
		cdstate = DC_CheckDrive();
			 if (cdstate == CD_STATUS_OPEN)		ModMenu_DrawWarning(CD_STATUS_OPEN);
		else if (cdstate == CD_STATUS_NO_DISC)	ModMenu_DrawWarning(CD_STATUS_NO_DISC);
		else if (cdstate == CD_STATUS_BUSY)		ModMenu_DrawWarning(CD_STATUS_BUSY);
		else if (!modlist_c) // keep scanning until a game is found
		{
			ModList_ReScan(basedir);
			if (!modlist_c)
				ModMenu_DrawWarning(NOGAMES);
			else
			{
			/*	if(modlist_c == 1) // there's only 1 game
				{
					ModMenu_Select(0);
					ModList_Free();
					free(bbuffer);
					return;
				}
			*/	md_select = 0;
				md_last = -1;
			}
		}
		else
			ModMenu_Draw();
		prevcdstate = cdstate;
		if (button)
			while(state->buttons & button)
				state = (cont_state_t *)maple_dev_status(dev);
		// Manoel Kasimier - end
	}

	ModList_Free();
	free(bbuffer);
}
