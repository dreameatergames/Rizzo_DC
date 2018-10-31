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

#ifdef _arch_dreamcast // BlackAura
#define	NET_MENUS 0
#else // BlackAura
#ifdef _WIN32
#include "winquake.h"
#define	NET_MENUS 1
#else
#define	NET_MENUS 0
#endif
#endif // BlackAura

// Manoel Kasimier - begin
#ifdef _arch_dreamcast
extern void VMU_FindConfig (void);
cvar_t	vmupath = {"vmupath","a1"};
void M_SelectVMU_f (int menu);
void SetVMUpath ()
{
	char	*s;
	// backup old string
	s = Z_Malloc (Q_strlen(vmupath.string)+1);
	Q_strcpy (s, vmupath.string);
	// free the old value string
	Z_Free (vmupath.string);

	vmupath.string = Z_Malloc (2+1);
	if (Q_strlen(s) == 2)
	{
		if (s[0] >= 'a' && s[0] <= 'd')
			vmupath.string[0] = s[0];
		else if (s[0] >= 'A' && s[0] <= 'D')
			vmupath.string[0] = s[0]+('a'-'A');
		else
			vmupath.string[0] = 'a';

		if (s[1] == '1' || s[1] == '2')
			vmupath.string[1] = s[1];
		else
			vmupath.string[1] = '1';
	}
	else
	{
		vmupath.string[0] = 'a';
		vmupath.string[1] = '1';
	}
	vmupath.string[2] = 0; // null termination
	vmupath.value = 0;
	Z_Free(s);
}
#endif
cvar_t	savename = {"savename","RIZZO___"}; // 8 uppercase characters
void SetSavename ()
{
	// savename must always be 8 uppercase alphanumeric characters
	// non-alphanumeric characters (for example, spaces) must be converted to underscores
	int		i;
	char	*s;
	// backup old string
	s = Z_Malloc (Q_strlen(savename.string)+1);
	Q_strcpy (s, savename.string);
	// free the old value string
	Z_Free (savename.string);

	savename.string = Z_Malloc (8+1);
	for (i=0; i<8; i++)
	{
		if (!s[i]) // string was shorter than 8 characters
			break;
		if ((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'A' && s[i] <= 'Z'))
			savename.string[i] = s[i];
		else if (s[i] >= 'a' && s[i] <= 'z')
			savename.string[i] = s[i]-('a'-'A');
		else
			savename.string[i] = '_';
	}
	for (; i<8; i++) // fill the remaining characters, if the string was shorter than 8 characters
		savename.string[i] = '_';
	savename.string[i] = 0; // null termination
	savename.value = 0;
	Z_Free(s);
}
cvar_t	help_pages = {"help_pages","6"};

extern	cvar_t	samelevel;

extern	cvar_t	sv_aim_h;
extern	cvar_t	sv_aim;
extern	cvar_t	scr_ofsy;
extern	cvar_t	crosshair;
extern	cvar_t	crosshair_color;
extern	cvar_t	r_drawviewmodel;
extern	cvar_t	cl_nobob;
extern	cvar_t	sv_idealpitchscale;
extern	cvar_t	sv_enable_use_button;
extern	cvar_t	chase_active;
extern	cvar_t	chase_back;
extern	cvar_t	chase_up;

extern	cvar_t	r_stereo_separation;
extern	cvar_t	r_drawentities;
extern	cvar_t	r_fullbright;
extern	cvar_t	r_waterwarp;
extern	cvar_t	d_mipscale;
extern	cvar_t	gl_polyblend;
extern	cvar_t	sw_stipplealpha;
extern	cvar_t	r_sprite_addblend;

/* // Manoel Kasimier - removed - begin
cvar_t	axis_x_function;
cvar_t	axis_y_function;
cvar_t	axis_l_function;
cvar_t	axis_r_function;
*/ // Manoel Kasimier - removed - end
cvar_t	axis_x_scale;
cvar_t	axis_y_scale;
cvar_t	axis_l_scale;
cvar_t	axis_r_scale;
cvar_t	axis_x_threshold;
cvar_t	axis_y_threshold;
cvar_t	axis_l_threshold;
cvar_t	axis_r_threshold;
/*/// Manoel Kasimier
extern	cvar_t	axis_pitch_dz;
extern	cvar_t	axis_yaw_dz;
extern	cvar_t	axis_walk_dz;
extern	cvar_t	axis_strafe_dz;
/*/// Manoel Kasimier

extern	cvar_t	snd_stereo;
extern	cvar_t	snd_swapstereo;

extern	cvar_t	sbar_show_scores;
extern	cvar_t	sbar_show_ammolist;
extern	cvar_t	sbar_show_weaponlist;
extern	cvar_t	sbar_show_keys;
extern	cvar_t	sbar_show_runes;
extern	cvar_t	sbar_show_powerups;
extern	cvar_t	sbar_show_armor;
extern	cvar_t	sbar_show_health;
extern	cvar_t	sbar_show_ammo;
extern	cvar_t	sbar_show_bg;
extern	cvar_t	sbar;

extern	cvar_t	scr_left;
extern	cvar_t	scr_right;
extern	cvar_t	scr_top;
extern	cvar_t	scr_bottom;

void Crosshair_Start (int x, int y);
void Host_WriteConfiguration (void);
// Manoel Kasimier - end

void (*vid_menudrawfn)(void);
void (*vid_menukeyfn)(int key);

enum {
m_none=0,
m_main,
m_singleplayer, m_load, m_save, m_loadsmall, m_savesmall,
m_options,
m_setup,
m_keys,
m_controller, m_mouse,
m_gameplay,
m_audio, m_video, m_videomodes, m_videosize,
m_developer,
m_credits, m_help, m_popup,

// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
m_vmu,
m_saveoptions, // hack
m_loadoptions, // hack
#endif
// Manoel Kasimier - VMU saves - end
m_fightoon,
m_nummenus // Manoel Kasimier - replaced cursor variables
} m_state; // Edited by Manoel Kasimier

int m_cursor[m_nummenus]; // Manoel Kasimier - replaced cursor variables

void M_Off (void); // Manoel Kasimier
void M_Main_f (void);
void M_Fightoon_f (void);
	void M_SinglePlayer_f (void);
		void M_LoadSmall_f (void); // Manoel Kasimier - small saves
		void M_SaveSmall_f (void); // Manoel Kasimier - small saves
		void M_Load_f (void);
		void M_Save_f (void);
		// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
		void M_VMUsave_f (void);
		void M_VMUsavesmall_f (void);
		void M_VMUload_f (void);
		void M_VMUloadsmall_f (void);
#endif
		// Manoel Kasimier - VMU saves - end
	void M_Options_f (void);
		void M_Setup_f (void);
		void M_Keys_f (void);
		void M_Controller_f (void);
		void M_Mouse_f (void);
		void M_Gameplay_f (void);
		void M_Audio_f (void);
		void M_Video_f (void);
		void M_VideoSize_f (void); // Manoel Kasimier
		void M_VideoModes_f (void); // Manoel Kasimier - for Windows version only
		void M_Developer_f (void);
	void M_Help_f (void);
	void M_Quit_f (void);

void M_Main_Draw (void);
void M_Fightoon_Draw (void);
	void M_SinglePlayer_Draw (void);
		void M_Save_Draw (void);
	void M_Options_Draw (void);
		void M_Setup_Draw (void);
		void M_Keys_Draw (void);
		void M_Controller_Draw (void);
		void M_Mouse_Draw (void);
		void M_Gameplay_Draw (void);
		void M_Audio_Draw (void);
		void M_Video_Draw (void);
		void M_VideoSize_Draw (void); // Manoel Kasimier
		void M_Developer_Draw (void);
	void M_Help_Draw (void);

void M_Main_Key (int key);
void M_Fightoon_Key (int key);
	void M_SinglePlayer_Key (int key);
		void M_Save_Key (int key);
	void M_Options_Key (int key);
		void M_Setup_Key (int key);
		void M_Keys_Key (int key);
		void M_Controller_Key (int key);
		void M_Mouse_Key (int key);
		void M_Gameplay_Key (int key);
		void M_Audio_Key (int key);
		void M_Video_Key (int key);
		void M_VideoSize_Key (int key); // Manoel Kasimier
		void M_Developer_Key (int key);
	void M_Help_Key (int key);


// Manoel Kasimier - begin
byte	m_inp_up, m_inp_down, m_inp_left, m_inp_right,
		m_inp_ok, m_inp_cancel, m_inp_no, m_inp_yes,
		m_inp_off, m_inp_backspace
		;
void M_CheckInput (int key)
{
	m_inp_up = m_inp_down = m_inp_left = m_inp_right =
	m_inp_ok = m_inp_cancel = m_inp_no = m_inp_yes =
	m_inp_off = m_inp_backspace = false;

	switch (key)
	{
	case K_UPARROW:
	case K_MWHEELUP:
	case K_JOY1_AXISY_MINUS:
	case K_JOY2_AXISY_MINUS:
#ifndef _arch_dreamcast
	case K_JOY1_POV_UP:
	case K_JOY2_POV_UP:
#else
//	case K_JOY1_AXISZ_MINUS:
	case K_JOY1_DPAD1_UP:
	case K_JOY1_DPAD2_UP:
//	case K_JOY2_AXISZ_MINUS:
	case K_JOY2_DPAD1_UP:
	case K_JOY2_DPAD2_UP:
#endif
		m_inp_up = true;
		break;

	case K_DOWNARROW:
	case K_MWHEELDOWN:
	case K_JOY1_AXISY_PLUS:
	case K_JOY2_AXISY_PLUS:
#ifndef _arch_dreamcast
	case K_JOY1_POV_DOWN:
	case K_JOY2_POV_DOWN:
#else
//	case K_JOY1_AXISZ_PLUS:
	case K_JOY1_DPAD1_DOWN:
	case K_JOY1_DPAD2_DOWN:
//	case K_JOY2_AXISZ_PLUS:
	case K_JOY2_DPAD1_DOWN:
	case K_JOY2_DPAD2_DOWN:
#endif
		m_inp_down = true;
		break;

	case K_LEFTARROW:
	case K_JOY1_AXISX_MINUS:
	case K_JOY2_AXISX_MINUS:
#ifndef _arch_dreamcast
	case K_JOY1_POV_LEFT:
	case K_JOY2_POV_LEFT:
#else
//	case K_JOY1_AXISR_MINUS:
	case K_JOY1_DPAD1_LEFT:
	case K_JOY1_DPAD2_LEFT:
//	case K_JOY2_AXISR_MINUS:
	case K_JOY2_DPAD1_LEFT:
	case K_JOY2_DPAD2_LEFT:
#endif
		m_inp_left = true;
		break;

	case K_RIGHTARROW:
	case K_MOUSE2:
	case K_JOY1_AXISX_PLUS:
	case K_JOY2_AXISX_PLUS:
#ifndef _arch_dreamcast
	case K_JOY1_POV_RIGHT:
	case K_JOY2_POV_RIGHT:
#else
//	case K_JOY1_AXISR_PLUS:
	case K_JOY1_DPAD1_RIGHT:
	case K_JOY1_DPAD2_RIGHT:
//	case K_JOY2_AXISR_PLUS:
	case K_JOY2_DPAD1_RIGHT:
	case K_JOY2_DPAD2_RIGHT:
#endif
		m_inp_right = true;
		break;

	case K_MOUSE1: // Mouse 1 enables both m_inp_left and m_inp_ok
		m_inp_left = true;
	case K_ENTER:
	case K_JOY1_USE:
	case K_JOY2_USE:
		m_inp_ok = true;
		break;

	case K_ESCAPE:
	case K_JOY1_BACK:
	case K_JOY2_BACK:
		m_inp_cancel = true;
	case 'n':
	case 'N':
//	case K_JOY1_NO:
//	case K_JOY2_NO:
		m_inp_no = true;
		break;

	case 'Y':
	case 'y':
	case K_JOY1_YES:
	case K_JOY2_YES:
		m_inp_yes = true;
		break;

	case K_BACKSPACE:
	case K_JOY1_BACKSPACE:
	case K_JOY2_BACKSPACE:
		m_inp_backspace = true;
		break;

#ifdef _arch_dreamcast
	case K_JOY1_ESCAPE:
	case K_JOY2_ESCAPE:
		m_inp_off = true;
		break;
#endif

#if 0
	case K_DEL:				// delete multiple bindings
	case K_SPACE:

	case K_MOUSE3:
#endif

	default:
		break;
	}
	if (key > K_PAUSE && keybindings[key])
		if (!Q_strcmp(keybindings[key], "togglemenu"))
			m_inp_off = true;
};
// Manoel Kasimier - end

qboolean	m_entersound;		// play after drawing a frame, so caching
								// won't disrupt the sound
qboolean	m_recursiveDraw;

int			m_save_demonum;

int			m_return_state;
qboolean	m_return_onerror;
char		m_return_reason [32];


// BlackAura (28-12-2002) - Alternate menu background fading
void Draw_FadeScreen2 (int level);
static int fade_level = 0;


byte identityTable[256];
byte translationTable[256];

// Manoel Kasimier - precaching menu pics - begin
void M_Precache (void)
{
	int i;
// text box
	Draw_CachePic ("gfx/box_tl.lmp"); Draw_CachePic ("gfx/box_tm.lmp"); Draw_CachePic ("gfx/box_tr.lmp");
	Draw_CachePic ("gfx/box_ml.lmp"); Draw_CachePic ("gfx/box_mm.lmp"); Draw_CachePic ("gfx/box_mr.lmp");
	Draw_CachePic ("gfx/box_bl.lmp"); Draw_CachePic ("gfx/box_bm.lmp"); Draw_CachePic ("gfx/box_br.lmp");
	Draw_CachePic ("gfx/box_mm2.lmp");
// big "q" cursor
	for (i=1; i<7; i++)
		Draw_CachePic(va("gfx/menudot%i.lmp", i));
// left plaque
	Draw_CachePic ("gfx/qplaque.lmp");
// main menu
	Draw_CachePic ("gfx/ttl_main.lmp");
	Draw_CachePic ("gfx/mainmenu.lmp");
// single player
	Draw_CachePic ("gfx/ttl_sgl.lmp");
//	Draw_CachePic ("gfx/sp_menu.lmp");
// load/save
	Draw_CachePic ("gfx/p_load.lmp");
	Draw_CachePic ("gfx/p_save.lmp");
// options
	Draw_CachePic ("gfx/p_option.lmp");
	Draw_CachePic ("gfx/bigbox.lmp");

	// sound precaches
	S_PrecacheSound ("misc/menu1.wav");
	S_PrecacheSound ("misc/menu2.wav");
	S_PrecacheSound ("misc/menu3.wav");

	Vibration_Stop (0);
	Vibration_Stop (1);
}
// Manoel Kasimier - precaching menu pics - end

