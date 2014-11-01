program testIfThenElse;

class testIfThenElse

BEGIN
	VAR aa, bb, cc, dd, ee: integer;

	FUNCTION testIfThenElse;
	BEGIN   
		aa := 7;
  	bb := cc+dd;
    ee := cc+dd;
    aa := 777;
    bb := ee+123;
    if aa = 777 THEN
      BEGIN
        aa := ee+123;
        aa := aa+bb+dd
      END
    ELSE
      BEGIN
        ee := 111;
        bb := ee+123
      END;
    dd := cc+dd;
    bb := aa+bb+cc+8
	END
END   
.
