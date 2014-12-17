#!/bin/sh
LICENSE=`cat license-header.txt`
EXCLUDE=license-exclude.txt

LIST=`ls src/*.c src/*.h`

for i in $LIST; do
	FILE=`echo $i | cut -d "/" -f 2`
	if ! grep -c $FILE license-exclude.txt > /dev/null; then
		cp $i ./tmpfile
		echo "$LICENSE" > $i
		cat ./tmpfile | sed -e 's/\/\* Generated by XDS Modula-2 to ANSI C v4.20 translator \*\///1' >> $i
	fi
done
rm ./tmpfile

