AITD_PakEdit v0.02
==================

https://github.com/jmimu/FITD/tree/master/AITD_PakEdit


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

Getting Started
---------------
Make a copy of your original PAK files.
Run AITD_PakEdit.
Open a database corresponding to the game (AITD1_PAK_DB.json for AITD1).
Open a PAK.
You see every file in the PAK, with there description and type from the database.
You can modify them and overwrite database (and send it to me for next release!).
When selecting a file, you can export it depending of its type.
For now only pictures and texts can be exported.
You can then modify the exported files, and re-import them.
Overwrite the PAK file (a .PAK.BAK file will be created to save previous version).
Run the game with the new PAK.



TODO
----
  * text export: remove last char, convert to iso-8859-1 with dos2unix -850
  * text import: add last char, convert to dos page 850
  * import pal+pic
  * export floor: cam orient, metadata...
  * make camera render with perfect perspective
  * where are the musics?
  * export and import other types of files
  
