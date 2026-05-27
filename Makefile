PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
BUILDTOOLS = $(PSPDEV)/share/psp-cfw-sdk/build-tools

.PHONY: dist Core Language Installers



all: dist Core Language Installers
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/FLASH150.ARK flash150.txt -s
	$(Q)cp Extras/660Addon/EBOOT.PBP dist/ARK150on660/
	$(Q)cp Extras/660Addon/Resources/LIBS/* dist/ARK150on660/
	$(Q)cp dist/FLASH150.ARK dist/ARK150on660/
	$(Q)cp dist/LANG150.ARK dist/ARK150on660/
	$(Q)echo "Done!"


dist:
	$(Q)mkdir -p dist/ARK150on660


Core:
	$(Q)make -C Core/Rebootex/Rebootex150
	$(Q)make -C Core/SystemControl150
	$(Q)make -C Core/VSHControl150
	$(Q)make -C Core/VSHMenu150
	$(Q)make -C Core/TimeMachine150
	$(Q)make -C Core/Reboot150
	$(Q)make -C Core/MSIPL/mainbinex
	$(Q)make -C Core/MSIPL/stage1


Language:
	$(Q)$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py Extras/Language/Translations/resources/satelite_chs_utf8.txt Extras/Language/quan.bdf Extras/Language/Translations/resources/satelite_chs.txt Extras/Language/Translations/resources/CHS.pf
	$(Q)$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py Extras/Language/Translations/resources/satelite_cht_utf8.txt Extras/Language/quan.bdf Extras/Language/Translations/resources/satelite_cht.txt Extras/Language/Translations/resources/CHT.pf
	$(Q)$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py Extras/Language/Translations/resources/satelite_jp_utf8.txt Extras/Language/quan.bdf Extras/Language/Translations/resources/satelite_jp.txt Extras/Language/Translations/resources/JP.pf
	$(Q)$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py Extras/Language/Translations/resources/satelite_kr_utf8.txt Extras/Language/quan.bdf Extras/Language/Translations/resources/satelite_kr.txt Extras/Language/Translations/resources/KR.pf
	$(Q)$(PY) $(BUILDTOOLS)/pack/pkg-res.py Extras/Language LANG150.ARK
	$(Q)mv Extras/Language/Translations/LANG150.ARK dist/


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
