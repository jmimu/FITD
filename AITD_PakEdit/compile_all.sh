#! /bin/bash

set -e
set -x
umask 0000
export PATH=/usr/lib/mxe/usr/bin/:$PATH

version=`cat /src/AITD_PakEdit/version.h | grep "VERSION" | sed 's/.*VERSION "//' | sed 's/[^0-9.a-zA-Z_].*//'`

NBRP=$(cat /proc/cpuinfo | grep processor | wc -l)
GREEN='\033[0;32m'
STOP='\033[0m'


echo -e "${GREEN}Remove all${STOP}"
rm -Rf /src/AITD_PakEdit/autobuild/ /src/AITD_PakEdit/autobuild-mxe/

echo -e "${GREEN}Linux compilation${STOP}"
cd /src/AITD_PakEdit
lrelease AITD_PakEdit.pro
mkdir -p autobuild/
cd autobuild/
qmake ../AITD_PakEdit.pro CONFIG+=release
make clean
make -j$NBRP

echo -e "${GREEN}Copy AppImage tools...${STOP}"
cp /linuxdeploy-x86_64.AppImage /src/AITD_PakEdit/
cp /linuxdeploy-plugin-qt-x86_64.AppImage /src/AITD_PakEdit/

echo -e "${GREEN}Creating AppImage...${STOP}"
cd /src/AITD_PakEdit/
distrib/make_appimage.sh  /src/AITD_PakEdit/autobuild/
distrib/make_ziplinux.sh AITD_PakEdit_$version-x86_64.AppImage

echo -e "${GREEN}Windows cross-compilation${STOP}"
cd /src/AITD_PakEdit/
mkdir -p autobuild-mxe/
cd autobuild-mxe/
x86_64-w64-mingw32.static-qmake-qt5 ../AITD_PakEdit.pro CONFIG+=release
make clean
make -j$NBRP

echo -e "${GREEN}Creating windows zip...${STOP}"
cd /src/AITD_PakEdit/
distrib/make_zipwin.sh autobuild-mxe/

