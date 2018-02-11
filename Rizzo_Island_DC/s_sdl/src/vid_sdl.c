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
// sdl_vid.c -- Very simple SDL-based video system

#include "quakedef.h"
#include "d_local.h"

#include <SDL.h>

viddef_t	vid;				// global video state

#define	BASEWIDTH	640
#define	BASEHEIGHT	480

short	zbuffer[BASEWIDTH*BASEHEIGHT];
byte	*surfcache;

unsigned short	d_8to16table[256];
unsigned	d_8to24table[256];

SDL_Surface *sdlscreen;

// Toggle fullscreen mode
void VID_ToggleFullscreen()
{
	SDL_WM_ToggleFullScreen(sdlscreen);
}

void	VID_SetPalette (unsigned char *palette)
{
	SDL_Color colors[256];
	int i;
	for(i = 0; i < 256; i++)
	{
		colors[i].r = palette[i*3+0];
		colors[i].g = palette[i*3+1];
		colors[i].b = palette[i*3+2];
	}
	SDL_SetPalette(sdlscreen, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);
}

void	VID_ShiftPalette (unsigned char *palette)
{
	VID_SetPalette(palette);
}

void	VID_Init (unsigned char *palette)
{
	int tsize;

	// Initialise SDL Video
	#ifdef _WIN32
	sdlscreen = SDL_SetVideoMode(BASEWIDTH, BASEHEIGHT, 8, SDL_HWSURFACE | SDL_DOUBLEBUF);
	#else
	sdlscreen = SDL_SetVideoMode(BASEWIDTH, BASEHEIGHT, 8, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	#endif
	SDL_WM_SetCaption("nxQuake SDL", "nxQuake");

	VID_SetPalette(palette);

	vid.width = vid.conwidth = BASEWIDTH;
	vid.height = vid.conheight = BASEHEIGHT;
	vid.aspect = 1.0;
	vid.numpages = 1;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));
	vid.buffer = vid.conbuffer = sdlscreen->pixels;
	vid.rowbytes = vid.conrowbytes = BASEWIDTH;

	vid.maxwarpwidth = WARP_WIDTH;
	vid.maxwarpheight = WARP_HEIGHT;

	// Allocate surface cache
	tsize = D_SurfaceCacheForRes(BASEWIDTH, BASEHEIGHT);
	surfcache = (byte *)malloc(tsize);
	if(!surfcache)
		Sys_Error("Can't allocate %d bytes for surface cache\n", tsize);

	d_pzbuffer = zbuffer;
	D_InitCaches (surfcache, tsize);

	// Hide cursor
	SDL_ShowCursor(0);
}

void	VID_Shutdown (void)
{
	SDL_FreeSurface(sdlscreen);
}

void	VID_Update (vrect_t *rects)
{
	SDL_Flip(sdlscreen);
}

/*
================
D_BeginDirectRect
================
*/
void D_BeginDirectRect (int x, int y, byte *pbitmap, int width, int height)
{
}


/*
================
D_EndDirectRect
================
*/
void D_EndDirectRect (int x, int y, int width, int height)
{
}


