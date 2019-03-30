all: DPLL LocalSearch WalkSAT

DPLL: DPLL.cpp
	g++ -O3 -o DPLL DPLL.cpp

LocalSearch: LocalSearch.cpp
	g++ -O3 -o LocalSearch LocalSearch.cpp

WalkSAT: WalkSAT.cpp
	g++ -O3 -o WalkSAT WalkSAT.cpp

clean:
	rm DPLL
	rm LocalSearch
	rm WalkSAT
