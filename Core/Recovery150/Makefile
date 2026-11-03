
PSPSDK=$(shell psp-config --pspsdk-path)

TARGET = recovery150

OBJS =  main.o proshell.o submenu.o config.o list.o settings.o plugins.o

CFLAGS = -std=c99 -Os -G0 -Wall
INCDIR = 
LIBDIR = 

LIBS = -lansic -lpspmalloc -lpspsystemctrl_user -lpspvshctrl -lpspkubridge -larkctrl -lpspusb -lpspusbdevice_driver

CXXFLAGS = $(CFLAGS)
ASFLAGS = $(CFLAGS)
LDFLAGS = -nostartfiles
BUILD_PRX = 1

USE_PSPSDK_LIBS = 1
USE_PSPSDK_LIBC = 1

include $(PSPSDK)/lib/build.mak