void M_BuildTranslationTable(int top, int bottom)
{
	int		j;
	byte	*dest, *source;

	for (j = 0; j < 256; j++)
		identityTable[j] = j;
	dest = translationTable;
	source = identityTable;
	memcpy (dest, source, 256);

	if (top < 128)	// the artists made some backwards ranges.  sigh.
		memcpy (dest + TOP_RANGE, source + top, 16);
	else
		for (j=0 ; j<16 ; j++)
			dest[TOP_RANGE+j] = source[top+15-j];

	if (bottom < 128)
		memcpy (dest + BOTTOM_RANGE, source + bottom, 16);
	else
		for (j=0 ; j<16 ; j++)
			dest[BOTTOM_RANGE+j] = source[bottom+15-j];
}

void M_DrawCharacter (int cx, int line, int num)
{
	Draw_Character ( cx + ((vid.width - 320)>>1), line, num);
}

void M_Print (int cx, int cy, char *str)
{
	while (*str)
	{
		M_DrawCharacter (cx, cy, (*str)+128);
		cx += 8;
		str++;
	}
}

void M_PrintWhite (int cx, int cy, char *str)
{
	while (*str)
	{
		M_DrawCharacter (cx, cy, *str);
		cx += 8;
		str++;
	}
}

void M_DrawTransPic (int x, int y, qpic_t *pic)
{
	Draw_TransPic (x + ((vid.width - 320)>>1), y, pic);
}

void M_DrawTextBox (int x, int y, int width, int lines)
{
	qpic_t	*p;
	int		cx, cy;
	int		n;
	int		w = -width & 7; // Manoel Kasimier
	int		h = -lines & 7; // Manoel Kasimier

	// draw left side
	cx = x;
	cy = y;
	p = Draw_CachePic ("gfx/box_tl.lmp");
	M_DrawTransPic (cx, cy, p);
	p = Draw_CachePic ("gfx/box_ml.lmp");
	for (n = 0; n < lines; n+=8)//-------------
	{
		cy += 8;
		M_DrawTransPic (cx, cy, p);
	}
	p = Draw_CachePic ("gfx/box_bl.lmp");
	M_DrawTransPic (cx, cy+8 - h, p);//------------

	// draw middle
	cx += 8;
	while (width > 0)
	{
		cy = y;
		p = Draw_CachePic ("gfx/box_tm.lmp");
		M_DrawTransPic (cx, cy, p);
		p = Draw_CachePic ("gfx/box_mm.lmp");
		for (n = 0; n < lines; n+=8) //---------
		{
			cy += 8;
			if (n/8 == 1)//-----------
				p = Draw_CachePic ("gfx/box_mm2.lmp");
			M_DrawTransPic (cx, cy, p);
		}
		p = Draw_CachePic ("gfx/box_bm.lmp");
		M_DrawTransPic (cx, cy+8 - h, p);//-----------!!!!!!!!
		width -= 2*8;
		cx += 16;
	}

	// draw right side
	cy = y;
	p = Draw_CachePic ("gfx/box_tr.lmp");
	M_DrawTransPic (cx - w, cy, p); // Manoel Kasimier - crosshair - edited
	p = Draw_CachePic ("gfx/box_mr.lmp");
	for (n = 0; n < lines; n+=8)//--------
	{
		cy += 8;
		M_DrawTransPic (cx - w, cy, p); // Manoel Kasimier - crosshair - edited
	}
	p = Draw_CachePic ("gfx/box_br.lmp");
	M_DrawTransPic (cx - w, cy+8 - h, p); // Manoel Kasimier - crosshair - edited
}

void M_DrawTransPicTranslate (int x, int y, qpic_t *pic)
{
	Draw_TransPicTranslate (x + ((vid.width - 320)>>1), y, pic, translationTable);
}

#define	SLIDER_RANGE	10
void M_DrawSlider (int x, int y, float range)
{
	int	i;

	if (range < 0)
		range = 0;
	if (range > 1)
		range = 1;
	M_DrawCharacter (x-8, y, 128);
	for (i=0 ; i<SLIDER_RANGE ; i++)
		M_DrawCharacter (x + i*8, y, 129);
	M_DrawCharacter (x+i*8, y, 130);
	M_DrawCharacter (x + (SLIDER_RANGE-1)*8 * range, y, 131);
}

void M_DrawCheckbox (int x, int y, int on)
{
#if 0
	if (on)
		M_DrawCharacter (x, y, 131);
	else
		M_DrawCharacter (x, y, 129);
#endif
	if (on)
		M_Print (x, y, "on");
	else
		M_Print (x, y, "off");
}

//=============================================================================
// Manoel Kasimier - begin
//=============================================================================
char *timetos (int mytime)
{
	int
	hours		= mytime/3600,
	min_tens	= ((mytime%3600)/60)/10,
	min_units	= ((mytime%3600)/60)%10,
	tens		= (mytime%60)/10,
	units		= (mytime%60)%10;
	return va("%i:%i%i:%i%i", hours, min_tens, min_units, tens, units);
}
char *skilltos(int i)
{
	if (i <= 0) return "Easy";
	if (i == 1) return "Normal";
	if (i == 2) return "Hard";
	if (i >= 3) return "Nightmare";
	return "";
}
void M_DrawPlaque (char *c, qboolean b)
{
	qpic_t	*p;
	if (b)
		M_DrawTransPic (16, screen_top, Draw_CachePic ("gfx/qplaque.lmp") );
	p = Draw_CachePic (c);
	M_DrawTransPic ( (320-p->width)/2, screen_top, p);
}
void M_DrawCursor (int x, int y, int itemindex)
{
	static int oldx, oldy, oldindex;
	static float cursortime;
	if ((oldx != x) || (oldy != y) || (oldindex != itemindex))
	{
		oldx = x;
		oldy = y;
		oldindex = itemindex;
		cursortime = realtime;
	}
	if ((cursortime + 0.5) >= realtime)
		M_DrawCharacter (x, y + itemindex*8, 13);
	else
		M_DrawCharacter (x, y + itemindex*8, 12+((int)(realtime*4)&1));
}
#define ALIGN_LEFT 0
#define ALIGN_RIGHT 8
#define ALIGN_CENTER 4
void M_PrintText (char *s, int x, int y, int w, int h, int alignment)
{
	int		l;
	int		line_w[64];
	int		xpos = x;
	char	*s_count = s;

	for (l=0; l<64; l++)
		line_w[l] = 0;

	l = 0;
	while (*s_count)
	{
		if (*s_count == 10)
			l++;
		else
			line_w[l]++;
		s_count++;
	}
	if (line_w[l]) // last line is empty
		l++;
	y += ((h - l)*4); // 0=top, 4=center, 8=bottom

	l = 0;
	while (*s)
	{
		if (*s == 10)
		{
			xpos = x;
			l++;
		}
		else
		{
			M_DrawCharacter (xpos + ((w - line_w[l])*alignment), y + (l*8), (*s)+128);
			xpos += 8;
		}
		s++;
	}
}
void ChangeCVar (char *cvar_s, float current, float interval, float min, float max, qboolean slider)
{
//	if (self.impulse == 21)
//		interval = interval * -1;
	current += interval;
	if (/*interval < 0 &&*/ current < min)
	{
		if (slider)
			current = min;
		else
			current = max;
	}
	else if (/*interval > 0 &&*/ current > max)
	{
		if (slider)
			current = max;
		else
			current = min;
	}
	Cvar_SetValue (cvar_s, current);
};
//=============================================================================
/* ON-SCREEN KEYBOARD */

#define	NUM_KEYCODES	52
int		keyboard_active;
int		keyboard_cursor;
int		keyboard_shift;

int	keycodes[] =
{
	'0','1','2','3','4','5','6','7','8','9','-','+','/','|', 92,'_',
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p',
	'q','r','s','t','u','v','w','x','y','z','"',';','.','?','~', K_SPACE,

	'<','>','(',')','[',']','{','}','@','#','$','%','^','&','*','=',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
	'Q','R','S','T','U','V','W','X','Y','Z', 39,':',',','!','`', K_SPACE,

	K_BACKSPACE, K_SHIFT, K_TAB, K_ENTER
};

void M_OnScreenKeyboard_Reset (void)
{
	keyboard_active = keyboard_cursor = keyboard_shift = 0;
	m_inp_up = m_inp_down = m_inp_left = m_inp_right = m_inp_ok = m_inp_cancel = false;
}

void M_OnScreenKeyboard_Draw (int y)
{
	int i;
	if (!keyboard_active)
		return;

	M_DrawTextBox (16, y, 33*8, 5*8);
	for (i=0; i<47; i++)
		M_PrintWhite (36 + i*16 - (256*(i/16)), y+12+(8*(i/16)), Key_KeynumToString (keycodes[i+keyboard_shift]));
	i=96;
	M_Print (36			, y+36, Key_KeynumToString (keycodes[i++]));
	M_Print (36+11*8	, y+36, Key_KeynumToString (keycodes[i++]));
	M_Print (36+18*8	, y+36, Key_KeynumToString (keycodes[i++]));
	M_Print (36+23*8	, y+36, Key_KeynumToString (keycodes[i++]));

	if (keyboard_cursor < 48)
	{
		M_DrawCharacter (27+ keyboard_cursor*16 - (256*(keyboard_cursor/16)), y+12+(8*(keyboard_cursor/16)), 16);
		M_DrawCharacter (44+ keyboard_cursor*16 - (256*(keyboard_cursor/16)), y+12+(8*(keyboard_cursor/16)), 17);
	}
	else if (keyboard_cursor == 48)
		M_DrawCharacter (28			, y+36, 12+((int)(realtime*4)&1));
	else if (keyboard_cursor == 49)
		M_DrawCharacter (28+11*8	, y+36, 12+((int)(realtime*4)&1));
	else if (keyboard_cursor == 50)
		M_DrawCharacter (28+18*8	, y+36, 12+((int)(realtime*4)&1));
	else if (keyboard_cursor == 51)
		M_DrawCharacter (28+23*8	, y+36, 12+((int)(realtime*4)&1));
}

int M_OnScreenKeyboard_Key (int key)
{
	if (!keyboard_active)
	{
		if (key == K_JOY1_USE || key == K_JOY2_USE)
		{
			M_OnScreenKeyboard_Reset();
			keyboard_active	= 1;
		}
		else
			return key;
	}
	else
	{
		m_inp_up = m_inp_down = m_inp_left = m_inp_right = m_inp_ok = m_inp_cancel = false;
		switch (key)
		{
		case K_JOY1_BACK:
		case K_JOY2_BACK:
			M_OnScreenKeyboard_Reset();
			break;

		case K_JOY1_AXISX_MINUS:
		case K_JOY2_AXISX_MINUS:
		#ifndef _arch_dreamcast
		case K_JOY1_POV_LEFT:
		case K_JOY2_POV_LEFT:
		#else
		case K_JOY1_DPAD1_LEFT:
		case K_JOY1_DPAD2_LEFT:
		case K_JOY2_DPAD1_LEFT:
		case K_JOY2_DPAD2_LEFT:
		#endif
			if (--keyboard_cursor < 0)
				keyboard_cursor = NUM_KEYCODES-1;
			break;

		case K_JOY1_AXISX_PLUS:
		case K_JOY2_AXISX_PLUS:
		#ifndef _arch_dreamcast
		case K_JOY1_POV_RIGHT:
		case K_JOY2_POV_RIGHT:
		#else
		case K_JOY1_DPAD1_RIGHT:
		case K_JOY1_DPAD2_RIGHT:
		case K_JOY2_DPAD1_RIGHT:
		case K_JOY2_DPAD2_RIGHT:
		#endif
			if (++keyboard_cursor >= NUM_KEYCODES)
				keyboard_cursor = 0;
			break;

		case K_JOY1_AXISY_MINUS:
		case K_JOY2_AXISY_MINUS:
		#ifndef _arch_dreamcast
		case K_JOY1_POV_UP:
		case K_JOY2_POV_UP:
		#else
		case K_JOY1_DPAD1_UP:
		case K_JOY1_DPAD2_UP:
		case K_JOY2_DPAD1_UP:
		case K_JOY2_DPAD2_UP:
		#endif
			if (keyboard_cursor >= 48)
				keyboard_cursor = 48;
			keyboard_cursor-=16;
			if (keyboard_cursor < 0)
				keyboard_cursor = NUM_KEYCODES-4;
			break;

		case K_JOY1_AXISY_PLUS:
		case K_JOY2_AXISY_PLUS:
		#ifndef _arch_dreamcast
		case K_JOY1_POV_DOWN:
		case K_JOY2_POV_DOWN:
		#else
		case K_JOY1_DPAD1_DOWN:
		case K_JOY1_DPAD2_DOWN:
		case K_JOY2_DPAD1_DOWN:
		case K_JOY2_DPAD2_DOWN:
		#endif
			if (keyboard_cursor < 32)
				keyboard_cursor+=16;
			else if (keyboard_cursor < 48)
				keyboard_cursor = NUM_KEYCODES-4;
			else
				keyboard_cursor = 0;
			break;

		case K_JOY1_PGDN:
		case K_JOY2_PGDN:
			return K_BACKSPACE;
		case K_JOY1_PGUP:
		case K_JOY2_PGUP:
			return K_SPACE;
		case K_JOY1_USE:
		case K_JOY2_USE:
			if (keyboard_cursor < 48)
				return keycodes[keyboard_cursor+keyboard_shift];
			else if (keyboard_cursor == 49)
				keyboard_shift = (!keyboard_shift)*48;
			else
				return keycodes[keyboard_cursor+48];
		#ifndef _arch_dreamcast
		case K_JOY1_4:
		case K_JOY2_4:
        #else
		case K_JOY1_X:
		case K_JOY2_X:
        #endif
			return K_TAB;
		case K_JOY1_ENTER:
		case K_JOY2_ENTER:
			return K_ENTER;
		default:
			return key;
		}
	}
	return 0;
}

//=============================================================================
/* POP-UP MENU */
int			m_prevstate;
qboolean	wasInMenus;
char		*popup_message;
char		*popup_command;

void M_PopUp_f (char *s, char *cmd)
{
	if (m_state != m_popup)
	{
		wasInMenus = (key_dest == key_menu);
		key_dest = key_menu;
		m_prevstate = m_state; // set previously active menu
		m_state = m_popup;
	}
	m_entersound = true;
	popup_message = s;
	if (Q_strlen(cmd))
	{
		popup_command = Z_Malloc (Q_strlen(cmd)+1);
		Q_strcpy (popup_command, cmd);
	}
	else
		popup_command = NULL;
}

void M_PopUp_Draw (void)
{
	if (wasInMenus)
	{
		m_state = m_prevstate;
		m_recursiveDraw = true;
		M_Draw ();
		m_state = m_popup;
	}
	M_DrawTextBox (56, (vid.height - 48) / 2, 24*8, 4*8);
	M_PrintText (popup_message, 64, (vid.height - 32) / 2, 24, 4, ALIGN_CENTER);
}

void M_PopUp_Key (int key)
{
	if (m_inp_yes && popup_command)
		Cbuf_AddText(popup_command);
	else if (!m_inp_no)
		return;

	if (wasInMenus)
	{
		m_state = m_prevstate;
		m_entersound = true;
	}
	else
	{
		fade_level = 0; // BlackAura - Menu background fading
		key_dest = key_game;
		m_state = m_none;
	}
	if (popup_command)
		Z_Free(popup_command);
}

