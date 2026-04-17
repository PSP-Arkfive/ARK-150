TARGET = satelite150
OBJS = blit.o ui.o menu.o vpl.o trans.o trans_en.o stubkk.o main.o

BUILD_PRX=1

INCDIR = 
CFLAGS = -std=c99 -Os -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

PSP_FW_VERSION = 150

PRX_EXPORTS = exports.exp

# should we use libansic or libpspsysc_user?
LIBS = -lansic -lpspsystemctrl_user #-lpspvshbridge
LIBDIR = . $(ARKROOT)/libs
LDFLAGS = 

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
