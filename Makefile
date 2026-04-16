PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
BUILDTOOLS = $(PSPDEV)/share/psp-cfw-sdk/build-tools

.PHONY: dist Core Installers



all: dist Core Installers
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/FLASH150.ARK flash150.txt -s
	$(Q)cp Extras/Addon660/EBOOT.PBP dist/ARK150Addon660/
	$(Q)cp Extras/Addon660/Resources/LIBS/* dist/ARK150Addon660/
	$(Q)cp dist/FLASH150.ARK dist/ARK150Addon660/
	$(Q)echo "Done!"


dist:
	$(Q)mkdir -p dist/ARK150Addon660
	$(Q)mkdir -p dist/ARKInstaller150
	$(Q)mkdir -p dist/ARKInstaller150%


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
	$(Q)make -C Extras/Addon660


clean:
	$(Q)make -C Core/Rebootex/Rebootex150 clean
	$(Q)make -C Core/SystemControl150 clean
	$(Q)make -C Core/VSHControl150 clean
	$(Q)make -C Core/VSHMenu150 clean
	$(Q)make -C Core/TimeMachine150 clean
	$(Q)make -C Core/Reboot150 clean
	$(Q)make -C Core/MSIPL/mainbinex clean
	$(Q)make -C Core/MSIPL/stage1 clean
	$(Q)make -C Extras/Addon660 clean
	$(Q)rm -rf dist