void M_Off (void) // turns the menu off
{
	key_dest = key_game;
	m_state = m_none;
	cls.demonum = m_save_demonum;
	if (cls.demonum != -1 && !cls.demoplayback && cls.state != ca_connected)
		CL_NextDemo ();
}
//=============================================================================
// Manoel Kasimier - end
//=============================================================================

/*
================
M_ToggleMenu_f
================
*/
void M_ToggleMenu_f (void)
{
	m_entersound = true;

	if (key_dest == key_menu)
	{
		if (m_state != m_main)
//		if (m_state != m_fightoon) // fightoon
		{
			M_Main_f ();
			return;
		}

		key_dest = key_game;
		m_state = m_none;
		return;
	}
	if (key_dest == key_console)
	{
		Con_ToggleConsole_f ();
	}
	else
	{
		M_Main_f ();
	}
}

//=============================================================================
/* MAIN MENU */

#define	MAIN_ITEMS	4


void M_Main_f (void)
{
//M_Fightoon_f(); // Fightoon
//return; // Fightoon
	if (key_dest != key_menu)
	{
		m_save_demonum = cls.demonum;
		cls.demonum = -1;
	}
	key_dest = key_menu;
	m_state = m_main;
	m_entersound = true;
	M_Precache(); // Manoel Kasimier - precaching menu pics
}


void M_Main_Draw (void)
{
	M_DrawPlaque ("gfx/ttl_main.lmp", true); // Manoel Kasimier

	M_DrawTransPic (72, screen_top + 28, Draw_CachePic ("gfx/mainmenu.lmp") );

	M_DrawTransPic (54, screen_top + 28 + m_cursor[m_state] * 20, Draw_CachePic( va("gfx/menudot%i.lmp", 1+((int)(realtime * 10)%6) ) ) ); // edited by Manoel Kasimier
}


void M_Main_Key (int key)
{
	// The DC_B shouldn't turn the main menu off
	if (key==K_ESCAPE)
		M_Off ();
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= MAIN_ITEMS)
			m_cursor[m_state] = 0;
	}
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = MAIN_ITEMS - 1;
	}
	else if (m_inp_ok)
	{
		switch (m_cursor[m_state]) // Manoel Kasimier - replaced cursor variables
		{
		case 0:
			M_SinglePlayer_f ();
			break;

		case 1:
			M_Options_f ();
			break;

		case 2:
			M_Help_f ();
			break;

		case 3:
			M_Quit_f ();
			break;
		}
	}
}

// Manoel Kasimier - begin
//=============================================================================
/* FIGHTOON MENU */

#define	FIGHTOON_ITEMS	9

void M_Fightoon_f (void)
{
	key_dest = key_menu;
	m_state = m_fightoon;
	m_entersound = true;
	M_Precache(); // Manoel Kasimier - precaching menu pics
}


void M_Fightoon_Draw (void)
{
	int y = screen_top + 20;

	qpic_t	*pic;
	pic = Draw_CachePic ("gfx/pause.lmp");
	Draw_TransPic (screen_left + (vid.width - screen_left - screen_right - pic->width)/2, (int)vid.height - screen_bottom - pic->height, pic);
//	M_DrawPlaque ("gfx/ttl_sgl.lmp", true);

    if (vid.height < 480)
        y += 80;
    else
        y += 160;

	M_Print (16, y += 8, "               Restart");
	M_Print (16, y += 8, "   Character Selection");
	M_Print (16, y += 8, "  Reset Player 1 Score");
    if (!sbar.value)
    {
        extern cvar_t sbar_lose1;
        extern cvar_t sbar_win1;
        M_Print (27*8, y, va("(L:%i,", (int)(sbar_lose1.value)));
		M_Print (27*8+6*8, y, va("W:%i)", (int)(sbar_win1.value)));
    }
	M_Print (16, y += 8, "  Reset Player 2 Score");
    if (!sbar.value)
    {
        extern cvar_t sbar_lose2;
        extern cvar_t sbar_win2;
        M_Print (27*8, y, va("(L:%i,", (int)(sbar_lose2.value)));
		M_Print (27*8+6*8, y, va("W:%i)", (int)(sbar_win2.value)));
    }
	M_Print (16, y += 8, "    Customize Controls    ...");
	M_Print (16, y += 8, "                 Audio    ...");
	M_Print (16, y += 8, "         Adjust Screen    ...");
	M_Print (16, y += 8, "            Save setup");
	M_Print (16, y += 8, "            Load setup");

    if (vid.height < 480)
        M_DrawCursor (200, screen_top + 28+80, m_cursor[m_state]);
    else
        M_DrawCursor (200, screen_top + 28+160, m_cursor[m_state]);
}


void M_Fightoon_Key (int key)
{
	// The DC_B shouldn't turn the main menu off
	if (key==K_ESCAPE)
		M_Off ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = FIGHTOON_ITEMS - 1;
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= FIGHTOON_ITEMS)
			m_cursor[m_state] = 0;
	}
	else if (m_inp_ok)
	{
		m_entersound = true;
		switch (m_cursor[m_state])
		{
		case 0:
			M_Off ();
			Cbuf_AddText("restart\n");
			break;
		case 1:
			Cbuf_AddText("disconnect\nmaxplayers 1\nmap start\n");
			break;
		case 2:
			Cbuf_AddText("sbar_lose1 0; sbar_win1 0\n");
			break;
		case 3:
			Cbuf_AddText("sbar_lose2 0; sbar_win2 0\n");
			break;
		case 4:
			M_Keys_f ();
			break;
		case 5:
			M_Audio_f ();
			break;
		case 6:
			M_VideoSize_f ();
			break;

		case 7:
			#ifdef _arch_dreamcast
			M_SelectVMU_f (m_saveoptions);
			#else
			m_entersound = true;
			Host_WriteConfiguration ();
			#endif
			break;
		case 8:
			#ifdef _arch_dreamcast
			M_SelectVMU_f (m_loadoptions);
			#else
			M_PopUp_f("Do you wish to load the\nsaved settings?", "exec config.cfg\n");
			#endif
			break;

		default:
			break;
		}
	}
}
// Manoel Kasimier - end

//=============================================================================
/* SINGLE PLAYER MENU */

#define	SINGLEPLAYER_ITEMS	3 // Manoel Kasimier - edited

void M_SinglePlayer_f (void)
{
	key_dest = key_menu;
	m_state = m_singleplayer;
	m_entersound = true;
}


void M_SinglePlayer_Draw (void)
{
	// Manoel Kasimier - begin
	int y = screen_top + 20;

	M_DrawPlaque ("gfx/ttl_sgl.lmp", true);

	M_Print (16, y += 8, "              New game");
	M_Print (16, y += 8, "             Load game    ...");
	M_Print (16, y += 8, "             Save game    ...");

	M_DrawCursor (200, screen_top + 28, m_cursor[m_state]);
	// Manoel Kasimier - end
}


void M_SinglePlayer_Key (int key)
{
	if (m_inp_cancel)
		M_Main_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = SINGLEPLAYER_ITEMS - 1;
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= SINGLEPLAYER_ITEMS)
			m_cursor[m_state] = 0;
	}
	else if (m_inp_ok)
	{
		m_entersound = true;
		switch (m_cursor[m_state]) // Manoel Kasimier - replaced cursor variables
		{
		case 0:
			if (sv.active)
				M_PopUp_f("Are you sure you want to\nstart a new game?", "disconnect\nmaxplayers 1\nmap start\n"); // Manoel Kasimier
			else
				Cbuf_AddText("disconnect\nmaxplayers 1\nmap start\n"); // Manoel Kasimier
			break;
		// Manoel Kasimier - small saves - begin

		case 1:
			// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
			M_VMUload_f ();
#else
			// Manoel Kasimier - VMU saves - end
			M_Load_f ();
#endif		// Manoel Kasimier - VMU saves
			break;

		case 2:
			// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
			M_VMUsave_f ();
#else
			// Manoel Kasimier - VMU saves - end
			M_Save_f ();
#endif		// Manoel Kasimier - VMU saves
			break;

		default:
			break;
		}
	}
}

//=============================================================================
/* LOAD/SAVE MENU */

// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
extern int VMU_CountFreeBlocks(int controller, int slot);
int	vmufreeblocks[4][2], vmu_cursor[2];
int	previousmenu, nextmenu;
void M_SelectVMU_f (int menu)
{
	int		i, i2;

	if (key_dest == key_menu && m_state == m_vmu)
	{
		M_Off ();
		return;
	}
	if (menu == m_save || menu == m_savesmall)
	{
		if (!sv.active)
			return;
		if (svs.maxclients != 1)
			return;
	}
	for (i =0; i <4; i ++)
		for (i2=1; i2<3; i2++)
			vmufreeblocks[i][i2-1] = VMU_CountFreeBlocks(i, i2);
	vmu_cursor[0] = vmupath.string[0]-'a';
	vmu_cursor[1] = vmupath.string[1]-'1';
	m_entersound = true;
	key_dest = key_menu;
	previousmenu = m_state;
	nextmenu = menu;
	m_state = m_vmu;
	M_Precache(); // Manoel Kasimier - precaching menu pics
}
void M_VMUsave_f (void)
{
	if (cl.stats[STAT_HEALTH] <= 0) // the player is dead
		return;
	if (cl.intermission)
		return;
/*	if (m_state == m_save)
	{
		m_state = m_none;
		M_Save_f ();
	}
	else
*/		M_SelectVMU_f (m_save);
}
void M_VMUsavesmall_f (void)
{
/*	if (m_state == m_savesmall)
	{
		m_state = m_none;
		M_SaveSmall_f ();
	}
	else
*/		M_SelectVMU_f (m_savesmall);
}
void M_VMUload_f (void)
{
	M_SelectVMU_f (m_load);
}
void M_VMUloadsmall_f (void)
{
	M_SelectVMU_f (m_loadsmall);
}
void M_SelectVMU_Draw (void)
{
	int i, i2, y = screen_top + 20;

	M_DrawPlaque ((nextmenu==m_save || nextmenu==m_savesmall || nextmenu==m_saveoptions) ? "gfx/p_save.lmp" : "gfx/p_load.lmp", true);

	M_PrintWhite (64+48, y+=8, "Select a VMU");
	M_Print (64+36, y+=16, "A      B      C      D");

	for (i2=1; i2<3; i2++)
	{
		M_Print (56, y + 16*i2, va("%i", i2));
		for (i=0;i<4;i++)
		{
			M_DrawTextBox (64+16 + 56*i, y-8 + 16*i2, 3*8, 8);
			if (vmufreeblocks[i][i2-1] >= 0)
				M_PrintWhite (64+28 + 56*i, y + 16*i2, va("%3i", vmufreeblocks[i][i2-1]));
		}
	}
	M_DrawCursor (64+8 + vmu_cursor[0]*56, screen_top+60 + vmu_cursor[1]*16, 0);
}

void M_SelectVMU_Key (int k)
{
	if (m_inp_cancel)
	{
		if (!previousmenu)
			M_Off ();
		else if (previousmenu == m_singleplayer)
			M_SinglePlayer_f ();
		else if (previousmenu == m_options)
			M_Options_f ();
	}
	else if (m_inp_ok)
	{
		if (vmufreeblocks[vmu_cursor[0]][vmu_cursor[1]] >= 0)
		{
			vmupath.string[0] = vmu_cursor[0]+'a';
			vmupath.string[1] = vmu_cursor[1]+'1';
			if (nextmenu == m_save)
				M_Save_f();
			if (nextmenu == m_savesmall)
				M_SaveSmall_f();
			if (nextmenu == m_load)
				M_Load_f();
			if (nextmenu == m_loadsmall)
				M_LoadSmall_f();
			if (nextmenu == m_saveoptions)
			{
				M_Options_f();
				Host_WriteConfiguration();
			}
			if (nextmenu == m_loadoptions)
			{
				M_Options_f();
				M_PopUp_f("Do you wish to load the\nsaved settings?", "exec config.cfg\n");
			}
		}
	}
	else
	{
		S_LocalSound ("misc/menu1.wav");
		if (m_inp_up || m_inp_down)
			vmu_cursor[1] = !vmu_cursor[1];
		else if (m_inp_right)
		{
			if (++vmu_cursor[0] > 3)
				vmu_cursor[0] = 0;
		}
		else if (m_inp_left)
		{
			if (--vmu_cursor[0] < 0)
				vmu_cursor[0] = 3;
		}
	}
}
#endif
// Manoel Kasimier - VMU saves - end

// -------------------------------------------------
// BlackAura (08-12-2002) - Replacing fscanf (start)
// -------------------------------------------------
static void DCM_ScanString(FILE *file, char *string)
{
	char newchar;
	fread(&newchar, 1, 1, file);
	while(newchar != '\n')
	{
		*string++ = newchar;
		fread(&newchar, 1, 1, file);
	}
	*string++ = '\0';
}

static int DCM_ScanInt(FILE *file)
{
	char sbuf[32768];
	DCM_ScanString(file, sbuf);
	return Q_atoi(sbuf);
}
// -------------------------------------------------
//  BlackAura (08-12-2002) - Replacing fscanf (end)
// -------------------------------------------------

int		liststart[4]; // Manoel Kasimier [m_state - m_load]
int		load_cursormax; // Manoel Kasimier
#define	MAX_SAVEGAMES		100 // Manoel Kasimier - edited
char	m_filenames[MAX_SAVEGAMES][SAVEGAME_COMMENT_LENGTH+1];
char	*m_fileinfo[MAX_SAVEGAMES]; // Manoel Kasimier
int		loadable[MAX_SAVEGAMES];

