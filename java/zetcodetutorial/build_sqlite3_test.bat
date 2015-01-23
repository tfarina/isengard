@echo off

if not exist ".\build" (
  md build\classes
)

javac -cp lib\* -sourcepath com\zetcode -d build\classes com\zetcode\Sqlite3Test.java
java -cp lib\*;build\classes com.zetcode.Sqlite3Test
