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

// SDL Input drivers

#include "quakedef.h"
#include <SDL.h>

// Toggle fullscreen when ALT+ENTER is pressed
void VID_ToggleFullscreen();

// ---------------------------------------------------------
// SDL stuff
// ---------------------------------------------------------
#define MAX_BUTTONS		5
static SDL_Joystick *dcpad;				// SDL controller struct
static int dcbuttons[MAX_BUTTONS];		// Last state of buttons
static int dchat;						// Last state of hat0

// ---------------------------------------------------------
// Axis mapping controls
// ---------------------------------------------------------
cvar_t	axis_x_function =	{"dc_axisx_function", "1", true};
cvar_t	axis_y_function =	{"dc_axisy_function", "4", true};
cvar_t	axis_l_function =	{"dc_axisl_function", "0", true};
cvar_t	axis_r_function =	{"dc_axisr_function", "0", true};
cvar_t	axis_x2_function =	{"dc_axisx2_function", "2", true};
cvar_t	axis_y2_function =	{"dc_axisy2_function", "3", true};

cvar_t	axis_x_scale =		{"dc_axisx_scale", "1", true};
cvar_t	axis_y_scale =		{"dc_axisy_scale", "1", true};
cvar_t	axis_l_scale =		{"dc_axisl_scale", "1", true};
cvar_t	axis_r_scale =		{"dc_axisr_scale", "1", true};
cvar_t	axis_x2_scale =		{"dc_axisx2_scale", "1", true};
cvar_t	axis_y2_scale =		{"dc_axisy2_scale", "1", true};

cvar_t	axis_pitch_dz =		{"dc_pitch_threshold", "0.15", true};
cvar_t	axis_yaw_dz =		{"dc_yaw_threshold", "0.15", true};
cvar_t	axis_walk_dz =		{"dc_walk_threshold", "0.15", true};
cvar_t	axis_strafe_dz =	{"dc_strafe_threshold", "0.15", true};

#define AXIS_NONE	'0'
#define	AXIS_TURN	'1'
#define	AXIS_WALK	'2'
#define	AXIS_STRAFE	'3'
#define	AXIS_LOOK	'4'

// ---------------------------------------------------------
// Update the controller, and parse buttons
// Call this from IN_Commands
// Button mapping is not the same as nxQuake/QuakeDC
// ---------------------------------------------------------
#define JOY_PARSEHAT(hatid, keyid) \
	if(changed & hatid) \
	{ \
		if(hat & hatid) \
			Key_Event(keyid, 1); \
		else \
			Key_Event(keyid, 0); \
	} \

void Joy_UpdateButtons()
{
	// Buttons to map DC buttons to
	static int qbuttons[MAX_BUTTONS] =
	{
		K_DC_A, K_DC_B, K_DC_X, K_DC_Y, K_DC_START
	};
	int i, hat, changed;

	if(!dcpad)
		return;
		
	// Update the controller status
	SDL_JoystickUpdate();

	// Parse the buttons
	for(i = 0; i < MAX_BUTTONS; i++)
	{
		if(dcbuttons[i] != SDL_JoystickGetButton(dcpad, i))
		{
			dcbuttons[i] = SDL_JoystickGetButton(dcpad, i);
			Key_Event(qbuttons[i], dcbuttons[i]);
		}
	}

	// Parse the POV Hat
	hat = SDL_JoystickGetHat(dcpad, 0);
	changed = dchat ^ hat;
	JOY_PARSEHAT(SDL_HAT_UP, K_UPARROW);
	JOY_PARSEHAT(SDL_HAT_DOWN, K_DOWNARROW);
	JOY_PARSEHAT(SDL_HAT_LEFT, K_LEFTARROW);
	JOY_PARSEHAT(SDL_HAT_RIGHT, K_RIGHTARROW);
	dchat = hat;
}

// ---------------------------------------------------------
// Updates a single axis
// Copied directly from nxQuake. You didn't think I was
// going to rewrite the axis mapping code again, did you?
// ---------------------------------------------------------
static void Joy_UpdateAxis(usercmd_t *cmd, char mode, float scale, int rawvalue)
{
	float value;
	float svalue;
	float speed, aspeed;

	// Don't bother if it's switched off
	if(mode == AXIS_NONE)
		return;

	// Convert value from -32768...32768 to -1...1, multiply by scale
	value = (rawvalue / 32768.0);
	svalue = value * scale;

	// Handle +speed
	if (in_speed.state & 1)
		speed = cl_movespeedkey.value;
	else
		speed = 1;
	aspeed = speed * host_frametime;

	switch(mode)
	{
		// Turning
		case AXIS_TURN:
			if(fabs(value) > axis_yaw_dz.value)
				cl.viewangles[YAW] -= svalue  * aspeed * cl_yawspeed.value;
			break;
		
		// Walking
		case AXIS_WALK:
			if(fabs(value) > axis_walk_dz.value)
				cmd->forwardmove -= svalue * speed * cl_forwardspeed.value;
			break;

		// Strafing
		case AXIS_STRAFE:
			if(fabs(value) > axis_strafe_dz.value)
				cmd->sidemove -= svalue * speed * cl_sidespeed.value;
			break;

		// Looking
		case AXIS_LOOK:
			if(fabs(value) > axis_pitch_dz.value)
			{
				cl.viewangles[PITCH] += svalue * aspeed * cl_pitchspeed.value;
				V_StopPitchDrift();
			}
			else if(lookspring.value == 0.0)
				V_StopPitchDrift();
			break;
	}

	// bounds check pitch
	if (cl.viewangles[PITCH] > 80.0)
		cl.viewangles[PITCH] = 80.0;
	if (cl.viewangles[PITCH] < -70.0)
		cl.viewangles[PITCH] = -70.0;
}