void M_ScanSaves (qboolean smallsave) // Manoel Kasimier - edited
{
	int		i, j;
	char	name[MAX_OSPATH];
	FILE	*f;
	int		version;
	// Manoel Kasimier - begin
	int		i1, i2;
	char	*s;
	char	t_enemies[16], k_enemies[16], t_secrets[16], f_secrets[16], str[64], fileinfo[64];
	// Manoel Kasimier - end
#ifdef _arch_dreamcast
	extern byte dc_foundsave[MAX_SAVEGAMES];
	extern void VMU_List (char c);
	VMU_List(smallsave?'G':'S');
#endif

	for (i=0 ; i<MAX_SAVEGAMES ; i++)
	{
		strcpy (m_filenames[i], "- empty -"); // Manoel Kasimier - edited
		loadable[i] = false;

		// Manoel Kasimier - begin
		if (m_fileinfo[i])
			Z_Free(m_fileinfo[i]);
		m_fileinfo[i] = NULL;

		//"%s/s%i.sav" - renamed savegames' names
#ifdef _arch_dreamcast
		if (!dc_foundsave[i])
			continue;
		sprintf (name, "/ram/%s.%c%i%i", savename.string, smallsave?'G':'S', i/10, i%10);
		VMU_Load(name, COM_SkipPath(name));
#else
		sprintf (name, "%s/%s.%c%i%i", com_gamedir, savename.string, smallsave?'G':'S', i/10, i%10);
#endif
		// Manoel Kasimier - end

		f = fopen (name, "r");
		if (!f)
			continue;

		// Manoel Kasimier - begin
		// set everything to "0"
		t_enemies[0] = k_enemies[0] = t_secrets[0] = f_secrets[0] = '0';
		t_enemies[1] = k_enemies[1] = t_secrets[1] = f_secrets[1] = 0;
		// Manoel Kasimier - end

		//  BlackAura (08-12-2002) - Replacing fscanf
		// fscanf (f, "%i\n", &version);
		// fscanf (f, "%79s\n", name);
		version = DCM_ScanInt(f);
		DCM_ScanString(f, name);
		strncpy (m_filenames[i], name, sizeof(m_filenames[i])-1 -17); // Manoel Kasimier - edited

	// change _ back to space
		for (j=0 ; j<SAVEGAME_COMMENT_LENGTH ; j++)
			if (m_filenames[i][j] == '_')
				m_filenames[i][j] = ' ';

		// Manoel Kasimier - begin
		// skill
		for (i1=0 ; i1 < 17; i1++)
			i2 = DCM_ScanInt(f);
		s = skilltos(i2);

		DCM_ScanString(f, fileinfo); // map
		strcat (fileinfo, va("\n%s\n", s)); // add skill
		if (!smallsave)
		{
			strcat (fileinfo, timetos(DCM_ScanInt(f))); // add time
			for (i2=0 ; i2 < 200 ; i2++)
			{
				DCM_ScanString(f, str);
				s = COM_Parse (str);
				if (com_token[0] == '}')
					break;
				if (!Q_strcmp(com_token, "total_secrets"))
				{
					s = COM_Parse (s);
					Q_strcpy(t_secrets, com_token);
					continue;
				}
				if (!Q_strcmp(com_token, "total_monsters"))
				{
					s = COM_Parse (s);
					Q_strcpy(t_enemies, com_token);
					continue;
				}
				if (!Q_strcmp(com_token, "found_secrets"))
				{
					s = COM_Parse (s);
					Q_strcpy(f_secrets, com_token);
					continue;
				}
				if (!Q_strcmp(com_token, "killed_monsters"))
				{
					s = COM_Parse (s);
					Q_strcpy(k_enemies, com_token);
					break;
				}
			}
			Q_strcat(fileinfo, va("\n%s / %s", k_enemies, t_enemies));
			Q_strcat(fileinfo, va("\n%s / %s", f_secrets, t_secrets));
		}
		m_fileinfo[i] = Z_Malloc(Q_strlen(fileinfo)+1);
		Q_strcpy(m_fileinfo[i], fileinfo);
		// Manoel Kasimier - end

		loadable[i] = true;
		fclose (f);
#ifdef _arch_dreamcast
		fs_unlink(name); // delete save from RAM
#endif
	}
}

// Manoel Kasimier - begin
void M_RefreshSaveList (void)
{
	int		i, i2;
	qboolean saving = (m_state == m_save || m_state == m_savesmall);
	qboolean smallsave = (m_state == m_loadsmall || m_state == m_savesmall);
	if (key_dest == key_menu && (smallsave || m_state == m_save || m_state == m_load))
	{
		M_ScanSaves (smallsave);
		for (i=i2=0; i< MAX_SAVEGAMES; i++)
			if (loadable[i] || (saving && (loadable[i-1] || i == 0)))
				i2++;
		if (m_cursor[m_state] >= i2)
			m_cursor[m_state] = i2 - 1;
		if (m_cursor[m_state] < 0)
			m_cursor[m_state] = 0;
		m_entersound = true;
	}
}
// info panel
void M_DrawFileInfo (int i, int y)
{
	if (m_state == m_loadsmall || m_state == m_savesmall)
	{
		M_DrawTextBox (72, y, 20*8, 2*8);
		M_PrintText ("map\nskill", 88, y+8, 20, 2, ALIGN_LEFT);
		M_PrintText (m_fileinfo[i], 160, y+8, 20, 2, ALIGN_LEFT);
	}
	else
	{
		M_DrawTextBox (72, y, 20*8, 5*8);
		M_PrintText ("map\nskill\ntime\nenemies\nsecrets", 88, y+8, 20, 5, ALIGN_LEFT);
		M_PrintText (m_fileinfo[i], 160, y+8, 20, 5, ALIGN_LEFT);
	}
}

// merged save/load menu functions

void M_Save_Common_f (int menustate)
{
	int		i, i2;
	qboolean saving = (menustate == m_save || menustate == m_savesmall);

	if (key_dest == key_menu && m_state == menustate)
	{
		M_Off ();
		return;
	}
	if (saving)
	{
		if (!sv.active)
			return;
		if (svs.maxclients != 1)
			return;
	}
	M_ScanSaves (menustate == m_savesmall || menustate == m_loadsmall);
	for (i=i2=0; i< MAX_SAVEGAMES; i++)
		if (loadable[i] || (saving && (loadable[i-1] || i == 0)))
			i2++;
	if (!saving && i2 == 0)
	{
		M_PopUp_f((menustate==m_loadsmall) ? "There isn't any save\nto load." : "There isn't any state\nto load.", "");
		return;
	}
	m_entersound = true;
	key_dest = key_menu;
	m_state = menustate;
	if (m_cursor[m_state] >= i2)
		m_cursor[m_state] = i2 - 1;
	if (m_cursor[m_state] < 0)
		m_cursor[m_state] = 0;
	M_Precache(); // Manoel Kasimier - precaching menu pics
}
void M_Save_f (void)
{
	if (cl.stats[STAT_HEALTH] <= 0) // the player is dead
		return;
	if (cl.intermission)
		return;
	M_Save_Common_f(m_save);
}
void M_SaveSmall_f (void)
{
	M_Save_Common_f(m_savesmall);
}
void M_Load_f (void)
{
	M_Save_Common_f(m_load);
}
void M_LoadSmall_f (void)
{
	M_Save_Common_f(m_loadsmall);
}

void M_Save_Draw (void)
{
	int		i, i2, numsaves;
	qboolean saving = (m_state == m_save || m_state == m_savesmall);
	qboolean smallsave = (m_state == m_loadsmall || m_state == m_savesmall);

	M_DrawPlaque (saving ? "gfx/p_save.lmp" : "gfx/p_load.lmp", true);

	numsaves = (int)((vid.height - (screen_top + 28) - 8*7 - screen_bottom) / 8);
	if (smallsave)
		numsaves += 3;

	for (i=i2=0 ; i< MAX_SAVEGAMES; i++)
		if (loadable[i] || (saving && (loadable[i-1] || i == 0)))
		{
			if ((i >= liststart[m_state - m_load]) && (i2 - liststart[m_state - m_load] >= 0) && (i2 - liststart[m_state - m_load] < numsaves))
			{
				M_Print (56 + (8*(i < 10)), screen_top + 28 + 8*(i2 - liststart[m_state - m_load]), va("%i", i));
				if (loadable[i] == true)
					M_Print		 (96, screen_top + 28 + 8*(i2 - liststart[m_state - m_load]), m_filenames[i]);
				else
					M_PrintWhite (96, screen_top + 28 + 8*(i2 - liststart[m_state - m_load]), m_filenames[i]);
			}
			i2++;
		}
	load_cursormax = i2;

	if (load_cursormax >= numsaves)
	if (liststart[m_state - m_load] > load_cursormax - numsaves)
		liststart[m_state - m_load] = load_cursormax - numsaves;

	for (i=i2=0 ; i< MAX_SAVEGAMES; i++)
		if (loadable[i] || (saving && (loadable[i-1] || i == 0)))
		{
			if (m_cursor[m_state] == i2)
			{
				if (loadable[i])
					M_DrawFileInfo(i, screen_top + 28 + (numsaves*8));
				break;
			}
			i2++;
		}
	M_DrawCursor (80, screen_top + 28, (m_cursor[m_state] - liststart[m_state - m_load])); // Manoel Kasimier
}
// Manoel Kasimier - end

void M_Save_Key (int k)
{
	// Manoel Kasimier - begin
	qboolean saving = (m_state == m_save || m_state == m_savesmall);
	int		i, i2, numsaves;
	numsaves = (int)((vid.height - (screen_top + 28) - 8*7 - screen_bottom) / 8) - 1;
	if (m_state == m_savesmall || m_state == m_loadsmall)
		numsaves += 3;
	// Manoel Kasimier - end

	if (m_inp_cancel)
		M_SinglePlayer_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		// Manoel Kasimier - begin
		if (--m_cursor[m_state] < 0)
		{
			m_cursor[m_state] = load_cursormax-1;
			liststart[m_state - m_load] = m_cursor[m_state] - numsaves;
		}
		if (m_cursor[m_state] < liststart[m_state - m_load])
			liststart[m_state - m_load] = m_cursor[m_state];
		if (liststart[m_state - m_load] < 0)
			liststart[m_state - m_load] = 0;
		// Manoel Kasimier - end
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		// Manoel Kasimier - begin
		if (++m_cursor[m_state] >= load_cursormax)
			liststart[m_state - m_load] = m_cursor[m_state] = 0;
		if (m_cursor[m_state] > liststart[m_state - m_load] + numsaves)
			liststart[m_state - m_load] = m_cursor[m_state] - numsaves;
		// Manoel Kasimier - end
	}
	else if (m_inp_ok)
	{
		// Manoel Kasimier - begin
		for (i=i2=0 ; i< MAX_SAVEGAMES; i++)
			if (loadable[i] || (saving && (loadable[i-1] || i == 0)))
			{
				if (m_cursor[m_state] == i2)
				{
				if (saving)
				{
					// the "menu_main;menu_save" part is a hack to refresh the list of saves
					if (loadable[i] == false)
// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
						Cbuf_AddText (va ("save%s %s.%c%i%i\n", (m_state==m_savesmall)?"small":"", savename.string, (m_state==m_savesmall)?'G':'S', i/10, i%10));
					else
						M_PopUp_f ((m_state==m_savesmall)?"Overwrite saved game?":"Overwrite saved state?", va ("save%s %s.%c%i%i\n", (m_state==m_savesmall)?"small":"", savename.string, (m_state==m_savesmall)?'G':'S', i/10, i%10));
#else
// Manoel Kasimier - VMU saves - end
						Cbuf_AddText (va ("save%s %s.%c%i%i\nmenu_main;menu_save%s\n", (m_state==m_savesmall)?"small":"", savename.string, (m_state==m_savesmall)?'G':'S', i/10, i%10, (m_state==m_savesmall)?"small":""));
					else
						M_PopUp_f ((m_state==m_savesmall)?"Overwrite saved game?":"Overwrite saved state?", va ("save%s %s.%c%i%i\nmenu_main;menu_save%s\n", (m_state==m_savesmall)?"small":"", savename.string, (m_state==m_savesmall)?'G':'S', i/10, i%10, (m_state==m_savesmall)?"small":""));
#endif // Manoel Kasimier - VMU saves
					return;
				}
				else
				{
		// Manoel Kasimier - end
					// Host_Loadgame_f can't bring up the loading plaque because too much
					// stack space has been used, so do it now
					SCR_BeginLoadingPlaque ();
					Cbuf_AddText (va ("load%s %s.%c%i%i\n", (m_state==m_loadsmall)?"small":"", savename.string, (m_state==m_loadsmall)?'G':'S', i/10, i%10) );
					fade_level = 0; // BlackAura - Menu background fading
					m_state = m_none;
					key_dest = key_game;
					return;
		// Manoel Kasimier - begin
				}
				}
				i2++;
			}
	}
	else if (k == K_DEL || k == K_BACKSPACE || k == K_JOY1_BACKSPACE || k == K_JOY2_BACKSPACE)
	{
		for (i=i2=0 ; i< MAX_SAVEGAMES; i++)
			if (loadable[i] || (saving && (loadable[i-1] || i == 0)))
			{
				if (loadable[i] && m_cursor[m_state] == i2)
				{
					M_PopUp_f ("Are you sure you want to\ndelete this save?", va ("del %s.%c%i%i\n",
						savename.string, (m_state==m_loadsmall||m_state==m_savesmall)?'G':'S', i/10, i%10));
					return;
				}
				i2++;
			}
	}
	// Manoel Kasimier - end
}


//=============================================================================
/* OPTIONS MENU */

#ifdef _arch_dreamcast
#define	OPTIONS_ITEMS	6
#define QUICKHACK	1
#else
#define	OPTIONS_ITEMS	7
#define QUICKHACK	0
#endif

void M_Options_f (void)
{
//M_Fightoon_f();
//return; // fightoon
	// Manoel Kasimier - begin
	if (key_dest == key_menu && m_state == m_options)
	{
		M_Off ();
		return;
	}
	// Manoel Kasimier - end
	key_dest = key_menu;
	m_state = m_options;
	m_entersound = true;
	M_Precache(); // Manoel Kasimier - precaching menu pics
}

void M_Options_Draw (void)
{
	// Manoel Kasimier - begin
	int y = screen_top + 20;

	M_DrawPlaque ("gfx/p_option.lmp", true);

#ifdef _arch_dreamcast
	M_Print (16, y += 8, "            Controller    ..."); // disabled - only works in the DC
#endif
	M_Print (16, y += 8, "                 Audio    ...");
	M_Print (16, y += 8, "                 Video    ...");
	// Manoel Kasimier - end
	M_Print (16, y += 8, "            Save setup"); // Manoel Kasimier - edited
	M_Print (16, y += 8, "            Load setup"); // Manoel Kasimier - edited
	M_Print (16, y += 8, "     Reset to defaults"); // Manoel Kasimier - edited

	M_DrawCursor (200, screen_top + 28, m_cursor[m_state]); // Manoel Kasimier
}


void M_Options_Key (int k)
{
	if (m_inp_cancel)
		M_Main_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0) // Manoel Kasimier
			m_cursor[m_state] = OPTIONS_ITEMS-1; // Manoel Kasimier - edited
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= OPTIONS_ITEMS) // Manoel Kasimier
			m_cursor[m_state] = 0; // Manoel Kasimier - edited
	}
	else if (m_inp_ok)
	{
		switch (m_cursor[m_state]) // Manoel Kasimier - replaced cursor variables
		{
		// Manoel Kasimier - end
		// Manoel Kasimier - begin
#ifdef _arch_dreamcast
		case 0:
			M_Controller_f ();
			break;
#endif
		case 1:
			M_Audio_f ();
			break;
		case 2:
			M_Video_f ();
			break;

		case 3:
		// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
			M_SelectVMU_f (m_saveoptions);
#else
		// Manoel Kasimier - VMU saves - end
			m_entersound = true;
			Host_WriteConfiguration ();
#endif	// Manoel Kasimier - VMU saves
			break;
		case 4:
		// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
			M_SelectVMU_f (m_loadoptions);
#else
		// Manoel Kasimier - VMU saves - end
			M_PopUp_f("Do you wish to load the\nsaved settings?", "exec config.cfg\n"); // Manoel Kasimier
#endif	// Manoel Kasimier - VMU saves
			break;
		case 5:
			M_PopUp_f("Do you wish to load the\ndefault settings?", "exec default.cfg\n"); // Manoel Kasimier
			break;
		// Manoel Kasimier - end
		default:
			break;
		}
	}

}


