#!/bin/bash

OUTFILE=top_picks_csv.dat

python3 process_CSV_qualtrics.py --file /Users/mverweij/wrk/UU/ExperimentalDesign/ProjectAllocation/ERP_test/ERPtest_April8_2025_07.08.csv --out $OUTFILE

./randomize_top_picks $OUTFILE

./matchmaking top_picks_shuffle.dat 10
