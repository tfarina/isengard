#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

mkdir -p build/classes
javac -sourcepath com/zetcode -d build/classes com/zetcode/EmptyWindow.java
java -cp build/classes com.zetcode.EmptyWindow
