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

dir_name=AITD_PakEdit_${version}_win

echo "dir_name: " $dir_name
rm -f $base/${dir_name}.zip
cd $1
  rm -rf $dir_name
  mkdir $dir_name
  cp release/AITD_PakEdit.exe $dir_name
  cp $base/LICENSE.txt $base/README.md $dir_name
  cp $base/*.json $dir_name
  rm -Rf $base/io_alone/__pycache__
  cp -R $base/io_alone/ $dir_name
  cp $base/AITD_PakEdit.xpm $dir_name
  zip -r $base/${dir_name}.zip $dir_name
cd -

echo "Zip created: " $base/${dir_name}.zip


