TARGET=qsPSP
OBJS=../mgf/vram.o ../mgf/mgfutils.o ../mgf/mgftexture.o ../mgf/tgatexture.o ../mgf/pngtexture.o ../mgf/jpgtexture.o ../mgf/mgfcallback.o ../mgf/mgf.o ../mgf/mgfsprite.o ../mgf/mgffont.o ../mgf/mp3player.o ../mgf/mgfaudio.o qsp/onig/regenc.o qsp/onig/enc/ascii.o qsp/onig/enc/cp1251.o qsp/onig/enc/koi8_r.o qsp/onig/enc/unicode.o qsp/onig/enc/utf16_le.o qsp/onig/enc/utf32_le.o qsp/onig/regcomp.o qsp/onig/regerror.o qsp/onig/regexec.o qsp/onig/regparse.o qsp/onig/regsyntax.o qsp/onig/regtrav.o qsp/onig/regversion.o qsp/onig/st.o qsp/_control.o qsp/game.o qsp/text.o qsp/callbacks.o qsp/actions.o qsp/codetools.o qsp/coding.o qsp/common.o qsp/locations.o qsp/mathops.o qsp/objects.o qsp/playlist.o qsp/statements.o qsp/variables.o qsp/variant.o qsp/errors.o qsp/menu.o qsp/time.o src/mhalist.o src/widget.o src/mainwidget.o src/maindescwidget.o src/varsdescwidget.o src/actionswidget.o src/objectswidget.o src/messageboxwidget.o src/inputboxwidget.o src/gamebrowserwidget.o src/savebrowserwidget.o src/objectmenuwidget.o src/picviewwidget.o src/appdialog.o src/main.o src/app.o
INCDIR =../mgf src
CFLAGS=-G0  -O2
CXXFLAGS=$(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS=$(CFLAGS)
BUILD_PRX=1
PSP_FW_VERSION=371
LIBDIR=
LDFLAGS=
LIBS=-lstdc++ -lpspgum -lpspgu -lpng -lz -ljpeg -lpsprtc -lm -lmad -lmikmod -lpspaudiolib -lpspaudio -lpsppower
EXTRA_TARGETS=EBOOT.PBP
PSP_EBOOT_TITLE=qsPSP 0.9.6 (QSP 5.6.3)
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
