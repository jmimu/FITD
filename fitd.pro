TEMPLATE = app
CONFIG += debug
CONFIG -= qt

QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wunused-but-set-parameter -Wunused-but-set-variable

DEFINES += INTERNAL_DEBUGGER

SOURCES += \
    src/zv.c \
    src/videoMode.c \
    src/version.c \
    src/vars.c \
    src/unpack.c \
    src/track.c \
    src/freezetime.c \
    src/threadCode.c \
    src/tatou.c \
    src/systemMenu.c \
    src/startupMenu.c \
    src/sequence.c \
    src/screen.c \
    src/save.c \
    src/room.c \
    src/renderer.c \
    src/polys.c \
    src/pak.c \
    src/osystemSDL_GL.c \
    src/osystemSDL.c \
    src/object.c \
    src/music.c \
    src/mainLoop.c \
    src/main.c \
    src/lines.c \
    src/lifeMacroTable.c \
    src/life_2.c \
    src/life.c \
    src/inventory.c \
    src/input.c \
    src/hqr.c \
    src/font.c \
    src/fmopl.c \
    src/floor.c \
    src/fileAccess.c \
    src/evalVar.c \
    src/debugger.c \
    src/debugFont.c \
    src/cosTable.c \
    src/bmp.c \
    src/animAction.c \
    src/anim.c \
    src/aitdBox.c \
    src/actorList.c


HEADERS += \
    src/zv.h \
    src/videoMode.h \
    src/version.h \
    src/vars.h \
    src/unpack.h \
    src/track.h \
    src/freezetime.h \
    src/threadCode.h \
    src/tatou.h \
    src/systemMenu.h \
    src/startupMenu.h \
    src/screen.h \
    src/save.h \
    src/room.h \
    src/renderer.h \
    src/pak.h \
    src/osystem.h \
    src/object.h \
    src/music.h \
    src/mainLoop.h \
    src/main.h \
    src/life_2.h \
    src/life.h \
    src/inventory.h \
    src/input.h \
    src/hqr.h \
    src/font.h \
    src/fmopl.h \
    src/floor.h \
    src/fileAccess.h \
    src/evalVar.h \
    src/debugger.h \
    src/debugFont.h \
    src/common.h \
    src/bmp.h \
    src/animAction.h \
    src/anim.h \
    src/aitdBox.h \
    src/actorList.h \
    src/types.h

LIBS +=  -lSDL -lSDL_mixer -lGL -lGLU -lz -L/usr/lib/i386-linux-gnu
INCLUDEPATH += "/usr/include/SDL"

QMAKE_LFLAGS += -m32
QMAKE_CFLAGS += -m32
