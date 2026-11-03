TARGET = hello150plugin
OBJS = \
	main.o \

BUILD_PRX = 1

INCDIR = include
CFLAGS = -Os -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

PRX_EXPORTS = exports.exp

LIBS = -lansic -ltinyfont -lpspsystemctrl_user
LIBDIR =
LDFLAGS = -nostartfiles

PSP_FW_VERSION = 150

USE_PSPSDK_LIBS = 1
USE_PSPSDK_LIBC = 1


PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
