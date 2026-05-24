PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
BUILDTOOLS = $(PSPDEV)/share/psp-cfw-sdk/build-tools

.PHONY: dist Core Installers



all: dist Core Installers
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/FLASH150.ARK flash150.txt -s
	$(Q)$(PY) $(BUILDTOOLS)/pack/pkg-res.py Extras/Language LANG150.ARK
	$(Q)mv Extras/Language/Translations/LANG150.ARK dist/
	$(Q)cp Extras/660Addon/EBOOT.PBP dist/ARK150on660/
	$(Q)cp Extras/660Addon/Resources/LIBS/* dist/ARK150on660/
	$(Q)cp dist/FLASH150.ARK dist/ARK150on660/
	$(Q)cp dist/LANG150.ARK dist/ARK150on660/
	$(Q)echo "Done!"


dist:
	$(Q)mkdir -p dist/ARK150on660
	$(Q)mkdir -p dist/ARK150Installer
	$(Q)mkdir -p dist/ARK150Installer%


Core:
	$(Q)make -C Core/Rebootex/Rebootex150
	$(Q)make -C Core/SystemControl150
	$(Q)make -C Core/VSHControl150
	$(Q)make -C Core/VSHMenu150
	$(Q)make -C Core/TimeMachine150
	$(Q)make -C Core/Reboot150
	$(Q)make -C Core/MSIPL/mainbinex
	$(Q)make -C Core/MSIPL/stage1


Installers:
	$(Q)make -C Extras/660Addon


clean:
	$(Q)make -C Core/Rebootex/Rebootex150 clean
	$(Q)make -C Core/SystemControl150 clean
	$(Q)make -C Core/VSHControl150 clean
	$(Q)make -C Core/VSHMenu150 clean
	$(Q)make -C Core/TimeMachine150 clean
	$(Q)make -C Core/Reboot150 clean
	$(Q)make -C Core/MSIPL/mainbinex clean
	$(Q)make -C Core/MSIPL/stage1 clean
	$(Q)make -C Extras/660Addon clean
	$(Q)rm -rf dist