//=============================================================================
/* KEYS MENU */

// Manoel Kasimier - begin
#define	MAXKEYS 16
#define MAXCMDS 256

qboolean dont_bind[K_NUMKEYS];
char	*bindnames[K_NUMKEYS][2];
int		numcommands;
int		m_keys_pages;
//int		m_keys_page_start[16]; // first command on the page
int		cmd_position[256];//[NUMCOMMANDS];
int		cmd_for_position[256];
int		keys_for_cmd[MAXCMDS];
int		keyliststart;
// Manoel Kasimier - end
int		bind_grab;

// Manoel Kasimier - begin
void M_Keys_Bindable (void)
{
	if (Cmd_Argc() != 3)
	{
		Con_Printf ("bindable <key> <0/1>\n");
		return;
	}
	dont_bind[Key_StringToKeynum(Cmd_Argv(1))] = !Q_atof(Cmd_Argv(2));
}
// Manoel Kasimier - end

void M_UnbindCommand (char *command)
{
	// unbinds all keys bound to the specified command
	int		j;
	int		l;
	char	*b;

	l = strlen(command);

	for (j=0 ; j<K_NUMKEYS ; j++)
	{
		if (dont_bind[j]) // Manoel Kasimier
			continue; // Manoel Kasimier
		b = keybindings[j];
		if (b) // Manoel Kasimier - edited
			if (!strcmp (b, command))//, l) ) // Manoel Kasimier - edited
				Key_SetBinding (j, "");
		// Manoel Kasimier - begin
		b = shiftbindings[j];
		if (b)
			if (!strcmp (b, command))//, l) )
				Key_SetShiftBinding (j, "");
		// Manoel Kasimier - end
	}
}

void M_FindKeysForCommand (char *command, int *keys) // Manoel Kasimier - edited
{
	// Finds how many keys are bound to the specified command
	int		count;
	int		j;
	int		l;
	char	*b;

	// Manoel Kasimier - begin
	for (count=0 ; count<MAXKEYS ; count++)
		keys[count] = -1;
	// Manoel Kasimier - end

	l = strlen(command);
	for (count=j=0 ; (j<K_NUMKEYS) || (count == MAXKEYS) ; j++) // Manoel Kasimier - edited
	{
		// look at each key
		b = keybindings[j];
		// Manoel Kasimier - edited - begin
		if (b) // if there is any command bound to this key,
		if (!strcmp (b, command))//, l) ) //strncmp // compare the string bound to the key with the string of the command
		{
			keys[count] = j; // puts the number of the key into the slot of the array
		// Manoel Kasimier - edited - end
			count++;		 // select the next slot in the array
			if (count == MAXKEYS) // Manoel Kasimier - edited
				break;
		}
		// Manoel Kasimier - begin
		// same stuff, now for shifted keys
		b = shiftbindings[j];
		if (b)
		if (!strcmp (b, command))//, l) ) //strncmp
		{
			keys[count] = -j; // dirty hack, using negative integers to identify the shifted keys
			count++;
		}
		// Manoel Kasimier - end
	}
}

// Manoel Kasimier - begin
void M_FillKeyList (void)
{
	int		i, i2, position;
	int		keys[MAXKEYS];
	position = 0;

	// finds how many keys are set for each command

	// it assumes that at least one key is bound, for practicity purposes, because the menu
	// always displays at least one line (either the key or the "---") for each command
	for (i=0 ; i<MAXCMDS ; i++)
		keys_for_cmd[i] = 1;
	for (i=0 ; i<numcommands ; i++)
	{
		cmd_for_position[position] = i;
		M_FindKeysForCommand (bindnames[i][0], keys);
		for (i2=1 ; i2<MAXKEYS ; i2++)
		{
			if (keys[i2] != -1)
			{
				keys_for_cmd[i]++;
				cmd_for_position[position + i2] = i;
			}
			else break;
		}
		cmd_position[i] = position; // the position of the command in the menu
		position += keys_for_cmd[i];
	}
}

void M_Keys_Clear (void)
{
	int i;
	if ((key_dest == key_menu) && (m_state == m_keys))
		return;
	for (i=0; i<numcommands; i++)
	{
		Z_Free (bindnames [i][0]);
		Z_Free (bindnames [i][1]);
	}
	keyliststart = m_cursor[m_state] = numcommands = 0;
}

void M_Keys_AddCmd (void)
{
	char	*s;

	if (Cmd_Argc() != 3)
	{
		Con_Printf ("menu_keys_addcmd <command> <description>\n");
		return;
	}
	if ((key_dest == key_menu) && (m_state == m_keys))
		return;

	s = Z_Malloc (strlen(Cmd_Argv(1))+1);
	strcpy (s, Cmd_Argv(1));
	bindnames [numcommands][0] = s;

	s = Z_Malloc (strlen(Cmd_Argv(2))+1);
	strcpy (s, Cmd_Argv(2));
	bindnames [numcommands][1] = s;

	numcommands++;
}
// Manoel Kasimier - end

// Manoel Kasimier - reordered the default commands
char *defaultbindnames[][2] = // Manoel Kasimier - renamed
{
{"+shift",			"function shift"}, // Manoel Kasimier
{"+use",			"use"}, // Manoel Kasimier
{"+attack", 		"attack"},
{"impulse 10", 		"next weapon"},
{"impulse 12",		"previous weapon"}, // BlackAura (11-12-2002) - Next/prev weapons
// Manoel Kasimier - begin
{"impulse 1",		"Axe"},
{"impulse 2",		"Shotgun"},
{"impulse 3",		"Super shotgun"},
{"impulse 4",		"Nailgun"},
{"impulse 5",		"Super nailgun"},
{"impulse 6",		"Grenade launcher"},
{"impulse 7",		"Rocket launcher"},
{"impulse 8",		"Thunderbolt"},
// Manoel Kasimier - end
{"+jump", 			"jump / swim up"},
{"+moveup",			"swim up"},
{"+movedown",		"swim down"},
{"+lookup", 		"look up"},
{"+lookdown", 		"look down"},
{"+left", 			"turn left"},
{"+right", 			"turn right"},
{"+forward", 		"walk forward"},
{"+back", 			"backpedal"},
{"+moveleft", 		"step left"},
{"+moveright", 		"step right"},
{"+strafe", 		"sidestep"},
{"+speed", 			"run"},
{"+klook", 			"keyboard look"},
{"centerview", 		"center view"},
{"+showscores",		"show scores"} // Manoel Kasimier
};
#define	NUMCOMMANDS (sizeof(defaultbindnames)/sizeof(defaultbindnames[0]))

// Manoel Kasimier - begin
void M_Keys_SetDefaultCmds (void)
{
	int i;

	keyliststart = m_cursor[m_state] = 0;
	numcommands = NUMCOMMANDS;
	for (i=0; i<numcommands; i++)
	{
		bindnames[i][0] = Z_Malloc (strlen(defaultbindnames[i][0])+1);
		strcpy (bindnames[i][0], defaultbindnames[i][0]);
		bindnames[i][1] = Z_Malloc (strlen(defaultbindnames[i][1])+1);
		strcpy (bindnames[i][1], defaultbindnames[i][1]);
	}
}
// Manoel Kasimier - end

void M_Keys_f (void)
{
	// Manoel Kasimier - begin
	if (key_dest == key_menu && m_state == m_keys)
	{
		M_Off ();
		return;
	}
	// Manoel Kasimier - end
	key_dest = key_menu;
	m_state = m_keys;
	m_entersound = true;
	// Manoel Kasimier - begin
	bind_grab = false;
	if (numcommands == 0)
		M_Keys_SetDefaultCmds();
	// Manoel Kasimier - end
	M_Precache(); // Manoel Kasimier - precaching menu pics
}

void M_Keys_Draw (void)
{
	// Manoel Kasimier - begin
	int		i, i2;
	int		keys[MAXKEYS];
	int	/*	x,*/ y;
	int		top, bottom;

	M_DrawPlaque ("gfx/p_option.lmp", true); // ttl_cstm Manoel Kasimier

//	m_keys_pages = 1; // for testing purposes

	if (m_keys_pages > 0)
		M_Print (128, screen_top + 28, "Page 1/1");
	top = screen_top + 28 + (m_keys_pages > 0)*16;
	bottom = vid.height - 48 - screen_bottom; // vid.height, textbox height

	M_DrawTextBox (0, bottom + 8, 37*8, 3*8);
	if (bind_grab)
		M_PrintText ("Hold Function Shift for \"FS + button\"\nPress a key or button for this action\nPress Start or ESC to cancel", 12, bottom + 16, 37, 3, ALIGN_CENTER);
	else
		M_PrintText ("A button or Enter to change\nX button or Backspace to clear\nDelete to clear all keys for command", 12, bottom + 16, 37, 3, ALIGN_CENTER);

	// search for known bindings
	y = top - (keyliststart*8) - 8;
	// Manoel Kasimier - end
	for (i=0 ; i<numcommands; i++) // Manoel Kasimier - edited
	{
		M_FindKeysForCommand (bindnames[i][0], keys);
		// Manoel Kasimier - begin
		if (((y+16) > top) && (y+7 < bottom))
		{
			// prints the description of the command
			M_Print (56, y += 8, bindnames[i][1]);
			// prints the first key bound to this command
			if (keys[0] == -1)
				M_PrintWhite (196, y, "---");
			else if (keys[0] < 0)
				M_Print (196, y, va("FS + %s", Key_KeynumToString (-keys[0])));
			else
				M_Print (196, y, Key_KeynumToString (keys[0]));
		}
		else
			y += 8;
		// prints the other keys bound to this command
		for (i2=1 ; i2<MAXKEYS ; i2++)
			if ((keys[i2] != -1) && (y+7 < bottom))
			{
				if ((y + 16) > top)
				{
					if (keys[i2] < 0)
						M_Print (196, y += 8, va("FS + %s", Key_KeynumToString (-keys[i2])));
					else
						M_Print (196, y += 8, Key_KeynumToString (keys[i2]));
				}
				else
					y += 8;
			}
			else break;
		// Manoel Kasimier - end
	}

	M_FillKeyList(); // Manoel Kasimier
	if (bind_grab)
		M_DrawCharacter (186, top + (cmd_position[cmd_for_position[m_cursor[m_state]]] - keyliststart)*8, '='); // Manoel Kasimier
	else
		M_DrawCursor (186, top, (m_cursor[m_state] - keyliststart)); // Manoel Kasimier
}

void M_Keys_Key (int k)
{
	char	cmd[80];
	int		keys[MAXKEYS]; // Manoel Kasimier
	int		menulines = (vid.height - screen_top - 28 - (m_keys_pages>0)*16 - 48 - screen_bottom) / 8; // Manoel Kasimier

	M_FillKeyList(); // Manoel Kasimier

	if (bind_grab)
	{	// defining a key
		S_LocalSound ("misc/menu3.wav"); // Edited by Manoel Kasimier
		// Manoel Kasimier - begin
		if (k>='A' && k<='Z')
			k = k + 'a' - 'A'; // convert upper case characters to lower case
		// Manoel Kasimier - end
		if ((k == K_ESCAPE) || m_inp_off) // Manoel Kasimier - edited
			bind_grab = false;
		// Manoel Kasimier - begin
		else if (keybindings[k] && !Q_strcmp(keybindings[k], "+shift"))
			return;
		else if (dont_bind[k] == false) // (k != '`')
		{
			if (shift_function)
				sprintf (cmd, "bindshift \"%s\" \"%s\"\n", Key_KeynumToString (k), bindnames[cmd_for_position[m_cursor[m_state]]][0]);
			else
		// Manoel Kasimier - end
				sprintf (cmd, "bind \"%s\" \"%s\"\n", Key_KeynumToString (k), bindnames[cmd_for_position[m_cursor[m_state]]][0]); // Manoel Kasimier - edited
			Cbuf_InsertText (cmd);
			bind_grab = false;
		}
		return;
	}

	if (m_inp_off)
		M_Off ();
	else if (m_inp_cancel)
		M_Options_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		// Manoel Kasimier - begin
		if (--m_cursor[m_state] < 0)
		{
			m_cursor[m_state] = cmd_position[numcommands-1] + keys_for_cmd[numcommands-1] -1;
			keyliststart = m_cursor[m_state] - menulines;
		}
		if (m_cursor[m_state] < keyliststart)
			keyliststart = m_cursor[m_state];
		if (keyliststart < 0)
			keyliststart = 0;
		// Manoel Kasimier - end
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		// Manoel Kasimier - begin
		if (++m_cursor[m_state] >= cmd_position[numcommands-1] + keys_for_cmd[numcommands-1])
			keyliststart = m_cursor[m_state] = 0;
		if (m_cursor[m_state] > keyliststart + menulines)
			keyliststart = m_cursor[m_state] - menulines;
		// Manoel Kasimier - end
	}
	else if (m_inp_ok)
	{
		S_LocalSound ("misc/menu3.wav"); // Edited by Manoel Kasimier
		bind_grab = true;
		if (keyliststart > cmd_position[cmd_for_position[m_cursor[m_state]]]) // Manoel Kasimier
			keyliststart = cmd_position[cmd_for_position[m_cursor[m_state]]]; // Manoel Kasimier
	}
	else if (m_inp_backspace) // delete binding
	{
		// Manoel Kasimier - begin
		S_LocalSound ("misc/menu3.wav");
		M_FindKeysForCommand (bindnames[cmd_for_position[m_cursor[m_state]]][0], keys);
		k = keys[m_cursor[m_state] - cmd_position[cmd_for_position[m_cursor[m_state]]]];
		if (dont_bind[k * -(k < 0)])
			return;
		if (k < 0)
			Key_SetShiftBinding (-k, "");
		else
			Key_SetBinding (k, "");
		M_FillKeyList();
		if (m_cursor[m_state] >= cmd_position[numcommands-1] + keys_for_cmd[numcommands-1])
		{
			m_cursor[m_state]  = cmd_position[numcommands-1] + keys_for_cmd[numcommands-1] -1;
			keyliststart = m_cursor[m_state] - menulines;
		}
		// Manoel Kasimier - end
	}
	else if (k == K_DEL) // delete multiple bindings
	{
		S_LocalSound ("misc/menu3.wav"); // Manoel Kasimier - edited
		M_UnbindCommand (bindnames[cmd_for_position[m_cursor[m_state]]][0]); // Manoel Kasimier - edited
	}
}

// Manoel Kasimier - begin
//=============================================================================
/* CONTROLLER OPTIONS MENU */
#define	CONTROLLER_ITEMS	8

