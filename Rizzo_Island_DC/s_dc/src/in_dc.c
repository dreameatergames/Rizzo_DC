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
// dc_input.c - Dreamcast/KOS input driver
// By BlackAura - 2003

#include "quakedef.h"
#include <kos.h>
extern cvar_t m_look; // Manoel Kasimier - m_look
// Manoel Kasimier - begin
// just for reference
#define K_CAPSLOCK K_F1-1
#define K_PRINT K_PAUSE-2
#define K_SCROLLOCK K_PAUSE-1
#define K_NUMLOCK K_UPARROW+1

int In_IsAxis(int key)
{
	switch (key)
	{
		case K_JOY1_AXISX_MINUS:
		case K_JOY1_AXISX_PLUS:
		case K_JOY1_AXISY_MINUS:
		case K_JOY1_AXISY_PLUS:
		case K_JOY1_AXISZ_MINUS:
		case K_JOY1_AXISZ_PLUS:
		case K_JOY1_AXISR_MINUS:
		case K_JOY1_AXISR_PLUS:
		case K_JOY1_TRIGL:
		case K_JOY1_TRIGR:

		case K_JOY2_AXISX_MINUS:
		case K_JOY2_AXISX_PLUS:
		case K_JOY2_AXISY_MINUS:
		case K_JOY2_AXISY_PLUS:
		case K_JOY2_AXISZ_MINUS:
		case K_JOY2_AXISZ_PLUS:
		case K_JOY2_AXISR_MINUS:
		case K_JOY2_AXISR_PLUS:
		case K_JOY2_TRIGL:
		case K_JOY2_TRIGR:
			return key;
	}
	return 0;
}
// Manoel Kasimier - end

// =============================================
// Keyboard
// =============================================
const static int q_keytrans[]= {
	/*0*/	  0,   0,   0,   0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
			'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
			'u', 'v', 'w', 'x', 'y', 'z',
	/*1e*/	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	/*28*/	K_ENTER, K_ESCAPE, K_BACKSPACE, K_TAB, K_SPACE, '-', '=', '[', ']', '\\', 0, ';', '\'',
	/*35*/	'`', ',', '.', '/', 0, K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, K_F7, K_F8, K_F9, K_F10, K_F11, K_F12,
	/*46*/	0, 0, K_PAUSE, K_INS, K_HOME, K_PGUP, K_DEL, K_END, K_PGDN, K_RIGHTARROW, K_LEFTARROW, K_DOWNARROW, K_UPARROW,
	// numeric keypad keys
	/*53*/	0, '/', '*', '-', '+', 13, '1', '2', '3', '4', '5', '6',
	/*5f*/	'7', '8', '9', '0', '.', 0
};
#define NUMKEYS	sizeof(q_keytrans)
static int		key_shift;
static uint8	key_matrix[NUMKEYS];
// Manoel Kasimier - begin
static float	key_repeat[NUMKEYS];
static float	joy_repeat[2][16];
#define			KEY_REPEAT_START	0.3
#define			KEY_REPEAT_INTERVAL	0.15
// Manoel Kasimier - end

static void DCKey_Init()
{
	int i;
	for(i = 0; i < NUMKEYS; i++)
		key_repeat[i] = key_matrix[i] = 0; // Manoel Kasimier - edited
	// Manoel Kasimier - begin
	for(i = 0; i < 16; i++)
		joy_repeat[0][i] = joy_repeat[1][i] = 0;
	// Manoel Kasimier - end
	key_shift = 0;
}

