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

/*

key up events are sent even if in console mode

*/


#define		MAXCMDLINE	256
char	key_lines[32][MAXCMDLINE];
int		key_linepos;
int		shift_down=false;
int		key_lastpress;

int		edit_line=0;
int		history_line=0;

keydest_t	key_dest;

int		key_count;			// incremented every key event

char	*keybindings[K_NUMKEYS];
char	*shiftbindings[K_NUMKEYS];	// Manoel Kasimier - function shift
qboolean	shift_function;		// Manoel Kasimier - function shift
qboolean	consolekeys[K_NUMKEYS];	// if true, can't be rebound while in console
qboolean	menubound[K_NUMKEYS];	// if true, can't be rebound while in menu
qboolean	demokeys[K_NUMKEYS];					// Manoel Kasimier
qboolean	key_menurepeats[K_NUMKEYS];			// Manoel Kasimier
int		keyboard_active;					// Manoel Kasimier - on-screen keyboard
int		M_OnScreenKeyboard_Key (int key);	// Manoel Kasimier - on-screen keyboard
int		keyshift[K_NUMKEYS];		// key to map to if shift held down in console
int		key_repeats[K_NUMKEYS];	// if > 1, it is autorepeating
qboolean	keydown[K_NUMKEYS];

typedef struct
{
	char	*name;
	int		keynum;
} keyname_t;

