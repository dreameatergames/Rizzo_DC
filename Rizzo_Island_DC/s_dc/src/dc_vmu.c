/*
Copyright (C) 2005 Manoel Kasimier ( manoelkasimier@yahoo.com.br )

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
// Marcus Comstedt - begin
uint16 calcCRC (char *buf, int size)
{
	int	i, c;
	uint16	n=0;
	for (i=0; i<size; i++)
	{
		n ^= (buf[i] << 8);
		for (c=0; c<8; c++)
		{
			if (n & 0x8000)
				n = (n << 1) ^ 4129;
			else
				n = (n << 1);
		}
	}
	return n;
}
// Marcus Comstedt - end

//=======================================================================
// Manoel Kasimier - begin
//=======================================================================

#include <zlib/zlib.h>
#include "quakedef.h"

#define DESC		16
#define VMAPP		DESC+32
#define ICONS		VMAPP+16
#define SPEED		ICONS+2
#define CATCH		SPEED+2
#define VMCRC		CATCH+2
#define	FSIZE		VMCRC+2
#define	RESERVED	FSIZE+4
#define PALETTE		RESERVED+20
#define ICON		PALETTE+32

#define HEADER_SIZE	128 // same as ICON
#define APP_NAME	"Rizzo Island    " // 16 characters

static uint16 icon_palette[] =
{
	0x7dff, 0x0000, 0x001f,0xf888,
	0xf000,0xf880,0xf800,0x0000,
	0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000
};

static uint8 icon_bitmap[] =
{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x11,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x00,0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x00,0x01,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x00,0x01,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x10,0x11,0x11,0x00,0x00,0x00,0x01,0x11,
		0x00,0x00,0x11,0x11,0x11,0x11,0x01,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x11,0x11,0x10,0x00,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x01,0x11,0x11,0x00,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x01,0x11,0x11,0x10,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,
		0x00,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,
		0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x10,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,
		0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22
};
extern cvar_t	vmupath;
extern cvar_t	savename;
extern int zlib_getlength (char *filename);

int VMU_CountFreeBlocks(int controller, int slot)
{
	maple_device_t	*vmu;
	uint8	buffer[512];
	int		i, freeblocks = 0;

	if ((vmu = maple_enum_dev(controller, slot)))
		if (!vmu_block_read(vmu, 255, buffer))
		{
			int blocks = buffer[80];
			if (!vmu_block_read(vmu, 254, buffer))
			{
				for (i=0; i<=(blocks-1)*2; i+=2)
					if (buffer[i]==252 && buffer[i+1]==255)
						freeblocks++;
				return freeblocks;
			}
		}
	return -1;
}

void VMU_Error (char *format, ...)
{
	// Partially copied from the "va" function
	va_list			argptr;
	static char		string[512];

	va_start (argptr, format);
	vsprintf (string, format,argptr);
	va_end (argptr);

	if (key_dest == key_menu)
		M_PopUp_f (string, "");
	else
		Con_Printf (string);
}

void VMU_Save (char *from, char *filename, char *title, char *desc)
{
// there's no support for custom icons yet
#define num_icons 1
#define icon_size 512*num_icons
	gzFile	gzfile;
	file_t	file;
	int		data_size, fileblocks, freeblocks;
	char	*buffer,
			app_name[16],
			vmufilepath[8+12+1]; // "/vmu/a1/" + "FILENAME.EXT" + null termination
	uint16	crc;

	// Count free blocks
	if ((freeblocks = VMU_CountFreeBlocks(vmupath.string[0]-'a', vmupath.string[1]-'0')) < 0)
	{
		VMU_Error ("Couldn't read VMU %c%c\n", vmupath.string[0]-('a'-'A'), vmupath.string[1]);
		fs_unlink(from);
		return;
	}

	// Copy file to buffer
	file = fs_open(from, O_RDONLY);
	data_size = fs_total(file);
	buffer = (char *)malloc(data_size+1);
	fs_read(file, buffer, data_size);
	fs_close(file);
	fs_unlink(from);
	// Compress buffer into file
	gzfile = gzopen(from, "wb9");
	gzwrite(gzfile, buffer, data_size);
	gzclose(gzfile);
	free(buffer);

	// Read file from RAM
	file = fs_open(from, O_RDONLY);
	data_size = fs_total(file);
	fileblocks = (HEADER_SIZE + icon_size + data_size + 511)/512;
	if (!(buffer = (char *)malloc(fileblocks*512 + 1)))
	{
		VMU_Error ("VMU_Save: Not enough RAM\n");
		fs_unlink(from);
		fs_close(file);
		return;
	}
	// Copy the file into the buffer, after the header and icons
	fs_read(file, buffer+HEADER_SIZE+icon_size, data_size);
	fs_close(file);

	// Set the full path to the VMU file
	Q_strcpy(vmufilepath, va("/vmu/%s/%s", vmupath.string, filename));

	// Delete the old VMU file if it exists and if there's enough space for the new one
	if ((file = fs_open(vmufilepath, O_RDONLY)) != -1)
	{
		// Read the old file's header to check if it wasn't made by another application
		// If it was, don't delete it and don't save the new file
		fs_seek(file, VMAPP, SEEK_SET);
		fs_read(file, app_name, 16);
		if (Q_strncmp(app_name, APP_NAME, 16))
		{
			VMU_Error ("Can't overwrite file", COM_SkipPath(vmufilepath), app_name);
			fs_close(file);
			return;
		}
		freeblocks += fs_total(file)/512;
		fs_close(file);
		if (freeblocks >= fileblocks)
			fs_unlink(vmufilepath);
	}

	// Check if there's enough free blocks
	if (freeblocks < fileblocks)
		VMU_Error ("Can't save %s\nYou need %d more blocks\n", COM_SkipPath(vmufilepath), fileblocks - freeblocks);
	// Copy buffer into VMU file
	else if ((file = fs_open (vmufilepath, O_WRONLY)) != -1)
	{
		// Set color of the icon's border
		if (Q_strcmp("id1", COM_SkipPath(com_gamedir)))
			icon_palette[0] = 0xf000 | ((filename[0]&0x000f)<<8) | ((filename[1]&0x000f)<<4) | (filename[2]&0x000f);
		// Fill header
		memcpy(buffer,				title,		16);
		memcpy(&buffer[DESC],		desc,		32);
		memcpy(&buffer[VMAPP],		APP_NAME,	16);
		memcpy(&buffer[ICONS],		"\x01\0",	 2);
		memcpy(&buffer[SPEED],		"\x0\0",	 2);
		memset(&buffer[CATCH],		0,			 2);
		memset(&buffer[VMCRC],		0,			 2);
		buffer[FSIZE  ] = (data_size&0x000000ff)>>0;
		buffer[FSIZE+1] = (data_size&0x0000ff00)>>8;
		buffer[FSIZE+2] = (data_size&0x00ff0000)>>16;
		buffer[FSIZE+3] = (data_size&0xff000000)>>24;
		memset(&buffer[RESERVED],	0,				20);
		memcpy(&buffer[PALETTE],	icon_palette, 	32);
		memcpy(&buffer[ICON],		icon_bitmap, icon_size);
		// calculate the CRC...
		crc = calcCRC(buffer, HEADER_SIZE+icon_size+data_size);
		// and store the resulting CRC in the header
		buffer[VMCRC  ] = (crc&0x00ff)>>0;
		buffer[VMCRC+1] = (crc&0xff00)>>8;
		// Write resulting buffer
		fs_write(file, buffer, fileblocks*512);
		fs_close(file);
		Con_Printf("Saved\n");
		// Refresh the list of saves
		fs_unlink(from);
		free(buffer);
		M_RefreshSaveList ();
		return;
	}
	else
		VMU_Error ("Can't save", COM_SkipPath(vmufilepath));

	// Delete file from RAM
	fs_unlink(from);
	free(buffer);
#undef num_icons
}

int VMU_Load (char *to, char *filename)
{
	gzFile	gzfile;
	file_t	file;
	uint32	data_size;
	char	*buffer,
			app_name[16],
			vmufilepath[8+12+1]; // "/vmu/a1/" + "FILENAME.EXT" + null termination
	uint16	num_icons, eyecatch, eyecatch_size[4]={0, 8064, 4544, 2048};

	// Set the full path to the VMU file
	sprintf(vmufilepath, "/vmu/%s/%s", vmupath.string, filename);

	if ((file = fs_open(vmufilepath, O_RDONLY)) == -1)
	{
		Con_DPrintf ("Couldn't load \"%s\"\n", vmufilepath);
		return false;
	}

	fs_seek(file, VMAPP, SEEK_SET);
	fs_read(file, app_name, 16);
	if (Q_strncmp(app_name, APP_NAME, 16))
	{
		VMU_Error ("Invalid file", COM_SkipPath(vmufilepath), app_name);
		fs_close(file);
		return false;
	}

	fs_read(file, &num_icons, 2);

	fs_seek(file, 2, SEEK_CUR);
	fs_read(file, &eyecatch, 2);

	fs_seek(file, 2, SEEK_CUR);
	fs_read(file, &data_size, 4);

	// Alloc memory for the buffer
	if (!(buffer = (char *)malloc(data_size+1)))
	{
		VMU_Error ("VMU_Load: Not enough RAM\n");
		fs_close(file);
		return false;
	}
	// Go to the starting position of the data
	fs_seek(file, 52 + num_icons*512 + eyecatch_size[eyecatch], SEEK_CUR);
	// Copy the data to the buffer
	fs_read(file, buffer, data_size);
	fs_close(file);

	// Save buffer into a RAM file
	if ((file = fs_open(to, O_WRONLY)) == -1)
	{
		VMU_Error ("Can't write", to);
		free(buffer);
		return false;
	}
	fs_write(file, buffer, data_size);
	fs_close(file);
	free(buffer);
	// Uncompress file into buffer
	data_size = zlib_getlength(to);
	buffer = (char *)malloc(data_size+1);
	gzfile = gzopen(to, "r");
	gzread(gzfile, buffer, data_size);
	gzclose(gzfile);
	fs_unlink(to);

	if (!Q_strcmp(Cmd_Argv(0), "exec")) // loading a config file
		Cbuf_InsertText(buffer); // it must be added to the console buffer here because COM_LoadHunkFile can't load files from the RAM dir
	else
	{
		// save the buffer into the RAM file
		if ((file = fs_open(to, O_WRONLY)) == -1)
		{
			VMU_Error ("Can't write", to);
			free(buffer);
			return false;
		}
		fs_write(file, buffer, data_size);
		fs_close(file);
	}
	free(buffer);
	Con_DPrintf("Loaded");
	return true;
}
int VMU_Delete (char *filename)
{
	file_t	file;
	char	app_name[16],
			vmufilepath[8+12+1]; // "/vmu/a1/" + "FILENAME.EXT" + null termination

	// Set the full path to the VMU file
	sprintf(vmufilepath, "/vmu/%s/%s", vmupath.string, filename);

	if ((file = fs_open(vmufilepath, O_RDONLY)) == -1)
	{
		Con_DPrintf ("Couldn't delete", vmufilepath);
		return false;
	}

	// Delete the VMU file if it exists
	if ((file = fs_open(vmufilepath, O_RDONLY)) != -1)
	{
		// Read the old file's header to check if it wasn't made by another application
		// If it was, don't delete it
		fs_seek(file, VMAPP, SEEK_SET);
		fs_read(file, app_name, 16);
		if (Q_strncmp(app_name, APP_NAME, 16))
		{
			VMU_Error ("Can't delete file", COM_SkipPath(vmufilepath), app_name);
			fs_close(file);
			return false;
		}
		fs_close(file);
		fs_unlink(vmufilepath);
	}
	return true;
}

#define MAX_SAVEGAMES 100 // must be the same in menu.c !
byte dc_foundsave[MAX_SAVEGAMES];
void VMU_List (char c)
{
	int			dir, i;
	dirent_t	*ent;

	for (i=0; i<MAX_SAVEGAMES; i++)
		dc_foundsave[i] = false;

	if((dir = fs_open(va("/vmu/%s", vmupath.string), O_RDONLY | O_DIR)) == -1)
		return;
	while((ent = fs_readdir(dir)))
	{
		// Skip directories
		if(ent->size < 0 || ent->name[0] == '.')
			continue;
		// Compare filenames
		for (i=0; i<MAX_SAVEGAMES; i++)
			if(!stricmp(ent->name, va("%s.%c%i%i", savename.string, c, i/10, i%10)))
				dc_foundsave[i] = true;
	}
	fs_close(dir);
}

static uint8 lcd_map [192] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xF1, 0xFD,
0xFF, 0xE0, 0x00, 0x1F, 0x80, 0xFC, 0x3F, 0xC0, 0x00, 0x1F, 0x80, 0x3C, 0x7E, 0x00, 0x00, 0x1F,
0x80, 0x3C, 0x7E, 0x00, 0x00, 0x1F, 0x80, 0x1E, 0x7E, 0x00, 0x00, 0x1F, 0x80, 0x0F, 0x7E, 0x00,
0x00, 0x1F, 0x80, 0x07, 0xFF, 0x00, 0x00, 0x1F, 0x80, 0x04, 0x7F, 0x00, 0x00, 0x1F, 0x80, 0x0C,
0x3E, 0x00, 0x00, 0x1F, 0x80, 0x0E, 0x3E, 0x00, 0x00, 0x1F, 0x80, 0x07, 0xFE, 0x00, 0x00, 0xFF,
0xF0, 0x03, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
int LCD_DrawIcon(int controller, int slot)
{
	maple_device_t	*vmu;
	if ((vmu = maple_enum_dev(controller, slot)))
		if (!vmu_draw_lcd(vmu, lcd_map))
			return 0;
	return -1;
}

// Sets vmupath to the first VMU where a config file is found
void VMU_FindConfig (void)
{
	int			dir, i1, i2;
	dirent_t	*ent;

	for (i2=1; i2<3; i2++)
	for (i1=0; i1<4; i1++)
	{
		if((dir = fs_open(va("/vmu/%c%c", i1+'a', i2+'0'), O_RDONLY | O_DIR)) == -1)
			continue;
		LCD_DrawIcon(i1, i2);
		while((ent = fs_readdir(dir)))
		{
			// Skip directories
			if(ent->size < 0 || ent->name[0] == '.')
				continue;
			// Compare filename
			if(!stricmp(ent->name, va("%s.CFG", savename.string)))
			{
				vmupath.string[0] = i1+'a';
				vmupath.string[1] = i2+'0';
				fs_close(dir);
				return;
			}
		}
		fs_close(dir);
	}
}
//=======================================================================
// Manoel Kasimier - end
//=======================================================================
