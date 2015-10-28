#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

javafiles="com/zetcode/HelloWorld.java"

mkdir -p output/classes
javac -sourcepath com/zetcode -d output/classes -source 1.8 -target 1.8 $javafiles
