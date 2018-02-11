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
// in_win.c -- windows 95 mouse and joystick code
// 02/21/97 JCB Added extended DirectInput code to support external controllers.

#include <dinput.h>
#include "quakedef.h"
#include "winquake.h"
#include "dosisms.h"

// Manoel Kasimier - begin
void Vibration_Update (void)
{
}
void Vibration_Stop (int player) // 0=player1, 1=player2
{
}
// Manoel Kasimier - end

#define DINPUT_BUFFERSIZE           16
#define iDirectInputCreate(a,b,c,d)	pDirectInputCreate(a,b,c,d)

HRESULT (WINAPI *pDirectInputCreate)(HINSTANCE hinst, DWORD dwVersion,
	LPDIRECTINPUT * lplpDirectInput, LPUNKNOWN punkOuter);

// mouse variables
cvar_t	m_filter = {"m_filter","0"};

int			mouse_buttons;
int			mouse_oldbuttonstate;
POINT		current_pos;
int			mouse_x, mouse_y, old_mouse_x, old_mouse_y, mx_accum, my_accum;

static qboolean	restore_spi;
static int		originalmouseparms[3], newmouseparms[3] = {0, 0, 1};

unsigned int uiWheelMessage;
qboolean	mouseactive;
qboolean		mouseinitialized;
static qboolean	mouseparmsvalid, mouseactivatetoggle;
static qboolean	mouseshowtoggle = 1;
static qboolean	dinput_acquired;

static unsigned int		mstate_di;

/*
==============================================================================

                        JOYSTICK INTERFACE - BEGIN

==============================================================================
*/
// joystick defines and variables
// where should defines be moved?
#define JOY_ABSOLUTE_AXIS	0x00000000		// control like a joystick
#define JOY_RELATIVE_AXIS	0x00000010		// control like a mouse, spinner, trackball
#define	JOY_MAX_AXES		6				// X, Y, Z, R, U, V
#define JOY_AXIS_X			0
#define JOY_AXIS_Y			1
#define JOY_AXIS_Z			2
#define JOY_AXIS_R			3
#define JOY_AXIS_U			4
#define JOY_AXIS_V			5

enum _ControlList
{
	AxisNada = 0, AxisForward, AxisLook, AxisSide, AxisTurn
};

DWORD	dwAxisFlags[JOY_MAX_AXES] =
{
	JOY_RETURNX, JOY_RETURNY, JOY_RETURNZ, JOY_RETURNR, JOY_RETURNU, JOY_RETURNV
};

// none of these cvars are saved over a session
// this means that advanced controller configuration needs to be executed
// each time.  this avoids any problems with getting back to a default usage
// or when changing from one controller to another.  this way at least something
// works.
cvar_t	in_joystick = {"joystick","0", true};
cvar_t	joy_name = {"joyname", "joystick"};
cvar_t	joy_advanced = {"joyadvanced", "0"};
cvar_t	joy_advaxisx = {"joyadvaxisx", "0"};
cvar_t	joy_advaxisy = {"joyadvaxisy", "0"};
cvar_t	joy_advaxisz = {"joyadvaxisz", "0"};
cvar_t	joy_advaxisr = {"joyadvaxisr", "0"};
cvar_t	joy_advaxisu = {"joyadvaxisu", "0"};
cvar_t	joy_advaxisv = {"joyadvaxisv", "0"};
cvar_t	joy_forwardthreshold = {"joyforwardthreshold", "0.15"};
cvar_t	joy_sidethreshold = {"joysidethreshold", "0.15"};
cvar_t	joy_pitchthreshold = {"joypitchthreshold", "0.15"};
cvar_t	joy_yawthreshold = {"joyyawthreshold", "0.15"};
cvar_t	joy_forwardsensitivity = {"joyforwardsensitivity", "-1.0"};
cvar_t	joy_sidesensitivity = {"joysidesensitivity", "-1.0"};
cvar_t	joy_pitchsensitivity = {"joypitchsensitivity", "1.0"};
cvar_t	joy_yawsensitivity = {"joyyawsensitivity", "-1.0"};
cvar_t	joy_wwhack1 = {"joywwhack1", "0.0"};
cvar_t	joy_wwhack2 = {"joywwhack2", "0.0"};

// de cada controle, assim dá pra definir qual controle cada player usa
/*
// TODO (Manoel#1#): adicionar um comando "joylist" para exibir os índices dos joysticks disponíveis,\
ou exibir os índices no menu de opções de controles,\
para se poder assinalar um índice a cada controle.
*/
// TODO (Manoel#1#):
/*os eixos dos controles devem ficar desativados por padrão.
o usuário deve ativar cada um dos eixos que deseja usar
através do menu de configuração de controles.*/
// TODO (Manoel#1#): usar cvars "joystick" e "joystick2" para armazenar o índice (ou a ordem)
// TODO (Manoel#9#): individualizar todas estas variáveis para cada controle

cvar_t	joy1_x_scale =		{"joy1_axisx_scale", "1", true};
cvar_t	joy1_y_scale =		{"joy1_axisy_scale", "1", true};
cvar_t	joy1_z_scale =		{"joy1_axisz_scale", "1", true};
cvar_t	joy1_r_scale =		{"joy1_axisr_scale", "1", true};
cvar_t	joy1_u_scale =		{"joy1_axisu_scale", "1", true};
cvar_t	joy1_v_scale =		{"joy1_axisv_scale", "1", true};
cvar_t	joy1_x_threshold =	{"joy1_axisx_threshold", "0.05", true};
cvar_t	joy1_y_threshold =	{"joy1_axisy_threshold", "0.05", true};
cvar_t	joy1_z_threshold =	{"joy1_axisz_threshold", "0.05", true};
cvar_t	joy1_r_threshold =	{"joy1_axisr_threshold", "0.05", true};
cvar_t	joy1_u_threshold =	{"joy1_axisu_threshold", "0.05", true};
cvar_t	joy1_v_threshold =	{"joy1_axisv_threshold", "0.05", true};

