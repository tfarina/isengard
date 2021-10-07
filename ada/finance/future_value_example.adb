with Ada.Text_IO; use Ada.Text_IO;

procedure Future_Value_Example is
  Fv : Float;

  function FutureValue(presentValue, interestRate : Float; n : Natural) return Float is
  begin
    return presentValue * ((1.0 + interestRate) ** n);
  end FutureValue;
   
begin
  Fv := FutureValue(2.20, 0.0972, 10);
  Put_Line(Float'Image(Fv));   
end Future_Value_Example;
