PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
BUILDTOOLS = $(PSPDEV)/share/psp-cfw-sdk/build-tools
LANGFOLDER = Extras/Language/Translations/resources

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
	$(Q)make -C Core/Recovery150
	$(Q)make -C Core/TimeMachine150
	$(Q)make -C Core/Reboot150
	$(Q)make -C Core/MSIPL/mainbinex
	$(Q)make -C Core/MSIPL/stage1


Language:
	$(Q)$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py $(LANGFOLDER)/satelite_chs_utf8.txt Extras/Language/quan.bdf $(LANGFOLDER)/satelite_chs.txt $(LANGFOLDER)/CHS.pf
	$(Q)$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py $(LANGFOLDER)/satelite_cht_utf8.txt Extras/Language/quan.bdf $(LANGFOLDER)/satelite_cht.txt $(LANGFOLDER)/CHT.pf
	$(Q)$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py $(LANGFOLDER)/satelite_jp_utf8.txt Extras/Language/quan.bdf $(LANGFOLDER)/satelite_jp.txt $(LANGFOLDER)/JP.pf
	$(Q)$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py $(LANGFOLDER)/satelite_kr_utf8.txt Extras/Language/quan.bdf $(LANGFOLDER)/satelite_kr.txt $(LANGFOLDER)/KR.pf
	$(Q)$(PY) $(BUILDTOOLS)/pack/pkg-res.py Extras/Language LANG150.ARK
	$(Q)mv Extras/Language/Translations/LANG150.ARK dist/


Installers:
	$(Q)make -C Extras/660Addon


clean:
	$(Q)make -C Core/Rebootex/Rebootex150 clean
	$(Q)make -C Core/SystemControl150 clean
	$(Q)make -C Core/VSHControl150 clean
	$(Q)make -C Core/VSHMenu150 clean
	$(Q)make -C Core/Recovery150 clean
	$(Q)make -C Core/TimeMachine150 clean
	$(Q)make -C Core/Reboot150 clean
	$(Q)make -C Core/MSIPL/mainbinex clean
	$(Q)make -C Core/MSIPL/stage1 clean
	$(Q)make -C Extras/660Addon clean
	$(Q)rm -rf dist