cvar_t	joy2_x_scale =		{"joy2_axisx_scale", "1", true};
cvar_t	joy2_y_scale =		{"joy2_axisy_scale", "1", true};
cvar_t	joy2_z_scale =		{"joy2_axisz_scale", "1", true};
cvar_t	joy2_r_scale =		{"joy2_axisr_scale", "1", true};
cvar_t	joy2_u_scale =		{"joy2_axisu_scale", "1", true};
cvar_t	joy2_v_scale =		{"joy2_axisv_scale", "1", true};
cvar_t	joy2_x_threshold =	{"joy2_axisx_threshold", "0.05", true};
cvar_t	joy2_y_threshold =	{"joy2_axisy_threshold", "0.05", true};
cvar_t	joy2_z_threshold =	{"joy2_axisz_threshold", "0.05", true};
cvar_t	joy2_r_threshold =	{"joy2_axisr_threshold", "0.05", true};
cvar_t	joy2_u_threshold =	{"joy2_axisu_threshold", "0.05", true};
cvar_t	joy2_v_threshold =	{"joy2_axisv_threshold", "0.05", true};

void BoundsCheckThreshold (void)
{
	if (joy1_x_threshold.value < 0)
		joy1_x_threshold.value = 0;
	if (joy1_y_threshold.value < 0)
		joy1_y_threshold.value = 0;
	if (joy1_z_threshold.value < 0)
		joy1_z_threshold.value = 0;
	if (joy1_r_threshold.value < 0)
		joy1_r_threshold.value = 0;
	if (joy1_u_threshold.value < 0)
		joy1_u_threshold.value = 0;
	if (joy1_v_threshold.value < 0)
		joy1_v_threshold.value = 0;

	if (joy1_x_threshold.value > 0.5)
		joy1_x_threshold.value = 0.5;
	if (joy1_y_threshold.value > 0.5)
		joy1_y_threshold.value = 0.5;
	if (joy1_z_threshold.value > 0.5)
		joy1_z_threshold.value = 0.5;
	if (joy1_r_threshold.value > 0.5)
		joy1_r_threshold.value = 0.5;
	if (joy1_u_threshold.value > 0.5)
		joy1_u_threshold.value = 0.5;
	if (joy1_v_threshold.value > 0.5)
		joy1_v_threshold.value = 0.5;

	if (joy2_x_threshold.value < 0)
		joy2_x_threshold.value = 0;
	if (joy2_y_threshold.value < 0)
		joy2_y_threshold.value = 0;
	if (joy2_z_threshold.value < 0)
		joy2_z_threshold.value = 0;
	if (joy2_r_threshold.value < 0)
		joy2_r_threshold.value = 0;
	if (joy2_u_threshold.value < 0)
		joy2_u_threshold.value = 0;
	if (joy2_v_threshold.value < 0)
		joy2_v_threshold.value = 0;

	if (joy2_x_threshold.value > 0.5)
		joy2_x_threshold.value = 0.5;
	if (joy2_y_threshold.value > 0.5)
		joy2_y_threshold.value = 0.5;
	if (joy2_z_threshold.value > 0.5)
		joy2_z_threshold.value = 0.5;
	if (joy2_r_threshold.value > 0.5)
		joy2_r_threshold.value = 0.5;
	if (joy2_u_threshold.value > 0.5)
		joy2_u_threshold.value = 0.5;
	if (joy2_v_threshold.value > 0.5)
		joy2_v_threshold.value = 0.5;
}

typedef struct joystick_s // Manoel Kasimier
{
//    DWORD	dwAxisMap[JOY_MAX_AXES];
//    DWORD	dwControlMap[JOY_MAX_AXES];
    PDWORD	pdwRawValue[JOY_MAX_AXES];

    qboolean	joy_avail, joy_advancedinit, joy_haspov;
    DWORD		joy_oldbuttonstate, joy_oldpovstate;

    int			joy_id;
    DWORD		joy_flags;
    DWORD		joy_numbuttons;

    float       old_x, old_y, old_z, old_r, old_u, old_v; // Manoel Kasimier

    JOYINFOEX	ji;
} joystick_t; // Manoel Kasimier

joystick_t joystick[2]; // Manoel Kasimier

// forward-referenced functions
void IN_StartupJoystick (void);
void Joy_AdvancedUpdate_f (void);
void IN_JoyMove (usercmd_t *cmd);
/*
==============================================================================

                        JOYSTICK INTERFACE - END

==============================================================================
*/
extern cvar_t m_look; // Manoel Kasimier - m_look

static LPDIRECTINPUT		g_pdi;
static LPDIRECTINPUTDEVICE	g_pMouse;

static HINSTANCE hInstDI;

static qboolean	dinput;

typedef struct MYDATA {
	LONG  lX;                   // X axis goes here
	LONG  lY;                   // Y axis goes here
	LONG  lZ;                   // Z axis goes here
	BYTE  bButtonA;             // One button goes here
	BYTE  bButtonB;             // Another button goes here
	BYTE  bButtonC;             // Another button goes here
	BYTE  bButtonD;             // Another button goes here
} MYDATA;

