#
# nxQuake Makefile for SDL/OpenGL (Linux)
# This will probably work on other Unix systems where sdl-config is present, but
# won't work on Windows - use s_win32gl instead
# August 2004
#

CC = gcc
CFLAGS = -I../common/include -I../r_gl/include -I../s_sdl/include `sdl-config --cflags` -O2 -DGLQUAKE
LIBS = -lm `sdl-config --libs` -lSDL_net -lGL
TARGET = nxquake

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
