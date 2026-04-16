PSPSDK=$(shell psp-config --pspsdk-path)
PYTHON=$(shell which python3)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk

TARGET = msipl

all: $(TARGET)


CC = psp-gcc
LD = psp-ld
STRIP = psp-strip
OBJCOPY = psp-objcopy
LINKFILE = linkfile.l
INCDIR = $(PSPSDK)/include
CFLAGS = -std=c99 -Wall -Os -G0 -fno-pic $(addprefix -I, $(INCDIR)) -DMS_IPL


C_OBJS = \
	crt0.o \
	main.o \

LIBS = -L $(PSPSDK)/lib -liplsdk -lansic

crt0.o : crt0.S
	$(CC) -I$(PSPSDK)/include -c $< -o crt0.o

main.elf: $(C_OBJS)

$(TARGET): main.elf
	$(Q)$(STRIP) -s $<
	$(Q)$(OBJCOPY) -O binary $< $(TARGET)
	@echo GET $@

clean:
	$(Q)rm -rf *~ *.s *.o *.elf msipl $(EXTRA_CLEAN)

include $(CFWSDK)/build-tools/beauty_bin.mak
