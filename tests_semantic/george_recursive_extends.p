program testValidClassDefinition;

class testValidClassDefinition

BEGIN
   
   VAR
      lightSwitch	      : boolean;
      compilerWorks	      : boolean;
      numberOfLines	      : integer;
      numberOfHours	      : integer;
      timeToWrite	      : ARRAY[4..9] OF integer;
      timeToTest	      : ARRAY[9..12] OF integer;

FUNCTION testValidClassDefinition;
BEGIN
   lightSwitch := compilerWorks;
   numberOfHours := 8;
   numberOfLines := 7593
END

END

class testExtendsOne extends testValidClassDefinition

BEGIN

  VAR 
    extendsOne  : integer;




END

class testExtendsTwo extends testExtendsOne

BEGIN 

  VAR
    extendsTwo  : integer;


END

.

