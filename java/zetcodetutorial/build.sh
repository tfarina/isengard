#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

mkdir -p output/classes
javac -sourcepath com/zetcode -d output/classes -source 1.8 -target 1.8 com/zetcode/HelloWorld.java