keyname_t keynames[] =
{
    // keyboard
	{"TAB", K_TAB},
	{"ENTER", K_ENTER},
	{"ESCAPE", K_ESCAPE},
	{"SPACE", K_SPACE},
	{"BACKSPACE", K_BACKSPACE},
	{"UPARROW", K_UPARROW},
	{"DOWNARROW", K_DOWNARROW},
	{"LEFTARROW", K_LEFTARROW},
	{"RIGHTARROW", K_RIGHTARROW},

	{"ALT", K_ALT},
	{"CTRL", K_CTRL},
	{"SHIFT", K_SHIFT},

	{"F1", K_F1},
	{"F2", K_F2},
	{"F3", K_F3},
	{"F4", K_F4},
	{"F5", K_F5},
	{"F6", K_F6},
	{"F7", K_F7},
	{"F8", K_F8},
	{"F9", K_F9},
	{"F10", K_F10},
	{"F11", K_F11},
	{"F12", K_F12},

	{"INS", K_INS},
	{"DEL", K_DEL},
	{"PGDN", K_PGDN},
	{"PGUP", K_PGUP},
	{"HOME", K_HOME},
	{"END", K_END},

	{"SEMICOLON", ';'},	// because a raw semicolon seperates commands
	{"PAUSE", K_PAUSE},

    // mouse
	{"MOUSEUP", K_MOUSEUP},
	{"MOUSEDOWN", K_MOUSEDOWN},
	{"MOUSELEFT", K_MOUSELEFT},
	{"MOUSERIGHT", K_MOUSERIGHT},
	{"MOUSE1", K_MOUSE1},
	{"MOUSE2", K_MOUSE2},
	{"MOUSE3", K_MOUSE3},
	{"MOUSE4", K_MOUSE4},
	{"MOUSE5", K_MOUSE5},
	{"MWHEELUP", K_MWHEELUP},
	{"MWHEELDOWN", K_MWHEELDOWN},

    // joystick 1
#ifndef _arch_dreamcast
	{"JOY1_LEFT",   K_JOY1_AXISX_MINUS},
	{"JOY1_RIGHT",  K_JOY1_AXISX_PLUS},
	{"JOY1_UP",     K_JOY1_AXISY_MINUS},
	{"JOY1_DOWN",   K_JOY1_AXISY_PLUS},
	{"JOY1_AXISZ-", K_JOY1_AXISZ_MINUS},
	{"JOY1_AXISZ+", K_JOY1_AXISZ_PLUS},
	{"JOY1_AXISR-", K_JOY1_AXISR_MINUS},
	{"JOY1_AXISR+", K_JOY1_AXISR_PLUS},
	{"JOY1_AXISU-", K_JOY1_AXISU_MINUS},
	{"JOY1_AXISU+", K_JOY1_AXISU_PLUS},
	{"JOY1_AXISV-", K_JOY1_AXISV_MINUS},
	{"JOY1_AXISV+", K_JOY1_AXISV_PLUS},
	{"JOY1_POV_UP",      K_JOY1_POV_UP},
	{"JOY1_POV_DOWN",    K_JOY1_POV_DOWN},
	{"JOY1_POV_LEFT",    K_JOY1_POV_LEFT},
	{"JOY1_POV_RIGHT",   K_JOY1_POV_RIGHT},
	{"JOY1_1",  K_JOY1_1},
	{"JOY1_2",  K_JOY1_2},
	{"JOY1_3",  K_JOY1_3},
	{"JOY1_4",  K_JOY1_4},
	{"JOY1_5",  K_JOY1_5},
	{"JOY1_6",  K_JOY1_6},
	{"JOY1_7",  K_JOY1_7},
	{"JOY1_8",  K_JOY1_8},
	{"JOY1_9",  K_JOY1_9},
	{"JOY1_10", K_JOY1_10},
	{"JOY1_11", K_JOY1_11},
	{"JOY1_12", K_JOY1_12},
	{"JOY1_13", K_JOY1_13},
	{"JOY1_14", K_JOY1_14},
	{"JOY1_15", K_JOY1_15},
	{"JOY1_16", K_JOY1_16},
	{"JOY1_17", K_JOY1_17},
	{"JOY1_18", K_JOY1_18},
	{"JOY1_19", K_JOY1_19},
	{"JOY1_20", K_JOY1_20},
	{"JOY1_21", K_JOY1_21},
	{"JOY1_22", K_JOY1_22},
	{"JOY1_23", K_JOY1_23},
	{"JOY1_24", K_JOY1_24},
	{"JOY1_25", K_JOY1_25},
	{"JOY1_26", K_JOY1_26},
	{"JOY1_27", K_JOY1_27},
	{"JOY1_28", K_JOY1_28},
	{"JOY1_29", K_JOY1_29},
	{"JOY1_30", K_JOY1_30},
	{"JOY1_31", K_JOY1_31},
	{"JOY1_32", K_JOY1_32},
	// joystick 2
	{"JOY2_LEFT",   K_JOY2_AXISX_MINUS},
	{"JOY2_RIGHT",  K_JOY2_AXISX_PLUS},
	{"JOY2_UP",     K_JOY2_AXISY_MINUS},
	{"JOY2_DOWN",   K_JOY2_AXISY_PLUS},
	{"JOY2_AXISZ-", K_JOY2_AXISZ_MINUS},
	{"JOY2_AXISZ+", K_JOY2_AXISZ_PLUS},
	{"JOY2_AXISR-", K_JOY2_AXISR_MINUS},
	{"JOY2_AXISR+", K_JOY2_AXISR_PLUS},
	{"JOY2_AXISU-", K_JOY2_AXISU_MINUS},
	{"JOY2_AXISU+", K_JOY2_AXISU_PLUS},
	{"JOY2_AXISV-", K_JOY2_AXISV_MINUS},
	{"JOY2_AXISV+", K_JOY2_AXISV_PLUS},
	{"JOY2_POV_UP",      K_JOY2_POV_UP},
	{"JOY2_POV_DOWN",    K_JOY2_POV_DOWN},
	{"JOY2_POV_LEFT",    K_JOY2_POV_LEFT},
	{"JOY2_POV_RIGHT",   K_JOY2_POV_RIGHT},
	{"JOY2_1",  K_JOY2_1},
	{"JOY2_2",  K_JOY2_2},
	{"JOY2_3",  K_JOY2_3},
	{"JOY2_4",  K_JOY2_4},
	{"JOY2_5",  K_JOY2_5},
	{"JOY2_6",  K_JOY2_6},
	{"JOY2_7",  K_JOY2_7},
	{"JOY2_8",  K_JOY2_8},
	{"JOY2_9",  K_JOY2_9},
	{"JOY2_10", K_JOY2_10},
	{"JOY2_11", K_JOY2_11},
	{"JOY2_12", K_JOY2_12},
	{"JOY2_13", K_JOY2_13},
	{"JOY2_14", K_JOY2_14},
	{"JOY2_15", K_JOY2_15},
	{"JOY2_16", K_JOY2_16},
	{"JOY2_17", K_JOY2_17},
	{"JOY2_18", K_JOY2_18},
	{"JOY2_19", K_JOY2_19},
	{"JOY2_20", K_JOY2_20},
	{"JOY2_21", K_JOY2_21},
	{"JOY2_22", K_JOY2_22},
	{"JOY2_23", K_JOY2_23},
	{"JOY2_24", K_JOY2_24},
	{"JOY2_25", K_JOY2_25},
	{"JOY2_26", K_JOY2_26},
	{"JOY2_27", K_JOY2_27},
	{"JOY2_28", K_JOY2_28},
	{"JOY2_29", K_JOY2_29},
	{"JOY2_30", K_JOY2_30},
	{"JOY2_31", K_JOY2_31},
	{"JOY2_32", K_JOY2_32},
#endif
#ifdef _arch_dreamcast
    // Dreamcast controller 1
	{"JOY1_LEFT",   K_JOY1_DPAD1_LEFT},
	{"JOY1_RIGHT",  K_JOY1_DPAD1_RIGHT},
	{"JOY1_UP",     K_JOY1_DPAD1_UP},
	{"JOY1_DOWN",   K_JOY1_DPAD1_DOWN},
	{"JOY1_D2_LEFT",	K_JOY1_DPAD2_LEFT},
	{"JOY1_D2_RIGHT",	K_JOY1_DPAD2_RIGHT},
	{"JOY1_D2_UP",		K_JOY1_DPAD2_UP},
	{"JOY1_D2_DOWN",	K_JOY1_DPAD2_DOWN},
	{"JOY1_AXIS_X-",	K_JOY1_AXISX_MINUS},
	{"JOY1_AXIS_X+",	K_JOY1_AXISX_PLUS},
	{"JOY1_AXIS_Y-",	K_JOY1_AXISY_MINUS},
	{"JOY1_AXIS_Y+",	K_JOY1_AXISY_PLUS},
	{"JOY1_AXIS_Y2-",	K_JOY1_AXISZ_MINUS},
	{"JOY1_AXIS_Y2+",	K_JOY1_AXISZ_PLUS},
	{"JOY1_AXIS_X2-",	K_JOY1_AXISR_MINUS},
	{"JOY1_AXIS_X2+",	K_JOY1_AXISR_PLUS},
	{"JOY1_A",		K_JOY1_A},
	{"JOY1_B",		K_JOY1_B},
	{"JOY1_C",		K_JOY1_C},
	{"JOY1_D",		K_JOY1_D},
	{"JOY1_X",		K_JOY1_X},
	{"JOY1_Y",		K_JOY1_Y},
	{"JOY1_Z",		K_JOY1_Z},
	{"JOY1_START",	K_JOY1_START},
	{"JOY1_TRIGL",	K_JOY1_TRIGL},
	{"JOY1_TRIGR",	K_JOY1_TRIGR},
    // Dreamcast controller 2
	{"JOY2_LEFT",   K_JOY2_DPAD1_LEFT},
	{"JOY2_RIGHT",  K_JOY2_DPAD1_RIGHT},
	{"JOY2_UP",     K_JOY2_DPAD1_UP},
	{"JOY2_DOWN",   K_JOY2_DPAD1_DOWN},
	{"JOY2_D2_LEFT",	K_JOY2_DPAD2_LEFT},
	{"JOY2_D2_RIGHT",	K_JOY2_DPAD2_RIGHT},
	{"JOY2_D2_UP",		K_JOY2_DPAD2_UP},
	{"JOY2_D2_DOWN",	K_JOY2_DPAD2_DOWN},
	{"JOY2_AXIS_X-",	K_JOY2_AXISX_MINUS},
	{"JOY2_AXIS_X+",	K_JOY2_AXISX_PLUS},
	{"JOY2_AXIS_Y-",	K_JOY2_AXISY_MINUS},
	{"JOY2_AXIS_Y+",	K_JOY2_AXISY_PLUS},
	{"JOY2_AXIS_Y2-",	K_JOY2_AXISZ_MINUS},
	{"JOY2_AXIS_Y2+",	K_JOY2_AXISZ_PLUS},
	{"JOY2_AXIS_X2-",	K_JOY2_AXISR_MINUS},
	{"JOY2_AXIS_X2+",	K_JOY2_AXISR_PLUS},
	{"JOY2_A",		K_JOY2_A},
	{"JOY2_B",		K_JOY2_B},
	{"JOY2_C",		K_JOY2_C},
	{"JOY2_D",		K_JOY2_D},
	{"JOY2_X",		K_JOY2_X},
	{"JOY2_Y",		K_JOY2_Y},
	{"JOY2_Z",		K_JOY2_Z},
	{"JOY2_START",	K_JOY2_START},
	{"JOY2_TRIGL",	K_JOY2_TRIGL},
	{"JOY2_TRIGR",	K_JOY2_TRIGR},
	// QuakeDC compatibility
	{"DC_DUP",		K_JOY1_DPAD1_UP},
	{"DC_DDOWN",	K_JOY1_DPAD1_DOWN},
	{"DC_DLEFT",	K_JOY1_DPAD1_LEFT},
	{"DC_DRIGHT",	K_JOY1_DPAD1_RIGHT},
	{"DC_A",        K_JOY1_A},
	{"DC_B",        K_JOY1_B},
	{"DC_X",        K_JOY1_X},
	{"DC_Y",        K_JOY1_Y},
	{"DC_TRIGL",	K_JOY1_TRIGL},
	{"DC_TRIGR",	K_JOY1_TRIGR},
	// PC compatibility
	{"JOY1_AXIS_Z-",	K_JOY1_AXISZ_MINUS},
	{"JOY1_AXIS_Z+",	K_JOY1_AXISZ_PLUS},
	{"JOY1_AXIS_R-",	K_JOY1_AXISR_MINUS},
	{"JOY1_AXIS_R+",	K_JOY1_AXISR_PLUS},
	{"JOY2_AXIS_Z-",	K_JOY2_AXISZ_MINUS},
	{"JOY2_AXIS_Z+",	K_JOY2_AXISZ_PLUS},
	{"JOY2_AXIS_R-",	K_JOY2_AXISR_MINUS},
	{"JOY2_AXIS_R+",	K_JOY2_AXISR_PLUS},
#endif
	// Manoel Kasimier - end
	{NULL,0} // end of list - don't remove
};

