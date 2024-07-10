#!/bin/sh

echo "running: aclocal"
aclocal
echo "running: autoconf"
autoconf
echo "running: automake"
automake --add-missing
echo "Done."
