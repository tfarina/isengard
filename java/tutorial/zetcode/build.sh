#!/bin/bash

# https://ant.apache.org/manual/tutorial-HelloWorldWithAnt.html

OUTPUT_CLASSES_DIR=output/classes
JAVA_VERSION=1.8
PARAMFILE=paramfile

mkdir -p ${OUTPUT_CLASSES_DIR}

find src/main/java -name "*.java" > $PARAMFILE

javac -sourcepath src/main/java \
      -d ${OUTPUT_CLASSES_DIR} \
      -source ${JAVA_VERSION} \
      -target ${JAVA_VERSION} \
      -encoding UTF-8 \
      "@${PARAMFILE}"
