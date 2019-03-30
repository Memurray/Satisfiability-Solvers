#include <string>
#include <iostream>
#include <vector> 
#include <cstdlib>
#include <ctime>

using namespace std;
void readRedirect(vector<vector<int> > &formulas, int &nVars, int &nClauses){
	string input;
	int holder;
	cin >> input;
	while(input == "c") { //if line starts with c, throw away next 256 chars or until newline
		cin.ignore(256,'\n');
		cin >> input;
	}
	cin >> input; // effectively trashes this
	cin >> nVars; 
	cin >> nClauses;
	for (int i=0; i <nClauses; i++){
		vector<int> currLine;
		for (int j=0; j<3; j++){  //fill current interior vector with elements of this clause
			cin >> holder;  
			currLine.push_back(holder);
		}
		cin.ignore(256,'\n');  //ignore until next line
		formulas.push_back(currLine);  //put clause in outer vector
	}
}


int scoreAssignment(const vector<vector<int> > &formulas, const vector<int> &assignment, int nVars, int nClauses){
	int satClauses = 0;
	for(int i=0; i< nClauses; i++){ //check all literals in all clauses
		for(int j = 0; j< formulas[i].size(); j++){
			int val = abs(formulas[i][j]);  
			int sign = val/formulas[i][j];
			if (assignment[val-1] == sign){  //if assignment of literal matches the sign of the literal, clause is satisfied
				satClauses++;
				break;
			}			
		}
	}
	return satClauses;  //returns the number of satisfied clauses
}

void sortClauses (const vector<vector<int> > &formulas, const vector<int> &assignment, int nVars, int nClauses,vector<int> &satClauses,vector<int> &unsatClauses){
	
	for(int i=0; i< nClauses; i++){ // for all elements of all clauses
		for(int j = 0; j< 3; j++){
			int val = abs(formulas[i][j]);
			int sign = val/formulas[i][j];
			if (assignment[val-1] == sign){  //if clause is satisfied, add clause index to new vector for tracking SAT clauses
				satClauses.push_back(i);
				break;
			}
			else if(j == 2){  //if no matches by end of clause, clause is unsat, add clause index to vector tracking UNSAT clauses
				unsatClauses.push_back(i);
			}
		}
	}	
}

int bestSwap ( const vector<vector<int> > &formulas, const vector<int> &assignment, int swapClause, int nClauses,vector<int> &satClauses){
	int mostIntact =0;  
	int best = -1;
	for(int m = 0; m < 3; m++){
		int intact = 0;  //number of SAT clauses that remained SAT after swap
		int toggle = abs(formulas[swapClause][m])-1;  //index to be toggled on this loop
		vector<int> newAssignment = assignment;
		newAssignment[toggle] = newAssignment[toggle]*-1;  //change assignment based on what was selected

		for(int i=0; i < satClauses.size(); i++){	//checking only the SAT clauses		
			for(int j = 0; j< 3; j++){
				int x = satClauses[i];
				
				int val = abs(formulas[x][j]);  
				int sign = val/formulas[x][j];
				if (newAssignment[val-1] == sign){  //if signs match, clauses is intact
					intact ++;
					break;
				}				
			}			
		}		
		if (intact > mostIntact){  //for the 3 tests, track which one kept the most intact
			mostIntact = intact;
			best = abs(formulas[swapClause][m]);
		}		
	}
	return best;
}


int main() {
	srand(time(NULL));
	int nVars = 0;
	int nClauses = 0;
	int iterations = 0;
	vector<vector<int> > formulas;
	readRedirect(formulas,nVars,nClauses);
	vector<int> assignment;
	for(int i=0; i < nVars; i++){  //make random assignment
		assignment.push_back(rand() % 2);
		if(assignment[i] == 0)
			assignment[i]=-1;
		
	}
	
	int currScore = scoreAssignment(formulas,assignment,nVars,nClauses);
	
	if (currScore != nClauses){
		while (iterations < 250000){  //at most 250,000 attempts
			vector<int> satClauses,unsatClauses;
			sortClauses(formulas,assignment,nVars,nClauses,satClauses,unsatClauses);  //pbr sat and unsat clause vectors to fill them
			int clauseSelect = unsatClauses[rand() % unsatClauses.size()];  //pick a random unsat clause

			int best = bestSwap(formulas,assignment,clauseSelect,nClauses,satClauses);  //get best index to swap
			assignment[best -1] = assignment[best-1] * -1;  //swap that index
			int newScore = scoreAssignment(formulas,assignment,nVars,nClauses);  
			if(newScore > currScore) //if newscore is the best, save it
				currScore = newScore;
			iterations ++;
			if (currScore == nClauses) //if SAT, stop iterations
				break;
		}

	}
	if (currScore == nClauses) //return results
		cout << " SAT " << currScore;
	else
		cout << " UNSAT " << currScore;

return 0;
}
