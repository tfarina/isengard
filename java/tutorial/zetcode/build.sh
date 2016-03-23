#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

javafiles="src/com/zetcode/HelloWorld.java"

JAVA_VERSION=1.8
OUTPUT_CLASSES_DIR=output/classes

PARAMFILE=paramfile

mkdir -p ${OUTPUT_CLASSES_DIR}

find src/main/java -name "*.java" > $PARAMFILE

javac -sourcepath src \
      -d ${OUTPUT_CLASSES_DIR} \
      -source ${JAVA_VERSION} \
      -target ${JAVA_VERSION} \
      -encoding UTF-8 \
      $javafiles

javac -sourcepath src/main/java \
      -d ${OUTPUT_CLASSES_DIR} \
      -source ${JAVA_VERSION} \
      -target ${JAVA_VERSION} \
      -encoding UTF-8 \
      "@${PARAMFILE}"
