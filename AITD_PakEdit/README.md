AITD_PakEdit
============


Alone in the Dark PakEdit allows to modify PAK file for AITD and FITD.
It is based on yaz0r's FITD PAK reading implementation.


License
-------

FITD is distributed under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version. A copy of this license 
an be found in the file COPYING included with the source code of this
program.

Uses jsoncpp (github.com/open-source-parsers/jsoncpp).

Compiling
---------
Windows:
get qtCreator for mingw
get zlib source and complied there http://www.winimage.com/zLibDll/
Copy zlib.h and zlib_bor.lib in C:\libs\
Open .pro file
compile

Linux :
sudo apt install qtcreator zlib
qmake
make

Usage
-----
Open a database (AITD1_PAK_DB.json), open a PAK,
then choose one of the file, export it, modify it and
re-import it.

TODO
----
  * make user choose offset and width for bmp export
  * how to import bmp with offset?
  * export and import other types of files
  
