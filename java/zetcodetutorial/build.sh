#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

javafiles="com/zetcode/HelloWorld.java"

JAVA_VERSION=1.8

mkdir -p output/classes
javac -sourcepath com/zetcode \
      -d output/classes\
      -source ${JAVA_VERSION} \
      -target ${JAVA_VERSION} \
      -encoding UTF-8 \
      $javafiles