static void Key_Update()
{
	int			i, keynum;
	kbd_state_t	*state;				// Current keyboard state
	maple_device_t	*dev;			// Keyboard maple device
	int			new_key_shift = 0;	// Keyboard shift states

	// Find a keyboard
	dev = maple_enum_type(0, MAPLE_FUNC_KEYBOARD);
	if(!dev)
		return;

	// Get keyboard state
	state = (kbd_state_t *)maple_dev_status(dev);
	if(!state)
		return;

	// Scan keys
	for(i = 0; i < NUMKEYS; i++)
	{
		if(key_matrix[i] != state->matrix[i])
		{
			// Convert scancode to Quake key number
			keynum = q_keytrans[i];
			if(keynum)
				Key_Event(keynum, state->matrix[i]);
			key_matrix[i] = state->matrix[i];
			key_repeat[i] = realtime + KEY_REPEAT_START; // Manoel Kasimier
		}
		// Manoel Kasimier - begin
		else if (key_matrix[i] && key_repeat[i] < realtime)
		{
			// Convert scancode to Quake key number
			keynum = q_keytrans[i];
			if(keynum)
				Key_Event(keynum, key_matrix[i]);
			key_repeat[i] = realtime + KEY_REPEAT_INTERVAL;
		}
		// Manoel Kasimier - end
	}

	// Deal with shift keys
	new_key_shift = 0x00;
	if((state->shift_keys & KBD_MOD_LSHIFT)||(state->shift_keys & KBD_MOD_RSHIFT))
		new_key_shift |= 0x01;
	if((state->shift_keys & KBD_MOD_LCTRL)||(state->shift_keys & KBD_MOD_RCTRL))
		new_key_shift |= 0x02;
	if((state->shift_keys & KBD_MOD_LALT)||(state->shift_keys & KBD_MOD_RALT))
		new_key_shift |= 0x04;

	if( (new_key_shift & 0x01) && !(key_shift & 0x01) )
		Key_Event(K_SHIFT, 1);
	if( !(new_key_shift & 0x01) && (key_shift & 0x01) )
		Key_Event(K_SHIFT, 0);

	if( (new_key_shift & 0x02) && !(key_shift & 0x02) )
		Key_Event(K_CTRL, 1);
	if( !(new_key_shift & 0x02) && (key_shift & 0x02) )
		Key_Event(K_CTRL, 0);

	if( (new_key_shift & 0x04) && !(key_shift & 0x04) )
		Key_Event(K_ALT, 1);
	if( !(new_key_shift & 0x04) && (key_shift & 0x04) )
		Key_Event(K_ALT, 0);

	key_shift = new_key_shift;
}

// =============================================
// Mouse
// =============================================
static void Mouse_UpdateButtons()
{
	maple_device_t	*dev;
	mouse_state_t	*state;
	int				i;
	int				q_mousekeys[] = {0, K_MOUSE2, K_MOUSE1, K_MOUSE3}; // Manoel Kasimier - edited
	static int		mouse_oldbuttons = 0;

	// Find a mouse
	dev = maple_enum_type(0, MAPLE_FUNC_MOUSE);
	if(!dev)
		return;

	// Get mouse state
	state = (mouse_state_t *)maple_dev_status(dev);
	if(!state)
		return;

	// Parse squeaker buttons
	for(i = 1; i < 4; i++) // Manoel Kasimier - edited
	{
		int mask = 1 << i;
		if( (state->buttons & mask) && (!(mouse_oldbuttons & mask)) )
			Key_Event(q_mousekeys[i], 1);
		else if( (!(state->buttons & mask)) && (mouse_oldbuttons & mask) )
			Key_Event(q_mousekeys[i], 0);
	}

	// Manoel Kasimier - mouse wheel - begin
#define	MW_UP 	(1<<4)
#define	MW_DOWN	(1<<5)

#if 1
	// faster, better code.
	// there's no need to check if the wheel was already "pressed" (rolling) in any direction
	if (state->dz < 0)
	{
		if (mouse_oldbuttons & MW_DOWN)
			Key_Event(K_MWHEELDOWN, 0);
		Key_Event(K_MWHEELUP, 1);
	}
	else if (state->dz > 0)
	{
		if (mouse_oldbuttons & MW_UP)
			Key_Event(K_MWHEELUP, 0);
		Key_Event(K_MWHEELDOWN, 1);
	}
	else
	{
		if (mouse_oldbuttons & MW_DOWN)
			Key_Event(K_MWHEELDOWN, 0);
		if (mouse_oldbuttons & MW_UP)
			Key_Event(K_MWHEELUP, 0);
	}

#else
	// slower code, won't send the event if the wheel is continuously rolling.
	if (state->dz < 0 && !(mouse_oldbuttons & MW_UP))
		Key_Event(K_MWHEELUP, 1);
	else if (state->dz >= 0 && (mouse_oldbuttons & MW_UP))
		Key_Event(K_MWHEELUP, 0);

	if (state->dz > 0 && !(mouse_oldbuttons & MW_DOWN))
		Key_Event(K_MWHEELDOWN, 1);
	else if (state->dz <= 0 && (mouse_oldbuttons & MW_DOWN))
		Key_Event(K_MWHEELDOWN, 0);
#endif
	// Manoel Kasimier - mouse wheel - end

	mouse_oldbuttons = state->buttons + (state->dz<0)*MW_UP + (state->dz>0)*MW_DOWN; // Manoel Kasimier - edited
}

