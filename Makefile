PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
BUILDTOOLS = $(PSPDEV)/share/psp-cfw-sdk/build-tools

.PHONY: Core FlashPackage Addon660 Installer150


all: FlashPackage
	$(Q)echo "Done!"


Core:
	$(Q)make -C Core/Rebootex/Rebootex150
	$(Q)make -C Core/SystemControl150
	$(Q)make -C Core/VSHControl150
	$(Q)make -C Core/VSHMenu150
	$(Q)make -C Core/TimeMachine150


FlashPackage: Core
	$(Q)mkdir -p dist/flash0
	$(PY) $(BUILDTOOLS)/gz/pspgz.py dist/flash0/ark_systemctrl150.prx $(BUILDTOOLS)/gz/SystemControl.hdr Core/SystemControl150/systemctrl150.prx SystemControl 0x3007
	$(PY) $(BUILDTOOLS)/gz/pspgz.py dist/flash0/ark_vshctrl150.prx $(BUILDTOOLS)/gz/SystemControl.hdr Core/VSHControl150/vshctrl150.prx VshControl 0x3007
	$(PY) $(BUILDTOOLS)/gz/pspgz.py dist/flash0/ark_satelite150.prx $(BUILDTOOLS)/gz/UserModule.hdr Core/VSHMenu150/satelite150.prx Satelite 0x0000
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/FLASH150.ARK flash150.txt -s


clean:
	$(Q)make -C Core/Rebootex/Rebootex150 clean
	$(Q)make -C Core/SystemControl150 clean
	$(Q)make -C Core/VSHControl150 clean
	$(Q)make -C Core/VSHMenu150 clean
	$(Q)make -C Core/TimeMachine150 clean
	$(Q)rm -rf dist
