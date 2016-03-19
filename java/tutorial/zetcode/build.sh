#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

javafiles="src/com/zetcode/HelloWorld.java"
javafiles_new="src/main/java/com/zetcode/HelloWorld.java"

JAVA_VERSION=1.8

mkdir -p output/classes

javac -sourcepath src \
      -d output/classes\
      -source ${JAVA_VERSION} \
      -target ${JAVA_VERSION} \
      -encoding UTF-8 \
      $javafiles

javac -sourcepath src/main/java \
      -d output/classes\
      -source ${JAVA_VERSION} \
      -target ${JAVA_VERSION} \
      -encoding UTF-8 \
      $javafiles_new
