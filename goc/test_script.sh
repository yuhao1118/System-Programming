#!/bin/bash
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

./gameoflife -i glider.txt
if [ $? -eq 0 ]
then
  echo Test passed: 1 input file
else
  echo Test failed: 1 input file
fi

./gameoflife -i glider.txt -z
if [ $? -eq 1 ]
then
  echo Test passed: Correct error code 1
else
  echo Test failed: Wrong error code 1
fi


./gameoflife -i glider.txt -g 7 -g 8
if [ $? -eq 2 ]
then
  echo Test passed: Correct error code 2
else
  echo Test failed: Wrong error code 2
fi

./gameoflife -i test_non_existing_input_file
if [ $? -eq 3 ]
then
  echo Test passed: Correct error code 3
else
  echo Test failed: Wrong error code 3
fi

diff  --ignore-trailing-space --ignore-blank-lines -q output.txt glider_output.txt
if [ $? -eq 0 ]
then
  echo Test passed: Correct output
else
  echo Test failed: Wrong output
fi
