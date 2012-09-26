#include "heap.h"

#include <vector>
#include <string>

//Constructor, makes a newhasht table as well
heap::heap(int capacity) {
	mapping = new hashTable(capacity+1);
	heap::capacity = capacity;
	size = 0;
	data.resize(capacity+1);
	data[0].key = -1000;
}

//Deconstructor, must delete new mapping
heap::~heap() {
	delete mapping;
}

//Gets position of the given node within heap
int heap::getPos(node* pn) {
	int pos = pn - &data[0];
	return pos;
}

//Percolates up the node in given position, re-sets pointers within mapping to new positions
void heap::percup(int hole) {
	node tmp = data[hole];
	for (; hole > 1 && tmp.key < data[hole/2].key; hole /= 2) {
		data[hole] = data[hole/2];
		mapping->setPointer(data[hole].id, &data[hole]);
	}
	data[hole] = tmp;
	mapping->setPointer(data[hole].id, &data[hole]);
}

//Percolates down the node in given position, re-sets pointers within mapping to new positions
void heap::percdown(int hole) {
	int child;
	node tmp = data[hole];
	for (; hole*2 <= size; hole = child) {
		child = hole*2;
		if (child != size && data[child + 1].key < data[child].key )
			++child;
		if (data[child].key < tmp.key) {
			data[hole] = data[child];
			mapping->setPointer(data[hole].id, &data[hole]);
		}
		else
			break;
	}
	data[hole] = tmp;
	mapping->setPointer(data[hole].id, &data[hole]);
}

//Inserts given id, key, and pointer as a node into the heap and percolates up
//Returns 1 if heap is filled
//Returns 2 if heap already contains the id
//Returns 0 on Success
//Inserts into the mapping the id and a pointer to the nopde
int heap::insert(const std::string &id, int key, void *pv) {
	if (capacity <= 0)
		return 1;
	else if (mapping->contains(id) == false) {
		data[++size].id = id;
		data[size].key = key;
		data[size].pData = pv;
		
		mapping->insert(id, &data[size]);
		
		percup(size);
		--capacity;
		return 0;
	}
	else
		return 2;
}

//Sets key of given id
//Returns 1 if id is not in heap
//Returns 0 on succcess
//Percolates up/down depending on new key's size compared to old key
int heap::setKey(const std::string &id, int key) {
	node* keynode = static_cast<node *> (mapping->getPointer(id));
	
	if (keynode == NULL)
		return 1;
	else {
		int tmpkey = keynode->key;
		keynode->key = key;
		if (key < tmpkey)
			percup(getPos(keynode));
		else if (key > tmpkey) {
			percdown(getPos(keynode));
		}
		return 0;
	}
}

//Deletes node with minimum valued key, removes from mapping
//Returns 1 if heap is empty, returns 0 on success
//Replaces deleted node space with node at the end, percolates it down
int heap::deleteMin(std::string *pId, int *pKey, void *ppData) {
	
	if (size == 0)
		return 1;
	
	if (pId != NULL)
		*pId = data[1].id;
	if (pKey != NULL)
		*pKey = data[1].key;
	if (ppData != NULL)
		*(static_cast<void **> (ppData)) = data[1].pData;
	
	mapping->remove(data[1].id);
	
	if (size == 1) {
		--size;
	}
	else {
		data[1] = data[size--];
		percdown(1);
	}
	
	++capacity;
	
	return 0;
}

//Deletes node with given id, removes from mapping
//Returns 1 given id does not exist in heap, returns 0 on success
//Replaces deleted node space with node at the end, percolates it up/down
int heap::remove(const std::string &id, int *pKey, void *ppData) {

	node* tmpnode = static_cast<node *> (mapping->getPointer(id));
	
	if (tmpnode == NULL)
		return 1;
	
	int hole = getPos(tmpnode);
	
	if (pKey != NULL)
		*pKey = data[hole].key;
	if (ppData != NULL)
		*(static_cast<void **> (ppData)) = data[hole].pData;
	
	mapping->remove(data[hole].id);
	
	data[hole] = data[size--];
	percdown(hole);
	percup(hole);
	++capacity;
	
	return 0;
}
