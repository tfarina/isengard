#!/bin/bash

SFML_VERSION=SFML-2.2
wget http://www.sfml-dev.org/files/$SFML_VERSION-sources.zip
extract $SFML_VERSION-sources.zip
cd $SFML_VERSION
mkdir build
cd build
cmake -GNinja ..
ninja
sudo ninja install
