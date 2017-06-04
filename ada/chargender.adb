-- http://www.functionx.com/ada/Lesson02.htm

with Ada.Text_IO;
use Ada.Text_IO;

procedure CharGender is
   gender : character := 'M';
begin
   Put_Line("Gender = " & character'image(gender));
end CharGender;
