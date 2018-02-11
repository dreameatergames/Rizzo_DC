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

int In_AnalogCommand(int key); // Manoel Kasimier
int In_IsAxis(int key); // Manoel Kasimier

// game-input axes
#define AXIS_NONE	0
#define	AXIS_TURN_U	1	// lookup
#define	AXIS_TURN_D	2	// lookdown
#define	AXIS_TURN_L	3	// left
#define	AXIS_TURN_R	4	// right
#define	AXIS_MOVE_U	5	// moveup
#define	AXIS_MOVE_D	6	// movedown
#define	AXIS_MOVE_L	7	// moveleft
#define	AXIS_MOVE_R	8	// moveright
#define	AXIS_MOVE_F	9	// forward
#define	AXIS_MOVE_B	10	// back

//
// these are the key numbers that should be passed to Key_Event
//
enum
{
    K_NONE=0
    // keyboard
	, K_TAB=9
	, K_ENTER=13
	, K_ESCAPE=27
	, K_SPACE=32

    // normal keys should be passed as lowercased ascii

	, K_BACKSPACE=127
	, K_UPARROW
	, K_DOWNARROW
	, K_LEFTARROW
	, K_RIGHTARROW
	, K_ALT
	, K_CTRL
	, K_SHIFT
	, K_F1
	, K_F2
	, K_F3
	, K_F4
	, K_F5
	, K_F6
	, K_F7
	, K_F8
	, K_F9
	, K_F10
	, K_F11
	, K_F12
	, K_INS
	, K_DEL
	, K_PGDN
	, K_PGUP
	, K_HOME
	, K_END

    , K_PAUSE=255
    // mouse
    , K_MOUSEUP
    , K_MOUSEDOWN
    , K_MOUSELEFT
    , K_MOUSERIGHT
    , K_MOUSE1
    , K_MOUSE2
    , K_MOUSE3
    , K_MOUSE4
    , K_MOUSE5
    , K_MWHEELUP
    , K_MWHEELDOWN
    // joystick 1
    , K_JOY1_AXISX_MINUS // left
    , K_JOY1_AXISX_PLUS
    , K_JOY1_AXISY_MINUS // up
    , K_JOY1_AXISY_PLUS
    , K_JOY1_AXISZ_MINUS // vertical
    , K_JOY1_AXISZ_PLUS
    , K_JOY1_AXISR_MINUS // horizontal
    , K_JOY1_AXISR_PLUS
    #ifndef _arch_dreamcast
    , K_JOY1_AXISU_MINUS
    , K_JOY1_AXISU_PLUS
    , K_JOY1_AXISV_MINUS
    , K_JOY1_AXISV_PLUS
    , K_JOY1_POV_UP
    , K_JOY1_POV_RIGHT
    , K_JOY1_POV_DOWN
    , K_JOY1_POV_LEFT
    , K_JOY1_1
    , K_JOY1_2
    , K_JOY1_3
    , K_JOY1_4
    , K_JOY1_5
    , K_JOY1_6
    , K_JOY1_7
    , K_JOY1_8
    , K_JOY1_9
    , K_JOY1_10
    , K_JOY1_11
    , K_JOY1_12
    , K_JOY1_13
    , K_JOY1_14
    , K_JOY1_15
    , K_JOY1_16
    , K_JOY1_17
    , K_JOY1_18
    , K_JOY1_19
    , K_JOY1_20
    , K_JOY1_21
    , K_JOY1_22
    , K_JOY1_23
    , K_JOY1_24
    , K_JOY1_25
    , K_JOY1_26
    , K_JOY1_27
    , K_JOY1_28
    , K_JOY1_29
    , K_JOY1_30
    , K_JOY1_31
    , K_JOY1_32
    #else
    // Dreamcast Controller 1
    , K_JOY1_DPAD1_LEFT
    , K_JOY1_DPAD1_RIGHT
    , K_JOY1_DPAD1_UP
    , K_JOY1_DPAD1_DOWN
    , K_JOY1_DPAD2_LEFT
    , K_JOY1_DPAD2_RIGHT
    , K_JOY1_DPAD2_UP
    , K_JOY1_DPAD2_DOWN
	, K_JOY1_A
	, K_JOY1_B
	, K_JOY1_C
	, K_JOY1_D
	, K_JOY1_X
	, K_JOY1_Y
	, K_JOY1_Z
	, K_JOY1_START
	, K_JOY1_TRIGL
	, K_JOY1_TRIGR
    #endif
    // joystick 2
    , K_JOY2_AXISX_MINUS
    , K_JOY2_AXISX_PLUS
    , K_JOY2_AXISY_MINUS
    , K_JOY2_AXISY_PLUS
    , K_JOY2_AXISZ_MINUS
    , K_JOY2_AXISZ_PLUS
    , K_JOY2_AXISR_MINUS
    , K_JOY2_AXISR_PLUS
    #ifndef _arch_dreamcast
    , K_JOY2_AXISU_MINUS
    , K_JOY2_AXISU_PLUS
    , K_JOY2_AXISV_MINUS
    , K_JOY2_AXISV_PLUS
    , K_JOY2_POV_UP
    , K_JOY2_POV_RIGHT
    , K_JOY2_POV_DOWN
    , K_JOY2_POV_LEFT
    , K_JOY2_1
    , K_JOY2_2
    , K_JOY2_3
    , K_JOY2_4
    , K_JOY2_5
    , K_JOY2_6
    , K_JOY2_7
    , K_JOY2_8
    , K_JOY2_9
    , K_JOY2_10
    , K_JOY2_11
    , K_JOY2_12
    , K_JOY2_13
    , K_JOY2_14
    , K_JOY2_15
    , K_JOY2_16
    , K_JOY2_17
    , K_JOY2_18
    , K_JOY2_19
    , K_JOY2_20
    , K_JOY2_21
    , K_JOY2_22
    , K_JOY2_23
    , K_JOY2_24
    , K_JOY2_25
    , K_JOY2_26
    , K_JOY2_27
    , K_JOY2_28
    , K_JOY2_29
    , K_JOY2_30
    , K_JOY2_31
    , K_JOY2_32
    #else
    // Dreamcast controller 2
    , K_JOY2_DPAD1_LEFT
    , K_JOY2_DPAD1_RIGHT
    , K_JOY2_DPAD1_UP
    , K_JOY2_DPAD1_DOWN
    , K_JOY2_DPAD2_LEFT
    , K_JOY2_DPAD2_RIGHT
    , K_JOY2_DPAD2_UP
    , K_JOY2_DPAD2_DOWN
	, K_JOY2_A
	, K_JOY2_B
	, K_JOY2_C
	, K_JOY2_D
	, K_JOY2_X
	, K_JOY2_Y
	, K_JOY2_Z
	, K_JOY2_START
	, K_JOY2_TRIGL
	, K_JOY2_TRIGR
    #endif