static void Mouse_UpdateAxes(usercmd_t *cmd)
{
	maple_device_t	*dev;
	mouse_state_t	*state;
	float	mouse_x, mouse_y;

	// Find a mouse
	dev = maple_enum_type(0, MAPLE_FUNC_MOUSE);
	if(!dev)
		return;

	// Get mouse state
	state = (mouse_state_t *)maple_dev_status(dev);
	if(!state)
		return;

	// Scale axes for sensitivity
	mouse_x = (float)state->dx * sensitivity.value;
	mouse_y = (float)state->dy * sensitivity.value;

	// Handle X-axis
	if ( (in_strafe.state & 1) || (lookstrafe.value && m_look.value )) // Manoel Kasimier - m_look - edited
		// Strafing
		cmd->sidemove += m_side.value * mouse_x;
	else
		// Turning
		cl.viewangles[YAW] -= m_yaw.value * mouse_x/fovscale; // Manoel Kasimier - FOV-based scaling - edited

	// Stop pitch drift when MouseLook is enabled
	if (m_look.value) // Manoel Kasimier - m_look
		V_StopPitchDrift ();

	// Handle Y-axis
	if ( m_look.value && !(in_strafe.state & 1)) // Manoel Kasimier - m_look - edited
	{
		// Mouselook
		cl.viewangles[PITCH] += m_pitch.value * mouse_y/fovscale; // Manoel Kasimier - FOV-based scaling - edited
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
}

// =============================================
// Joystick
// =============================================

static void Joy_UpdateButtons()
{

	maple_device_t	*dev;
	cont_state_t	*state;
	int				i;
	static int		joy_oldbuttons = 0;
	static int		old_left = 0, old_right = 0;
	static int		old_x = 0, old_y = 0; // Manoel Kasimier
	static int		old_2x = 0, old_2y = 0; // Manoel Kasimier
	int				q_joykeys[] =
	{
		K_JOY1_C, K_JOY1_B, K_JOY1_A, K_JOY1_START,
		K_JOY1_DPAD1_UP, K_JOY1_DPAD1_DOWN, K_JOY1_DPAD1_LEFT, K_JOY1_DPAD1_RIGHT,
		K_JOY1_Z, K_JOY1_Y, K_JOY1_X, K_JOY1_D,
		K_JOY1_DPAD2_UP, K_JOY1_DPAD2_DOWN, K_JOY1_DPAD2_LEFT, K_JOY1_DPAD2_RIGHT
	};

	// Find a controller
	dev = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
	if(dev) // Manoel Kasimier
	{
		// Get controller state
		state = (cont_state_t *)maple_dev_status(dev);
		if(!state)
			goto joy2; // Manoel Kasimier
		//	return;

		// Parse buttons
		for(i = 0; i < 16; i++)
		{
			int mask = 1 << i;
			// Manoel Kasimier - begin
			if (state->buttons & mask)
			{
				if (!(joy_oldbuttons & mask))
				{
					Key_Event(q_joykeys[i], 1);
					joy_repeat[0][i] = realtime + KEY_REPEAT_START;
				}
				else if (joy_repeat[0][i] < realtime)
				{
					Key_Event(q_joykeys[i], 1);
					joy_repeat[0][i] = realtime + KEY_REPEAT_INTERVAL;
				}
			}
			else if (joy_oldbuttons & mask)
			// Manoel Kasimier - end
				Key_Event(q_joykeys[i], 0);
		}
		joy_oldbuttons = state->buttons;

		// Parse left/right triggers
		if((state->ltrig > 0) && (old_left == 0))
			Key_Event(K_JOY1_TRIGL, 1);
		else if((state->ltrig == 0) && (old_left > 0))
			Key_Event(K_JOY1_TRIGL, 0);
		old_left = state->ltrig;

		if((state->rtrig > 0) && (old_right == 0))
			Key_Event(K_JOY1_TRIGR, 1);
		else if((state->rtrig == 0) && (old_right > 0))
			Key_Event(K_JOY1_TRIGR, 0);
		old_right = state->rtrig;

	// Manoel Kasimier - begin
		// Parse analog sticks
		if (state->joyx >= -32 && old_x < -32)
			Key_Event(K_JOY1_AXISX_MINUS, 0);
		if (state->joyx <=  32 && old_x >  32)
			Key_Event(K_JOY1_AXISX_PLUS, 0);
		if (state->joyx < -32 && old_x >= -32)
			Key_Event(K_JOY1_AXISX_MINUS, 1);
		if (state->joyx >  32 && old_x <=  32)
			Key_Event(K_JOY1_AXISX_PLUS, 1);
		old_x = state->joyx;

		if (state->joyy >= -32 && old_y < -32)
			Key_Event(K_JOY1_AXISY_MINUS, 0);
		if (state->joyy <=  32 && old_y >  32)
			Key_Event(K_JOY1_AXISY_PLUS, 0);
		if (state->joyy < -32 && old_y >= -32)
			Key_Event(K_JOY1_AXISY_MINUS, 1);
		if (state->joyy >  32 && old_y <=  32)
			Key_Event(K_JOY1_AXISY_PLUS, 1);
		old_y = state->joyy;

		if (state->joy2x >= -32 && old_2x < -32)
			Key_Event(K_JOY1_AXISR_MINUS, 0);
		if (state->joy2x <=  32 && old_2x >  32)
			Key_Event(K_JOY1_AXISR_PLUS, 0);
		if (state->joy2x < -32 && old_2x >= -32)
			Key_Event(K_JOY1_AXISR_MINUS, 1);
		if (state->joy2x >  32 && old_2x <=  32)
			Key_Event(K_JOY1_AXISR_PLUS, 1);
		old_2x = state->joy2x;

		if (state->joy2y >= -32 && old_2y < -32)
			Key_Event(K_JOY1_AXISZ_MINUS, 0);
		if (state->joy2y <=  32 && old_2y >  32)
			Key_Event(K_JOY1_AXISZ_PLUS, 0);
		if (state->joy2y < -32 && old_2y >= -32)
			Key_Event(K_JOY1_AXISZ_MINUS, 1);
		if (state->joy2y >  32 && old_2y <=  32)
			Key_Event(K_JOY1_AXISZ_PLUS, 1);
		old_2y = state->joy2y;
	}
joy2:
	dev = maple_enum_type(1, MAPLE_FUNC_CONTROLLER);
	if(dev)
	{
		static int		joy2_oldbuttons = 0;
		static int		old_left2 = 0, old_right2 = 0;
		static int		old_x2 = 0, old_y2 = 0;
		static int		old_2x2 = 0, old_2y2 = 0;
		int				q_joykeys2[] =
		{
			K_JOY2_C, K_JOY2_B, K_JOY2_A, K_JOY2_START,
			K_JOY2_DPAD1_UP, K_JOY2_DPAD1_DOWN, K_JOY2_DPAD1_LEFT, K_JOY2_DPAD1_RIGHT,
			K_JOY2_Z, K_JOY2_Y, K_JOY2_X, K_JOY2_D,
			K_JOY2_DPAD2_UP, K_JOY2_DPAD2_DOWN, K_JOY2_DPAD2_LEFT, K_JOY2_DPAD2_RIGHT
		};

		// Get controller state
		state = (cont_state_t *)maple_dev_status(dev);
		if(!state)
			return;

		// Parse buttons
		for(i = 0; i < 16; i++)
		{
			int mask = 1 << i;
			// Manoel Kasimier - begin
			if (state->buttons & mask)
			{
				if (!(joy2_oldbuttons & mask))
				{
					Key_Event(q_joykeys2[i], 1);
					joy_repeat[1][i] = realtime + KEY_REPEAT_START;
				}
				else if (joy_repeat[1][i] < realtime)
				{
					Key_Event(q_joykeys2[i], 1);
					joy_repeat[1][i] = realtime + KEY_REPEAT_INTERVAL;
				}
			}
			else if (joy2_oldbuttons & mask)
			// Manoel Kasimier - end
				Key_Event(q_joykeys2[i], 0);
		}
		joy2_oldbuttons = state->buttons;

		// Parse left/right triggers
		if((state->ltrig > 0) && (old_left2 == 0))
			Key_Event(K_JOY2_TRIGL, 1);
		else if((state->ltrig == 0) && (old_left2 > 0))
			Key_Event(K_JOY2_TRIGL, 0);
		old_left2 = state->ltrig;

		if((state->rtrig > 0) && (old_right2 == 0))
			Key_Event(K_JOY2_TRIGR, 1);
		else if((state->rtrig == 0) && (old_right2 > 0))
			Key_Event(K_JOY2_TRIGR, 0);
		old_right2 = state->rtrig;

		// Parse analog sticks
		if (state->joyx >= -32 && old_x2 < -32)
			Key_Event(K_JOY2_AXISX_MINUS, 0);
		if (state->joyx <=  32 && old_x2 >  32)
			Key_Event(K_JOY2_AXISX_PLUS, 0);
		if (state->joyx < -32 && old_x2 >= -32)
			Key_Event(K_JOY2_AXISX_MINUS, 1);
		if (state->joyx >  32 && old_x2 <=  32)
			Key_Event(K_JOY2_AXISX_PLUS, 1);
		old_x2 = state->joyx;

		if (state->joyy >= -32 && old_y2 < -32)
			Key_Event(K_JOY2_AXISY_MINUS, 0);
		if (state->joyy <=  32 && old_y2 >  32)
			Key_Event(K_JOY2_AXISY_PLUS, 0);
		if (state->joyy < -32 && old_y2 >= -32)
			Key_Event(K_JOY2_AXISY_MINUS, 1);
		if (state->joyy >  32 && old_y2 <=  32)
			Key_Event(K_JOY2_AXISY_PLUS, 1);
		old_y2 = state->joyy;

		if (state->joy2x >= -32 && old_2x2 < -32)
			Key_Event(K_JOY2_AXISR_MINUS, 0);
		if (state->joy2x <=  32 && old_2x2 >  32)
			Key_Event(K_JOY2_AXISR_PLUS, 0);
		if (state->joy2x < -32 && old_2x2 >= -32)
			Key_Event(K_JOY2_AXISR_MINUS, 1);
		if (state->joy2x >  32 && old_2x2 <=  32)
			Key_Event(K_JOY2_AXISR_PLUS, 1);
		old_2x2 = state->joy2x;

		if (state->joy2y >= -32 && old_2y2 < -32)
			Key_Event(K_JOY2_AXISZ_MINUS, 0);
		if (state->joy2y <=  32 && old_2y2 >  32)
			Key_Event(K_JOY2_AXISZ_PLUS, 0);
		if (state->joy2y < -32 && old_2y2 >= -32)
			Key_Event(K_JOY2_AXISZ_MINUS, 1);
		if (state->joy2y >  32 && old_2y2 <=  32)
			Key_Event(K_JOY2_AXISZ_PLUS, 1);
		old_2y2 = state->joy2y;
	}
	// Manoel Kasimier - end
}

/* // Manoel Kasimier - removed - begin
cvar_t	axis_x_function =	{"dc_axisx_function", "1", true};
cvar_t	axis_y_function =	{"dc_axisy_function", "4", true};
cvar_t	axis_l_function =	{"dc_axisl_function", "0", true};
cvar_t	axis_r_function =	{"dc_axisr_function", "0", true};
cvar_t	axis_x2_function =	{"dc_axisx2_function", "2", true};
cvar_t	axis_y2_function =	{"dc_axisy2_function", "3", true};
*/ // Manoel Kasimier - removed - end

cvar_t	axis_x_scale =		{"dc_axisx_scale", "1", true};
cvar_t	axis_y_scale =		{"dc_axisy_scale", "1", true};
cvar_t	axis_l_scale =		{"dc_axisl_scale", "1", true};
cvar_t	axis_r_scale =		{"dc_axisr_scale", "1", true};
cvar_t	axis_x2_scale =		{"dc_axisx2_scale", "1", true};
cvar_t	axis_y2_scale =		{"dc_axisy2_scale", "1", true};

// Manoel Kasimier - begin
cvar_t	axis_x_threshold =	{"dc_axisx_threshold", "0.05", true};
cvar_t	axis_y_threshold =	{"dc_axisy_threshold", "0.05", true};
cvar_t	axis_l_threshold =	{"dc_axisl_threshold", "0.05", true};
cvar_t	axis_r_threshold =	{"dc_axisr_threshold", "0.05", true};
cvar_t	axis_x2_threshold =	{"dc_axisx2_threshold", "0.05", true};
cvar_t	axis_y2_threshold =	{"dc_axisy2_threshold", "0.05", true};
// Manoel Kasimier - end
/* // Manoel Kasimier - removed - begin
cvar_t	axis_pitch_dz =		{"dc_pitch_threshold", "0.15", true};
cvar_t	axis_yaw_dz =		{"dc_yaw_threshold", "0.15", true};
cvar_t	axis_walk_dz =		{"dc_walk_threshold", "0.15", true};
cvar_t	axis_strafe_dz =	{"dc_strafe_threshold", "0.15", true};

#define AXIS_NONE	'0'
#define	AXIS_TURN	'1'
#define	AXIS_WALK	'2'
#define	AXIS_STRAFE	'3'
#define	AXIS_LOOK	'4'
*/ // Manoel Kasimier - removed - end

static void Joy_UpdateAxis(usercmd_t *cmd, char mode, float scale, int rawvalue, float threshold) // Manoel Kasimier - edited
{
	float value;
	float svalue;
	float speed, aspeed;

	// Don't bother if it's switched off
	if(mode == AXIS_NONE)
		return;

	// Convert value from -128...128 to -1...1, multiply by scale
	value = (rawvalue / 128.0);

	svalue = fabs(value); // Manoel Kasimier
	if (svalue > threshold) // Manoel Kasimier
	{
		// Manoel Kasimier - begin
		if (value < 0)
			value = (svalue - threshold) * (-1.0/(1.0-threshold));
		else
			value = (svalue - threshold) * (1.0/(1.0-threshold));
		// Manoel Kasimier - end
		svalue = value * scale;

		// Handle +speed
		speed = (in_speed.state & 1) ? cl_movespeedkey.value : 1;
// 2001-10-20 TIMESCALE extension by Tomaz/Maddes  start
	//	aspeed = speed * host_frametime;
		aspeed = speed * host_org_frametime;
// 2001-10-20 TIMESCALE extension by Tomaz/Maddes  end
		switch(mode)
		{
			case AXIS_TURN_U:
				cl.viewangles[PITCH] -= svalue * aspeed * cl_pitchspeed.value/fovscale; // Manoel Kasimier - FOV-based scaling - edited
				V_StopPitchDrift();
				break;
			case AXIS_TURN_D:
				cl.viewangles[PITCH] += svalue * aspeed * cl_pitchspeed.value/fovscale; // Manoel Kasimier - FOV-based scaling - edited
				V_StopPitchDrift();
				break;
			case AXIS_TURN_L:
				cl.viewangles[YAW] += svalue  * aspeed * cl_yawspeed.value/fovscale; // Manoel Kasimier - FOV-based scaling - edited
				break;
			case AXIS_TURN_R:
				cl.viewangles[YAW] -= svalue  * aspeed * cl_yawspeed.value/fovscale; // Manoel Kasimier - FOV-based scaling - edited
				break;
			case AXIS_MOVE_B:
				cmd->forwardmove -= svalue * speed * cl_forwardspeed.value;
				break;
			case AXIS_MOVE_F:
				cmd->forwardmove += svalue * speed * cl_forwardspeed.value;
				break;
			case AXIS_MOVE_L:
				cmd->sidemove -= svalue * speed * cl_sidespeed.value;
				break;
			case AXIS_MOVE_R:
				cmd->sidemove += svalue * speed * cl_sidespeed.value;
				break;
			case AXIS_MOVE_U:
				cmd->upmove += svalue * speed * cl_upspeed.value;
				break;
			case AXIS_MOVE_D:
				cmd->upmove -= svalue * speed * cl_upspeed.value;
				break;
		}
	}
	else if ((mode == AXIS_TURN_U || mode == AXIS_TURN_D) && !lookspring.value) // Manoel Kasimier
		V_StopPitchDrift();

	// bounds check pitch
	if (cl.viewangles[PITCH] > 80.0)
		cl.viewangles[PITCH] = 80.0;
	else if (cl.viewangles[PITCH] < -70.0)
		cl.viewangles[PITCH] = -70.0;
}

static void Joy_UpdateAxes(usercmd_t *cmd)
{
	maple_device_t	*dev;
	cont_state_t	*state;

	// Find a controller
	dev = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
	if(!dev)
		goto joy2; // Manoel Kasimier

	// Get controller state
	state = (cont_state_t *)maple_dev_status(dev);
	if(!state)
		goto joy2; // Manoel Kasimier

	// Update all axes
	// Manoel Kasimier - begin
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISY_MINUS),  axis_y_scale.value,  abs(state->joyy) * (state->joyy<0), axis_y_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISY_PLUS),  axis_y_scale.value,  abs(state->joyy) * (state->joyy>0), axis_y_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISX_MINUS),  axis_x_scale.value,  abs(state->joyx) * (state->joyx<0), axis_x_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISX_PLUS),  axis_x_scale.value,  abs(state->joyx) * (state->joyx>0), axis_x_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_TRIGL),  axis_l_scale.value, (state->ltrig >> 1), axis_l_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_TRIGR),  axis_r_scale.value, (state->rtrig >> 1), axis_r_threshold.value);
//	Joy_UpdateAxis(cmd, axis_x2_function.string[0], axis_x2_scale.value, state->joy2x, axis_x2_threshold.value);
//	Joy_UpdateAxis(cmd, axis_y2_function.string[0], axis_y2_scale.value, state->joy2y, axis_y2_threshold.value);
	// Manoel Kasimier - end
