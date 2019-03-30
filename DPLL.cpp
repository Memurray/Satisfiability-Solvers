#include <string>
#include <iostream>
#include <vector> 
#include <cmath>

using namespace std;
static int counter = 0;

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

bool checkSAT(vector<vector<int> > &formulas, int &nVars, int &nClauses){
	if(formulas.size() == 0)
		return true;
	vector<int> constantCheck(nVars+1);  //lookup vector based on whether current variable hasn't been seen (0)
										 //has been seen positive (1), or has been seen negative (-1)
	constantCheck.assign(nVars+1,0);
	int value, sign;
	for (int i = 0; i< nClauses; i++){
		for(int j =0; j < formulas[i].size(); j++){  //I will end up popping items so i need it to not seg fault with a static vector length
			value = formulas[i][j];  //save value of current inspection
			if (value > 0){  //positive assigned sign of 1
				sign = 1;
			}
			else{ // negative assigned as -1, change value to positive since i use it for index
				sign = -1;
				value = abs(value);
			}
			if (constantCheck[value] == 0)  //if current value has yet to be seen, mark sign
				constantCheck[value] = sign;
			else{  //if it has been seen
				if (constantCheck[value] != sign)  //if seen differs from current, this is not SAT and can return
					return false;
			}
		}
	}
	return true;  //if all elements checked without any literal disputes, then is SAT
}

bool checkUNSAT(vector<vector<int> > &formulas, int &nVars, int &nClauses){
	if(formulas.size() == 0)  //if there are no clauses, not-UNSAT
		return false;
	for (int i = 0; i < nClauses; i++){ //check each clause, if size is 0, UNSAT
		if(formulas[i].size() == 0){
			return true;
		}
	}
	return false; // If we make it here, no conclusion is drawn
}

bool unitProp(vector<vector<int> > &formulas, int &nVars, int &nClauses){
	bool loopCheck = true;
	while(loopCheck){
		for (int i = 0; i < nClauses; i++){  //check all clauses
			if(formulas[i].size()==1){  // if clause has only 1 element
				int hold = formulas[i][0];  //save the value in the clause
				nClauses--;  // reduce clause variable
				formulas.erase(formulas.begin() + i);  //delete the clause from the vector
				i--;  //move the clause iterator back 1
				for (int m =0; m < nClauses; m++){  //for all clauses
					if (formulas[m].size() == 0)  // if we enounter an empty clause, this is UNSAT, this is a minor short circuit
						return false;
					for (int n =0; n <formulas[m].size(); n++){  //for all elements in the clause
						if (formulas[m][n] == hold){  //if the element is the same as the unit literal
							formulas.erase(formulas.begin() + m);  //delete the clause
							nClauses --;  
							if( m <= i)  // if the clauses we're deleting is below position of iterator, move unit iterator back 1
								i--;
							m--;  //move matching iterator back 1 unconditionally
							break;
						}
						else if (formulas[m][n] == hold * -1){  //if not(unit literal)
							formulas[m].erase(formulas[m].begin() + n);  //delete the literal in current clause
							break;  
						}
					}
				}
			}
		}
		loopCheck = false;  // by default sets loop to finished, but
		for (int i = 0; i < nClauses; i++){  //if another unit literal is present after changes
			if(formulas[i].size()==1){
				loopCheck = true;  //loop enabled again for further unit prop
				break;
			}
		}
	}
	return true;
}

