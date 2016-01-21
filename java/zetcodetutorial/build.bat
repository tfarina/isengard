@echo off

if not exist ".\output" (
  md output\classes
)

javac -sourcepath com\zetcode -d output\classes com\zetcode\HelloWorld.java
