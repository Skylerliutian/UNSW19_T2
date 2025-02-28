#!/bin/sh

QNAME="puzzle"
BIN="puzzle"

echo "***  Testing $QNAME  ***"

if [ ! -f "$BIN" ]
then
	echo "No such executable: $BIN"
	exit 1
fi

if [ ! -x "$BIN" ]
then
	echo "$BIN is not executable"
	exit 1
fi

if [ ! -d Tests ]
then
	echo "No Tests/ directory here. Are you in the right directory?"
	exit 1
fi

echo "Usage-1: autotest  "
echo "Usage-2: autotest <test-file name> "

filter="Tests/*.inp"

if test $# -gt 0
then
    $filter="$@"
fi

for pattern in "$filter"
do
    IFS=$'\n'
    for file in $(find $pattern -type f|sort);
    do

        file_name=$(basename "$file" .inp)

        ./$BIN < "$file" | head -500 > Tests/$file_name.temp
     	echo " "
     	echo " "
     	echo "------------------------------ "
     	if cmp -s Tests/$file_name.temp Tests/$file_name.out
     	then
     		echo "** Passed Test $file_name"
     	else
     		echo "** Failed Test $file_name"
     		echo "> Your output (in Tests/$file_name.temp): "
     		cat  Tests/$file_name.temp
     		echo "> #### Expected output (in Tests/$file_name.out): "
     		cat  Tests/$file_name.out
     		echo "> Compare files Tests/$file_name.temp and Tests/$file_name.out to see differences"

     	fi
     	echo "------------------------------ "

    done
    unset IFS
done

echo "        "
