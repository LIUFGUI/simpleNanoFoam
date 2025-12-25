#!/bin/bash
#

for i in processor*; do
	(cd $i && foamListTimes -rm)
done
rm -rf *Flow.out
rm -rf poroPerm.csv
foamListTimes -rm
rm -r 0