/*
==============================================================================

			LINE TYPING INTO THE CONSOLE

==============================================================================
*/


/*
====================
Key_Console

Interactive line editing and console scrollback
====================
*/
void Key_Console (int key)
{
	char	*cmd;
//key_dest = key_game; // fightoon
//return; // fightoon

	key_dest = key_console; // Manoel Kasimier - on-screen keyboard

	key = M_OnScreenKeyboard_Key (key); // Manoel Kasimier - on-screen keyboard

	switch (key) // Manoel Kasimier - controller functionality to the console
	{
	// enter command
	case K_ENTER:
	case K_JOY1_ENTER:
	case K_JOY2_ENTER:
		Cbuf_AddText (key_lines[edit_line]+1);	// skip the >
		Cbuf_AddText ("\n");
		Con_Printf ("%s\n",key_lines[edit_line]);
		edit_line = (edit_line + 1) & 31;
		history_line = edit_line;
		key_lines[edit_line][0] = ']';
		key_linepos = 1;
		if (cls.state == ca_disconnected)
			SCR_UpdateScreen ();	// force an update, because the command may take some time
		return;

	// autocomplete
	case K_TAB:
		// command completion
		cmd = Cmd_CompleteCommand (key_lines[edit_line]+1);
		if (!cmd)
			cmd = Cvar_CompleteVariable (key_lines[edit_line]+1);
		if (!cmd) // Manoel Kasimier - auto-completion of aliases
			cmd = Cmd_CompleteAlias (key_lines[edit_line]+1); // Manoel Kasimier - auto-completion of aliases
		if (cmd)
		{
			Q_strcpy (key_lines[edit_line]+1, cmd);
			key_linepos = Q_strlen(cmd)+1;
			key_lines[edit_line][key_linepos] = ' ';
			key_linepos++;
			key_lines[edit_line][key_linepos] = 0;
		}
		return;

	case K_BACKSPACE:
	case K_LEFTARROW:
		if (key_linepos > 1)
			key_linepos--;
		return;

	// previous commands
	case K_UPARROW:
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
		do
		{
			history_line = (history_line - 1) & 31;
		} while (history_line != edit_line
				&& !key_lines[history_line][1]);
		if (history_line == edit_line)
			history_line = (edit_line+1)&31;
		Q_strcpy(key_lines[edit_line], key_lines[history_line]);
		key_linepos = Q_strlen(key_lines[edit_line]);
		return;

	case K_DOWNARROW:
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
		if (history_line == edit_line) return;
		do
		{
			history_line = (history_line + 1) & 31;
		}
		while (history_line != edit_line
			&& !key_lines[history_line][1]);
		if (history_line == edit_line)
		{
			key_lines[edit_line][0] = ']';
			key_linepos = 1;
		}
		else
		{
			Q_strcpy(key_lines[edit_line], key_lines[history_line]);
			key_linepos = Q_strlen(key_lines[edit_line]);
		}
		return;

	// scrolling
	case K_PGUP:
	case K_MWHEELUP:
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
		con_backscroll += 2;
		if (con_backscroll > con_totallines - (vid.height>>3) - 1)
			con_backscroll = con_totallines - (vid.height>>3) - 1;
		return;

	case K_PGDN:
	case K_MWHEELDOWN:
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
		con_backscroll -= 2;
		if (con_backscroll < 0)
			con_backscroll = 0;
		return;

	case K_HOME:
	case K_JOY1_PGUP:
	case K_JOY2_PGUP:
		con_backscroll = con_totallines - (vid.height>>3) - 1;
		return;

	case K_END:
	case K_JOY1_PGDN:
	case K_JOY2_PGDN:
		con_backscroll = 0;
		return;
    default:
        break;
	}

	// commandline editing
	if (key < 32 || key > 127)
		return;	// non printable

	if (key_linepos < MAXCMDLINE-1)
	{
		key_lines[edit_line][key_linepos] = key;
		key_linepos++;
		key_lines[edit_line][key_linepos] = 0;
	}

}

