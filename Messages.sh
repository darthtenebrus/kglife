#!/usr/bin/env bash

# call xgettext on all source files. If your sources have other filename
# extensions besides .cc, .cpp, and .h, just add them in the find call.
$EXTRACTRC $(find . -name '*.rc') >> rc.cpp
$XGETTEXT $(find . -name \*.c -o -name \*.cpp -o -name \*.h -o -name \*.qml) -o $podir/kglife.pot
rm -f rc.cpp
