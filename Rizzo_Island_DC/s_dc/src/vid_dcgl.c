#ifdef GLQUAKE
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
// vid_null.c -- null video driver to aid porting efforts

#include "quakedef.h"
#include <kos.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

viddef_t	vid;				// global video state

#define WARP_WIDTH	320
#define WARP_HEIGHT	200

unsigned short	d_8to16table[256];
unsigned	d_8to24table[256];
unsigned char d_15to8table[1<<15];

/* cvar_t		vid_mode = {"vid_mode","5",false}; */
cvar_t		vid_redrawfull = {"vid_redrawfull","0",false};
cvar_t		vid_waitforrefresh = {"vid_waitforrefresh","0",true};

int		texture_mode = GL_LINEAR;
 

int		texture_extension_number = 1;

float		gldepthmin, gldepthmax;
float vid_gamma;

cvar_t	gl_ztrick = {"gl_ztrick","0"};

const char *gl_vendor;
const char *gl_renderer;
const char *gl_version;
const char *gl_extensions;

qboolean is8bit = false;

qboolean isPermedia = false;
qboolean gl_mtexable = false;

#define	PACK_RGB565(r,g,b)	( (((r)>>3)<<11) |(((g)>>2)<<5)|((b)>>3) )

static void Check_Gamma (unsigned char *pal)
{
	float	f, inf;
	unsigned char	palette[768];
	int		i;

	vid_gamma = 1.0;

	for (i=0 ; i<768 ; i++)
	{
		f = pow ( (pal[i]+1)/256.0 , vid_gamma );
		inf = f*255 + 0.5;
		if (inf < 0)
			inf = 0;
		if (inf > 255)
			inf = 255;
		palette[i] = inf;
	}

	memcpy (pal, palette, sizeof(palette));
}

void	VID_SetPalette (unsigned char *palette)
{
	byte	*pal;
	unsigned r,g,b;
	unsigned v;
	int     r1,g1,b1;
	int		j,k,l,m;
	unsigned short i;
	unsigned	*table;
	char s[255];
	int dist, bestdist;
	static qboolean palflag = false;

//
// 8 8 8 encoding
//
	pal = palette;
	table = d_8to24table;
	for (i=0 ; i<256 ; i++)
	{
		r = pal[0];
		g = pal[1];
		b = pal[2];
		pal += 3;
		
		v = (255<<24) + (r<<0) + (g<<8) + (b<<16);
		*table++ = v;
	}
	d_8to24table[255] &= 0xffffff;	// 255 is transparent

	// JACK: 3D distance calcs - k is last closest, l is the distance.
	for (i=0; i < (1<<15); i++) {
		/* Maps
		000000000000000
		000000000011111 = Red  = 0x1F
		000001111100000 = Blue = 0x03E0
		111110000000000 = Grn  = 0x7C00
		*/
		r = ((i & 0x1F) << 3)+4;
		g = ((i & 0x03E0) >> 2)+4;
		b = ((i & 0x7C00) >> 7)+4;
		pal = (unsigned char *)d_8to24table;
		for (v=0,k=0,bestdist=10000*10000; v<256; v++,pal+=4) {
			r1 = (int)r - (int)pal[0];
			g1 = (int)g - (int)pal[1];
			b1 = (int)b - (int)pal[2];
			dist = (r1*r1)+(g1*g1)+(b1*b1);
			if (dist < bestdist) {
				k=v;
				bestdist = dist;
			}
		}
		d_15to8table[i]=k;
	}
}

void	VID_ShiftPalette (unsigned char *palette)
{
	VID_SetPalette(palette);
}

void VID_Init8bitPalette(unsigned char *pal)
{
	is8bit = true;

	int i;

	/* Select RGB565 palette format */
	pvr_set_pal_format(PVR_PAL_RGB565);

	/* Upload all the colours */
	for(i = 0; i < 256; i++)
	{
		uint16 c;
		uint8 r, g, b;
		r = *pal++;
		g = *pal++;
		b = *pal++;
		c = PACK_RGB565(r,g,b);
		pvr_set_pal_entry(i, c);
	}
}

static void dmyfunc()
{
}

void	VID_Init (unsigned char *palette)
{
	int width = 640;
	int height = 480;
	int vmode = DM_640x480;

/*	Cvar_RegisterVariable (&vid_mode); */
	Cvar_RegisterVariable (&vid_redrawfull);
	Cvar_RegisterVariable (&vid_waitforrefresh);
	Cvar_RegisterVariable (&gl_ztrick);

	vid.maxwarpwidth = WARP_WIDTH;
	vid.maxwarpheight = WARP_HEIGHT;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));

	vid.width = vid.conwidth = width;
	vid.height = vid.conheight = height;
	vid.aspect = ((float)vid.height / (float)vid.width) * (320.0 / 240.0);
	vid.numpages = 2;
//	vid.buffer = vid.conbuffer = vid_buffer;
	vid.rowbytes = vid.conrowbytes = 640;

//	d_pzbuffer = zbuffer;
//	D_InitCaches (surfcache, sizeof(surfcache));

	vid_set_mode(vmode, PM_RGB565);
	pvr_init_defaults();

	GL_Init();

	Check_Gamma(palette);
	VID_SetPalette(palette);

	/* Initialise colour palette */
	VID_Init8bitPalette(palette);

	qglMTexCoord2fSGIS = dmyfunc;
	qglSelectTextureSGIS = dmyfunc;
	gl_mtexable = false;

	vid.recalc_refdef = 1;				// force a surface cache flush
}

void	VID_Shutdown (void)
{
}

qboolean VID_Is8bit(void)
{
	return is8bit;
}

/*
===============
GL_Init
===============
*/
void GL_Init (void)
{
	glKosInit();

	gl_vendor = glGetString (GL_VENDOR);
	Con_Printf ("GL_VENDOR: %s\n", gl_vendor);
	gl_renderer = glGetString (GL_RENDERER);
	Con_Printf ("GL_RENDERER: %s\n", gl_renderer);

	gl_version = glGetString (GL_VERSION);
	Con_Printf ("GL_VERSION: %s\n", gl_version);
	gl_extensions = glGetString (GL_EXTENSIONS);
	Con_Printf ("GL_EXTENSIONS: %s\n", gl_extensions);

//	Con_Printf ("%s %s\n", gl_renderer, gl_version);

//	CheckMultiTextureExtensions ();

	glClearColor (1,0,0,0);
	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
/*
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.666);

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
*/
	glShadeModel (GL_FLAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_FILTER_BILINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_FILTER_BILINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);

//	glKosBeginFrame();
}

/*
=================
GL_BeginRendering

=================
*/
void GL_BeginRendering (int *x, int *y, int *width, int *height)
{
	extern cvar_t gl_clear;
	float w,h;

	glKosGetScreenSize(&w,&h);

	*x = *y = 0;
	*width = w;
	*height = h;

	glKosBeginFrame();
}


void GL_EndRendering (void)
{
	glKosFinishFrame();
}


#endif
