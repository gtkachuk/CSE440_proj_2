program testValidClassDefinition;

class ta

BEGIN
   
   VAR
      lightSwitch	      : boolean;
      compilerWorks	      : boolean;
      numberOfLines	      : integer;
      numberOfHours	      : integer;
      timeToWrite	      : ARRAY[4..9] OF integer;
      timeToTest	      : ARRAY[9..12] OF integer;
      v1 : integer;
      v2 : boolean;

FUNCTION testValidClassDefinition;
BEGIN
   lightSwitch := compilerWorks;
   numberOfHours := 8;
   numberOfLines := 7593
END

END

class ta1 extends ta

BEGIN

  VAR 
    ex1  : ta;
    va1  : ta;

END

class ta2 extends ta1

BEGIN

  VAR 
    ex2  : ta1;
    va2  : ta;

END

class ta3 extends ta2

BEGIN 

  VAR
    ex3  : ta2;
    v1  : ta1;
    extendsTwo  : integer;

FUNCTION testLocalDeclared(da : integer): integer;
VAR 
    de : integer;

BEGIN
    de := 5
END

END

.

