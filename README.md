# ERP-matching
code to match student preferences with projects

## Prerequisites
* `C++20` compiler
* `python3`
* For visualization the `ROOT` package is used. You can get a procompiled version from here: https://root.cern/install/

## Installation
The C++ code can be compiled using the `Makefile`. Just type `make`.

## Dataset
### Pseudodata
You can generate a mock dataset using `generate_pseudodata.cc`
```sh
./generate_pseudodata
```
This generates a plain text file called `top_picks_gaus.dat` which can be inspected by just opening it.
For the pseudodata a top 3 of preferred projects for 42 student groups is randomly generated. The popularity of the projects is drawn from a Gaussian with a mean of 0.7 and standard deviation of 0.15. This means some projects will appear more often in the top 3 of the groups than others. 

### Survey data
The CSV file that will be obtained from `qualtrics` survey can be converted to plain text format with `process_CSV_qualtrics.cc` or `process_CSV_qualtrics.py`

## Matching projects to students
To avoid biases we first randomize the order in which groups are considered with `randomize_top_picks.cc`.
Next we will run the matching code (`matchmaking.cc`) many times (for example 100 thousand) to make sure we sample a large phase space of possible configurations.
The matching code performs the following steps:
1. Generate a random number, $A$, to select a project for the group being considered ($p_{i}$ is the probability to get the i<sup>th</sup> preferred project and is an adjustable parameter of the code)
  * $0<A<p_{1}$ → top 1 selected
  * $p_{1} \le A<(p_{1}+p_{2})$ → top 2 selected
  * $(p_{1}+p_{2}) \le A<(p_{1}+p_{2}+p_{3})$ → top 3 selected
2. Is the project already assigned to another group?
  * Yes: repeat step 1
  * No: assign project to group
3. If no project from top 3 is available, select a project randomly
4. Calculate happiness factor of the student grouop
  * {100,80,60,10} points for getting top {1,2,3,random} choice. Note: happiness scores are adjustable.
5. Calculate average happiness of the student population when all groups have an assigned project.
5. Repeat process thousands of times to find optimal configuration. For the optimal configuration we maximize the average happiness score.

The full workflow can be run using `runMatchmaking.sh`.
