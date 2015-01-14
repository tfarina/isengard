#!/bin/bash

# TODO(tfarina): Change this to install from a pinned version from the tarball
# instead: http://www.sfml-dev.org/files/SFML-2.2-linux-gcc-64-bit.tar.gz
git clone https://github.com/LaurentGomila/SFML.git
cd SFML
mkdir build
cd build
cmake -GNinja -DSFML_BUILD_EXAMPLES=true ..
ninja
sudo ninja install
