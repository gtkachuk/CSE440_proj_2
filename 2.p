program testIfThenElse;

class testIfThenElse

BEGIN
   VAR aa, bb: integer;

FUNCTION testIfThenElse;
BEGIN   
	aa := 7;
	if aa = 13 THEN
		BEGIN
			bb := 9
		END
	ELSE
		BEGIN
			bb := 11
		END;
	bb := 19
END
   
END   
.