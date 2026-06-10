PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
BUILDTOOLS = $(PSPDEV)/share/psp-cfw-sdk/build-tools
LANGFOLDER = Extras/Language/Translations/resources

.PHONY: dist Core Language Installers



all: dist Core Language Installers
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/FLASH150.ARK flash150.txt -s
	cp Extras/660Addon/EBOOT.PBP dist/ARK150on660/
	cp Extras/660Addon/Resources/LIBS/* dist/ARK150on660/
	cp dist/FLASH150.ARK dist/ARK150on660/
	cp dist/LANG150.ARK dist/ARK150on660/
	echo "Done!"


dist:
	mkdir -p dist/ARK150on660


Core:
	make -C Core/Rebootex/Rebootex150
	make -C Core/SystemControl150
	make -C Core/VSHControl150
	make -C Core/VSHMenu150
	make -C Core/Recovery150
	make -C Core/TimeMachine150
	make -C Core/Reboot150
	make -C Core/MSIPL/mainbinex
	make -C Core/MSIPL/stage1


Language:
	$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py $(LANGFOLDER)/satelite_chs_utf8.txt Extras/Language/quan.bdf $(LANGFOLDER)/satelite_chs.txt $(LANGFOLDER)/CHS.pf
	$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py $(LANGFOLDER)/satelite_cht_utf8.txt Extras/Language/quan.bdf $(LANGFOLDER)/satelite_cht.txt $(LANGFOLDER)/CHT.pf
	$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py $(LANGFOLDER)/satelite_jp_utf8.txt Extras/Language/quan.bdf $(LANGFOLDER)/satelite_jp.txt $(LANGFOLDER)/JP.pf
	$(PY) $(BUILDTOOLS)/pftools/bdf_to_pf.py $(LANGFOLDER)/satelite_kr_utf8.txt Extras/Language/quan.bdf $(LANGFOLDER)/satelite_kr.txt $(LANGFOLDER)/KR.pf
	$(PY) $(BUILDTOOLS)/pack/pkg-res.py Extras/Language LANG150.ARK
	mv Extras/Language/Translations/LANG150.ARK dist/


Installers:
	make -C Extras/660Addon


clean:
	make -C Core/Rebootex/Rebootex150 clean
	make -C Core/SystemControl150 clean
	make -C Core/VSHControl150 clean
	make -C Core/VSHMenu150 clean
	make -C Core/Recovery150 clean
	make -C Core/TimeMachine150 clean
	make -C Core/Reboot150 clean
	make -C Core/MSIPL/mainbinex clean
	make -C Core/MSIPL/stage1 clean
	make -C Extras/660Addon clean
	rm -rf dist
