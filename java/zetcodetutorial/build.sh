#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

mkdir -p build/classes
javac -sourcepath com/zetcode -d build/classes -source 1.8 -target 1.8 com/zetcode/HelloWorld.java
