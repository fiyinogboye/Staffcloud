i#!/usr/bin/env bash

# Super simple coverage test script

set -e  # stop if anything fails

echo "== Compiling with gcov flags =="
gcc --coverage -o staffcloud *.c

echo "== Running program on all .txt files =="
for f in *.txt; do
  echo "---- Running with $f ----"
  ./staffcloud "$f"
done

echo "== Generating gcov reports for all .c files =="
gcov *.c

echo
echo "Done. Check the .gcov files to see line coverage (aim for >= 95%)."

