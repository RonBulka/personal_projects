#!/bin/bash
rm -f ./tests/example*.act
rm -f ./tests/n_example*.act
rm -f ./tests/small_btb_example*.act
rm -f ./tests/segel/segel_example*.act
rm -f ./input_examples/example*.act
rm -f ./150_tests/test*.act

make clean
make

for ((i=4; i<150; i++)); 
do
    # echo "Comparing actual and expected output for test $i:"
    ./bp_main tests/example${i}.trc > tests/example${i}.act
    diff -q tests/example${i}.out tests/example${i}.act
    ./bp_main tests/n_example${i}.trc > tests/n_example${i}.act
    diff -q tests/n_example${i}.out tests/n_example${i}.act
    ./bp_main tests/small_btb_example${i}.trc > tests/small_btb_example${i}.act
    diff -q tests/small_btb_example${i}.out tests/small_btb_example${i}.act
done
echo "done testing on tests/ directory"

for ((i=1; i<21; i++)); 
do
    # echo "Comparing actual and expected output for segel test $i:"
    ./bp_main tests/segel/example${i}.trc > tests/segel/segel_example${i}.act
    diff -q tests/segel/example${i}.out tests/segel/segel_example${i}.act
done
echo "done testing on tests/segel directory"

for ((i=1; i<4; i++));
do
    # echo "Comparing actual and expected output for input example $i:"
    ./bp_main input_examples/example${i}.trc > input_examples/example${i}.act
    diff -q input_examples/example${i}.out input_examples/example${i}.act
done
echo "done testing on input_examples/ directory"

for ((i=1; i<150; i++));
do
    # echo "comparing actual and expected output for 150_tests/test${i}:"
    ./bp_main 150_tests/test${i}.trc > 150_tests/test${i}.act
    diff -q 150_tests/test${i}.out 150_tests/test${i}.act 
done    
echo "done testing on 150_tests/ directory"	




