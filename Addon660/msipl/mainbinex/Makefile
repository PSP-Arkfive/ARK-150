PSPSDK=$(shell psp-config --pspsdk-path)
PYTHON=$(shell which python3)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk

TARGET = mainbinex

all: $(TARGET)

CC = psp-gcc
LD = psp-ld
STRIP = psp-strip
OBJCOPY = psp-objcopy
LINKFILE = linkfile.l
LIBS = -L $(PSPSDK)/lib -lansic
INCDIR = $(PSPSDK)/include
CFLAGS = -std=c99 -Wall -Os -G0 -fno-pic $(addprefix -I, $(INCDIR)) -DMS_IPL
OBJS = main.o cache.o

main.elf: $(OBJS)

$(TARGET): main.elf
	$(Q)$(STRIP) -s $<
	$(Q)$(OBJCOPY) -O binary $< $(TARGET)
	@echo GET $@

clean:
	$(Q)rm -rf *~ *.s *.o *.elf $(TARGET) $(EXTRA_CLEAN)

include $(CFWSDK)/build-tools/beauty_bin.mak
