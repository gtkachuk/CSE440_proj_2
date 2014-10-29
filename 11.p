program testIfThenElse;

class testIfThenElse

BEGIN
	VAR aa, bb, cc, dd: integer;

	FUNCTION testIfThenElse;
	BEGIN   
		aa := 7;
  	bb := 9;
		GOTO ll;
    aa := 777;
    if aa = 777 THEN
      BEGIN
        bb := 9;
  ll:   cc := 1
      END
    ELSE
      BEGIN
        bb := 99
      END;
    dd := cc
	END
END   
.
