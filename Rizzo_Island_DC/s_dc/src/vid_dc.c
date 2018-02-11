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
// dc_vid.c
// Very basic software mode video driver for Dreamcast/KOS

#include "quakedef.h"
#include "d_local.h"
#include <kos.h> // BlackAura - KallistiOS headers

#define PACK_RGB565(r,g,b) (((r>>3)<<11)|((g>>2)<<5)|(b>>3)) // Copied from nxDoom

unsigned short	paletteData[256];	// Copied from nxDoom
unsigned short	d_8to16table[256];
viddef_t		vid;				// global video state
byte			*surfcache;			// BlackAura (15-08-2004) - Dynamically sized surface cache
// Manoel Kasimier - begin
byte			*vid_buffer;
int				basewidth, baseheight, screenheight, vram_offset=0, kos_vidmode, dcvidmode=1;

void SelectVidMode ()
{
	vram_offset	= 0;
	switch (dcvidmode)
	{
	case 8:
		kos_vidmode	= DM_800x608;
		basewidth	= 800;
		baseheight	= 608;
		screenheight= 608;
		break;
	case 7:
		kos_vidmode	= DM_768x576;
		basewidth	= 768;
		baseheight	= 576;
		screenheight= 576;
		break;
	case 6:
		kos_vidmode	= DM_768x480;
		basewidth	= 768;
		baseheight	= 480;
		screenheight= 480;
		break;
	case 5:
		kos_vidmode	= DM_640x480;
		basewidth	= 640;
		baseheight	= 480;
		screenheight= 480;
		break;
	case 4:
		kos_vidmode	= DM_640x480;
		basewidth	= 640;
		baseheight	= 400;
		screenheight= 480;
		break;
	case 3:
		kos_vidmode	= DM_640x480;
		basewidth	= 640;
		baseheight	= 240;
		screenheight= 480;
		break;
	case 2:
		kos_vidmode	= DM_640x480;
		basewidth	= 320;
		baseheight	= 480;
		screenheight= 480;
		break;
	case 1:
		kos_vidmode	= DM_320x240;
		basewidth	= 320;
		baseheight	= 240;
		screenheight= 240;
		break;
	default: //case 0:
		kos_vidmode	= DM_320x240;
		basewidth	= 320;
		baseheight	= 200;
		screenheight= 240;
		break;
	}
	if (dcvidmode != 2 && dcvidmode != 3)
		vram_offset	= (basewidth*(screenheight-baseheight)*3)/8; // 4800 = 320*(240-200) * (3/8)
	vid_buffer	= (byte  *)malloc(basewidth*baseheight);
	d_pzbuffer 	= (short *)malloc(basewidth*baseheight*sizeof(*d_pzbuffer));
}
// Manoel Kasimier - end

// Adapted from nxDoom
void	VID_SetPalette (unsigned char *palette)
{
	int r, g, b, i;
	for(i = 0; i < 256; i++)
	{
		r = *palette++;
		g = *palette++;
		b = *palette++;
		paletteData[i] = PACK_RGB565(r, g, b);
	}
}

void	VID_ShiftPalette (unsigned char *palette)
{
	VID_SetPalette (palette);
}

// Clears VRAM - gets rid of annoying debris
// Copied from nxDoom
void VID_ClearVRam()
{
	int x, y, ofs;
	// Clear VRAM
	for(y = 0; y < screenheight; y++)
	{
		ofs = (basewidth * y);
		for(x = 0; x < basewidth; x++)
			vram_s[ofs + x] = 0;
	}
}

void	VID_Init (unsigned char *palette)
{
	// BlackAura (15-08-2004) - Dynamically sized surface cache
	int tsize;

	SelectVidMode(); // Manoel Kasimier

	// BlackAura (15-08-2004) - Fix waterwarp crashes at higher resolution
//	vid.maxwarpwidth = WARP_WIDTH; // Manoel Kasimier - hi-res waterwarp & buffered video - removed
//	vid.maxwarpheight = WARP_HEIGHT; // Manoel Kasimier - hi-res waterwarp & buffered video - removed
	vid.width = vid.conwidth = basewidth;
	vid.height = vid.conheight = baseheight;
	vid.maxwarpwidth = vid.width; // Manoel Kasimier - hi-res waterwarp & buffered video
	vid.maxwarpheight = vid.height; // Manoel Kasimier - hi-res waterwarp & buffered video
	vid.aspect = 1.0;
	vid.numpages = 1;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));
	vid.buffer = vid.conbuffer = vid_buffer;
	vid.rowbytes = vid.conrowbytes = basewidth;

	// BlackAura (15-08-2004) - Dynamically sized surface cache
	tsize = D_SurfaceCacheForRes(basewidth, baseheight);
	surfcache = (byte *)malloc(tsize);
	if(!surfcache)
		Sys_Error("Can't allocate %d bytes for surface cache\n", tsize);
	D_InitCaches (surfcache, tsize);

	// Initialise the KOS crap
	VID_ClearVRam();
	vid_set_mode(kos_vidmode, PM_RGB565);

	// Set up the default palette
	VID_SetPalette(palette);
}

