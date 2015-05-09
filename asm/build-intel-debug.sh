#!/bin/sh

# Die if any command dies.
set -e

as -mnaked-reg -msyntax=intel add_intel_x64.s -o add_def.o
clang -g -c add_main.c -o add_main.o
clang add_main.o add_def.o -o add_intel_debug
