#!/usr/bin/env python
import os,sys
import optparse
import csv

usage = 'usage: %prog [options]'
parser = optparse.OptionParser(usage)
parser.add_option('-f', '--file'      ,dest='file'  ,help='input csv file'     ,default='/Users/mverweij/wrk/UU/ExperimentalDesign/ProjectAllocation/ERP_test/ERPtest_April8_2025_07.08.csv', type='string')
parser.add_option('-o', '--out'      ,dest='out'  ,help='output file'     ,default='top_picks_csv.dat', type='string')
(opt, args) = parser.parse_args()

scriptFile = open('%s'%(opt.out), 'w')

with open(opt.file, newline='') as f:
    reader = csv.DictReader(f)
    next(reader, None) #skip the headers
    next(reader, None) #skip the headers
    for row in reader:
        print(row)
        print(f"group:  {row['Q1']}")
        print(f"{row['Q5']}")
        print(f"{row['Q2_1']}")
        print(f"{row['Q2_2']}")
        print(f"{row['Q2_3']}")

        scriptFile.write('# group %s email contactperson: %s\n' % (row['Q1'],row['Q5']))
        scriptFile.write('%s\n' % row['Q2_1'])
        scriptFile.write('%s\n' % row['Q2_2'])
        scriptFile.write('%s\n' % row['Q2_3'])

scriptFile.close()

