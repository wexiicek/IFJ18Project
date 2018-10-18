#/bin/bash

# IFJ18 - Priklady pro studenty pro dokresleni zadani

COMPILER=cat  # zde mùze byt vas prekladac IFJ18 (napr. ./ifj18)
INTERPRETER="ruby -r ./ifj18.rb"  # zde muze byt interpret IFJcode18 (./ic18int)

for i in *.src; do

	IN=`echo $i | sed 's/\(.*\)\.src/\1.in/g'`
	OUT=`echo $i | sed 's/\(.*\)\.src/\1.out/g'`
	PRG=`echo $i | sed 's/\(.*\)\.src/\1.prg/g'`

	$COMPILER $i > $PRG   # nekontroluji se chyby pekladu (pro chybove testy nutno rozsirit)
	RETURNED_COMPILER=$?

	echo -n "DIFF: $INTERPRETER $i: "
	cat $IN | $INTERPRETER $PRG | diff - $OUT > /dev/null 2> /dev/null
	DIFFOK=$?
	if [ "x$DIFFOK" == "x0" ]; then
		echo "OK"
	else
		echo "DIFFERENCE to $OUT"
		cat $IN | $INTERPRETER $PRG | diff - $OUT
	fi
done;

rm *.prg

exit 0
