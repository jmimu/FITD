#!/bin/bash
#set -x
set -e

if [ ! -d distrib ]
then
  echo "Error ! Must be run from base directory"
  exit $E_BADARGS
fi

version=`cat version.h | grep "VERSION" | sed 's/.*VERSION "//' | sed 's/[^0-9.a-zA-Z_].*//'`
echo "version : $version"

echo "[Desktop Entry]" > distrib/AITD_PakEdit.desktop
echo "Version=1.0" >> distrib/AITD_PakEdit.desktop
echo "Name=AITD_PakEdit $version" >> distrib/AITD_PakEdit.desktop
echo "Type=Application" >> distrib/AITD_PakEdit.desktop
echo "Comment=Alone in the Dark PAK Editor" >> distrib/AITD_PakEdit.desktop
echo "Exec=AITD_PakEdit_$version" >> distrib/AITD_PakEdit.desktop
echo "Icon=AITD_PakEdit_$version" >> distrib/AITD_PakEdit.desktop
echo "Terminal=false" >> distrib/AITD_PakEdit.desktop
echo "Categories=Education;" >> distrib/AITD_PakEdit.desktop

