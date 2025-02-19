#!/bin/sh

rm -f test.output* test.timing*

echo "Get all nodes"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -A > test.output1 2> test.timing1
grep "real" test.timing1

echo "Get all attrs"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -l > test.output2 2> test.timing2
grep "real" test.timing2

echo "Get all attrs for 1 node"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -l mycluster1 > test.output3 2> test.timing3
grep "real" test.timing3

echo "Get all nodes with attr"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -q compute > test.output4 2> test.timing4
grep "real" test.timing4

echo "Get all values of altname"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -V altname > test.output5 2> test.timing5
grep "real" test.timing5