//============================================================================

char chat_buffer[32];
qboolean team_message = false;

void Key_Message (int key)
{
	static int chat_bufferlen = 0;

	key = M_OnScreenKeyboard_Key (key); // Manoel Kasimier - on-screen keyboard

	if (key == K_ENTER)
	{
		if (team_message)
			Cbuf_AddText ("say_team \"");
		else
			Cbuf_AddText ("say \"");
		Cbuf_AddText(chat_buffer);
		Cbuf_AddText("\"\n");

		key_dest = key_game;
		chat_bufferlen = 0;
		chat_buffer[0] = 0;
		return;
	}

	if (key == K_ESCAPE || key == K_JOY1_BACK || key == K_JOY1_ESCAPE || key == K_JOY2_BACK || key == K_JOY2_ESCAPE) // Manoel Kasimier - on-screen keyboard - edited
	{
		M_OnScreenKeyboard_Reset(); // Manoel Kasimier - on-screen keyboard
		key_dest = key_game;
		chat_bufferlen = 0;
		chat_buffer[0] = 0;
		return;
	}

	if (key < 32 || key > 127)
		return;	// non printable

	if (key == K_BACKSPACE)
	{
		if (chat_bufferlen)
		{
			chat_bufferlen--;
			chat_buffer[chat_bufferlen] = 0;
		}
		return;
	}

	if (chat_bufferlen == 31)
		return; // all full

	chat_buffer[chat_bufferlen++] = key;
	chat_buffer[chat_bufferlen] = 0;
}

//============================================================================


/*
===================
Key_StringToKeynum

Returns a key number to be used to index keybindings[] by looking at
the given string.  Single ascii characters return themselves, while
the K_* names are matched up.
===================
*/
int Key_StringToKeynum (char *str)
{
	keyname_t	*kn;

	if (!str || !str[0])
		return -1;
	if (!str[1])
		return str[0];

	for (kn=keynames ; kn->name ; kn++)
	{
		if (!Q_strcasecmp(str,kn->name))
			return kn->keynum;
	}
	return -1;
}

/*
===================
Key_KeynumToString

Returns a string (either a single ascii char, or a K_* name) for the
given keynum.
FIXME: handle quote special (general escape sequence?)
===================
*/
char *Key_KeynumToString (int keynum)
{
	keyname_t	*kn;
	static	char	tinystr[2];

	if (keynum == -1)
		return "<KEY NOT FOUND>";
	if (keynum > 32 && keynum < 127)
	{	// printable ascii
		tinystr[0] = keynum;
		tinystr[1] = 0;
		return tinystr;
	}

	for (kn=keynames ; kn->name ; kn++)
		if (keynum == kn->keynum)
			return kn->name;

	return va("UNKNOWN KEYNUM (%i)", keynum); // edited
}

