#
# nxQuake Makefile for SDL/OpenGL (Windows)
# This is intended for the MinGW cross compiler hosted on Linux, and won't work
# on a Windows system using MinGW. Yet.
# August 2004
#

#CC = i586-mingw32msvc-gcc
#SDL_CONFIG = /usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config
CC = gcc
SDL_CONFIG = i386-mingw32msvc-sdl-config

CFLAGS = -I../common/include -I../r_gl/include -I../s_sdl/include `$(SDL_CONFIG) --cflags` -O2 -DGLQUAKE
LIBS = -lm `$(SDL_CONFIG) --libs` -lwsock32 -lopengl32
TARGET = nxglquake.exe

COMMON_OBJS = chase.o cl_demo.o cl_input.o cl_main.o cl_parse.o cl_tent.o cmd.o
COMMON_OBJS += common.o console.o crc.o cvar.o host.o host_cmd.o
COMMON_OBJS += keys.o mathlib.o menu.o net_dgrm.o net_loop.o net_main.o net_vcr.o
COMMON_OBJS += nonintel.o pr_cmds.o pr_edict.o pr_exec.o r_part.o sbar.o
COMMON_OBJS += sv_main.o sv_move.o sv_phys.o sv_user.o view.o wad.o
COMMON_OBJS += world.o zone.o
COMMON_OBJS += snd_dma.o snd_mem.o snd_mix.o

GL_OBJS = gl_draw.o gl_mesh.o gl_model.o gl_refrag.o gl_rlight.o gl_rmain.o gl_rmisc.o gl_rsurf.o gl_screen.o gl_warp.o

SYS_OBJS = cd_sdl.o in_sdl.o net_sdl.o net_sdludp.o snd_sdl.o sys_sdl.o vid_sdlgl.o

OBJS = $(COMMON_OBJS) $(GL_OBJS) $(SYS_OBJS)

all: $(TARGET)

clean:
	-rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: ../s_sdl/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: ../common/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: ../r_gl/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
