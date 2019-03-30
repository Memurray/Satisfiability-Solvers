The code for each of the satisfiability solvers are implemented in their own .cpp files
DPLL.cpp
LocalSearch.cpp
WalkSAT.cpp

This code was intended to be run in a linux environment
From inside the directory containing these files,
All are compiled at the same time using the command:
make

The compiled programs are designed to take no arguments, 
instead reading from a file as user input achieved through redirection.

If testing a single formula on DPLL the command would be
./DPLL < <Relative File Path>

However for the purposes of this assignment where there are 350 different formulas to test
Shell scripts were written to automate this process.

Each solver has it's own testing scripts named:
Test_DPLL
Test_LocalSearch
Test_WalkSAT

These all work almost identically
Each will loop through each file in hardcoded folder names where either the CNF Formulas or the Hard CNF Formulas are
The associated program will be run on this file and the output will be sent to stdout
The only difference for Local Search and WalkSAT is due to the specification of this assignment
Each formula is run 10 times on the solver instead of the 1 time for DPLL.

Using the command:
./Test_DPLL > DPLL_Result
Will generate a file named DPLL_Results with each of the 350 formulas tested on DPLL.

For personal testing, it may be nessecary to manually set execute permission for the shell scripts.