// ---------------------------------------------------------
// Updates all axes
// Call this from IN_Move
// Joy_UpdateButtons already updated the SDL stick
// Adapted from nxQuake
// ---------------------------------------------------------
void Joy_UpdateAxes(usercmd_t *cmd)
{
	if(!dcpad)
		return;

	Joy_UpdateAxis(cmd, axis_x_function.string[0], axis_x_scale.value, SDL_JoystickGetAxis(dcpad, 0));
	Joy_UpdateAxis(cmd, axis_y_function.string[0], axis_y_scale.value, SDL_JoystickGetAxis(dcpad, 1));
	Joy_UpdateAxis(cmd, axis_l_function.string[0], axis_l_scale.value, SDL_JoystickGetAxis(dcpad, 2));
	Joy_UpdateAxis(cmd, axis_r_function.string[0], axis_r_scale.value, SDL_JoystickGetAxis(dcpad, 3));
	Joy_UpdateAxis(cmd, axis_x2_function.string[0], axis_x2_scale.value, SDL_JoystickGetAxis(dcpad, 4));
	Joy_UpdateAxis(cmd, axis_y2_function.string[0], axis_y2_scale.value, SDL_JoystickGetAxis(dcpad, 5));
}

// ---------------------------------------------------------
// Initialise the controller system
// Call this from IN_Init()
// ---------------------------------------------------------
void Joy_Init()
{
	int joy_count, i;

	// Enumerate the controllers
	joy_count = SDL_NumJoysticks();
	Con_Printf("Joy_Init: %i controllers found\n", joy_count);
	for(i = 0; i < joy_count; i++)
		Con_Printf("    %i - %s\n", i + 1, SDL_JoystickName(i));

	// Abort if no controllers found
	if(joy_count < 1)
	{
		Con_Printf("    No controllers\n");
		dcpad = NULL;
		return;
	}

	// Open the first joystick
	dcpad = SDL_JoystickOpen(0);
	if(!dcpad)
	{
		Con_Printf("Joy_Init: Can't open %s\n", SDL_JoystickName(0));
		return;
	}

	// Register cvars
	Cvar_RegisterVariable(&axis_x_function);
	Cvar_RegisterVariable(&axis_y_function);
	Cvar_RegisterVariable(&axis_l_function);
	Cvar_RegisterVariable(&axis_r_function);
	Cvar_RegisterVariable(&axis_x2_function);
	Cvar_RegisterVariable(&axis_y2_function);
	Cvar_RegisterVariable(&axis_x_scale);
	Cvar_RegisterVariable(&axis_y_scale);
	Cvar_RegisterVariable(&axis_l_scale);
	Cvar_RegisterVariable(&axis_r_scale);
	Cvar_RegisterVariable(&axis_x2_scale);
	Cvar_RegisterVariable(&axis_y2_scale);
	Cvar_RegisterVariable(&axis_pitch_dz);
	Cvar_RegisterVariable(&axis_yaw_dz);
	Cvar_RegisterVariable(&axis_walk_dz);
	Cvar_RegisterVariable(&axis_strafe_dz);

	// Clear the last button state struct
	for(i = 0; i < MAX_BUTTONS; i++)
		dcbuttons[i] = 0;
	dchat = SDL_HAT_CENTERED;
}

// ---------------------------------------------------------
// Shutdown the controller system
// Call this from IN_Shutdown()
// ---------------------------------------------------------
void Joy_Shutdown()
{
	if(dcpad)
		SDL_JoystickClose(dcpad);
	dcpad = NULL;
}

