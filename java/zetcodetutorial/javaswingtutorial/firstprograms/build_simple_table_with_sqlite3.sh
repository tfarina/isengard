#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

mkdir -p build/classes
javac -cp lib/* -sourcepath com/zetcode -d build/classes com/zetcode/SimpleTableWithSqlite3.java
