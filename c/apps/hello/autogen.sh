#!/bin/sh

if [ -d po ] && command -v gettextize >/dev/null 2>&1; then
    echo "Running gettextize..."
    gettextize --force --copy || exit 1
fi

echo "running: aclocal"
aclocal
echo "running: autoconf"
autoconf
echo "running: header"
autoheader
echo "running: automake"
automake --add-missing
echo "Done."
