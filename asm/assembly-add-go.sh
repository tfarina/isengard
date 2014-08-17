#!/bin/sh

# Die if any command dies.
set -e

go tool 6g -S add.go
