#!/bin/sh

echo "Aclocal..."
aclocal
echo "Autoconf..."
autoconf
echo "Automake..."
automake --add-missing
echo "Done."