static DIOBJECTDATAFORMAT rgodf[] = {
  { &GUID_XAxis,    FIELD_OFFSET(MYDATA, lX),       DIDFT_AXIS | DIDFT_ANYINSTANCE,   0,},
  { &GUID_YAxis,    FIELD_OFFSET(MYDATA, lY),       DIDFT_AXIS | DIDFT_ANYINSTANCE,   0,},
  { &GUID_ZAxis,    FIELD_OFFSET(MYDATA, lZ),       0x80000000 | DIDFT_AXIS | DIDFT_ANYINSTANCE,   0,},
  { 0,              FIELD_OFFSET(MYDATA, bButtonA), DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
  { 0,              FIELD_OFFSET(MYDATA, bButtonB), DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
  { 0,              FIELD_OFFSET(MYDATA, bButtonC), 0x80000000 | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
  { 0,              FIELD_OFFSET(MYDATA, bButtonD), 0x80000000 | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
};

#define NUM_OBJECTS (sizeof(rgodf) / sizeof(rgodf[0]))

static DIDATAFORMAT	df = {
	sizeof(DIDATAFORMAT),       // this structure
	sizeof(DIOBJECTDATAFORMAT), // size of object data format
	DIDF_RELAXIS,               // absolute axis coordinates
	sizeof(MYDATA),             // device data size
	NUM_OBJECTS,                // number of objects
	rgodf,                      // and here they are
};


/*
===========
Force_CenterView_f
===========
*/
void Force_CenterView_f (void)
{
	cl.viewangles[PITCH] = 0;
}


/*
===========
IN_UpdateClipCursor
===========
*/
void IN_UpdateClipCursor (void)
{

	if (mouseinitialized && mouseactive && !dinput)
	{
		ClipCursor (&window_rect);
	}
}


/*
===========
IN_ShowMouse
===========
*/
void IN_ShowMouse (void)
{

	if (!mouseshowtoggle)
	{
		ShowCursor (TRUE);
		mouseshowtoggle = 1;
	}
}


/*
===========
IN_HideMouse
===========
*/
void IN_HideMouse (void)
{

	if (mouseshowtoggle)
	{
		ShowCursor (FALSE);
		mouseshowtoggle = 0;
	}
}


/*
===========
IN_ActivateMouse
===========
*/
void IN_ActivateMouse (void)
{

	mouseactivatetoggle = true;

	if (mouseinitialized)
	{
		if (dinput)
		{
			if (g_pMouse)
			{
				if (!dinput_acquired)
				{
					IDirectInputDevice_Acquire(g_pMouse);
					dinput_acquired = true;
				}
			}
			else
			{
				return;
			}
		}
		else
		{
			if (mouseparmsvalid)
				restore_spi = SystemParametersInfo (SPI_SETMOUSE, 0, newmouseparms, 0);

			SetCursorPos (window_center_x, window_center_y);
			SetCapture (mainwindow);
			ClipCursor (&window_rect);
		}

		mouseactive = true;
	}
}


/*
===========
IN_SetQuakeMouseState
===========
*/
void IN_SetQuakeMouseState (void)
{
	if (mouseactivatetoggle)
		IN_ActivateMouse ();
}


/*
===========
IN_DeactivateMouse
===========
*/
void IN_DeactivateMouse (void)
{

	mouseactivatetoggle = false;

	if (mouseinitialized)
	{
		if (dinput)
		{
			if (g_pMouse)
			{
				if (dinput_acquired)
				{
					IDirectInputDevice_Unacquire(g_pMouse);
					dinput_acquired = false;
				}
			}
		}
		else
		{
			if (restore_spi)
				SystemParametersInfo (SPI_SETMOUSE, 0, originalmouseparms, 0);

			ClipCursor (NULL);
			ReleaseCapture ();
		}

		mouseactive = false;
	}
}


/*
===========
IN_RestoreOriginalMouseState
===========
*/
void IN_RestoreOriginalMouseState (void)
{
	if (mouseactivatetoggle)
	{
		IN_DeactivateMouse ();
		mouseactivatetoggle = true;
	}

// try to redraw the cursor so it gets reinitialized, because sometimes it
// has garbage after the mode switch
	ShowCursor (TRUE);
	ShowCursor (FALSE);
}


/*
===========
IN_InitDInput
===========
*/
qboolean IN_InitDInput (void)
{
    HRESULT		hr;
	DIPROPDWORD	dipdw = {
		{
			sizeof(DIPROPDWORD),        // diph.dwSize
			sizeof(DIPROPHEADER),       // diph.dwHeaderSize
			0,                          // diph.dwObj
			DIPH_DEVICE,                // diph.dwHow
		},
		DINPUT_BUFFERSIZE,              // dwData
	};

	if (!hInstDI)
	{
		hInstDI = LoadLibrary("dinput.dll");

		if (hInstDI == NULL)
		{
			Con_SafePrintf ("Couldn't load dinput.dll\n");
			return false;
		}
	}

	if (!pDirectInputCreate)
	{
		pDirectInputCreate = (void *)GetProcAddress(hInstDI,"DirectInputCreateA");

		if (!pDirectInputCreate)
		{
			Con_SafePrintf ("Couldn't get DI proc addr\n");
			return false;
		}
	}

// register with DirectInput and get an IDirectInput to play with.
	hr = iDirectInputCreate(global_hInstance, DIRECTINPUT_VERSION, &g_pdi, NULL);

	if (FAILED(hr))
	{
		return false;
	}

// obtain an interface to the system mouse device.
	hr = IDirectInput_CreateDevice(g_pdi, &GUID_SysMouse, &g_pMouse, NULL);

	if (FAILED(hr))
	{
		Con_SafePrintf ("Couldn't open DI mouse device\n");
		return false;
	}

// set the data format to "mouse format".
	hr = IDirectInputDevice_SetDataFormat(g_pMouse, &df);

	if (FAILED(hr))
	{
		Con_SafePrintf ("Couldn't set DI mouse format\n");
		return false;
	}

// set the cooperativity level.
	hr = IDirectInputDevice_SetCooperativeLevel(g_pMouse, mainwindow,
			DISCL_EXCLUSIVE | DISCL_FOREGROUND);

	if (FAILED(hr))
	{
		Con_SafePrintf ("Couldn't set DI coop level\n");
		return false;
	}


// set the buffer size to DINPUT_BUFFERSIZE elements.
// the buffer size is a DWORD property associated with the device
	hr = IDirectInputDevice_SetProperty(g_pMouse, DIPROP_BUFFERSIZE, &dipdw.diph);

	if (FAILED(hr))
	{
		Con_SafePrintf ("Couldn't set DI buffersize\n");
		return false;
	}

	return true;
}


/*
===========
IN_StartupMouse
===========
*/
void IN_StartupMouse (void)
{
	HDC			hdc;

	if ( COM_CheckParm ("-nomouse") )
		return;

	mouseinitialized = true;

	if (COM_CheckParm ("-dinput"))
	{
		dinput = IN_InitDInput ();

		if (dinput)
		{
			Con_SafePrintf ("DirectInput initialized\n");
		}
		else
		{
			Con_SafePrintf ("DirectInput not initialized\n");
		}
	}

	if (!dinput)
	{
		mouseparmsvalid = SystemParametersInfo (SPI_GETMOUSE, 0, originalmouseparms, 0);

		if (mouseparmsvalid)
		{
			if ( COM_CheckParm ("-noforcemspd") )
				newmouseparms[2] = originalmouseparms[2];

			if ( COM_CheckParm ("-noforcemaccel") )
			{
				newmouseparms[0] = originalmouseparms[0];
				newmouseparms[1] = originalmouseparms[1];
			}

			if ( COM_CheckParm ("-noforcemparms") )
			{
				newmouseparms[0] = originalmouseparms[0];
				newmouseparms[1] = originalmouseparms[1];
				newmouseparms[2] = originalmouseparms[2];
			}
		}
	}

	mouse_buttons = 3;

// if a fullscreen video mode was set before the mouse was initialized,
// set the mouse state appropriately
	if (mouseactivatetoggle)
		IN_ActivateMouse ();
}


/*
===========
IN_Init
===========
*/
void IN_Init (void)
{
	// mouse variables
	Cvar_RegisterVariable (&m_filter);

	// joystick variables
	Cvar_RegisterVariable (&in_joystick);
	Cvar_RegisterVariable (&joy_name);
	Cvar_RegisterVariable (&joy_advanced);
	Cvar_RegisterVariable (&joy_advaxisx);
	Cvar_RegisterVariable (&joy_advaxisy);
	Cvar_RegisterVariable (&joy_advaxisz);
	Cvar_RegisterVariable (&joy_advaxisr);
	Cvar_RegisterVariable (&joy_advaxisu);
	Cvar_RegisterVariable (&joy_advaxisv);
	Cvar_RegisterVariable (&joy_forwardthreshold);
	Cvar_RegisterVariable (&joy_sidethreshold);
	Cvar_RegisterVariable (&joy_pitchthreshold);
	Cvar_RegisterVariable (&joy_yawthreshold);
	Cvar_RegisterVariable (&joy_forwardsensitivity);
	Cvar_RegisterVariable (&joy_sidesensitivity);
	Cvar_RegisterVariable (&joy_pitchsensitivity);
	Cvar_RegisterVariable (&joy_yawsensitivity);
	Cvar_RegisterVariable (&joy_wwhack1);
	Cvar_RegisterVariable (&joy_wwhack2);

    // Manoel Kasimier - begin
	Cvar_RegisterVariable(&joy1_x_scale);
	Cvar_RegisterVariable(&joy1_y_scale);
	Cvar_RegisterVariable(&joy1_z_scale);
	Cvar_RegisterVariable(&joy1_r_scale);
	Cvar_RegisterVariable(&joy1_u_scale);
	Cvar_RegisterVariable(&joy1_v_scale);
	Cvar_RegisterVariableWithCallback(&joy1_x_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy1_y_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy1_z_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy1_r_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy1_u_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy1_v_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariable(&joy2_x_scale);
	Cvar_RegisterVariable(&joy2_y_scale);
	Cvar_RegisterVariable(&joy2_z_scale);
	Cvar_RegisterVariable(&joy2_r_scale);
	Cvar_RegisterVariable(&joy2_u_scale);
	Cvar_RegisterVariable(&joy2_v_scale);
	Cvar_RegisterVariableWithCallback(&joy2_x_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy2_y_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy2_z_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy2_r_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy2_u_threshold, BoundsCheckThreshold);
	Cvar_RegisterVariableWithCallback(&joy2_v_threshold, BoundsCheckThreshold);
    // Manoel Kasimier - end

	Cmd_AddCommand ("force_centerview", Force_CenterView_f);
	Cmd_AddCommand ("joyadvancedupdate", Joy_AdvancedUpdate_f);

	uiWheelMessage = RegisterWindowMessage ( "MSWHEEL_ROLLMSG" );

	IN_StartupMouse ();
	IN_StartupJoystick ();
}

/*
===========
IN_Shutdown
===========
*/
void IN_Shutdown (void)
{

	IN_DeactivateMouse ();
	IN_ShowMouse ();

    if (g_pMouse)
	{
		IDirectInputDevice_Release(g_pMouse);
		g_pMouse = NULL;
	}

    if (g_pdi)
	{
		IDirectInput_Release(g_pdi);
		g_pdi = NULL;
	}
}


/*
===========
IN_MouseEvent
===========
*/
void IN_MouseEvent (int mstate)
{
	int	i;

	if (mouseactive && !dinput)
	{
	// perform button actions
		for (i=0 ; i<mouse_buttons ; i++)
		{
			if ( (mstate & (1<<i)) &&
				!(mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event (K_MOUSE1 + i, true);
			}

			if ( !(mstate & (1<<i)) &&
				(mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event (K_MOUSE1 + i, false);
			}
		}

		mouse_oldbuttonstate = mstate;
	}
}


/*
===========
IN_MouseMove
===========
*/
void IN_MouseMove (usercmd_t *cmd)
{
	int					mx, my;
	HDC					hdc;
	int					i;
	DIDEVICEOBJECTDATA	od;
	DWORD				dwElements;
	HRESULT				hr;

	if (!mouseactive)
		return;

	if (dinput)
	{
		mx = 0;
		my = 0;

		for (;;)
		{
			dwElements = 1;

			hr = IDirectInputDevice_GetDeviceData(g_pMouse,
					sizeof(DIDEVICEOBJECTDATA), &od, &dwElements, 0);

			if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
			{
				dinput_acquired = true;
				IDirectInputDevice_Acquire(g_pMouse);
				break;
			}

			/* Unable to read data or no data available */
			if (FAILED(hr) || dwElements == 0)
			{
				break;
			}

			/* Look at the element to see what happened */

			switch (od.dwOfs)
			{
				case DIMOFS_X:
					mx += od.dwData;
					break;

				case DIMOFS_Y:
					my += od.dwData;
					break;

				case DIMOFS_BUTTON0:
					if (od.dwData & 0x80)
						mstate_di |= 1;
					else
						mstate_di &= ~1;
					break;

				case DIMOFS_BUTTON1:
					if (od.dwData & 0x80)
						mstate_di |= (1<<1);
					else
						mstate_di &= ~(1<<1);
					break;

				case DIMOFS_BUTTON2:
					if (od.dwData & 0x80)
						mstate_di |= (1<<2);
					else
						mstate_di &= ~(1<<2);
					break;
			}
		}

	// perform button actions
		for (i=0 ; i<mouse_buttons ; i++)
		{
			if ( (mstate_di & (1<<i)) &&
				!(mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event (K_MOUSE1 + i, true);
			}

			if ( !(mstate_di & (1<<i)) &&
				(mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event (K_MOUSE1 + i, false);
			}
		}

		mouse_oldbuttonstate = mstate_di;
	}
	else
	{
		GetCursorPos (&current_pos);
		mx = current_pos.x - window_center_x + mx_accum;
		my = current_pos.y - window_center_y + my_accum;
		mx_accum = 0;
		my_accum = 0;
	}

//if (mx ||  my)
//	Con_DPrintf("mx=%d, my=%d\n", mx, my);

	if (m_filter.value)
	{
		mouse_x = (mx + old_mouse_x) * 0.5;
		mouse_y = (my + old_mouse_y) * 0.5;
	}
	else
	{
		mouse_x = mx;
		mouse_y = my;
	}

	old_mouse_x = mx;
	old_mouse_y = my;

	mouse_x *= sensitivity.value;
	mouse_y *= sensitivity.value;

// add mouse X/Y movement to cmd
	if ( (in_strafe.state & 1) || (lookstrafe.value && m_look.value )) // Manoel Kasimier - m_look - edited
		cmd->sidemove += m_side.value * mouse_x;
	else
		cl.viewangles[YAW] -= m_yaw.value * mouse_x/fovscale; // Manoel Kasimier - FOV-based scaling - edited

	if (m_look.value) // Manoel Kasimier - m_look
		V_StopPitchDrift ();

	if ( m_look.value && !(in_strafe.state & 1)) // Manoel Kasimier - m_look - edited
	{
		cl.viewangles[PITCH] += m_pitch.value * mouse_y/fovscale; // Manoel Kasimier - FOV-based scaling - edited
		if (cl.viewangles[PITCH] > 80)
			cl.viewangles[PITCH] = 80;
		if (cl.viewangles[PITCH] < -70)
			cl.viewangles[PITCH] = -70;
	}
	else
	{
		if ((in_strafe.state & 1) && noclip_anglehack)
			cmd->upmove -= m_forward.value * mouse_y;
		else
			cmd->forwardmove -= m_forward.value * mouse_y;
	}

// if the mouse has moved, force it to the center, so there's room to move
	if (mx || my)
	{
		SetCursorPos (window_center_x, window_center_y);
	}
}


/*
===========
IN_Move
===========
*/
void IN_Move (usercmd_t *cmd)
{

	if (ActiveApp && !Minimized)
	{
		IN_MouseMove (cmd);
		IN_JoyMove (cmd);
	}
}


/*
===========
IN_Accumulate
===========
*/
void IN_Accumulate (void)
{
	int		mx, my;
	HDC	hdc;

	if (mouseactive)
	{
		if (!dinput)
		{
			GetCursorPos (&current_pos);

			mx_accum += current_pos.x - window_center_x;
			my_accum += current_pos.y - window_center_y;

		// force the mouse to the center, so there's room to move
			SetCursorPos (window_center_x, window_center_y);
		}
	}
}


/*
===================
IN_ClearStates
===================
*/
void IN_ClearStates (void)
{

	if (mouseactive)
	{
		mx_accum = 0;
		my_accum = 0;
		mouse_oldbuttonstate = 0;
	}
}

// Manoel Kasimier - begin
/*
==============================================================================

                            JOYSTICK INTERFACE

==============================================================================
*/
int In_IsAxis(int key)
{
	switch (key)
	{
	    // joystick 1
		case K_JOY1_AXISX_PLUS:
		case K_JOY1_AXISX_MINUS:
		case K_JOY1_AXISY_PLUS:
		case K_JOY1_AXISY_MINUS:
		case K_JOY1_AXISZ_PLUS:
		case K_JOY1_AXISZ_MINUS:
		case K_JOY1_AXISR_PLUS:
		case K_JOY1_AXISR_MINUS:
		case K_JOY1_AXISU_PLUS:
		case K_JOY1_AXISU_MINUS:
		case K_JOY1_AXISV_PLUS:
		case K_JOY1_AXISV_MINUS:
        // joystick 2
		case K_JOY2_AXISX_PLUS:
		case K_JOY2_AXISX_MINUS:
		case K_JOY2_AXISY_PLUS:
		case K_JOY2_AXISY_MINUS:
		case K_JOY2_AXISZ_PLUS:
		case K_JOY2_AXISZ_MINUS:
		case K_JOY2_AXISR_PLUS:
		case K_JOY2_AXISR_MINUS:
		case K_JOY2_AXISU_PLUS:
		case K_JOY2_AXISU_MINUS:
		case K_JOY2_AXISV_PLUS:
		case K_JOY2_AXISV_MINUS:
			return key;
        default:
            return 0;
	}
}
// Manoel Kasimier - end


/*
===============
IN_StartupJoystick
===============
*/
void IN_StartupJoystick_ (int j, int j1)
{
	int			i, numdevs;
	JOYCAPS		jc;
	MMRESULT	mmr;

 	// assume no joystick
	joystick[j].joy_avail = false;

	// abort startup if user requests no joystick
	if ( COM_CheckParm ("-nojoy") )
		return;

	// verify joystick driver is present
	if ((numdevs = joyGetNumDevs ()) == 0)
	{
		Con_Printf ("\njoystick not found -- driver not present\n\n");
		return;
	}

	// cycle through the joystick ids for the first valid one
	for (joystick[j].joy_id=0 ; joystick[j].joy_id<numdevs ; joystick[j].joy_id++)
	{
	    if (joystick[j].joy_id == j1) // Manoel Kasimier
            continue; // Manoel Kasimier
		memset (&joystick[j].ji, 0, sizeof(JOYINFOEX));
		joystick[j].ji.dwSize = sizeof(JOYINFOEX);
		joystick[j].ji.dwFlags = JOY_RETURNCENTERED;

		if ((mmr = joyGetPosEx (joystick[j].joy_id, &joystick[j].ji)) == JOYERR_NOERROR)
			break;
	}

	// abort startup if we didn't find a valid joystick
	if (mmr != JOYERR_NOERROR)
	{
		Con_Printf ("\njoystick not found -- no valid joysticks (%x)\n\n", mmr);
		return;
	}

	// get the capabilities of the selected joystick
	// abort startup if command fails
	memset (&jc, 0, sizeof(jc));
	if ((mmr = joyGetDevCaps (joystick[j].joy_id, &jc, sizeof(jc))) != JOYERR_NOERROR)
	{
		Con_Printf ("\njoystick not found -- invalid joystick capabilities (%x)\n\n", mmr);
		return;
	}

	// save the joystick's number of buttons and POV status
	joystick[j].joy_numbuttons = jc.wNumButtons;
	joystick[j].joy_haspov = jc.wCaps & JOYCAPS_HASPOV;

	// old button and POV states default to no buttons pressed
	joystick[j].joy_oldbuttonstate = joystick[j].joy_oldpovstate = 0;

	// mark the joystick as available and advanced initialization not completed
	// this is needed as cvars are not available during initialization

	joystick[j].joy_avail = true;
	joystick[j].joy_advancedinit = false;

	Con_DPrintf ("\njoystick detected\n\n"); // edited
}
void IN_StartupJoystick (void)
{
    IN_StartupJoystick_ (0, -1);
    IN_StartupJoystick_ (1, joystick[0].joy_id);
}


/*
===========
RawValuePointer
===========
*/
/*
PDWORD RawValuePointer (int axis)
{
	switch (axis)
	{
	case JOY_AXIS_X:
		return &ji.dwXpos;
	case JOY_AXIS_Y:
		return &ji.dwYpos;
	case JOY_AXIS_Z:
		return &ji.dwZpos;
	case JOY_AXIS_R:
		return &ji.dwRpos;
	case JOY_AXIS_U:
		return &ji.dwUpos;
	case JOY_AXIS_V:
		return &ji.dwVpos;
	default: // compiler warning fix
		return NULL; // compiler warning fix
	}
}
*/


/*
===========
Joy_AdvancedUpdate_f
===========
*/
void Joy_AdvancedUpdate (int j)
{

	// called once by IN_ReadJoystick and by user whenever an update is needed
	// cvars are now available
	int	i;
/*	DWORD dwTemp;

	// initialize all the maps
	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		j->dwAxisMap[i] = AxisNada;
		j->dwControlMap[i] = JOY_ABSOLUTE_AXIS;
		j->pdwRawValue[i] = RawValuePointer(i);
	}

	if( joy_advanced.value == 0.0)
	{
		// default joystick initialization
		// 2 axes only with joystick control
		j->dwAxisMap[JOY_AXIS_X] = AxisTurn;
		// dwControlMap[JOY_AXIS_X] = JOY_ABSOLUTE_AXIS;
		j->dwAxisMap[JOY_AXIS_Y] = AxisForward;
		// dwControlMap[JOY_AXIS_Y] = JOY_ABSOLUTE_AXIS;
	}
	else
	{
		if (Q_strcmp (joy_name.string, "joystick") != 0)
		{
			// notify user of advanced controller
			Con_Printf ("\n%s configured\n\n", joy_name.string);
		}

		// advanced initialization here
		// data supplied by user via joy_axisn cvars
		dwTemp = (DWORD) joy_advaxisx.value;
		j->dwAxisMap[JOY_AXIS_X] = dwTemp & 0x0000000f;
		j->dwControlMap[JOY_AXIS_X] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisy.value;
		j->dwAxisMap[JOY_AXIS_Y] = dwTemp & 0x0000000f;
		j->dwControlMap[JOY_AXIS_Y] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisz.value;
		j->dwAxisMap[JOY_AXIS_Z] = dwTemp & 0x0000000f;
		j->dwControlMap[JOY_AXIS_Z] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisr.value;
		j->dwAxisMap[JOY_AXIS_R] = dwTemp & 0x0000000f;
		j->dwControlMap[JOY_AXIS_R] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisu.value;
		j->dwAxisMap[JOY_AXIS_U] = dwTemp & 0x0000000f;
		j->dwControlMap[JOY_AXIS_U] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisv.value;
		j->dwAxisMap[JOY_AXIS_V] = dwTemp & 0x0000000f;
		j->dwControlMap[JOY_AXIS_V] = dwTemp & JOY_RELATIVE_AXIS;
	}
*/
    joystick[j].pdwRawValue[JOY_AXIS_X] = &joystick[j].ji.dwXpos;
    joystick[j].pdwRawValue[JOY_AXIS_Y] = &joystick[j].ji.dwYpos;
    joystick[j].pdwRawValue[JOY_AXIS_Z] = &joystick[j].ji.dwZpos;
    joystick[j].pdwRawValue[JOY_AXIS_R] = &joystick[j].ji.dwRpos;
    joystick[j].pdwRawValue[JOY_AXIS_U] = &joystick[j].ji.dwUpos;
    joystick[j].pdwRawValue[JOY_AXIS_V] = &joystick[j].ji.dwVpos;

	// compute the axes to collect from DirectInput
	joystick[j].joy_flags = JOY_RETURNCENTERED | JOY_RETURNBUTTONS | JOY_RETURNPOV;
	for (i = 0; i < JOY_MAX_AXES; i++)
//		if (j->dwAxisMap[i] != AxisNada)
			joystick[j].joy_flags |= dwAxisFlags[i];
    Con_Printf ("\n%s configured\n\n", joy_name.string);
}
void Joy_AdvancedUpdate_f (void)
{
    Joy_AdvancedUpdate (0);
    Joy_AdvancedUpdate (1);
}


/*
===============
IN_ReadJoystick
===============
*/
qboolean IN_ReadJoystick_ (int i)
{
	memset (&joystick[i].ji, 0, sizeof(JOYINFOEX));
	joystick[i].ji.dwSize = sizeof(JOYINFOEX);
	joystick[i].ji.dwFlags = joystick[i].joy_flags;

	if (joyGetPosEx (joystick[i].joy_id, &joystick[i].ji) == JOYERR_NOERROR)
	{
		// this is a hack -- there is a bug in the Logitech WingMan Warrior DirectInput Driver
		// rather than having 32768 be the zero point, they have the zero point at 32668
		// go figure -- anyway, now we get the full resolution out of the device
		if (joy_wwhack1.value != 0.0)
		{
			joystick[i].ji.dwUpos += 100;
		}
		return true;
	}
	else
	{
		// read error occurred
		// turning off the joystick seems too harsh for 1 read error,\
		// but what should be done?
		// Con_Printf ("IN_ReadJoystick: no response\n");
		// joy_avail = false;
		return false;
	}
}
qboolean IN_ReadJoystick (void)
{
    return (IN_ReadJoystick_ (0) || IN_ReadJoystick_ (1));
}


/*
===========
IN_Commands
===========
*/
float Axis_Event (int i, int axis, float oldaxis, int negkey, int poskey)
{
	float	fAxisValue;
	if (!(joystick[i].pdwRawValue[axis])) // this axis doesn't exist
		return 0.0;
	// get the floating point zero-centered, potentially-inverted data for the current axis
    fAxisValue = (float) *joystick[i].pdwRawValue[axis];
    // move centerpoint to zero
    fAxisValue -= 32768.0;
    // Parse axis
    if (fAxisValue >= -8192.0 && oldaxis < -8192.0)//32
        Key_Event(negkey, false);
    if (fAxisValue <=  8192.0 && oldaxis >  8192.0)
        Key_Event(poskey, false);
    if (fAxisValue < -8192.0 && oldaxis >= -8192.0)
        Key_Event(negkey, true);
    if (fAxisValue >  8192.0 && oldaxis <=  8192.0)
        Key_Event(poskey, true);
    return fAxisValue;
}
void IN_Commands_ (int j)
{
	int		i, key_index;
	DWORD	buttonstate, povstate;

	if (!(joystick[j].joy_avail))
	{
		return;
	}


	// loop through the joystick buttons
	// key a joystick event or auxillary event for higher number buttons for each state change
	buttonstate = joystick[j].ji.dwButtons;
	for (i=0 ; i < joystick[j].joy_numbuttons ; i++)
	{
		if ( (buttonstate & (1<<i)) && !(joystick[j].joy_oldbuttonstate & (1<<i)) )
		{
			key_index = (j == 0) ? K_JOY1_1 : K_JOY2_1;
			Key_Event (key_index + i, true);
		}

		if ( !(buttonstate & (1<<i)) && (joystick[j].joy_oldbuttonstate & (1<<i)) )
		{
			key_index = (j == 0) ? K_JOY1_1 : K_JOY2_1;
			Key_Event (key_index + i, false);
		}
	}
	joystick[j].joy_oldbuttonstate = buttonstate;

	if (joystick[j].joy_haspov)
	{
		// convert POV information into 4 bits of state information
		// this avoids any potential problems related to moving from one
		// direction to another without going through the center position
		povstate = 0;
		if(joystick[j].ji.dwPOV != JOY_POVCENTERED)
		{
			if (joystick[j].ji.dwPOV == JOY_POVFORWARD)
				povstate |= 0x01;
			else if (joystick[j].ji.dwPOV == JOY_POVRIGHT)
				povstate |= 0x02;
			else if (joystick[j].ji.dwPOV == JOY_POVBACKWARD)
				povstate |= 0x04;
			else if (joystick[j].ji.dwPOV == JOY_POVLEFT)
				povstate |= 0x08;
            // Manoel Kasimier - POV diagonals support - begin
            else if (joystick[j].ji.dwPOV == 4500) // upper right
            {
				povstate |= 0x01;
				povstate |= 0x02;
            }
            else if (joystick[j].ji.dwPOV == 13500) // lower right
            {
				povstate |= 0x02;
				povstate |= 0x04;
            }
            else if (joystick[j].ji.dwPOV == 22500) // lower left
            {
				povstate |= 0x04;
				povstate |= 0x08;
            }
            else if (joystick[j].ji.dwPOV == 31500) // upper left
            {
				povstate |= 0x08;
				povstate |= 0x01;
            }
            // Manoel Kasimier - POV diagonals support - end
		}
		// determine which bits have changed and key an auxillary event for each change
		for (i=0 ; i < 4 ; i++)
		{
			if ( (povstate & (1<<i)) && !(joystick[j].joy_oldpovstate & (1<<i)) )
			{
			    if (j == 0)
				Key_Event (K_JOY1_POV_UP + i, true);
			    if (j == 1)
				Key_Event (K_JOY2_POV_UP + i, true);
			}

			if ( !(povstate & (1<<i)) && (joystick[j].joy_oldpovstate & (1<<i)) )
			{
			    if (j == 0)
				Key_Event (K_JOY1_POV_UP + i, false);
			    if (j == 1)
				Key_Event (K_JOY2_POV_UP + i, false);
			}
		}
		joystick[j].joy_oldpovstate = povstate;
	}
}
void IN_Commands (void)
{
    IN_Commands_ (0);
	if (IN_ReadJoystick_ (0) == true)
	{
        joystick[0].old_x = Axis_Event (0, JOY_AXIS_X, joystick[0].old_x, K_JOY1_AXISX_MINUS, K_JOY1_AXISX_PLUS);
        joystick[0].old_y = Axis_Event (0, JOY_AXIS_Y, joystick[0].old_y, K_JOY1_AXISY_MINUS, K_JOY1_AXISY_PLUS);
        joystick[0].old_z = Axis_Event (0, JOY_AXIS_Z, joystick[0].old_z, K_JOY1_AXISZ_MINUS, K_JOY1_AXISZ_PLUS);
        joystick[0].old_r = Axis_Event (0, JOY_AXIS_R, joystick[0].old_r, K_JOY1_AXISR_MINUS, K_JOY1_AXISR_PLUS);
        joystick[0].old_u = Axis_Event (0, JOY_AXIS_U, joystick[0].old_u, K_JOY1_AXISU_MINUS, K_JOY1_AXISU_PLUS);
        joystick[0].old_v = Axis_Event (0, JOY_AXIS_V, joystick[0].old_v, K_JOY1_AXISV_MINUS, K_JOY1_AXISV_PLUS);
	}
    IN_Commands_ (1);
	if (IN_ReadJoystick_ (1) == true)
	{
        joystick[1].old_x = Axis_Event (1, JOY_AXIS_X, joystick[1].old_x, K_JOY2_AXISX_MINUS, K_JOY2_AXISX_PLUS);
        joystick[1].old_y = Axis_Event (1, JOY_AXIS_Y, joystick[1].old_y, K_JOY2_AXISY_MINUS, K_JOY2_AXISY_PLUS);
        joystick[1].old_z = Axis_Event (1, JOY_AXIS_Z, joystick[1].old_z, K_JOY2_AXISZ_MINUS, K_JOY2_AXISZ_PLUS);
        joystick[1].old_r = Axis_Event (1, JOY_AXIS_R, joystick[1].old_r, K_JOY2_AXISR_MINUS, K_JOY2_AXISR_PLUS);
        joystick[1].old_u = Axis_Event (1, JOY_AXIS_U, joystick[1].old_u, K_JOY2_AXISU_MINUS, K_JOY2_AXISU_PLUS);
        joystick[1].old_v = Axis_Event (1, JOY_AXIS_V, joystick[1].old_v, K_JOY2_AXISV_MINUS, K_JOY2_AXISV_PLUS);
	}
}


/*
===========
IN_JoyMove
===========
*/
void Joy_UpdateAxis (usercmd_t *cmd, int mode, float scale, float rawvalue, float threshold, float speed, float aspeed)
{
	float value;
	float svalue;
	float fTemp;

	// Don't bother if it's switched off
	if(mode == AXIS_NONE)
		return;

    if (joy_wwhack2.value != 0.0)
        if (mode == AXIS_TURN_L || mode == AXIS_TURN_R) // Manoel Kasimier - edited
        {
            // this is a special formula for the Logitech WingMan Warrior
            // y=ax^b; where a = 300 and b = 1.3
            // also x values are in increments of 800 (so this is factored out)
            // then bounds check result to level out excessively high spin rates
            fTemp = 300.0 * pow(abs(rawvalue) / 800.0, 1.3);
            if (fTemp > 14000.0)
                fTemp = 14000.0;
            // restore direction information
            rawvalue = (rawvalue > 0.0) ? fTemp : -fTemp;
        }

	// convert range from -32768..32767 to -1..1
	value = rawvalue / 32768.0;

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

    //-------------------------
/*
    switch (j->dwAxisMap[i])
    {
    case AxisTurn:
        if (fabs(fAxisValue) > joy_yawthreshold.value)
        {
            if(j->dwControlMap[i] == JOY_ABSOLUTE_AXIS)
                cl.viewangles[YAW] += (fAxisValue * joy_yawsensitivity.value/fovscale) * aspeed * cl_yawspeed.value; // Manoel Kasimier - FOV-based scaling - edited
            else
                cl.viewangles[YAW] += (fAxisValue * joy_yawsensitivity.value/fovscale) * speed * 180.0; // Manoel Kasimier - FOV-based scaling - edited
        }
        break;

    case AxisLook:
        if (fabs(fAxisValue) > joy_pitchthreshold.value)
        {
            // pitch movement detected and pitch movement desired by user
            if(j->dwControlMap[i] == JOY_ABSOLUTE_AXIS)
                cl.viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity.value/fovscale) * aspeed * cl_pitchspeed.value; // Manoel Kasimier - FOV-based scaling - edited
            else
                cl.viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity.value/fovscale) * speed * 180.0; // Manoel Kasimier - FOV-based scaling - edited
            V_StopPitchDrift();
        }
        else
            // no pitch movement
            // disable pitch return-to-center unless requested by user
            // *** this code can be removed when the lookspring bug is fixed
            // *** the bug always has the lookspring feature on
            if(lookspring.value == 0.0)
                V_StopPitchDrift();
        break;
    }
*/
}
float AxisValue (int i, int axis)
{
	float	fAxisValue;
	if (!(joystick[i].pdwRawValue[axis])) // this axis doesn't exist
		return 0.0;
	// get the floating point zero-centered, potentially-inverted data for the current axis
	fAxisValue = (float) *joystick[i].pdwRawValue[axis];
	// move centerpoint to zero
	return (fAxisValue - 32768.0);
}
void IN_JoyMove_ (usercmd_t *cmd, int i)
{
	float	speed, aspeed;
	float	fAxisValue;

	// complete initialization if first time in
	// this is needed as cvars are not available at initialization time
	if (joystick[i].joy_advancedinit != true )
	{
		Joy_AdvancedUpdate (i); // Manoel Kasimier - edited
		joystick[i].joy_advancedinit = true;
	}

	// verify joystick is available and that the user wants to use it
	if (!(joystick[i].joy_avail) || !in_joystick.value)
		return;

	// collect the joystick data, if possible
	if (IN_ReadJoystick_ (i) != true) // Manoel Kasimier - edited
		return;

	if (in_speed.state & 1)
		speed = cl_movespeedkey.value;
	else
		speed = 1;
	aspeed = speed * host_org_frametime; // 2001-10-20 TIMESCALE extension by Tomaz/Maddes

    if (i == 0)
    {
        fAxisValue = AxisValue (i, JOY_AXIS_X);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISX_MINUS),  joy1_x_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy1_x_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISX_PLUS),  joy1_x_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy1_x_threshold.value, speed, aspeed);
        fAxisValue = AxisValue (i, JOY_AXIS_Y);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISY_MINUS),  joy1_y_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy1_y_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISY_PLUS),  joy1_y_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy1_y_threshold.value, speed, aspeed);
		fAxisValue = AxisValue (i, JOY_AXIS_Z);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISZ_MINUS),  joy1_z_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy1_z_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISZ_PLUS),  joy1_z_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy1_z_threshold.value, speed, aspeed);
        fAxisValue = AxisValue (i, JOY_AXIS_R);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISR_MINUS),  joy1_r_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy1_r_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISR_PLUS),  joy1_r_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy1_r_threshold.value, speed, aspeed);
        fAxisValue = AxisValue (i, JOY_AXIS_U);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISU_MINUS),  joy1_u_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy1_u_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISU_PLUS),  joy1_u_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy1_u_threshold.value, speed, aspeed);
        fAxisValue = AxisValue (i, JOY_AXIS_V);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISV_MINUS),  joy1_v_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy1_v_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY1_AXISV_PLUS),  joy1_v_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy1_v_threshold.value, speed, aspeed);
	}
    else //if (*j == joystick[1])
    {
        fAxisValue = AxisValue (i, JOY_AXIS_X);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISX_MINUS),  joy2_x_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy2_x_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISX_PLUS),  joy2_x_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy2_x_threshold.value, speed, aspeed);
        fAxisValue = AxisValue (i, JOY_AXIS_Y);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISY_MINUS),  joy2_y_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy2_y_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISY_PLUS),  joy2_y_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy2_y_threshold.value, speed, aspeed);
		fAxisValue = AxisValue (i, JOY_AXIS_Z);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISZ_MINUS),  joy2_z_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy2_z_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISZ_PLUS),  joy2_z_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy2_z_threshold.value, speed, aspeed);
        fAxisValue = AxisValue (i, JOY_AXIS_R);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISR_MINUS),  joy2_r_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy2_r_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISR_PLUS),  joy2_r_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy2_r_threshold.value, speed, aspeed);
        fAxisValue = AxisValue (i, JOY_AXIS_U);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISU_MINUS),  joy2_u_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy2_u_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISU_PLUS),  joy2_u_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy2_u_threshold.value, speed, aspeed);
        fAxisValue = AxisValue (i, JOY_AXIS_V);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISV_MINUS),  joy2_v_scale.value,  abs(fAxisValue) * (fAxisValue<0), joy2_v_threshold.value, speed, aspeed);
        Joy_UpdateAxis(cmd, In_AnalogCommand(K_JOY2_AXISV_PLUS),  joy2_v_scale.value,  abs(fAxisValue) * (fAxisValue>0), joy2_v_threshold.value, speed, aspeed);
	}
}
void IN_JoyMove (usercmd_t *cmd)
{
	IN_JoyMove_ (cmd, 0);
	IN_JoyMove_ (cmd, 1);

	// bounds check pitch
	if (cl.viewangles[PITCH] > 80.0)
		cl.viewangles[PITCH] = 80.0;
	else if (cl.viewangles[PITCH] < -70.0)
		cl.viewangles[PITCH] = -70.0;
}
