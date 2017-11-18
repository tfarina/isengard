#!/bin/bash

# https://bitbucket.org/xerial/sqlite-jdbc#markdown-header-usage

mkdir -p output/classes

javac -classpath third_party/sqlite3/sqlite-jdbc-3.8.7.jar \
      -sourcepath com/zetcode \
      -d output/classes com/zetcode/Sqlite3Test.java

java -cp third_party/sqlite3/*:output/classes com.zetcode.Sqlite3Test
