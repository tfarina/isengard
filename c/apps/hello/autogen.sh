#!/bin/sh

echo "running: autopoint"
autopoint --force
echo "running: aclocal"
aclocal
echo "running: autoconf"
autoconf
echo "running: header"
autoheader
echo "running: automake"
automake --add-missing
echo "Done."