/*	// Manoel Kasimier - edited - begin
	Joy_UpdateAxis(cmd, axis_x_function.string[0],  axis_x_scale.value,  state->joyx , axis_x_threshold.value);
	Joy_UpdateAxis(cmd, axis_y_function.string[0],  axis_y_scale.value,  state->joyy , axis_y_threshold.value);
	Joy_UpdateAxis(cmd, axis_l_function.string[0],  axis_l_scale.value, (state->ltrig >> 1), axis_l_threshold.value);
	Joy_UpdateAxis(cmd, axis_r_function.string[0],  axis_r_scale.value, (state->rtrig >> 1), axis_r_threshold.value);
	Joy_UpdateAxis(cmd, axis_x2_function.string[0], axis_x2_scale.value, state->joy2x, axis_x2_threshold.value);
	Joy_UpdateAxis(cmd, axis_y2_function.string[0], axis_y2_scale.value, state->joy2y, axis_y2_threshold.value);
*/	// Manoel Kasimier - edited - end
	// Manoel Kasimier - begin
joy2:
	// Find a controller
	dev = maple_enum_type(1, MAPLE_FUNC_CONTROLLER);
	if(!dev)
		return;

	// Get controller state
	state = (cont_state_t *)maple_dev_status(dev);
	if(!state)
		return;

	// Update all axes
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISY_MINUS),  axis_y_scale.value,  abs(state->joyy) * (state->joyy<0), axis_y_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISY_PLUS),  axis_y_scale.value,  abs(state->joyy) * (state->joyy>0), axis_y_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISX_MINUS),  axis_x_scale.value,  abs(state->joyx) * (state->joyx<0), axis_x_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISX_PLUS),  axis_x_scale.value,  abs(state->joyx) * (state->joyx>0), axis_x_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_TRIGL),  axis_l_scale.value, (state->ltrig >> 1), axis_l_threshold.value);
	Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_TRIGR),  axis_r_scale.value, (state->rtrig >> 1), axis_r_threshold.value);
