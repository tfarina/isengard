#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

mkdir -p build/classes
javac -sourcepath src -d build/classes src/Foo.java
java -cp build/classes Foo
