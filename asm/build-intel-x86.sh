#!/bin/sh

# Die if any command dies.
set -e

as -msyntax=intel -mnaked-reg add_intel_x86.s -o add_intel_x86.o