void M_Controller_f (void)
{
	key_dest = key_menu;
	m_state = m_controller;
	m_entersound = true;
}
void M_Controller_Draw (void)
{
	int y = screen_top + 20;

	M_DrawPlaque ("gfx/p_option.lmp", true);

	M_Print (16, y+=8, "       Axis X Deadzone"); M_DrawSlider (220, y, axis_x_threshold.value * 2);
	M_Print (16, y+=8, "       Axis Y Deadzone"); M_DrawSlider (220, y, axis_y_threshold.value * 2);
	M_Print (16, y+=8, "    Trigger L Deadzone"); M_DrawSlider (220, y, axis_l_threshold.value * 2);
	M_Print (16, y+=8, "    Trigger R Deadzone"); M_DrawSlider (220, y, axis_r_threshold.value * 2);
	M_Print (16, y+=8, "    Axis X Sensitivity"); M_DrawSlider (220, y, axis_x_scale.value - 0.5);
	M_Print (16, y+=8, "    Axis Y Sensitivity"); M_DrawSlider (220, y, axis_y_scale.value - 0.5);
	M_Print (16, y+=8, "         L Sensitivity"); M_DrawSlider (220, y, axis_l_scale.value - 0.5);
	M_Print (16, y+=8, "         R Sensitivity"); M_DrawSlider (220, y, axis_r_scale.value - 0.5);

	M_DrawCursor (200, screen_top + 28, m_cursor[m_state]);
}
void M_Controller_Change (int dir)
{
	int c = m_cursor[m_state];
	int i = 0;

	S_LocalSound ("misc/menu3.wav");

	// threshold
	if (c == i++) ChangeCVar("dc_axisx_threshold", axis_x_threshold.value, dir * 0.05, 0, 0.5, true);
	if (c == i++) ChangeCVar("dc_axisy_threshold", axis_y_threshold.value, dir * 0.05, 0, 0.5, true);
	if (c == i++) ChangeCVar("dc_axisl_threshold", axis_l_threshold.value, dir * 0.05, 0, 0.5, true);
	if (c == i++) ChangeCVar("dc_axisr_threshold", axis_r_threshold.value, dir * 0.05, 0, 0.5, true);
	// sensitivity
	if (c == i++) ChangeCVar("dc_axisx_scale", axis_x_scale.value, dir * 0.1, 0.5, 1.5, true);
	if (c == i++) ChangeCVar("dc_axisy_scale", axis_y_scale.value, dir * 0.1, 0.5, 1.5, true);
	if (c == i++) ChangeCVar("dc_axisl_scale", axis_l_scale.value, dir * 0.1, 0.5, 1.5, true);
	if (c == i++) ChangeCVar("dc_axisr_scale", axis_r_scale.value, dir * 0.1, 0.5, 1.5, true);
}
void M_Controller_Key (int k)
{
	if (m_inp_cancel)
		M_Options_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = CONTROLLER_ITEMS-1;
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= CONTROLLER_ITEMS)
			m_cursor[m_state] = 0;
	}
	else if (m_inp_left)
		M_Controller_Change (-1);
	else if (m_inp_right || m_inp_ok)
		M_Controller_Change (1);
}
//=============================================================================
/* MOUSE CONFIG MENU */
#define	MOUSE_ITEMS	5

void M_Mouse_f (void)
{
	key_dest = key_menu;
	m_state = m_mouse;
	m_entersound = true;
}
void M_Mouse_Draw (void)
{
	int y = screen_top + 20;

	M_DrawPlaque ("gfx/p_option.lmp", true);

	M_Print (16, y+=8, "           Sensitivity"); M_DrawSlider (220, y, (sensitivity.value - 1)/10);
	M_Print (16, y+=8, "            Walk Speed"); M_DrawSlider (220, y, m_forward.value - 0.5);
	M_Print (16, y+=8, "          Strafe Speed"); M_DrawSlider (220, y, m_side.value - 0.5);
	M_Print (16, y+=8, "          Invert Mouse"); M_DrawCheckbox (220, y, m_pitch.value < 0);
	M_Print (16, y+=8, "            Mouse Look"); M_DrawCheckbox (220, y, m_look.value);

	M_DrawCursor (200, screen_top + 28, m_cursor[m_state]);
}
void M_Mouse_Change (int dir)
{
	int c = m_cursor[m_state];
	int i = 0;

	S_LocalSound ("misc/menu3.wav");

	if (c == i++) ChangeCVar("sensitivity", sensitivity.value, dir * 0.5, 1, 11, true);
	if (c == i++) ChangeCVar("m_forward", m_forward.value, dir * 0.1, 0.5, 1.5, true);
	if (c == i++) ChangeCVar("m_side", m_side.value, dir * 0.1, 0.5, 1.5, true);
	if (c == i++) Cvar_SetValue ("m_pitch", -m_pitch.value);
	if (c == i++) Cvar_SetValue ("m_look", !m_look.value);
}
void M_Mouse_Key (int k)
{
	if (m_inp_cancel)
		M_Options_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = MOUSE_ITEMS-1;
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= MOUSE_ITEMS)
			m_cursor[m_state] = 0;
	}
	else if (m_inp_left)
		M_Mouse_Change (-1);
	else if (m_inp_right || m_inp_ok)
		M_Mouse_Change (1);
}
//=============================================================================
/* GAMEPLAY OPTIONS MENU */
#ifdef _arch_dreamcast
#define	GAMEPLAY_ITEMS	13
#else
#define	GAMEPLAY_ITEMS	12
#endif

void M_Gameplay_f (void)
{
	key_dest = key_menu;
	m_state = m_gameplay;
	m_entersound = true;
}
void M_Gameplay_Draw (void)
{
	int y = screen_top + 20;

	M_DrawPlaque ("gfx/p_option.lmp", true);

	M_Print (16, y+=8, "        Aim Assistance");// M_DrawCheckbox (220, y, sv_aim.value < 1);
	if (sv_aim_h.value < 1)
		M_Print (220, y, "on");
	else if (sv_aim.value < 1)
		M_Print (220, y, "vertical");
	else
		M_Print (220, y, "off");
	M_Print (16, y+=8, "       Weapon Position");
	if (scr_ofsy.value > 0)
		M_Print (220, y, "Left");
	else if (scr_ofsy.value < 0)
		M_Print (220, y, "Right");
	else
		M_Print (220, y, "Center");
	M_Print (16, y+=8, "    View Player Weapon"); M_DrawCheckbox (220, y, r_drawviewmodel.value);
	M_Print (16, y+=8, "               Bobbing"); M_DrawCheckbox (220, y, !cl_nobob.value);
	M_Print (16, y+=8, "            Slope Look"); M_DrawCheckbox (220, y, sv_idealpitchscale.value > 0);
	M_Print (16, y+=8, "      Auto Center View"); M_DrawCheckbox (220, y, lookspring.value);
	M_Print (16, y+=8, "            Lookstrafe"); M_DrawCheckbox (220, y, lookstrafe.value);
	M_Print (16, y+=8, "            Always Run"); M_DrawCheckbox (220, y, cl_forwardspeed.value > 200);
	M_Print (16, y+=8, "    Allow \"use\" button"); M_DrawCheckbox (220, y, sv_enable_use_button.value);
#ifdef _arch_dreamcast
	M_Print (16, y+=8, "             Vibration");
	if (cl_vibration.value == 2)
		M_Print (220, y, "QC only");
	else
		M_DrawCheckbox (220, y, cl_vibration.value);
#endif
	M_Print (16, y+=8, "             Body View"); M_DrawCheckbox (220, y, chase_active.value);
	M_Print (16, y+=8, "    Body View Distance"); M_DrawSlider (220, y, (chase_back.value - 50) / 100);
	M_Print (16, y+=8, "      Body View Height"); M_DrawSlider (220, y, chase_up.value / 50);

	M_DrawCursor (200, screen_top + 28, m_cursor[m_state]);
}
void M_Gameplay_Change (int dir)
{
	int c = m_cursor[m_state];
	int i = 0;

	S_LocalSound ("misc/menu3.wav");

	if (c == i++)
	{
//		(sv_aim.value < 1) ? Cvar_SetValue ("sv_aim", 1) : Cvar_SetValue ("sv_aim", 0.93);
		if (dir == 1)
		{
			if (sv_aim_h.value < 1) // on
			{
				Cvar_SetValue ("sv_aim_h", 1);
				Cvar_SetValue ("sv_aim", 1);
			}
			else if (sv_aim.value < 1) // vertical
				Cvar_SetValue ("sv_aim_h", 0.98);
			else // off
				Cvar_SetValue ("sv_aim", 0.93);
		}
		else
		{
			if (sv_aim_h.value < 1) // on
			{
				Cvar_SetValue ("sv_aim_h", 1);
				Cvar_SetValue ("sv_aim", 0.93);
			}
			else if (sv_aim.value < 1) // vertical
				Cvar_SetValue ("sv_aim", 1);
			else // off
				Cvar_SetValue ("sv_aim_h", 0.98);
		}
	}
	if (c == i++) ChangeCVar("scr_ofsy", scr_ofsy.value, dir * -7, -7, 7, false);
	if (c == i++) Cvar_SetValue ("r_drawviewmodel", !r_drawviewmodel.value);
	if (c == i++) Cvar_SetValue ("cl_nobob", !cl_nobob.value);
	if (c == i++) Cvar_SetValue ("sv_idealpitchscale", !sv_idealpitchscale.value * 0.8);
	if (c == i++) Cvar_SetValue ("lookspring", !lookspring.value);
	if (c == i++) Cvar_SetValue ("lookstrafe", !lookstrafe.value);
	if (c == i++) // always run
	{
		if (cl_forwardspeed.value > 200)
		{
			Cvar_SetValue ("cl_forwardspeed", 200);
			Cvar_SetValue ("cl_backspeed", 200);
		}
		else
		{
			Cvar_SetValue ("cl_forwardspeed", 400);
			Cvar_SetValue ("cl_backspeed", 400);
		}
	}
	if (c == i++) Cvar_SetValue ("sv_enable_use_button", !sv_enable_use_button.value);
#ifdef _arch_dreamcast
	if (c == i++) ChangeCVar("cl_vibration", cl_vibration.value, dir, 0, 2, false);
#endif
	if (c == i++) Cvar_SetValue ("chase_active", !chase_active.value);
	if (c == i++) ChangeCVar("chase_back", chase_back.value, dir * 10, 50, 150, true);
	if (c == i++) ChangeCVar("chase_up", chase_up.value, dir * 5, 0, 50, true);
}
void M_Gameplay_Key (int k)
{
	if (m_inp_cancel)
		M_Options_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = GAMEPLAY_ITEMS-1;
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= GAMEPLAY_ITEMS)
			m_cursor[m_state] = 0;
	}
	else if (m_inp_left)
		M_Gameplay_Change (-1);
	else if (m_inp_right || m_inp_ok)
		M_Gameplay_Change (1);
}
//=============================================================================
/* AUDIO OPTIONS MENU */
#ifndef _arch_dreamcast
#ifndef _WIN32
#define	AUDIO_ITEMS	12
#else
#define	AUDIO_ITEMS	11
#endif
#else
#define	AUDIO_ITEMS	9
#endif
byte cd_cursor;

void M_Audio_f (void)
{
	key_dest = key_menu;
	m_state = m_audio;
	m_entersound = true;
	cd_cursor = playTrack;
}

void M_Audio_Draw (void)
{
	int y = screen_top + 20;

	M_DrawPlaque ("gfx/p_option.lmp", true);

	M_Print (16, y+=8, "          Stereo Sound"); M_DrawCheckbox (220, y, snd_stereo.value);
	M_Print (16, y+=8, "     Swap L/R Channels"); M_DrawCheckbox (220, y, snd_swapstereo.value);
	M_Print (16, y+=8, "          Sound Volume"); M_DrawSlider (220, y, volume.value);
#ifndef _WIN32
	M_Print (16, y+=8, "       CD Music Volume"); M_DrawSlider (220, y, bgmvolume.value);
#endif
	M_Print (16, y+=8, "              CD Music"); M_DrawCheckbox (220, y, cd_enabled.value);
	M_Print (16, y+=8, "            Play Track");
	if (cdValid)
	{
		(cd_cursor == playTrack) ? M_Print (220, y, va("%u", cd_cursor)) : M_PrintWhite (220, y, va("%u", cd_cursor)); M_Print (244, y, va("/ %u", maxTrack));
	}
	else if (maxTrack)
		M_Print (220, y, "No tracks");
	else
		M_Print (220, y, "Drive empty");
	M_Print (16, y+=8, "                  Loop"); M_DrawCheckbox (220, y, playLooping);
	M_Print (16, y+=8, "                 Pause"); M_DrawCheckbox (220, y, !playing && wasPlaying);
	M_Print (16, y+=8, "                  Stop");
#ifndef _arch_dreamcast
	M_Print (16, y+=8, "            Open Drive");
	M_Print (16, y+=8, "           Close Drive");
	M_Print (16, y+=8, "        Reset CD Audio");
#endif
	M_DrawCursor (200, screen_top + 28, m_cursor[m_state]);
}
void M_Audio_Change (int dir)
{
	int c = m_cursor[m_state];
	int i = 0;

	S_LocalSound ("misc/menu3.wav");

	if (c == i++) Cvar_SetValue ("snd_stereo", !snd_stereo.value);
	if (c == i++) Cvar_SetValue ("snd_swapstereo", !snd_swapstereo.value);
	if (c == i++) ChangeCVar("volume", volume.value, dir * 0.1, 0, 1, true);
#ifndef _WIN32
	if (c == i++) ChangeCVar("bgmvolume", bgmvolume.value, dir * 0.1, 0, 1, true);
#endif
	if (c == i++) Cvar_SetValue ("cd_enabled", !cd_enabled.value);
	if (c == i++)
	{
		if (!cdValid)
			return;
		do
		{
			cd_cursor += dir;
			if (cd_cursor > maxTrack)
				cd_cursor = 1;
			if (cd_cursor < 1)
				cd_cursor = maxTrack;
		}
		while (audioTrack[cd_cursor] == false);
	}
	if (c == i++) playLooping = !playLooping;
	if (c == i++) (!playing && wasPlaying) ? CDAudio_Resume() : CDAudio_Pause();
	if (c == i++) CDAudio_Stop();
#ifndef _arch_dreamcast
	if (c == i++) Cbuf_AddText ("cd eject\n");
	if (c == i++) Cbuf_AddText ("cd close\n");
	if (c == i++) Cbuf_AddText ("cd reset\n");
#endif
}
void M_Audio_Key (int k)
{
	if (m_inp_cancel)
		M_Options_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = AUDIO_ITEMS-1;
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= AUDIO_ITEMS)
			m_cursor[m_state] = 0;
	}
#ifndef _WIN32
	else if (m_cursor[m_state] == 5 && m_inp_ok && cdValid && audioTrack[cd_cursor])
#else
	else if (m_cursor[m_state] == 4 && m_inp_ok && cdValid && audioTrack[cd_cursor])
