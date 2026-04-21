#!/usr/bin/env python
import os,sys
import argparse
import csv

parser = argparse.ArgumentParser()
parser.add_argument('-f', '--file'      ,dest='file'  ,help='input csv file'     ,default='/Users/mverweij/wrk/UU/ExperimentalDesign/ProjectAllocation/ERP_test/ERPtest_April8_2025_07.08.csv', type=str)
parser.add_argument('-o', '--out'       ,dest='out'   ,help='output file'     ,default='top_picks_csv.dat', type=str)
parser.add_argument('-n','--npref'      ,dest='npref' ,help='number of preferred projects in survey' , default=3, type=int) 
opt = parser.parse_args()

scriptFile = open('%s'%(opt.out), 'w')

with open(opt.file, newline='') as f:
    reader = csv.DictReader(f)
    next(reader, None) #skip the headers
    next(reader, None) #skip the headers
    for row in reader:
        print(row)
        print(f"group:  {row['Q1']}")
        print(f"{row['Q5']}")
        for i in range(1, opt.npref + 1):
            print(f"{row[f'Q2_{i}']}")

        scriptFile.write('# group %s  email contactperson: %s\n' % (row['Q1'],row['Q5']))
        for i in range(1, opt.npref + 1):
            scriptFile.write('%s\n' % row[f'Q2_{i}'])

scriptFile.close()

