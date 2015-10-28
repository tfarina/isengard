@echo off

if not exist ".\build" (
  md build\classes
)

javac -cp third_party\sqlite3\* -sourcepath com\zetcode -d build\classes com\zetcode\Sqlite3Test.java
java -cp third_party\sqlite3\*;build\classes com.zetcode.Sqlite3Test