#endif
		CDAudio_Play(cd_cursor, playLooping);
	else if (m_inp_left)
		M_Audio_Change (-1);
	else if (m_inp_right || m_inp_ok)
		M_Audio_Change (1);
}
// Manoel Kasimier - end

//=============================================================================
/* VIDEO MENU */
// Manoel Kasimier - begin
#ifdef	GLQUAKE
#define	VIDEO_ITEMS	16
#else

#ifndef _arch_dreamcast
#define	VIDEO_ITEMS	19
#else
#define	VIDEO_ITEMS	18
#endif

#endif

void M_Video_f (void)
{
	if (key_dest == key_menu && m_state == m_video)
	{
		M_Off ();
		return;
	}
	key_dest = key_menu;
	m_state = m_video;
	m_entersound = true;
}
// Manoel Kasimier - end


void M_Video_Draw (void)
{
	// Manoel Kasimier - begin
	int y = screen_top + 20;

	M_DrawPlaque ("gfx/p_option.lmp", true);

//	M_Print (16, y+=8, "           Screen size"); M_DrawSlider (220, y, (scr_viewsize.value - 50) / 50);
//	M_Print (16, y+=8, "           Screen left"); M_DrawSlider (220, y, scr_left.value / 15);
//	M_Print (16, y+=8, "          Screen right"); M_DrawSlider (220, y, (15 - scr_right.value) / 15);
//	M_Print (16, y+=8, "            Screen top"); M_DrawSlider (220, y, (15 - scr_top.value) / 15);
//	M_Print (16, y+=8, "         Screen bottom"); M_DrawSlider (220, y, scr_bottom.value / 15);
#ifndef _arch_dreamcast
	M_Print (16, y+=8, "           Video modes    ...");
#endif
	M_Print (16, y+=8, "         Adjust screen    ...");
	M_Print (16, y+=8, "            Brightness"); M_DrawSlider (220, y, (1.0 - v_gamma.value) / 0.5);
	M_Print (16, y+=8, "            Status bar"); M_DrawSlider (220, y, sbar.value / 4.0);
	M_Print (16, y+=8, "            Background"); M_DrawCheckbox (220, y, sbar_show_bg.value);
	M_Print (16, y+=8, "          Level status"); M_DrawCheckbox (220, y, sbar_show_scores.value);
	M_Print (16, y+=8, "           Weapon list"); M_DrawCheckbox (220, y, sbar_show_weaponlist.value);
	M_Print (16, y+=8, "             Ammo list"); M_DrawCheckbox (220, y, sbar_show_ammolist.value);
	M_Print (16, y+=8, "                  Keys"); M_DrawCheckbox (220, y, sbar_show_keys.value);
	M_Print (16, y+=8, "                 Runes"); M_DrawCheckbox (220, y, sbar_show_runes.value);
	M_Print (16, y+=8, "              Powerups"); M_DrawCheckbox (220, y, sbar_show_powerups.value);
	M_Print (16, y+=8, "                 Armor"); M_DrawCheckbox (220, y, sbar_show_armor.value);
	M_Print (16, y+=8, "                Health"); M_DrawCheckbox (220, y, sbar_show_health.value);
	M_Print (16, y+=8, "                  Ammo"); M_DrawCheckbox (220, y, sbar_show_ammo.value);
#ifndef GLQUAKE
	M_Print (16, y+=8, "      Classic lighting"); M_DrawCheckbox (220, y, !r_light_style.value);
	M_Print (16, y+=8, "         Stipple alpha"); M_DrawCheckbox (220, y, sw_stipplealpha.value);
#endif
	M_Print (16, y+=8, "         Water opacity"); M_DrawSlider (220, y, r_wateralpha.value);
	M_Print (16, y+=8, " Translucent particles"); M_DrawCheckbox (220, y, r_particlealpha.value);
	M_Print (16, y+=8, " Additive sprite blend"); M_DrawCheckbox (220, y, r_sprite_addblend.value);

	M_DrawCursor (200, screen_top + 28, m_cursor[m_state]);
	// Manoel Kasimier - end
}
// Manoel Kasimier - begin
void M_Video_Change (int dir)
{
	int c = m_cursor[m_state];
#ifndef _arch_dreamcast
	int i = 2;
#else
	int i = 1;
#endif

	S_LocalSound ("misc/menu3.wav");

//	if (c == i++) ChangeCVar("viewsize", scr_viewsize.value, dir * 5, 50, 100, true);
//	if (c == i++) ChangeCVar("scr_left",	scr_left.value, dir * 0.5, 0, 15, true);
//	if (c == i++) ChangeCVar("scr_right",	scr_right.value, -dir * 0.5, 0, 15, true);
//	if (c == i++) ChangeCVar("scr_top",		scr_top.value, -dir * 0.5, 0, 15, true);
//	if (c == i++) ChangeCVar("scr_bottom",	scr_bottom.value, dir * 0.5, 0, 15, true);
	if (c == i++) ChangeCVar("gamma", v_gamma.value, dir * -0.05, 0.5, 1, true);
	if (c == i++) ChangeCVar("sbar", sbar.value, dir, 0, 4, true);
	if (c == i++) Cvar_SetValue ("sbar_show_bg", !sbar_show_bg.value);
	if (c == i++) Cvar_SetValue ("sbar_show_scores", !sbar_show_scores.value);
	if (c == i++) Cvar_SetValue ("sbar_show_weaponlist", !sbar_show_weaponlist.value);
	if (c == i++) Cvar_SetValue ("sbar_show_ammolist", !sbar_show_ammolist.value);
	if (c == i++) Cvar_SetValue ("sbar_show_keys", !sbar_show_keys.value);
	if (c == i++) Cvar_SetValue ("sbar_show_runes", !sbar_show_runes.value);
	if (c == i++) Cvar_SetValue ("sbar_show_powerups", !sbar_show_powerups.value);
	if (c == i++) Cvar_SetValue ("sbar_show_armor", !sbar_show_armor.value);
	if (c == i++) Cvar_SetValue ("sbar_show_health", !sbar_show_health.value);
	if (c == i++) Cvar_SetValue ("sbar_show_ammo", !sbar_show_ammo.value);
#ifndef GLQUAKE
	if (c == i++) Cvar_SetValue ("r_light_style", !r_light_style.value);
	if (c == i++) Cvar_SetValue ("sw_stipplealpha", !sw_stipplealpha.value);
#endif
	if (c == i++) ChangeCVar("r_wateralpha", r_wateralpha.value, dir*0.2, 0, 1, true);
	if (c == i++) Cvar_SetValue ("r_particlealpha", !r_particlealpha.value);
	if (c == i++) Cvar_SetValue ("r_sprite_addblend", !r_sprite_addblend.value);
}
// Manoel Kasimier - end
void M_Video_Key (int k) // int key Edited by Manoel Kasimier
{
//	(*vid_menukeyfn) (key);
	// Manoel Kasimier - begin
	if (m_inp_cancel)
		M_Options_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = VIDEO_ITEMS-1;
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= VIDEO_ITEMS)
			m_cursor[m_state] = 0;
	}
	else if (m_cursor[m_state] == 0 && m_inp_ok)
#ifndef _arch_dreamcast
		M_VideoModes_f ();
	else if (m_cursor[m_state] == 1 && m_inp_ok)
#endif
		M_VideoSize_f ();
	else if (m_inp_left)
		M_Video_Change (-1);
	else if (m_inp_right || m_inp_ok)
		M_Video_Change (1);
	// Manoel Kasimier - end
}

// Manoel Kasimier - begin
//=============================================================================
/* VIDEO SIZE MENU */
#define	VIDEOSIZE_ITEMS	2

byte adjusting;

void M_VideoSize_f (void)
{
	if (key_dest == key_menu && m_state == m_videosize)
	{
		M_Off ();
		return;
	}
	key_dest = key_menu;
	m_state = m_videosize;
	m_entersound = true;
	adjusting = 0;
}

void M_VideoSize_Draw (void)
{
	int y = screen_top + 20;

	Draw_Fill (0, 0, vid.width, vid.height, 0);
	Draw_Fill (screen_left					, 0, 1, vid.height, 15);
	Draw_Fill (vid.width - screen_right - 1 , 0, 1, vid.height, 15);
	Draw_Fill (0, screen_top					, vid.width, 1, 15);
	Draw_Fill (0, vid.height - screen_bottom - 1, vid.width, 1, 15);

	M_Print (72, y+=8, "Adjust top / left");
	M_Print (72, y+=8, "Adjust bottom / right");

	if (adjusting == 1)
		M_PrintText (va("[ %2.1f %% ]\n\n[ %2.1f %% ]      %2.1f %%  \n\n%2.1f %%", scr_top.value, scr_left.value, scr_right.value, scr_bottom.value), 0, y+=64, 38, 3, ALIGN_CENTER);
	else if (adjusting == 2)
		M_PrintText (va("%2.1f %%\n\n  %2.1f %%      [ %2.1f %% ]\n\n[ %2.1f %% ]", scr_top.value, scr_left.value, scr_right.value, scr_bottom.value), 0, y+=64, 38, 3, ALIGN_CENTER);
	else
	{
		M_DrawCursor (56, screen_top + 28, m_cursor[m_state]);
		M_PrintText (va("%2.1f %%\n\n%2.1f %%        %2.1f %%\n\n%2.1f %%", scr_top.value, scr_left.value, scr_right.value, scr_bottom.value), 0, y+=64, 38, 3, ALIGN_CENTER);
	}
}
void M_VideoSize_Key (int k)
{
	if (!adjusting)
	{
		if (m_inp_cancel)
			M_Video_f ();
//			M_Fightoon_f (); // Fightoon
		else if (m_inp_up)
		{
			S_LocalSound ("misc/menu1.wav");
			if (--m_cursor[m_state] < 0)
				m_cursor[m_state] = VIDEOSIZE_ITEMS-1;
		}
		else if (m_inp_down)
		{
			S_LocalSound ("misc/menu1.wav");
			if (++m_cursor[m_state] >= VIDEOSIZE_ITEMS)
				m_cursor[m_state] = 0;
		}
		else if (m_inp_ok)
		{
			S_LocalSound ("misc/menu3.wav");
			adjusting = m_cursor[m_state] + 1;
		}
	}
	else if (adjusting == 1) // top left
	{
		if (m_inp_up)
			ChangeCVar("scr_top", scr_top.value, -0.5, 0, 12, true);
		else if (m_inp_down)
			ChangeCVar("scr_top", scr_top.value, 0.5, 0, 12, true);
		else if (m_inp_left)
			ChangeCVar("scr_left", scr_left.value, -0.5, 0, 12, true);
		else if (m_inp_right)
			ChangeCVar("scr_left", scr_left.value, 0.5, 0, 12, true);
		else if (m_inp_cancel || m_inp_ok)
		{
			S_LocalSound ("misc/menu3.wav");
			adjusting = 0;
		}
	}
	else if (adjusting == 2) // bottom right
	{
		if (m_inp_up)
			ChangeCVar("scr_bottom", scr_bottom.value, 0.5, 0, 15, true);
		else if (m_inp_down)
			ChangeCVar("scr_bottom", scr_bottom.value, -0.5, 0, 15, true);
		else if (m_inp_left)
			ChangeCVar("scr_right", scr_right.value, 0.5, 0, 12, true);
		else if (m_inp_right)
			ChangeCVar("scr_right", scr_right.value, -0.5, 0, 12, true);
		else if (m_inp_cancel || m_inp_ok)
		{
			S_LocalSound ("misc/menu3.wav");
			adjusting = 0;
		}
	}
}
// Manoel Kasimier - end

//=============================================================================
/* VIDEO MODES MENU */
// Manoel Kasimier - re-added the video modes menu for the PC version
void M_VideoModes_f (void)
{
	key_dest = key_menu;
	m_state = m_videomodes;
	m_entersound = true;
}

// Manoel Kasimier - begin
//=============================================================================
/* DEVELOPER MENU */
#ifndef GLQUAKE
#define	DEVELOPER_ITEMS	16
#else
#define	DEVELOPER_ITEMS	14
#endif

void M_Developer_f (void)
{
	if (key_dest == key_menu && m_state == m_developer)
	{
		M_Off ();
		return;
	}
	key_dest = key_menu;
	m_state = m_developer;
	m_entersound = true;
}


cvar_t	loadas8bit; // workaround for Dreamcast only =P
void M_Developer_Draw (void)
{
	int y = screen_top + 20;

	M_DrawPlaque ("gfx/p_option.lmp", false);

	M_Print (16, y+=8, "              Show FPS"); M_DrawCheckbox (220, y, cl_showfps.value);
	M_Print (16, y+=8, "  3D stereo separation"); M_DrawSlider (220, y, (r_stereo_separation.value + 10.0) / 20.0);
	M_Print (16, y+=8, "                   fog"); M_DrawSlider (220, y, (developer.value-100.0)/255.0);
	M_Print (16, y+=8, "        developer mode"); M_DrawCheckbox (220, y, developer.value);
	M_Print (16, y+=8, "       registered mode"); M_DrawCheckbox (220, y, registered.value);
	M_Print (16, y+=8, "         draw entities"); M_DrawCheckbox (220, y, r_drawentities.value);
	M_Print (16, y+=8, "           full bright"); M_DrawCheckbox (220, y, r_fullbright.value);
#ifndef GLQUAKE
	M_Print (16, y+=8, "underwater warp effect"); M_DrawCheckbox (220, y, r_waterwarp.value);
#endif
	M_Print (16, y+=8, "  screen color effects"); M_DrawCheckbox (220, y, gl_polyblend.value);
	M_Print (16, y+=8, "   load sounds as 8bit"); M_DrawCheckbox (220, y, loadas8bit.value);
	M_Print (16, y+=8, "              timedemo");
	M_Print (16, y+=8, "                noclip");
	M_Print (16, y+=8, "                   fly");
	M_Print (16, y+=8, "              notarget");
	M_Print (16, y+=8, "                   god");
	M_Print (16, y+=8, "             impulse 9");

	M_DrawCursor (200, screen_top + 28, m_cursor[m_state]);
}
void M_Developer_Change (int dir)
{
	int c = m_cursor[m_state];
	int i = 0;

	S_LocalSound ("misc/menu3.wav");

	if (c == i++) Cvar_SetValue ("cl_showfps", !cl_showfps.value);
	if (c == i++) ChangeCVar("r_stereo_separation", r_stereo_separation.value, dir*0.5, -10.0, 10.0, true);
	if (c == i++) ChangeCVar("developer", developer.value, dir, 100, 355, false);
	if (c == i++) Cvar_SetValue ("developer", !developer.value);
	if (c == i++) Cvar_SetValue ("registered", !registered.value);
	if (c == i++) Cvar_SetValue ("r_drawentities", !r_drawentities.value);
	if (c == i++) Cvar_SetValue ("r_fullbright", !r_fullbright.value);
#ifndef GLQUAKE
	if (c == i++) Cvar_SetValue ("r_waterwarp", !r_waterwarp.value);
#endif
	if (c == i++) Cvar_SetValue ("gl_polyblend", !gl_polyblend.value);
	if (c == i++) Cvar_SetValue ("loadas8bit", !loadas8bit.value);
	if (c == i++){M_Off(); Cbuf_AddText ("timedemo demo1.dem\n");}
	if (c == i++) Cbuf_AddText ("noclip\n");
	if (c == i++) Cbuf_AddText ("fly\n");
	if (c == i++) Cbuf_AddText ("notarget\n");
	if (c == i++) Cbuf_AddText ("god\n");
	if (c == i++){M_Off(); Cbuf_AddText ("impulse 9\n");}
}
void M_Developer_Key (int k)
{
	if (m_inp_cancel)
		M_Options_f ();
	else if (m_inp_up)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = DEVELOPER_ITEMS-1;
	}
	else if (m_inp_down)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= DEVELOPER_ITEMS)
			m_cursor[m_state] = 0;
	}
	else if (m_inp_left)
		M_Developer_Change (-1);
	else if (m_inp_right || m_inp_ok)
		M_Developer_Change (1);
}
// Manoel Kasimier - end
//=============================================================================
/* HELP MENU */
double m_credits_starttime;
void M_Credits_f (void)
{
	if (key_dest != key_menu)
	{
		m_save_demonum = cls.demonum;
		cls.demonum = -1;
	}
	key_dest = key_menu;
	m_state = m_credits;
	m_cursor[m_state] = 1;//help_pages.value;
	m_credits_starttime = realtime;
}

