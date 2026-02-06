#!/bin/sh

srcdir=`dirname "$0"`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd "$srcdir"

echo "Running autoreconf..."
autoreconf --verbose --force --install || exit 1

cd "$ORIGDIR" || exit $?

if [ -z "$NOCONFIGURE" ]; then
    echo "Running $srcdir/configure..."
    "$srcdir"/configure "$@"
else
    echo "Skipping configure process because NOCONFIGURE is set."
fi
