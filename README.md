# ERP-matching
code to match student preferences with projects

## Prerequisites
For visualization the `ROOT` package is used. You can get a procompiled version from here: https://root.cern/install/

## Installation
The C++ code can be compiled using the `Makefile`. Just type `make`.

## Dataset
### Pseudodata
You can generate a mock dataset using `generate_pseudodata.cc`
```sh
./generate_pseudodata
```
This generates a plain text file called `top_picks_gaus.dat` which can be inspected by just opening it.
The pseudodata generates a top 3 of preferred projects for 42 student groups. The popularity of the projects is randomly generated by using a Gaussian with a mean of 0.7 and standard deviation of 0.15. This means some projects will appear more often in the top 3 of the groups than others. 

### Survey data
The CSV file that will be obtained from `qualtrics` survey can be converted to plain text format with `process_CSV_qualtrics.cc` or `process_CSV_qualtrics.py`

## Matching projects to students
To avoid biases we first randomize the order in which groups are considered with `randomize_top_picks.cc`.
Next we will run the matching code many times (for example 100 thousand) to make sure we sample a large phase space of possible configurations.
The matching code performs the following steps:
* Generate a random number, $A$, to select a project
  * $0<A<p_{1}$ → top 1 selected
  * $p_{1}<A<(p_{1}+p_{2})$ → top 2 selected
  * $(p_{1}+p_{2})<A<(p_{1}+p_{2}+p_{3})$ → top 3 selected
