#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <string>
#include <list>
#include <fstream>
#include "hash.h"
#include "heap.h"

class graph {
	int size;	//Number of nodes in graph
	
	class node {
		public: 
		
		//wnode contains botha node pointer and the distance,
		//meant for adjacency list
		struct wnode {
			node* vertex;
			int dist;
		};
		
		std::string name;	//name of node
		std::list<wnode> adj;	//adjacency list
		bool known;	//if min dsitance is known or not
		int dist;	//min distance
		node* path;	//previous node in path to original node
	};
	
	std::list<node> data;	//graph list containing all nodes
	hashTable *mapping;	//hash table of graph
	heap *scrapHeap;	//heap of graph
	
	public:
	
	graph( std::ifstream &input );	//makes graph from input file
	~graph();	//Deconstructor
	void insert(std::string &one, std::string &two, int dist);	//adds path to adjacency list, adds new node if necessary
																//to graph list
	
	void dexter(std::string validnode);	//Dijstra's algorithm
	void heapItOn();	//make heap
	void moddist(node *v, int a);	//modify distance in heap
	std::string GoldenPath(node* v);	//output shortest path
	void outSource(std::ofstream &output);	//write shortest paths to output
	bool wheresWaldo(std::string &s);	//see if node exists in graph
	
};

#endif //_GRAPH_H
