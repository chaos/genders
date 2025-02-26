#!/bin/sh

rm -f test*.out test*.time

echo "Get all nodes"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -A > test1.out 2> test1.time
cat test1.out | sort > test1.out_sorted
diff test1.out_sorted test1.exp
if [ $? -ne 0 ]; then
    exit 1
fi
grep "real" test1.time

echo "Get all attrs"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -l > test2.out 2> test2.time
cat test2.out | sort > test2.out_sorted
diff test2.out_sorted test2.exp
if [ $? -ne 0 ]; then
    exit 1
fi
grep "real" test2.time

echo "Get all attrs for 1 node"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -l mycluster1 > test3.out 2> test3.time
cat test3.out | sort > test3.out_sorted
diff test3.out_sorted test3.exp
if [ $? -ne 0 ]; then
    exit 1
fi
grep "real" test3.time

echo "Get all nodes with attr"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -q compute > test4.out 2> test4.time
cat test4.out | sort > test4.out_sorted
diff test4.out_sorted test4.exp
if [ $? -ne 0 ]; then
    exit 1
fi
grep "real" test4.time

echo "Get all values of altname"

/usr/bin/time -p ../../nodeattr/nodeattr -f testdatabases/genders.16knodes_300attrs -V altname > test5.out 2> test5.time
cat test5.out | sort > test5.out_sorted
diff test5.out_sorted test5.exp
if [ $? -ne 0 ]; then
    exit 1
fi
grep "real" test5.time

exit 0