//	Joy_UpdateAxis(cmd, axis_x2_function.string[0], axis_x2_scale.value, state->joy2x, axis_x2_threshold.value);
//	Joy_UpdateAxis(cmd, axis_y2_function.string[0], axis_y2_scale.value, state->joy2y, axis_y2_threshold.value);
	// Manoel Kasimier - end
}

// Manoel Kasimier - begin
void BoundsCheckThreshold (void)
{
	if (axis_x_threshold.value < 0)
		axis_x_threshold.value = 0;
	if (axis_y_threshold.value < 0)
		axis_y_threshold.value = 0;
	if (axis_x2_threshold.value < 0)
		axis_x2_threshold.value = 0;
	if (axis_y2_threshold.value < 0)
		axis_y2_threshold.value = 0;
	if (axis_l_threshold.value < 0)
		axis_l_threshold.value = 0;
	if (axis_r_threshold.value < 0)
		axis_r_threshold.value = 0;

	if (axis_x_threshold.value > 0.5)
		axis_x_threshold.value = 0.5;
	if (axis_y_threshold.value > 0.5)
		axis_y_threshold.value = 0.5;
	if (axis_x2_threshold.value > 0.5)
		axis_x2_threshold.value = 0.5;
	if (axis_y2_threshold.value > 0.5)
		axis_y2_threshold.value = 0.5;
	if (axis_l_threshold.value > 0.5)
		axis_l_threshold.value = 0.5;
	if (axis_r_threshold.value > 0.5)
		axis_r_threshold.value = 0.5;
}
// Manoel Kasimier - begin