    , K_NUMKEYS
} in_keys;

// Manoel Kasimier - controllers' buttons for menu input - begin
#ifdef _arch_dreamcast
// joystick 1
#define K_JOY1_ENTER    K_JOY1_Y
#define K_JOY1_YES      K_JOY1_Y
#define K_JOY1_NO       K_JOY1_B
#define K_JOY1_USE      K_JOY1_A
#define K_JOY1_BACK     K_JOY1_B
#define K_JOY1_PGUP     K_JOY1_TRIGR
#define K_JOY1_PGDN     K_JOY1_TRIGL
#define K_JOY1_ESCAPE   K_JOY1_START
#define K_JOY1_BACKSPACE    K_JOY1_X
// joystick 2
#define K_JOY2_ENTER    K_JOY2_Y
#define K_JOY2_YES      K_JOY2_Y
#define K_JOY2_NO       K_JOY2_B
#define K_JOY2_USE      K_JOY2_A
#define K_JOY2_BACK     K_JOY2_B
#define K_JOY2_PGUP     K_JOY2_TRIGR
#define K_JOY2_PGDN     K_JOY2_TRIGL
#define K_JOY2_ESCAPE   K_JOY2_START
#define K_JOY2_BACKSPACE    K_JOY2_X
#else
// joystick 1
#define K_JOY1_ENTER    K_JOY1_3
#define K_JOY1_YES      K_JOY1_3
#define K_JOY1_NO       K_JOY1_2
#define K_JOY1_USE      K_JOY1_1
#define K_JOY1_BACK     K_JOY1_2
#define K_JOY1_PGUP     K_JOY1_8
#define K_JOY1_PGDN     K_JOY1_7
#define K_JOY1_ESCAPE   K_JOY1_10
#define K_JOY1_BACKSPACE    K_JOY1_4
// joystick 2
#define K_JOY2_ENTER    K_JOY2_3
#define K_JOY2_YES      K_JOY2_3
#define K_JOY2_NO       K_JOY2_2
#define K_JOY2_USE      K_JOY2_1
#define K_JOY2_BACK     K_JOY2_2
#define K_JOY2_PGUP     K_JOY2_8
#define K_JOY2_PGDN     K_JOY2_7
#define K_JOY2_ESCAPE   K_JOY2_10
#define K_JOY2_BACKSPACE    K_JOY2_4
#endif
// Manoel Kasimier - controllers' buttons for menu input - end

typedef enum {key_game, key_console, key_message, key_menu} keydest_t;

extern qboolean shift_function; // Manoel Kasimier - function shift
extern keydest_t	key_dest;
extern char *keybindings[K_NUMKEYS]; //256
extern char *shiftbindings[K_NUMKEYS]; //256 // Manoel Kasimier - function shift
extern	int		key_repeats[K_NUMKEYS]; //256
extern	int		key_count;			// incremented every key event
extern	int		key_lastpress;

void Key_Event (int key, qboolean down);
void Key_Init (void);
void Key_WriteBindings (FILE *f);
void Key_SetBinding (int keynum, char *binding);
void Key_SetShiftBinding (int keynum, char *binding); // Manoel Kasimier - function shift
void Key_ClearStates (void);
int Key_StringToKeynum (char *str); // Manoel Kasimier

// Manoel Kasimier - on-screen keyboard - begin
int M_OnScreenKeyboard_Key (int key);
void M_OnScreenKeyboard_Reset (void);
// Manoel Kasimier - on-screen keyboard - end
