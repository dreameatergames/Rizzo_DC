#
# nxMakaqu Makefile for Sega Dreamcast (KallistiOS)
# Compiled with a KOS 1.3.x snapshot
# November 2005
#

TARGET = nxMakaqu.elf
BIN = nxMakaqu.bin
FIRSTREAD = 1st_read.bin
KOS_LOCAL_CFLAGS = -I../common/include -I../r_soft/include -Iinclude -Os
LIBS=-lm -lz -lSDL
# -lSDL
# -lSDL_mixer -lSDL_audio

CDRECORD_ID = ATAPI:0,0,0
CDRECORD_DEV = /dev/hdc

# Common source files for all versions
COMMON_OBJS = chase.o cl_demo.o cl_input.o cl_main.o cl_parse.o cl_tent.o cmd.o
COMMON_OBJS += common.o console.o crc.o cvar.o host.o host_cmd.o
COMMON_OBJS += keys.o mathlib.o menu.o net_loop.o net_main.o net_vcr.o
COMMON_OBJS += nonintel.o pr_cmds.o pr_edict.o pr_exec.o r_part.o sbar.o
COMMON_OBJS += sv_main.o sv_move.o sv_phys.o sv_user.o view.o wad.o
COMMON_OBJS += world.o zone.o

# Uncomment this when we have a working sound driver
COMMON_OBJS += snd_dma.o snd_mem.o snd_mix.o

# Software rendering version
SOFT_OBJS = d_edge.o d_fill.o d_init.o d_modech.o d_part.o d_polyse.o d_scan.o
SOFT_OBJS += d_sky.o d_sprite.o d_surf.o d_vars.o d_zpoint.o draw.o r_aclip.o
SOFT_OBJS += r_alias.o r_bsp.o r_draw.o r_edge.o r_efrag.o r_light.o r_main.o
SOFT_OBJS += r_misc.o r_sky.o r_sprite.o r_surf.o r_vars.o model.o screen.o
SOFT_OBJS += r_2d.o

# System specific stuff
SYS_OBJS = cd_dc.o dc_menu.o in_dc.o net_dc.o sys_dc.o vid_dc.o snd_sdl.o
# snd_dc.o

SYS_OBJS += dc_vmu.o dc_vibe.o
SYS_OBJS += kos_zlib.o

OBJS = $(COMMON_OBJS) $(SOFT_OBJS) $(SYS_OBJS)

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	-rm -f $(TARGET) $(BIN) $(FIRSTREAD) $(OBJS)

rm-elf:
	-rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $(TARGET) $(KOS_START) \
		$(OBJS) $(OBJEXTRA) -L$(KOS_BASE)/lib $(LIBS) -lm $(KOS_LIBS)

%.o: src/%.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LOCAL_CFLAGS) $(OPT_FLAGS) -c $< -o $@

%.o: ../common/src/%.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LOCAL_CFLAGS) $(OPT_FLAGS) -c $< -o $@

%.o: ../r_soft/src/%.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LOCAL_CFLAGS) $(OPT_FLAGS) -c $< -o $@

%.o: ../r_common/src/%.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LOCAL_CFLAGS) $(OPT_FLAGS) -c $< -o $@

# make binary files
Binaries := (shell bash binaries.sh)
