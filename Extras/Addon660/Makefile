TARGET = ark150addon660
OBJS = main.o installer.o

CFLAGS = -std=c99 -Os -G0 -Wall
INCDIR =

LIBS = -lmini2d -lpng -lpspgu -lpspgum -lz -lpspkubridge -lpsppower -lpspdecrypt -lpsppsar

CXXFLAGS = $(CFLAGS)
ASFLAGS = $(CFLAGS)
LDFLAGS =
BUILD_PRX = 1
PSP_EBOOT_ICON = Resources/ICON0.PNG
PSP_EBOOT_PIC1 = Resources/PIC1.PNG
PSP_EBOOT_TITLE += ARK-150 Addon for 6.60

EXTRA_TARGETS = EBOOT.PBP

PSP_FW_VERSION = 660

all: $(TARGET).prx


PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
