#!/bin/bash

FILE_PATH=$1

# I should remove null-strings:
sed -i '/null/d' ${FILE_PATH}
