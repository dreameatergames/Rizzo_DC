/*
Copyright (C) 2006 Manoel Kasimier ( manoelkasimier@yahoo.com.br )

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

#include <kos.h>
#include "quakedef.h"

extern cvar_t	cl_vibration;
extern int  vibration_update[2];
byte dc_vibe[2][4];
/*
dc_vibe elements:
player1
	special
	effect1
	effect2
	duration
player2
	special
	effect1
	effect2
	duration
*/
/*
void Vibration_Set (byte c, float i, byte d)
{
	if (i > 0)
	{
		if (((float)purupuru_intensity + i) > 255)
			purupuru_intensity = 255;
		else 
			purupuru_intensity += (byte)(i);
	}
}
*/
void Vibration_Stop (int player) // 0=player1, 1=player2
{
	maple_device_t *dev;
	if ( !(dev=maple_enum_dev(player, 2)) )
		return;
	purupuru_rumble_raw(dev, 0x00000000);
}

void Vibration_Update (void)
{
	purupuru_effect_t effect;
	int i;
	maple_device_t *dev[2];

	if (cl.paused) // stop vibration
		return;
	if (!cl_vibration.value || cls.demoplayback || key_dest != key_game)
	{
		vibration_update[0] = false;
		vibration_update[1] = false;
		return;
	}
	for (i=0; i<2; i++)
	{
		if (!vibration_update[i])
			continue;
		if ( !(dev[i]=maple_enum_dev(i, 2)) )
			continue;
#if 0
		dc_vibe[i][0] = PURUPURU_SPECIAL_MOTOR1/*|PURUPURU_SPECIAL_PULSE*/;
		dc_vibe[i][1] = PURUPURU_EFFECT1_INTENSITY(2);
		dc_vibe[i][2] = 0;
		dc_vibe[i][3] = 0x10;
#endif
		effect.special = dc_vibe[i][0];
		effect.effect1 = dc_vibe[i][1];
		effect.effect2 = dc_vibe[i][2];
		effect.duration = dc_vibe[i][3];
		purupuru_rumble(dev[i], &effect);
#if 0
		dc_vibe[i][0] = 0;
		dc_vibe[i][1] = 0;
		dc_vibe[i][2] = 0;
		dc_vibe[i][3] = 0;
#endif
		vibration_update[i] = false;
	}
}
