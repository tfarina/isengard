#!/bin/sh

# Die if any command dies.
set -e

as -mnaked-reg -msyntax=intel add-intel.s -o add_def.o
clang++ -g -c add_main.cc -o add_main.o
clang++ add_main.o add_def.o -o add_intel_debug