//reused checkSAT template
void pureLiteral(vector<vector<int> > &formulas, int &nVars, int &nClauses){
	vector<int> constantCheck(nVars+1);  //lookup vector based on whether current variable hasn't been seen (0)
										 //has been seen positive (1), or has been seen negative (-1), notPure(2)
	constantCheck.assign(nVars+1,0);
	int value, sign;
	for (int i = 0; i< nClauses; i++){
		for(int j =0; j < formulas[i].size(); j++){  //I will end up popping items so i need it to not seg fault with a static vector length
			value = formulas[i][j];  //save value of current inspection
			if (value > 0){  //positive assigned sign of 1
				sign = 1;
			}
			else{ // negative assigned as -1, change value to positive since i use it for index
				sign = -1;
				value = abs(value);
			}
			if (constantCheck[value] == 0)  //if current value has yet to be seen, mark sign
				constantCheck[value] = sign;
			else{  //if it has been seen
				if (constantCheck[value] != sign)  //if seen differs from current, not pure
					constantCheck[value] = 2;
			}
		}
	}

	for (int i = nClauses-1; i>=0; i--){  //once pure has been found, check all literals
		for (int j =0; j < formulas[i].size(); j++){
			if(abs(constantCheck[abs(formulas[i][j])]) == 1){    //if abs value of the current literal is a 1 or -1 in constant check then it's pure
				formulas.erase(formulas.begin() + i);  //erase the whole clause
				nClauses --;
				break;
			}

		}
			
	}
}

int mostOccurant(const vector<vector<int> > &formulas, int nVars, int nClauses){
	if(formulas.size() == 0)
		return -2;
	vector<int> constantCheck(nVars+1);  //lookup vector based on number of times seen regardless of sign
	constantCheck.assign(nVars+1,0);
	int value;
	int largestSeen = 0;
	int retVal = -1;
	for (int i = 0; i< nClauses; i++){
		for(int j =0; j < formulas[i].size(); j++){  //I will end up popping items so i need it to not seg fault with a static vector length
			value = abs(formulas[i][j]);  //save value of current inspection
			constantCheck[value] = constantCheck[value] + 1;
		}
	}
	for (int k=1; k<nVars+1; k++){  //goes through constant check looking for highest occurance
		if(constantCheck[k] > largestSeen){
			largestSeen = constantCheck[k];
			retVal = k;
		}
	}
	return retVal;  //returns literal that occurs the most counting both x and notx as occurance of x
}
	
vector<vector<int> > shortCircuit(const vector<vector<int> > &formulas,int sign, int sTarget){
	vector<vector<int> > v = formulas;
	int match = sTarget * sign;		//sign is 1 or -1 so match is sTarget or -sTarget depending on final arg

	for (int i = v.size()-1; i>=0; i--){//erase all clauses with a direct literal match
		for (int j =0; j < v[i].size(); j++){
			if(v[i][j] ==  match){
				v.erase(v.begin() + i);
				break;
			}
			else if(v[i][j] == match * -1){  //erase all occurances of inverse literal
				v[i].erase(v[i].begin() + j);
				break;
			}
		}			
	}
	return v;
}

bool dpll(const vector<vector<int> > &formulas,int nVars){
	vector<vector<int> > v = formulas;
	int nClauses = v.size();

	// Check UNSAT
	// I run UNSAT first just because it's quicker than SAT check and catches edge cases where SAT check may get confused 
	if(checkUNSAT(v,nVars,nClauses))  
		return false;
	//check if SAT
	if(checkSAT(v,nVars,nClauses))
		return true;
	//Perform unit propogation, if unit prop creates an empty clause I shortcircuit the process and declare UNSAT	
	if(!unitProp(v,nVars,nClauses))
		return false;
	//Remove pure literals
	pureLiteral(v,nVars,nClauses);
	// Determine the best literal to choose based on frequency of occurance, this is not random so multiple runs should result same each time
	int sTarget = mostOccurant(v,nVars,nClauses);
	if(sTarget == -2) // //mostOccurant will return -2 when the current formula contains no clauses, technically this saves only 1 iteration
		return true;
	//generate the modified vectors to be sent off recursively
	vector<vector<int> > v1 = shortCircuit(v,1,sTarget);  
	vector<vector<int> > v2 = shortCircuit(v,-1,sTarget);

	return (dpll(v1,nVars) || dpll(v2,nVars));

}

int main() {
	int nVars = 0;
	int nClauses = 0;
	vector<vector<int> > formulas;
	readRedirect(formulas,nVars,nClauses); //read in single cnf file
	
	cout << nVars << " " << nClauses << " ";
	if(dpll(formulas,nVars))
		cout << "SAT";
		
	else
		cout << "UNSAT";
	

return 0;
}