//=============================================================================
// Manoel Kasimier - function shift - begin
//=============================================================================
// These are modified versions of the binding functions
void Key_SetShiftBinding (int keynum, char *binding)
{
	char	*new;
	int		l;

	if (keynum == -1)
		return;

// free old bindings
	if (shiftbindings[keynum])
	{
		Z_Free (shiftbindings[keynum]);
		shiftbindings[keynum] = NULL;
	}

// allocate memory for new binding
	l = Q_strlen (binding);
	if (!l) return; // Manoel Kasimier - true unbinding
	new = Z_Malloc (l+1);
	Q_strcpy (new, binding);
	new[l] = 0;
	shiftbindings[keynum] = new;
}
void Key_UnbindShift_f (void)
{
	int		b;

	if (Cmd_Argc() != 2)
	{
		Con_Printf ("unbindshift <key> : remove commands from a shifted key\n");
		return;
	}

	b = Key_StringToKeynum (Cmd_Argv(1));
	if (b==-1)
	{
		Con_Printf ("\"%s\" isn't a valid key\n", Cmd_Argv(1));
		return;
	}

	Key_SetShiftBinding (b, "");
}
void Key_UnbindallShifts_f (void)
{
	int		i;
	for (i=0 ; i<K_NUMKEYS ; i++)
		if (shiftbindings[i])
			Key_SetShiftBinding (i, "");
}
void Key_BindShift_f (void)
{
	int			i, c, b;
	char		cmd[1024];

	c = Cmd_Argc();

	if (c == 1)
	{
		Con_Printf ("bindshift <key> [command] : attach a command to a shifted key\n");
		return;
	}
	b = Key_StringToKeynum (Cmd_Argv(1));
	if (b==-1)
	{
		Con_Printf ("\"%s\" isn't a valid key\n", Cmd_Argv(1));
		return;
	}

	if (c == 2)
	{
		if (shiftbindings[b])
			Con_Printf ("\"%s\" = \"%s\"\n", Cmd_Argv(1), shiftbindings[b] );
		else
			Con_Printf ("\"%s\" is not bound\n", Cmd_Argv(1) );
		return;
	}

// copy the rest of the command line
	cmd[0] = 0;		// start out with a null string
	for (i=2 ; i< c ; i++)
	{
		if (i > 2)
			strcat (cmd, " ");
		strcat (cmd, Cmd_Argv(i));
	}

	Key_SetShiftBinding (b, cmd);
}
//=============================================================================
// Manoel Kasimier - function shift - end
//=============================================================================

/*
===================
Key_SetBinding
===================
*/
void Key_SetBinding (int keynum, char *binding)
{
	char	*new;
	int		l;

	if (keynum == -1)
		return;

// free old bindings
	if (keybindings[keynum])
	{
		Z_Free (keybindings[keynum]);
		keybindings[keynum] = NULL;
	}

// allocate memory for new binding
	l = Q_strlen (binding);
	if (!l) return; // Manoel Kasimier - true unbinding
	new = Z_Malloc (l+1);
	Q_strcpy (new, binding);
	new[l] = 0;
	keybindings[keynum] = new;
}

/*
===================
Key_Unbind_f
===================
*/
void Key_Unbind_f (void)
{
	int		b;

	if (Cmd_Argc() != 2)
	{
		Con_Printf ("unbind <key> : remove commands from a key\n");
		return;
	}

	b = Key_StringToKeynum (Cmd_Argv(1));
	if (b==-1)
	{
		Con_Printf ("\"%s\" isn't a valid key\n", Cmd_Argv(1));
		return;
	}

	Key_SetBinding (b, "");
}

void Key_Unbindall_f (void)
{
	int		i;

	for (i=0 ; i<K_NUMKEYS ; i++)
		if (keybindings[i])
			Key_SetBinding (i, "");
}


/*
===================
Key_Bind_f
===================
*/
void Key_Bind_f (void)
{
	int			i, c, b;
	char		cmd[1024];

	c = Cmd_Argc();

//	if (c != 2 && c != 3)
	if (c == 1) // Manoel Kasimier
	{
		Con_Printf ("bind <key> [command] : attach a command to a key\n");
		return;
	}
	b = Key_StringToKeynum (Cmd_Argv(1));
	if (b==-1)
	{
		Con_Printf ("\"%s\" isn't a valid key\n", Cmd_Argv(1));
		return;
	}

	if (c == 2)
	{
		if (keybindings[b])
			Con_Printf ("\"%s\" = \"%s\"\n", Cmd_Argv(1), keybindings[b] );
		else
			Con_Printf ("\"%s\" is not bound\n", Cmd_Argv(1) );
		return;
	}

// copy the rest of the command line
	cmd[0] = 0;		// start out with a null string
	for (i=2 ; i< c ; i++)
	{
		if (i > 2)
			strcat (cmd, " ");
		strcat (cmd, Cmd_Argv(i));
	}

	Key_SetBinding (b, cmd);
}

// Manoel Kasimier - reduced config file - begin
qboolean HaveSemicolon (char *s)
{
	while (*s)
	{
		if (*s == ';')
			return true;
		s++;
	}
	return false;
}
// Manoel Kasimier - reduced config file - end
/*
============
Key_WriteBindings

Writes lines containing "bind key value"
============
*/
void Key_WriteBindings (FILE *f)
{
	int		i;

	for (i=0 ; i<K_NUMKEYS ; i++)
		if (keybindings[i])
			if (*keybindings[i])
			// Manoel Kasimier - reduced config file - begin
			{
				fprintf (f, "bind ");
				if (i == ';')
					fprintf (f, "\";\" ");
				else
					fprintf (f, "%s ", Key_KeynumToString(i));
				if (HaveSemicolon(keybindings[i]))
					fprintf (f, "\"%s\"\n", keybindings[i]);
				else
					fprintf (f, "%s\n", keybindings[i]);
			}
			// Manoel Kasimier - reduced config file - end
	// Manoel Kasimier - function shift - begin
	for (i=0 ; i<K_NUMKEYS ; i++)
		if (shiftbindings[i])
			if (*shiftbindings[i])
			// Manoel Kasimier - reduced config file - begin
			{
				fprintf (f, "bindshift ");
				if (i == ';')
					fprintf (f, "\";\" ");
				else
					fprintf (f, "%s ", Key_KeynumToString(i));
				if (HaveSemicolon(shiftbindings[i]))
					fprintf (f, "\"%s\"\n", shiftbindings[i]);
				else
					fprintf (f, "%s\n", shiftbindings[i]);
			}
			// Manoel Kasimier - reduced config file - end
	// Manoel Kasimier - function shift - end
}