// ---------------------------------------------------------
// Handle keyboard s***
// ---------------------------------------------------------
static void Key_Update()
{
	SDL_Event event;
	int realkey = 0;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				Sys_Quit();
				break;

			case SDL_KEYUP:
			case SDL_KEYDOWN:
				// Handle ASCII characters first
				if(event.key.keysym.sym < 0x80)
					realkey = event.key.keysym.sym;

				// Fxx Keys
				else if((event.key.keysym.sym >= SDLK_F1)&&(event.key.keysym.sym <= SDLK_F12))
					realkey = (event.key.keysym.sym - SDLK_F1) + K_F1;

				// Numeric keypad
				else if( (event.key.keysym.sym >= SDLK_KP0) && (event.key.keysym.sym <= SDLK_KP9))
					realkey = (event.key.keysym.sym - SDLK_KP0) + '0';

				// Look for special cases
				switch(event.key.keysym.sym)
				{
					// Coupled to return - hijack for fullscreen
					case SDLK_RETURN:
						if((event.key.keysym.mod & KMOD_ALT)&&(event.key.state))
							VID_ToggleFullscreen();
						break;
					case SDLK_BACKSPACE:
						realkey = K_BACKSPACE;
						break;
					case SDLK_PAUSE:
						realkey = K_PAUSE;
						break;
					case SDLK_UP:
						realkey = K_UPARROW;
						break;
					case SDLK_DOWN:
						realkey = K_DOWNARROW;
						break;
					case SDLK_LEFT:
						realkey = K_LEFTARROW;
						break;
					case SDLK_RIGHT:
						realkey = K_RIGHTARROW;
						break;
					case SDLK_RALT:
					case SDLK_LALT:
						realkey = K_ALT;
						break;
					case SDLK_RCTRL:
					case SDLK_LCTRL:
						realkey = K_CTRL;
						break;
					case SDLK_RSHIFT:
					case SDLK_LSHIFT:
						realkey = K_SHIFT;
						break;
					case SDLK_INSERT:
						realkey = K_INS;
						break;
					case SDLK_DELETE:
						realkey = K_DEL;
						break;
					case SDLK_PAGEDOWN:
						realkey = K_PGDN;
						break;
					case SDLK_PAGEUP:
						realkey = K_PGUP;
						break;
					case SDLK_HOME:
						realkey = K_HOME;
						break;
					case SDLK_END:
						realkey = K_END;
						break;
					case SDLK_KP_PERIOD:
						realkey = '.';
						break;
					case SDLK_KP_DIVIDE:
						realkey = '/';
						break;
					case SDLK_KP_MULTIPLY:
						realkey = '*';
						break;
					case SDLK_KP_MINUS:
						realkey = '-';
						break;
					case SDLK_KP_PLUS:
						realkey = '+';
						break;
					case SDLK_KP_ENTER:
						realkey = K_ENTER;
						break;
					case SDLK_KP_EQUALS:
						realkey = '=';
						break;
					default:
						break;
				}

				// Pass the Quake scancode to the Quake control system
				if(realkey)
					Key_Event(realkey, event.key.state);
				break;
		}
	}
}

// ---------------------------------------------------------
// Update mouse
// ---------------------------------------------------------
static int old_mb = 0;
static void Mouse_Update(usercmd_t *cmd)
{
	int mouse_x, mouse_y;
	int buttons, bi;
	int q_mousekeys[] = {0, K_MOUSE1, K_MOUSE3, K_MOUSE2, K_MWHEELUP, K_MWHEELDOWN};

	buttons = SDL_GetRelativeMouseState(&mouse_x, &mouse_y);

	// Scale axes for sensitivity
	mouse_x *= sensitivity.value;
	mouse_y *= sensitivity.value;

	// Handle X-axis
	if ( (in_strafe.state & 1) || (lookstrafe.value && (in_mlook.state & 1) ))
		// Strafing
		cmd->sidemove += m_side.value * mouse_x;
	else
		// Turning
		cl.viewangles[YAW] -= m_yaw.value * mouse_x;
	
	// Stop pitch drift when MouseLook is enabled
	if (in_mlook.state & 1)
		V_StopPitchDrift ();

	// Handle Y-axis
	if ( (in_mlook.state & 1) && !(in_strafe.state & 1))
	{
		// Mouselook
		cl.viewangles[PITCH] += m_pitch.value * mouse_y;
		if (cl.viewangles[PITCH] > 80)
			cl.viewangles[PITCH] = 80;
		if (cl.viewangles[PITCH] < -70)
			cl.viewangles[PITCH] = -70;
	}
	else
	{
		// Walking
		if ((in_strafe.state & 1) && noclip_anglehack)
			cmd->upmove -= m_forward.value * mouse_y;
		else
			cmd->forwardmove -= m_forward.value * mouse_y;
	}

	// Handle buttons
	for(bi = 1; bi < 6; bi++)
	{
		int mask = SDL_BUTTON(bi);
		if( (buttons & mask) && (!(old_mb & mask)) )
			Key_Event(q_mousekeys[bi], 1);
		else if( (!(buttons & mask)) && (old_mb & mask) )
			Key_Event(q_mousekeys[bi], 0);
	}
	old_mb = buttons;
}

// ---------------------------------------------------------
// Quake input interface
// ---------------------------------------------------------
void IN_Init (void)
{
	Joy_Init();
}

void IN_Shutdown (void)
{
	Joy_Shutdown();
}

void IN_Commands (void)
{
	Joy_UpdateButtons();
}

void IN_Move (usercmd_t *cmd)
{
	Joy_UpdateAxes(cmd);
	Mouse_Update(cmd);
}

void Sys_SendKeyEvents (void)
{
	Key_Update();
}
