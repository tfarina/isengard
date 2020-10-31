with Ada.Environment_Variables;
with Ada.Text_IO; use Ada.Text_IO;

procedure Home is
begin
   if Ada.Environment_Variables.Exists("HOME") then
      Put_Line(Ada.Environment_Variables.Value("HOME"));
   end if;
end Home;
