TARGET = vshctrl150

OBJS = \
	stubkk.o \
	vshmenu.o \
	main.o \

all: $(TARGET).prx

INCDIR = 
CFLAGS = -std=c99 -Os -G0 -Wall -fno-pic $(addprefix -I, $(INCDIR))

CFLAGS += -I include -fshort-wchar

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1

LIBDIR = 
LDFLAGS =  -nostartfiles
LIBS = -lpspsystemctrl_kernel

PSP_FW_VERSION = 150

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