/*
===================
Key_Init
===================
*/
void Key_Init (void)
{
	int		i;

	for (i=0 ; i<32 ; i++)
	{
		key_lines[i][0] = ']';
		key_lines[i][1] = 0;
	}
	key_linepos = 1;

//
// init ascii characters in console mode
//
	for (i=32 ; i<128 ; i++)
		consolekeys[i] = true;
	consolekeys[K_ENTER] = true;
	consolekeys[K_TAB] = true;
	consolekeys[K_LEFTARROW] = true;
	consolekeys[K_RIGHTARROW] = true;
	consolekeys[K_UPARROW] = true;
	consolekeys[K_DOWNARROW] = true;
	consolekeys[K_BACKSPACE] = true;
	consolekeys[K_HOME] = true; // Manoel Kasimier
	consolekeys[K_END] = true; // Manoel Kasimier
	consolekeys[K_PGUP] = true;
	consolekeys[K_PGDN] = true;
	consolekeys[K_SHIFT] = true;
	consolekeys['`'] = false;
	consolekeys['~'] = false;
	// mouse
	consolekeys[K_MWHEELUP] = true;
	consolekeys[K_MWHEELDOWN] = true;
	// Manoel Kasimier - controller functionality to the console - begin
	// joystick 1
	consolekeys[K_JOY1_AXISX_MINUS] = true;
	consolekeys[K_JOY1_AXISX_PLUS] = true;
	consolekeys[K_JOY1_AXISY_MINUS] = true;
	consolekeys[K_JOY1_AXISY_PLUS] = true;
	#ifndef _arch_dreamcast
	consolekeys[K_JOY1_POV_UP] = true;
	consolekeys[K_JOY1_POV_RIGHT] = true;
	consolekeys[K_JOY1_POV_DOWN] = true;
	consolekeys[K_JOY1_POV_LEFT] = true;
	#else
	consolekeys[K_JOY1_DPAD1_LEFT] = true;
	consolekeys[K_JOY1_DPAD1_RIGHT] = true;
	consolekeys[K_JOY1_DPAD1_UP] = true;
	consolekeys[K_JOY1_DPAD1_DOWN] = true;
	consolekeys[K_JOY1_DPAD2_LEFT] = true;
	consolekeys[K_JOY1_DPAD2_RIGHT] = true;
	consolekeys[K_JOY1_DPAD2_UP] = true;
	consolekeys[K_JOY1_DPAD2_DOWN] = true;
	#endif
	consolekeys[K_JOY1_ENTER] = true;
	consolekeys[K_JOY1_USE] = true;
	consolekeys[K_JOY1_BACK] = true;
	consolekeys[K_JOY1_PGDN] = true;
	consolekeys[K_JOY1_PGUP] = true;
	consolekeys[K_JOY1_ESCAPE] = true;
	consolekeys[K_JOY1_BACKSPACE] = true;
    // joystick 2
	consolekeys[K_JOY2_AXISX_MINUS] = true;
	consolekeys[K_JOY2_AXISX_PLUS] = true;
	consolekeys[K_JOY2_AXISY_MINUS] = true;
	consolekeys[K_JOY2_AXISY_PLUS] = true;
	#ifndef _arch_dreamcast
	consolekeys[K_JOY2_POV_UP] = true;
	consolekeys[K_JOY2_POV_RIGHT] = true;
	consolekeys[K_JOY2_POV_DOWN] = true;
	consolekeys[K_JOY2_POV_LEFT] = true;
	#else
	consolekeys[K_JOY2_DPAD1_LEFT] = true;
	consolekeys[K_JOY2_DPAD1_RIGHT] = true;
	consolekeys[K_JOY2_DPAD1_UP] = true;
	consolekeys[K_JOY2_DPAD1_DOWN] = true;
	consolekeys[K_JOY2_DPAD2_LEFT] = true;
	consolekeys[K_JOY2_DPAD2_RIGHT] = true;
	consolekeys[K_JOY2_DPAD2_UP] = true;
	consolekeys[K_JOY2_DPAD2_DOWN] = true;
	#endif
	consolekeys[K_JOY2_ENTER] = true;
	consolekeys[K_JOY2_USE] = true;
	consolekeys[K_JOY2_BACK] = true;
	consolekeys[K_JOY2_PGDN] = true;
	consolekeys[K_JOY2_PGUP] = true;
	consolekeys[K_JOY2_ESCAPE] = true;
	consolekeys[K_JOY2_BACKSPACE] = true;
	// Manoel Kasimier - controller functionality to the console - end

	for (i=0 ; i<K_NUMKEYS ; i++)
		keyshift[i] = i;
	for (i='a' ; i<='z' ; i++)
		keyshift[i] = i - 'a' + 'A';
	keyshift['1'] = '!';
	keyshift['2'] = '@';
	keyshift['3'] = '#';
	keyshift['4'] = '$';
	keyshift['5'] = '%';
	keyshift['6'] = '^';
	keyshift['7'] = '&';
	keyshift['8'] = '*';
	keyshift['9'] = '(';
	keyshift['0'] = ')';
	keyshift['-'] = '_';
	keyshift['='] = '+';
	keyshift[','] = '<';
	keyshift['.'] = '>';
	keyshift['/'] = '?';
	keyshift[';'] = ':';
	keyshift['\''] = '"';
	keyshift['['] = '{';
	keyshift[']'] = '}';
	keyshift['`'] = '~';
	keyshift['\\'] = '|';

	menubound[K_ESCAPE] = true;
	for (i=0 ; i<12 ; i++)
		menubound[K_F1+i] = true;

	// Manoel Kasimier - begin
	for (i=0 ; i<200 ; i++)
		demokeys[i] = true;
	demokeys[K_PAUSE] = true;
	demokeys[K_JOY1_ESCAPE] = true;
	demokeys[K_JOY2_ESCAPE] = true;
	for (i=0 ; i<12 ; i++)
		demokeys[K_F1+i] = false;
	demokeys['`'] = false;
	demokeys['~'] = false;

	key_menurepeats[K_UPARROW] = true;
	key_menurepeats[K_DOWNARROW] = true;
	key_menurepeats[K_LEFTARROW] = true;
	key_menurepeats[K_RIGHTARROW] = true;
	key_menurepeats[K_JOY1_AXISX_MINUS] = true;
	key_menurepeats[K_JOY1_AXISX_PLUS] = true;
	key_menurepeats[K_JOY1_AXISY_MINUS] = true;
	key_menurepeats[K_JOY1_AXISY_PLUS] = true;
	#ifndef _arch_dreamcast
	key_menurepeats[K_JOY1_POV_UP] = true;
	key_menurepeats[K_JOY1_POV_RIGHT] = true;
	key_menurepeats[K_JOY1_POV_DOWN] = true;
	key_menurepeats[K_JOY1_POV_LEFT] = true;
	#else
	key_menurepeats[K_JOY1_DPAD1_LEFT] = true;
	key_menurepeats[K_JOY1_DPAD1_RIGHT] = true;
	key_menurepeats[K_JOY1_DPAD1_UP] = true;
	key_menurepeats[K_JOY1_DPAD1_DOWN] = true;
	key_menurepeats[K_JOY1_DPAD2_LEFT] = true;
	key_menurepeats[K_JOY1_DPAD2_RIGHT] = true;
	key_menurepeats[K_JOY1_DPAD2_UP] = true;
	key_menurepeats[K_JOY1_DPAD2_DOWN] = true;
	#endif
	// Manoel Kasimier - end

//
// register our functions
//
	Cmd_AddCommand ("bind",Key_Bind_f);
	Cmd_AddCommand ("unbind",Key_Unbind_f);
	Cmd_AddCommand ("unbindall",Key_Unbindall_f);


	// Manoel Kasimier - function shift - begin
	Cmd_AddCommand ("bindshift",Key_BindShift_f);
	Cmd_AddCommand ("unbindshift",Key_UnbindShift_f);
	Cmd_AddCommand ("unbindallshifts",Key_UnbindallShifts_f);
	// Manoel Kasimier - function shift - end
}

