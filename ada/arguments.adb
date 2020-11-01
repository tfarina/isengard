with Ada.Command_Line; use Ada.Command_Line;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Text_IO; use Ada.Text_IO;

procedure Arguments is
   Arguments : Natural;
begin
   Arguments := Argument_Count;
   Ada.Text_IO.Put("Argument Count: ");
   Ada.Integer_Text_IO.Put(Arguments);
   Ada.Text_IO.New_Line;
end Arguments;
