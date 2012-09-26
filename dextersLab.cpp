#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "graph.h"

using namespace std;

int main() {
	ifstream input;
	ofstream output;
	string dummy;
	
	cout << "Enter name of graph file: ";
	cin >> dummy;
	input.open(dummy.c_str());
	
	//error check to see if input can be opened
	if(!input.is_open()) {
		cerr << "DANGER DANGER DANGER WILL ROBINSON\n";
		exit(1);
	}
	
	//Make the graph
	graph TheWorld(input);
	
	input.close();
	
	cout << "Enter a valid vertex id for the starting vertex: ";
	cin >> dummy;
	
	//Checks to see if valid vertex was entered
	while(!TheWorld.wheresWaldo(dummy)) {
		cout << "Try Again: ";
		cin >> dummy;
	}
	
	//begin clock
	clock_t t1 = clock();
	
	//Apply Dijstra's algorithm
	TheWorld.dexter(dummy);
	
	//end clock
	clock_t t2 = clock();
	double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;
	
	cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << timeDiff << "\n";
	
	cout << "Enter name of output file: ";
	cin >> dummy;
	
	//write paths to output
	output.open(dummy.c_str());
	TheWorld.outSource(output);
	output.close();
}
