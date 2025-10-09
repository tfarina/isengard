#!/bin/sh

if [ -d po ] && command -v gettextize >/dev/null 2>&1; then
    echo "Running gettextize..."
    gettextize --force --copy || exit 1
fi

echo "Running autoreconf..."
autoreconf --verbose --force --install || exit 1

if [ -z "$NOCONFIGURE" ]; then
    echo "Running ./configure..."
    ./configure "$@"
else
    echo "Skipping configure process because NOCONFIGURE is set."
fi
