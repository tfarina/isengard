#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

mkdir -p build/classes
javac -sourcepath com/zetcode -d build/classes com/zetcode/HelloWorld.java
