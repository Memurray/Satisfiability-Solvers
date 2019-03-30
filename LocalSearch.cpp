#include <string>
#include <iostream>
#include <vector> 
#include <cstdlib>
#include <ctime>

using namespace std;

static int fitness = 0;
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

// some relics of previous design present
int maxNeighbor(const vector<vector<int> > &formulas, vector<int> &assignment, int nVars, int nClauses){
	int bestNeighbor = -1;  // defaults to -1 meaning that no neighbor was superior
	int bestScore = scoreAssignment(formulas,assignment,nVars,nClauses);  //bestScore is currently based on current assignment
	vector<int> storeAssignment = assignment;  //copy original assignment
	for(int i=0; i< nVars; i++){  // for all variables that can be flipped
		vector<int> newAssignment = assignment;
		newAssignment[i] = newAssignment[i]*-1;  //generate an assignment that is 1 bit flipped
		int rScore = scoreAssignment(formulas,newAssignment,nVars,nClauses); //score this assign
		if (rScore > bestScore){ //if it improves
			bestScore = rScore;  //save as new best score
			storeAssignment = newAssignment;  //and update the stored assign
			bestNeighbor = 1;  //return value is now 1 meaning that we have changed the assignment, this could have been boolean but it made more sense in original design
		}
	}
	assignment = storeAssignment;  //since assignment was passed by reference updating assignment here to best neighbor will change main's assignment
	return bestNeighbor;  //returns -1 or 1 depending on whether we changed something

}

int main() {
	srand(time(NULL));
	int nVars = 0;
	int nClauses = 0;
	vector<vector<int> > formulas;
	readRedirect(formulas,nVars,nClauses);
	for (int runLoops = 0; runLoops < 500; runLoops++){  //500 random restarts
		vector<int> assignment;
		for(int i=0; i < nVars; i++){  //make a random assignment
			assignment.push_back(rand() % 2);
			if(assignment[i] == 0)
				assignment[i]=-1;
			
		}
		
		while (true){
			
			int bestNeighbor = maxNeighbor(formulas,assignment,nVars,nClauses);
			if( bestNeighbor <0){  //if no changes were made, we're at a local max
				int score = scoreAssignment(formulas,assignment,nVars,nClauses);
				if(score > fitness){  //if current restart is better than best previous, fitness set to this run
					fitness = score;
				}
				break;
			}
		}

		assignment.clear();
		if (fitness == nClauses){  //if SAT we dont need more restarts
			break;
		}
	}
	if (fitness == nClauses){
		cout << "SAT " << nClauses;
	}
	else{
		cout << "UNSAT " << fitness;
	}



return 0;
}
