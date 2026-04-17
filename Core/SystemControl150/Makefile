TARGET = systemctrl150

OBJS = \
	main.o \
	src/flushcache.o \
	src/hooknids.o \
	src/loadexec.o \
	src/modulemanager.o \
	src/rebootex.o \
	src/sctrl.o \
	src/sctrl_se.o \
	src/sctrl_hen.o \
	src/kubridge.o \
	src/syspatch.o \

all: $(TARGET).prx
INCDIR = include
CFLAGS = -std=c99 -Os -G0 -Wall -fno-pic $(addprefix -I, $(INCDIR))

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1

LIBDIR = lib
LDFLAGS =  -nostartfiles
LIBS = -lpspsemaphore -lpspdisplay_driver_weak -lpspreg_driver_weak

PSP_FW_VERSION = 150

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
