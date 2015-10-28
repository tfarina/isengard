#!/bin/bash

# https://bitbucket.org/xerial/sqlite-jdbc#markdown-header-usage

mkdir -p output/classes
javac -cp lib/* -sourcepath com/zetcode -d output/classes com/zetcode/Sqlite3Test.java
java -cp lib/*:output/classes com.zetcode.Sqlite3Test