// Manoel Kasimier - begin
int In_AnalogCommand(int key)
{
	if (shift_function)
	{
		if (!shiftbindings[key]) // if it's pointing to 0000-0000
			return AXIS_NONE;
		// angles
		if (!Q_strcmp(shiftbindings[key], "+lookup"))
			return AXIS_TURN_U;
		if (!Q_strcmp(shiftbindings[key], "+lookdown"))
			return AXIS_TURN_D;
		if (!Q_strcmp(shiftbindings[key], "+left"))
			return AXIS_TURN_L;
		if (!Q_strcmp(shiftbindings[key], "+right"))
			return AXIS_TURN_R;
		// movement
		if (!Q_strcmp(shiftbindings[key], "+forward"))
			return AXIS_MOVE_F;
		if (!Q_strcmp(shiftbindings[key], "+back"))
			return AXIS_MOVE_B;
		if (!Q_strcmp(shiftbindings[key], "+moveup"))
			return AXIS_MOVE_U;
		if (!Q_strcmp(shiftbindings[key], "+movedown"))
			return AXIS_MOVE_D;
		if (!Q_strcmp(shiftbindings[key], "+moveleft"))
			return AXIS_MOVE_L;
		if (!Q_strcmp(shiftbindings[key], "+moveright"))
			return AXIS_MOVE_R;
	}
	else
	{
		if (!keybindings[key]) // if it's pointing to 0000-0000
			return AXIS_NONE;
		// angles
		if (!Q_strcmp(keybindings[key], "+lookup"))
			return AXIS_TURN_U;
		if (!Q_strcmp(keybindings[key], "+lookdown"))
			return AXIS_TURN_D;
		if (!Q_strcmp(keybindings[key], "+left"))
			return AXIS_TURN_L;
		if (!Q_strcmp(keybindings[key], "+right"))
			return AXIS_TURN_R;
		// movement
		if (!Q_strcmp(keybindings[key], "+forward"))
			return AXIS_MOVE_F;
		if (!Q_strcmp(keybindings[key], "+back"))
			return AXIS_MOVE_B;
		if (!Q_strcmp(keybindings[key], "+moveup"))
			return AXIS_MOVE_U;
		if (!Q_strcmp(keybindings[key], "+movedown"))
			return AXIS_MOVE_D;
		if (!Q_strcmp(keybindings[key], "+moveleft"))
			return AXIS_MOVE_L;
		if (!Q_strcmp(keybindings[key], "+moveright"))
			return AXIS_MOVE_R;
	}
	return AXIS_NONE;
}
// Manoel Kasimier - end
/*
===================
Key_Event

Called by the system between frames for both key up and key down events
Should NOT be called during an interrupt!
===================
*/

