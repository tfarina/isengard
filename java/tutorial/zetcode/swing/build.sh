#!/bin/bash

mkdir -p output/classes

javac -sourcepath src -d output/classes src/com/zetcode/*.java
