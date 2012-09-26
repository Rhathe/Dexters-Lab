#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>
#include "graph.h"

//maximum distance
#define OVER9000 1000000000

//Insert a new node into the graph
void graph::insert (std::string &one, std::string &two, int dist) {
	
	node* n1;
	node* n2;
	node::wnode wn1;
	node::wnode wn2;
	
	//Create and insert new node into graph list fro the first one if it doesn't already exist
	if (!mapping->contains(one)) {
		node tmp1;
		tmp1.name = one;
		tmp1.known = false;
		tmp1.dist = OVER9000;
		tmp1.path = NULL;
		data.push_back(tmp1);
		mapping->insert(one, &data.back());
		++size;
	}
	
	//Create and insert new node into graph list fro the second one if it doesn't already exist
	if (!mapping->contains(two)) {
		node tmp2;
		tmp2.name = two;
		tmp2.known = false;
		tmp2.dist = OVER9000;
		tmp2.path = NULL;
		data.push_back(tmp2);
		mapping->insert(two, &data.back());
		++size;
	}
	
	//Get the node pointers
	n1 = static_cast<node *> (mapping->getPointer(one));
	n2 = static_cast<node *> (mapping->getPointer(two));
	
	//Create new wnode with distance between nodes, put on adjacency list
	wn2.vertex = n2;
	wn2.dist = dist;
	n1->adj.push_back(wn2);
}


//From list of nodes in graph, create a heap
void graph::heapItOn() {
	scrapHeap = new heap(size + 1);
	
	std::list<node>::iterator it = data.begin();
	
	for(; it != data.end(); ++it) {
		scrapHeap->insert(it->name, OVER9000, &(*it));
	}
}

//Constructor
graph::graph(std::ifstream &input) {
	std::string one;
	std::string two;
	int dist;
	size = 0;
	mapping = new hashTable(1000);
	
	//Take a line from the input, update graph
	while(!input.eof()) {
		input >> one;
		input >> two;
		input >> dist;
		
		insert(one, two, dist);
	}
	
	heapItOn();	//makes the heap
}

//Deconstructor, must delete new heaps/hashes
graph::~graph() {
	delete scrapHeap;
	delete mapping;
}

//Modify the distance in the heap
void graph::moddist(node* v, int a) {
	v->dist = a;
	scrapHeap->setKey(v->name, a);
}

//Djikstra's algorithm
void graph::dexter(std::string v1) {
	
	//Get the node pointer of the given node
	node* v = static_cast<node *> (mapping->getPointer(v1));
	
	std::list<node::wnode>::iterator it;
	
	moddist(v, 0);	//distance from self is 0
	v->path = v;	//Previous is self
	
	for (;;) {
		//if there is no longer any nodes on the heap, break
		if (scrapHeap->deleteMin(NULL, NULL, &v) == 1)
			break;
		
		//if the distance of the minimum node is considereed infinite, break
		if (v->dist >= OVER9000)
			break;
		v->known = true;
		
		//Check every node in adjacency list of given node
		for (it = v->adj.begin(); it != v->adj.end(); ++it) {
			//if the vertex in the adjacency node has its min distance already, skip
			if (!(it->vertex->known)) {
				//Check if this path to this node is smaller than the previous path
				//Adjust distance and path if so
				if (v->dist + it->dist < it->vertex->dist) {
					moddist((it->vertex), v->dist + it->dist);
					it->vertex->path = v;
				}
			}
		}
	}
}

//Backtrack and find the shortest path from the two nodes, retrun a string
std::string graph::GoldenPath(node* v) {
	std::string s (v->name + ": ");
	std::string subs(v->name);
	
	//If there is no path, say there is no path
	if (v->known == false)
		s += "NO PATH\n";
	
	//Add node paths to string, going back until reaching
	//original node
	else {
		std::ostringstream buffer;
		buffer << v->dist;
		s += buffer.str() + " [";
		
		
		while (v->name != v->path->name) {
			subs.insert(0, v->path->name + ", ");
			v = v->path;
		}
		
		s += subs + "]\n";
	}
	
	return s;
}

//Write the shortest path to the output
void graph::outSource(std::ofstream &output) {
	std::list<node>::iterator it = data.begin();
	
	for(; it != data.end(); ++it) {
		output << GoldenPath(&(*it));
	}
}

//Check if the node name exists in the graph
bool graph::wheresWaldo(std::string &s) {
	return mapping->contains(s);
}

