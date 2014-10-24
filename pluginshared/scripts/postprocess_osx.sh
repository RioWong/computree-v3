#!/bin/bash

THISDIR="`pwd`"
DESTDIR="`pwd`/../../ComputreeInstall"

cd "$THISDIR"

# Generate translations
lrelease *.pro
mkdir -p $DESTDIR/languages
cp languages/*.qm $DESTDIR/languages

mkdir -p $DESTDIR/shaders
cp resources/shaders/*.* $DESTDIR/shaders