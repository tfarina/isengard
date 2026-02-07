#!/bin/sh

srcdir=$(dirname "$0")
test -z "$srcdir" && srcdir=.

origdir=$(pwd)
cd "$srcdir"

echo "Running autoreconf..."
autoreconf --verbose --force --install || exit 1

cd "$origdir" || exit $?

if [ -z "$NOCONFIGURE" ]; then
    echo "Running $srcdir/configure..."
    "$srcdir"/configure "$@"

    echo
    echo "Now type 'make' to compile." || exit 1
else
    echo "Skipping configure process."
fi
