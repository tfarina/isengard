-- http://perso.telecom-paristech.fr/~pautet/Ada95/e_c03_p1.ada

with Ada.Text_IO;         use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

procedure Int is
   Index : Integer;
begin
   Index := 23;
   Put("The value of Index is");
   Put(Index);                -- The default field width is 11 columns
   New_Line;
   Index := Index + 12;
   Put("The value of Index is");
   Put(Index, 8);
   New_Line;
end Int;
