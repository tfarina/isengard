@echo off

if not exist ".\build" (
  md build\classes
)

javac -sourcepath com\zetcode -d build\classes com\zetcode\*.java
