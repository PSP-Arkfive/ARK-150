TARGET = satelite150
OBJS = \
	main.o \
	src/blit.o\
	src/ui.o \
	src/menu.o \
	src/advanced.o \
	src/battery.o \
	src/vsh.o \
	src/config.o \
	src/color.o \
	src/fonts.o \
	src/clock.o \
	src/registry.o \
	src/vpl.o \
	src/trans.o \
	src/trans_en.o \
	src/launcher.o

BUILD_PRX = 1

INCDIR = include
CFLAGS = -Os -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

PRX_EXPORTS = exports.exp

LIBS = -lansic -lpspsystemctrl_user -lpspkubridge -lpspvshctrl -larkctrl -lpspreg -lpsppower
LIBDIR =
LDFLAGS = -nostartfiles

PSP_FW_VERSION = 150

USE_PSPSDK_LIBS = 1
USE_PSPSDK_LIBC = 1


PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
