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
	$(Q)make -C Core/Reboot150
	$(Q)make -C Core/MSIPL/mainbinex
	$(Q)make -C Core/MSIPL/stage1


Addon660:
	$(Q)make -C Addon660


FlashPackage: Core
	$(Q)mkdir -p dist/
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/FLASH150.ARK flash150.txt -s


clean:
	$(Q)make -C Core/Rebootex/Rebootex150 clean
	$(Q)make -C Core/SystemControl150 clean
	$(Q)make -C Core/VSHControl150 clean
	$(Q)make -C Core/VSHMenu150 clean
	$(Q)make -C Core/TimeMachine150 clean
	$(Q)make -C Core/Reboot150 clean
	$(Q)make -C Core/MSIPL/mainbinex clean
	$(Q)make -C Core/MSIPL/stage1 clean
	$(Q)rm -rf dist
