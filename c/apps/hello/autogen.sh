#!/bin/sh

if [ -d po ] && command -v gettextize >/dev/null 2>&1; then
    echo "Running gettextize..."
    gettextize --force --copy || exit 1
fi

echo "Running autoreconf..."
autoreconf --verbose --force --install || exit 1
