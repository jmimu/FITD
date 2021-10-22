FITD
====

https://github.com/jmimu/FITD

To be able to compile in 32 bits from 64 bit Ubuntu 20.04:

    sudo apt install libsdl1.2-dev:i386 libsdl-mixer1.2-dev:i386 libglu1-mesa-dev:i386 libzzip-dev:i386 libgc-dev:i386 libstdc++-dev:i386
    sudo apt-get install gcc-multilib g++-multilib


Original Readme from yaz0r
==========================

What is FITD
------------

FITD ("Free in the Dark") is an open source vesion of the
engine used in the Alone in the Dark (AITD) and sequels.
AITD_PakEdit allows to modify PAK file for AITD and FITD.

License
-------

FITD is distributed under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version. A copy of this license 
an be found in the file COPYING included with the source code of this
program.


*NIX version special notice:
----------------------------

This version had music diseabled at the last second. The music is still
enabled in the win32 build. If someone is interested to look for the
exact issue under *nix, please drop me a mail.

What is supported
-----------------

AITD1:
      Should be completable. Saving and loading is supported,
      but only one slot is available (the screen to select the
      save position is not implemented). It's possible to load
      a savegame from the original game, provided it's file is
      named "save0.itd" (rename it if necessary). Like wise,
      it is possible to load a FITD save in the original engine.
      There is a big issue concerning the projectils. Basically,
      guns, won't work, and throwing objects may produce strange
      result. Save before throwing anything, as the object may
      apprear in a wall...

AITD2:
      Supported but uncompletable. The introduction is buggy
      due to a regression in the script engine. Hit 'esc' to skip
      the intro (both parts) to bypass the bug .Save/load is
      unsupported.

JACK:
      Supported but uncompletable.
      Save/load is unsupported.

AITD3:
      Supported but uncompletable.
      Save/load is unsupported.

How to use FITD
---------------

The depences are : qmake, sdl, sdl-mixer, glu, zlib
Build (with qmake and make) and put the binary in the game
data directory (where all the .pak files are) and start FITD.
The CD version of AITD1 wasn't tested and may not work.

Version 0.1 Authors
-------------------

- Vincent Hamm "yaz0r" (yazoo@yaz0r.net)
      Project leader and main programmer

- Nicolas Noble "Pixel"
      Cross platform issues. Helped understanding some
      complex aspect of the 3d engine


Greetings
---------

- All the Alone in the Dark 1 team:
      Frederick Raynal
      Yael Barroz
      Didier Chanfray
      Philippe Vachey
      Franck De Girolami
      Jean-Marc Torroella
      Hubert Chardot
      Franck Manzetti

- Laurent Paret, you know what for ;)

- Sebastien Viannay, maybe this project wouldn't
      have existed without you...

- All those I forgot....