void M_Credits_Key (int key)
{
	if (m_cursor[m_state] == 1)
	{
		m_cursor[m_state] += 1;
		m_credits_starttime = realtime;
	}
	else if (m_cursor[m_state] == 2)
	{
		m_cursor[m_state] += 1;
		m_credits_starttime = realtime-8.0;//8.5
		Cbuf_InsertText ("exec quake.rc\n");
	}
	else if (m_cursor[m_state] == 3)
		M_Off ();
}

void M_Help_f (void)
{
	// Manoel Kasimier - begin
	int i;
	if (key_dest == key_menu && m_state == m_help)
	{
		M_Off ();
		return;
	}
	for (i=0; i<help_pages.value; i++)
		Draw_CachePic(va("gfx/help%i.lmp", i));
	m_cursor[m_state] = 0;
	// Manoel Kasimier - end
	key_dest = key_menu;
	m_state = m_help;
	m_entersound = true;
}

void M_Help_Draw (void)
{
	// Manoel Kasimier - begin
	qpic_t	*p;
	int y = r_refdef.vrect.y + ((r_refdef.vrect.height-200)/2);
	if (y < 0)
		y = 0;
	if (y+200 > vid.height)
		y = vid.height - 200;

	if (m_state == m_credits)
	{
		if ((int)(realtime-m_credits_starttime) > 9)
			M_Credits_Key(0);
		if (m_cursor[m_state] == 1)
			goto credits1;
		else if (m_cursor[m_state] == 2)
			goto credits2;
		return;
	}

	if (m_cursor[m_state] < help_pages.value)
	{
		p = Draw_CachePic ( va("gfx/help%i.lmp", m_cursor[m_state]));
		M_DrawTransPic (0, y, p);
	}
	else
	{
		if (m_cursor[m_state] == help_pages.value)
		{
credits1:
			M_DrawTextBox (0, y, 38*8, 23*8);
			M_PrintWhite(16+4,	y+=12, "Platformer-Quake Dreamcast Demo");
			M_Print		(16,	 y+=8, " Version 0.0.1");
//			M_PrintWhite(16+4,	y+=12, "Rizzo Island Ver. 1.0");
			M_PrintWhite(16,	y+=16, "Based on Makaqu 1.3.1");
			
			M_PrintWhite(16,	y+=16, "Game Design");
			M_Print		(16,	 y+=8, " David Croshaw");

// Commented Out For Demo			
			M_PrintWhite(16,	y+=16, "Music By");
//			M_Print		(16,	 y+=8, " Tom Rizzo");
			M_Print		(16,	 y+=8, " David Croshaw");	
			
			M_PrintWhite(16,	y+=16, "Programming");
			M_Print		(16,	 y+=8, " Manoel Kasimier");
			M_Print		(16,	 y+=8, " BlackAura");
			M_Print		(16,	 y+=8, " David Croshaw");
			M_Print		(16,	 y+=8, " Everyone else who contributed!");			

			M_PrintWhite(16,	y+=16, "QA/Playtesting");
			M_Print		(16,	 y+=8, " Tim Mask");

//Commented out for Demo
//			M_PrintWhite(16,	y+=8, "Dedicated to my Uncle Tom Rizzo");
		}
		else
		{
credits2:
			M_DrawTextBox (0, y, 38*8, 23*8);
			M_PrintWhite(16+4,	y+=12, "     Quake engine version 1.09     ");

			M_PrintWhite(16,	y+=16, "Programming");
			M_Print		(16,	 y+=8, " John Carmack");
			M_Print		(16,	 y+=8, " Michael Abrash");
			M_Print		(16,	 y+=8, " John Cash");
			M_Print		(16,	 y+=8, " Dave 'Zoid' Kirsch");

			M_PrintWhite(16,	y+=16, "Quake is a trademark of Id Software,");
			M_PrintWhite(16,	 y+=8, "inc., (c)1996-1997 Id Software, inc.");
			M_PrintWhite(16,	 y+=8, "All rights reserved.");

			M_PrintWhite(16,	y+=16, "This engine is distributed in the");
			M_PrintWhite(16,	 y+=8, "hope that it will be useful, but");
			M_PrintWhite(16,	 y+=8, "WITHOUT ANY WARRANTY.");
			M_PrintWhite(16,	 y+=8, "Its code is licensed under the terms");
			M_PrintWhite(16,	 y+=8, "of the GNU General Public License.");
			M_PrintWhite(16,	 y+=8, "If you distribute modified binary");
			M_PrintWhite(16,	 y+=8, "versions of this engine, you must");
			M_PrintWhite(16,	 y+=8, "also make its source code available.");
			M_PrintWhite(16,	 y+=8, "See the GNU General Public License");
			M_PrintWhite(16,	 y+=8, "for more details.");
		}
	}
	// Manoel Kasimier - end
}

void M_Help_Key (int key)
{
	if (m_inp_cancel)
		M_Main_f ();
	else if (m_inp_down || m_inp_right)
	{
		S_LocalSound ("misc/menu1.wav");
		if (++m_cursor[m_state] >= (help_pages.value+2))
			m_cursor[m_state] = 0;
	}
	else if (m_inp_up || m_inp_left)
	{
		S_LocalSound ("misc/menu1.wav");
		if (--m_cursor[m_state] < 0)
			m_cursor[m_state] = help_pages.value+1;
	}

}

//=============================================================================
/* QUIT MENU */

char *quitMessage =
{
// Manoel Kasimier - edited - begin
"Go Back To\n\
Dreamcast Menu?\n\
B = No Y = Yes"
};

void M_Quit_f (void)
{
	M_PopUp_f (quitMessage, "quit"); // Manoel Kasimier
}
// fazer menu c/ opções de reiniciar o mapa atual, resetar o jogo, resetar o console
// e sair pra BIOS
// usar arch_reboot pra resetar o DC

//=============================================================================
/* Menu Subsystem */

// BlackAura (28-12-2002) - Fade menu background
void M_FadeBackground()
{
//	fade_level += 1; // Manoel Kasimier - edited
//	if(fade_level > 20)
		fade_level = 20;
#ifndef _arch_dreamcast
	Draw_FadeScreen ();
#else
	Draw_FadeScreen2 (fade_level);
#endif
}

void M_Draw (void)
{
	scr_copyeverything = 1;

	if (m_state == m_none || key_dest != key_menu)
	{
		// BlackAura (28-12-2002) - Fade menu background
		if(fade_level > 0)
		{
			scr_copyeverything = 1;
		//	fade_level -= 1; // Manoel Kasimier - edited
		//	if(fade_level < 0)
				fade_level = 0;
			scr_fullupdate = 1;
#ifndef _arch_dreamcast
			Draw_FadeScreen ();
#else
			Draw_FadeScreen2 (fade_level);
#endif
		}

		return;
	}

	if (!m_recursiveDraw)
	{
		scr_copyeverything = 1;

		if (scr_con_current)
		{
			Draw_ConsoleBackground (vid.height);
			VID_UnlockBuffer ();
			S_ExtraUpdate ();
			VID_LockBuffer ();
		}
		else
			// BlackAura (28-12-2002) - Fade menu background
			M_FadeBackground();

		scr_fullupdate = 0;
	}
	else
	{
		m_recursiveDraw = false;
	}

	switch (m_state)
	{
	case m_none:
		break;

	case m_main:
		M_Main_Draw ();
		break;

	case m_fightoon:
		M_Fightoon_Draw ();
		break;

	case m_singleplayer:
		M_SinglePlayer_Draw ();
		break;

	case m_loadsmall: // Manoel Kasimier - small saves
	case m_savesmall: // Manoel Kasimier - small saves
	case m_load:
	case m_save:
		M_Save_Draw ();
		break;


	case m_options:
		M_Options_Draw ();
		break;


	case m_keys:
		M_Keys_Draw ();
		break;

// Manoel Kasimier - begin
	// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
	case m_vmu:
		M_SelectVMU_Draw ();
		break;
#endif
	// Manoel Kasimier - VMU saves - end

	case m_controller:
		M_Controller_Draw ();
		break;

	case m_mouse:
		M_Mouse_Draw ();
		break;

	case m_gameplay:
		M_Gameplay_Draw ();
		break;

	case m_audio:
		M_Audio_Draw ();
		break;

	case m_developer:
		M_Developer_Draw ();
		break;

	case m_popup:
		M_PopUp_Draw ();
		break;

	case m_videomodes:
		(*vid_menudrawfn) ();
		break;

	case m_videosize:
		M_VideoSize_Draw ();
		break;
// Manoel Kasimier - end

	case m_video:
		M_Video_Draw ();
		break;

	case m_credits: // Manoel Kasimier
	case m_help:
		M_Help_Draw ();
		break;
		
	default:
		break;
	}
	if (m_entersound)
	{
		S_LocalSound ("misc/menu2.wav");
		m_entersound = false;
	}

	VID_UnlockBuffer ();
	S_ExtraUpdate ();
	VID_LockBuffer ();
}


void M_Keydown (int key)
{
	M_CheckInput (key); // sets input
	// Manoel Kasimier - menus which aren't turned off by m_inp_off
	switch (m_state)
	{
	// Manoel Kasimier - begin
	case m_popup: // must be verified before all other menus!
		M_PopUp_Key (key);
		return;

	case m_credits:
		M_Credits_Key (key);
		return;
	// Manoel Kasimier - end


	case m_keys:
		M_Keys_Key (key);
		return;

	default:
		break;
	}

	// Manoel Kasimier - begin
	if (m_inp_off)
	{
		M_Off ();
		return;
	}
#ifdef _WIN32
	if (key == '-')
	{
		Cbuf_AddText ("vid_minimize\n");
		return;
	}
#endif
	// Manoel Kasimier - end

	switch (m_state)
	{
	case m_none:
		return;

	case m_main:
		M_Main_Key (key);
		return;

	case m_fightoon:
		M_Fightoon_Key (key);
		return;

	case m_singleplayer:
		M_SinglePlayer_Key (key);
		return;

	case m_loadsmall: // Manoel Kasimier - small saves
	case m_savesmall: // Manoel Kasimier - small saves
	case m_load:
	case m_save:
		M_Save_Key (key);
		return;

	case m_options:
		M_Options_Key (key);
		return;

// Manoel Kasimier - begin
	// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
	case m_vmu:
	M_SelectVMU_Key (key);
		return;
#endif
	// Manoel Kasimier - VMU saves - end

	case m_controller:
		M_Controller_Key (key);
		return;

	case m_mouse:
		M_Mouse_Key (key);
		return;

	case m_gameplay:
		M_Gameplay_Key (key);
		return;

	case m_audio:
		M_Audio_Key (key);
		return;

	case m_developer:
		M_Developer_Key (key);
		return;

	case m_videomodes:
		(*vid_menukeyfn) (key);
		return;

	case m_videosize:
		M_VideoSize_Key (key);
		return;
// Manoel Kasimier - end

	case m_video:
		M_Video_Key (key);
		return;

	case m_help:
		M_Help_Key (key);
		return;

	default:
		break;
	}
}


void M_Init (void)
{
	// Manoel Kasimier - setting default menu items - begin
	int i;

	// Manoel Kasimier - registering stuff - begin
	Cvar_RegisterVariable (&help_pages);
	Cvar_RegisterVariableWithCallback (&savename, SetSavename);
#ifdef _arch_dreamcast // Manoel Kasimier
	if (Q_strcmp("id1", COM_SkipPath(com_gamedir)))
		Cvar_Set("savename", COM_SkipPath(com_gamedir));
	Cvar_RegisterVariableWithCallback (&vmupath, SetVMUpath);
	VMU_FindConfig();
#endif
	for (i=0 ; i<MAX_SAVEGAMES ; i++)
		m_fileinfo[i] = NULL;

	dont_bind['~'] = true;
	dont_bind['`'] = true;
	Cmd_AddCommand ("menu_keys_clear", M_Keys_Clear);
	Cmd_AddCommand ("menu_keys_addcmd", M_Keys_AddCmd);
	Cmd_AddCommand ("bindable", M_Keys_Bindable);
	// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
	Cmd_AddCommand ("menu_loadsmall", M_VMUloadsmall_f);
	Cmd_AddCommand ("menu_savesmall", M_VMUsavesmall_f);
#else
	// Manoel Kasimier - VMU saves - end
	Cmd_AddCommand ("menu_loadsmall", M_LoadSmall_f);
	Cmd_AddCommand ("menu_savesmall", M_SaveSmall_f);
#endif		// Manoel Kasimier - VMU saves
	// Manoel Kasimier - registering stuff - end

	Cmd_AddCommand ("togglemenu", M_ToggleMenu_f);

	Cmd_AddCommand ("menu_main", M_Main_f);
	Cmd_AddCommand ("menu_singleplayer", M_SinglePlayer_f);
	// Manoel Kasimier - VMU saves - begin
#ifdef _arch_dreamcast
	Cmd_AddCommand ("menu_load", M_VMUload_f);
	Cmd_AddCommand ("menu_save", M_VMUsave_f);
#else
	// Manoel Kasimier - VMU saves - end
	Cmd_AddCommand ("menu_load", M_Load_f);
	Cmd_AddCommand ("menu_save", M_Save_f);
#endif		// Manoel Kasimier - VMU saves
	Cmd_AddCommand ("menu_options", M_Options_f);
	Cmd_AddCommand ("menu_keys", M_Keys_f);
	Cmd_AddCommand ("menu_video", M_Video_f);
	Cmd_AddCommand ("menu_developer", M_Developer_f); // Manoel Kasimier
	Cmd_AddCommand ("help", M_Help_f);
	Cmd_AddCommand ("menu_quit", M_Quit_f);
}
