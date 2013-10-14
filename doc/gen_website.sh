#!/bin/sh


#looping to find all files we need to generate documents for
FILES=`ls *.txt`

for f in $FILES
do
	echo "Processing $f file..."
	# take action on each file. $f store current file name
	python2.5 docutils/tools/rst2html.py $f > website/${f%.*}.html
done