// =============================================
// Quake control interface
// =============================================
void IN_Init (void)
{
	// Register cvars
	/* // Manoel Kasimier - removed - begin
	Cvar_RegisterVariable(&axis_x_function);
	Cvar_RegisterVariable(&axis_y_function);
	Cvar_RegisterVariable(&axis_l_function);
	Cvar_RegisterVariable(&axis_r_function);
	Cvar_RegisterVariable(&axis_x2_function);
	Cvar_RegisterVariable(&axis_y2_function);
	*/ // Manoel Kasimier - removed - end
	Cvar_RegisterVariable(&axis_x_scale);
	Cvar_RegisterVariable(&axis_y_scale);
	Cvar_RegisterVariable(&axis_l_scale);
	Cvar_RegisterVariable(&axis_r_scale);
	Cvar_RegisterVariable(&axis_x2_scale);
	Cvar_RegisterVariable(&axis_y2_scale);
	// Manoel Kasimier - begin
	Cvar_RegisterVariableWithCallback(&axis_x_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&axis_y_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&axis_l_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&axis_r_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&axis_x2_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&axis_y2_threshold, BoundsCheckThreshold);
	// Manoel Kasimier - end
	/* // Manoel Kasimier - removed - begin
	Cvar_RegisterVariable(&axis_pitch_dz);
	Cvar_RegisterVariable(&axis_yaw_dz);
	Cvar_RegisterVariable(&axis_walk_dz);
	Cvar_RegisterVariable(&axis_strafe_dz);
	*/ // Manoel Kasimier - removed - end

	DCKey_Init();
}

void IN_Shutdown (void)
{
}

void IN_Commands (void)
{
	Joy_UpdateButtons();
	Mouse_UpdateButtons();
}

void IN_Move (usercmd_t *cmd)
{
	Joy_UpdateAxes(cmd);
	Mouse_UpdateAxes(cmd);
}

void Sys_SendKeyEvents (void)
{
	Key_Update();
}
