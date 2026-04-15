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
	$(Q)mkdir -p dist/
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/FLASH150.ARK flash150.txt -s


clean:
	$(Q)make -C Core/Rebootex/Rebootex150 clean
	$(Q)make -C Core/SystemControl150 clean
	$(Q)make -C Core/VSHControl150 clean
	$(Q)make -C Core/VSHMenu150 clean
	$(Q)make -C Core/TimeMachine150 clean
	$(Q)rm -rf dist
