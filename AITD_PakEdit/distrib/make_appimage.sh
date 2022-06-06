#!/bin/bash
#set -x
set -e

if [ $# -ne 1 ]
then
  echo "Usage: `basename $0` build_dir"
  exit $E_BADARGS
fi

if [ ! -d distrib ]
then
  echo "Error ! Must be run from base directory"
  exit $E_BADARGS
fi


base=$(pwd)
echo "base dir: $base"
version=`cat version.h | grep "VERSION" | sed 's/.*VERSION "//' | sed 's/[^0-9.a-zA-Z_].*//'`
echo "version : $version"
distrib/make_shortcut.sh

cd $1
  rm -Rf AppDir/
  make -j5
  make install INSTALL_ROOT=AppDir
  mkdir -p AppDir/usr/share/applications/
  mkdir -p AppDir/usr/share/pixmaps/
  cp AppDir/usr/bin/AITD_PakEdit AppDir/usr/bin/AITD_PakEdit_$version
  cp $base/distrib/AITD_PakEdit.desktop AppDir/usr/share/applications/AITD_PakEdit_$version.desktop
  cp $base/AITD_PakEdit.xpm AppDir/usr/share/pixmaps/AITD_PakEdit_$version.xpm
cd -
./linuxdeploy-x86_64.AppImage --appdir $1/AppDir --output appimage --plugin qt

echo "Ok!"