void Key_Event (int key, qboolean down)
{
	char	*kb;
	char	cmd[1024];

	keydown[key] = down;

	if (!down)
		key_repeats[key] = 0;

	key_lastpress = key;
	key_count++;
	if (key_count <= 0)
	{
		return;		// just catching keys for Con_NotifyBox
	}

// update auto-repeat status
	if (down)
	{
		key_repeats[key]++;
		if (key_repeats[key] > 1) // Manoel Kasimier - edited
		if (!(key_dest == key_menu && key_menurepeats[key])) // Manoel Kasimier
		if (!(key_dest == key_console && consolekeys[key])) // Manoel Kasimier
		{
			return;	// ignore most autorepeats
		}

//		if (key >= 200 && !keybindings[key]) // Manoel Kasimier - removed
//			Con_Printf ("%s is unbound, hit F4 to set.\n", Key_KeynumToString (key) ); // Manoel Kasimier - removed
	}

	if (key == K_SHIFT)
		shift_down = down;

//
// handle escape specialy, so the user can never unbind it
//
	if (key == K_ESCAPE)
	{
		if (!down)
			return;
		switch (key_dest)
		{
		case key_message:
			Key_Message (key);
			break;
		case key_menu:
			M_Keydown (key);
			break;
		case key_game:
		case key_console:
			M_ToggleMenu_f ();
			break;
		default:
			Sys_Error ("Bad key_dest");
		}
		return;
	}
	// Manoel Kasimier - on-screen keyboard - begin
	if (key_dest == key_console)
		if ((((key == K_JOY1_BACK || key == K_JOY2_BACK) && !keyboard_active) || key == K_JOY1_ESCAPE || key == K_JOY2_ESCAPE) && down)
		{
			keyboard_active	= 0;
			M_ToggleMenu_f ();
			return;
		}
	// Manoel Kasimier - on-screen keyboard - end

//
// key up events only generate commands if the game key binding is
// a button command (leading + sign).  These will occur even in console mode,
// to keep the character from continuing an action started before a console
// switch.  Button commands include the kenum as a parameter, so multiple
// downs can be matched with ups
//
	if (!down)
	{
		// Manoel Kasimier - begin
		if (In_AnalogCommand(In_IsAxis(key)))
			return;
		// Manoel Kasimier - end
		// Manoel Kasimier - function shift - begin
		kb = shiftbindings[key];
		if (kb && kb[0] == '+')
		{
			sprintf (cmd, "-%s %i\n", kb+1, key);
			Cbuf_AddText (cmd);
		}
		// Manoel Kasimier - function shift - end
		kb = keybindings[key];
		if (kb && kb[0] == '+')
		{
			sprintf (cmd, "-%s %i\n", kb+1, key);
			Cbuf_AddText (cmd);
		}
		if (keyshift[key] != key)
		{
			kb = keybindings[keyshift[key]];
			if (kb && kb[0] == '+')
			{
				sprintf (cmd, "-%s %i\n", kb+1, key);
				Cbuf_AddText (cmd);
			}
		}
		return;
	}

//
// during demo playback, most keys bring up the main menu
//
	if (cls.demoplayback && down && demokeys[key] && key_dest == key_game) // Manoel Kasimier - edited
	{
		M_ToggleMenu_f ();
		return;
	}
//if (key > 255)
//Con_Printf("key %i: %s, bind:%s\n", key - 255, Key_KeynumToString(key), keybindings[key]);
//
// if not a consolekey, send to the interpreter no matter what mode is
//
    // Manoel Kasimier - begin
	if (In_AnalogCommand(In_IsAxis(key)))
	{
		if (key_dest == key_game)
			return;
	}
	else
    // Manoel Kasimier - end
	if ( (key_dest == key_menu && menubound[key])
	|| (key_dest == key_console && !consolekeys[key])
	|| (key_dest == key_game && ( !con_forcedup || !consolekeys[key] ) ) )
	{
		// Manoel Kasimier - function shift - begin
		if (shift_function && shiftbindings[key])
			kb = shiftbindings[key];
		else
		// Manoel Kasimier - function shift - end
		kb = keybindings[key];
		if (kb)
		{
			if (kb[0] == '+')
			{	// button commands add keynum as a parm
				sprintf (cmd, "%s %i\n", kb, key);
				Cbuf_AddText (cmd);
			}
			else
			{
				Cbuf_AddText (kb);
				Cbuf_AddText ("\n");
			}
		}
        // Manoel Kasimier - begin
        // if the start button isn't bound to any command, toggle the menu
		else if (key == K_JOY1_ESCAPE || key == K_JOY2_ESCAPE)
		{
			Cbuf_AddText ("togglemenu");
			Cbuf_AddText ("\n");
		}
        // Manoel Kasimier - end
		return;
	}

	// Manoel Kasimier - function shift - begin
	if (key_dest == key_menu)
		if (keybindings[key] && !Q_strcmp(keybindings[key], "+shift"))
			Cbuf_AddText (va("+shift %i\n", key)); // hack for the "customize controls" menu
	// Manoel Kasimier - function shift - end

//	if (!down)	// Manoel Kasimier - removed - there's a return earlier on this function
//		return;		// other systems only care about key down events

	if (shift_down)
	{
		key = keyshift[key];
	}

	switch (key_dest)
	{
	case key_message:
		Key_Message (key);
		break;

	case key_menu:
		M_Keydown (key);
		break;

	case key_game:
	case key_console:
		Key_Console (key);
		break;

	default:
		Sys_Error ("Bad key_dest");
	}
}


/*
===================
Key_ClearStates
===================
*/
void Key_ClearStates (void)
{
	int		i;

	for (i=0 ; i<K_NUMKEYS ; i++)
	{
		keydown[i] = false;
		key_repeats[i] = 0;
	}
}