// Update using store queues - taken from Bero's Quake port
static void sq_update(unsigned short *dest,unsigned char *s,unsigned short *pal,int n)
{
	int v;
	unsigned int *d = (unsigned int *)(void *)
		(0xe0000000 | (((unsigned long)dest) & 0x03ffffe0));

	// Set store queue memory area as desired
	QACR0 = ((((unsigned int)dest)>>26)<<2)&0x1c;
	QACR1 = ((((unsigned int)dest)>>26)<<2)&0x1c;
    
	// fill/write queues as many times necessary
	n>>=5;
	while(n--) {
		v = pal[*s++];
		v|= pal[*s++]<<16;
		d[0] = v;
		v = pal[*s++];
		v|= pal[*s++]<<16;
		d[1] = v;
		v = pal[*s++];
		v|= pal[*s++]<<16;
		d[2] = v;
		v = pal[*s++];
		v|= pal[*s++]<<16;
		d[3] = v;
		v = pal[*s++];
		v|= pal[*s++]<<16;
		d[4] = v;
		v = pal[*s++];
		v|= pal[*s++]<<16;
		d[5] = v;
		v = pal[*s++];
		v|= pal[*s++]<<16;
		d[6] = v;
		v = pal[*s++];
		v|= pal[*s++]<<16;
		d[7] = v;

		asm("pref @%0" : : "r" (d));
		d += 8;
	}
	// Wait for both store queues to complete
	d = (unsigned int *)0xe0000000;
	d[0] = d[8] = 0;
}

// Manoel Kasimier - begin
// modified versions of the sq_update function
// 320x480
static void sq_update2(unsigned short *dest,unsigned char *s,unsigned short *pal,int n)
{
	int v;
	unsigned int *d = (unsigned int *)(void *)
		(0xe0000000 | (((unsigned long)dest) & 0x03ffffe0));

	// Set store queue memory area as desired
	QACR0 = ((((unsigned int)dest)>>26)<<2)&0x1c;
	QACR1 = ((((unsigned int)dest)>>26)<<2)&0x1c;
    
	// fill/write queues as many times necessary
	n>>=5;
	while(n--) {
		v = pal[*s];
		v|= pal[*s++]<<16;
		d[0] = v;
		v = pal[*s];
		v|= pal[*s++]<<16;
		d[1] = v;
		v = pal[*s];
		v|= pal[*s++]<<16;
		d[2] = v;
		v = pal[*s];
		v|= pal[*s++]<<16;
		d[3] = v;
		v = pal[*s];
		v|= pal[*s++]<<16;
		d[4] = v;
		v = pal[*s];
		v|= pal[*s++]<<16;
		d[5] = v;
		v = pal[*s];
		v|= pal[*s++]<<16;
		d[6] = v;
		v = pal[*s];
		v|= pal[*s++]<<16;
		d[7] = v;

		asm("pref @%0" : : "r" (d));
		d += 8;
	}
	// Wait for both store queues to complete
	d = (unsigned int *)0xe0000000;
	d[0] = d[8] = 0;
}
// 640x240
static void sq_update3(unsigned short *dest,unsigned char *s,unsigned short *pal, int lines, int cols)
{
	int v, n;
	unsigned int *d = (unsigned int *)(void *)
		(0xe0000000 | (((unsigned long)dest) & 0x03ffffe0));

	// Set store queue memory area as desired
	QACR0 = ((((unsigned int)dest)>>26)<<2)&0x1c;
	QACR1 = ((((unsigned int)dest)>>26)<<2)&0x1c;
    
	// fill/write queues as many times necessary
	while(lines--)
	{
		n = cols;
		n>>=5;
		while(n--)
		{
			v = pal[*s++];
			v|= pal[*s++]<<16;
			d[0] = v;
			v = pal[*s++];
			v|= pal[*s++]<<16;
			d[1] = v;
			v = pal[*s++];
			v|= pal[*s++]<<16;
			d[2] = v;
			v = pal[*s++];
			v|= pal[*s++]<<16;
			d[3] = v;
			v = pal[*s++];
			v|= pal[*s++]<<16;
			d[4] = v;
			v = pal[*s++];
			v|= pal[*s++]<<16;
			d[5] = v;
			v = pal[*s++];
			v|= pal[*s++]<<16;
			d[6] = v;
			v = pal[*s++];
			v|= pal[*s++]<<16;
			d[7] = v;

			asm("pref @%0" : : "r" (d));
			asm("pref @%0" : : "r" (d+320));
			d += 8;
		}
		d += 320;
	}
	// Wait for both store queues to complete
	d = (unsigned int *)0xe0000000;
	d[0] = d[8] = 0;
}
// Manoel Kasimier - end

// Adapted from nxDoom
void	VID_Update (vrect_t *rects)
{
	// Update using store queues
	// Manoel Kasimier - begin
	if (dcvidmode == 3) // 640*240
		sq_update3(vram_s + vram_offset + rects->y*basewidth, vid.buffer + rects->y*basewidth, paletteData, rects->height, basewidth * 2);
	else if (dcvidmode == 2) // 320*480
		sq_update2(vram_s + vram_offset + rects->y*basewidth*2, vid.buffer + rects->y*basewidth, paletteData, rects->height*basewidth * 2*2);
	else
	// Manoel Kasimier - end
		sq_update(vram_s + vram_offset + rects->y*basewidth, vid.buffer + rects->y*basewidth, paletteData, rects->height*basewidth * 2); // Manoel Kasimier - edited
}

void VID_Shutdown (void)
{
	// Manoel Kasimier - begin
	free(vid_buffer);
	free(d_pzbuffer);
	free(surfcache);
	// Manoel Kasimier - end
}
// Not used
void D_BeginDirectRect (int x, int y, byte *pbitmap, int width, int height){}
void D_EndDirectRect (int x, int y, int width, int height){}


