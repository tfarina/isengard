@echo off

if not exist ".\output" (
  md output\classes
)

javac -sourcepath src -d output\classes src\com\zetcode\*.java
