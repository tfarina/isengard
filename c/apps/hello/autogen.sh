#!/bin/sh

srcdir=`dirname "$0"`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd "$srcdir"

if [ -d po ] && command -v gettextize >/dev/null 2>&1; then
    echo "Running gettextize..."
    gettextize --force --copy || exit 1
fi

echo "Running autoreconf..."
autoreconf --verbose --force --install || exit 1
cd "$ORIGDIR" || exit $?

if [ -z "$NOCONFIGURE" ]; then
    echo "Running ./configure..."
    "$srcdir"/configure "$@"
else
    echo "Skipping configure process because NOCONFIGURE is set."
fi
