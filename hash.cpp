#include "hash.h"

//List of prime numbers, each around twice as big as the last one
int prime[26] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
					196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653,
					100663319, 201326611, 402653189, 805306457, 1610612741};

//How may numbers are currently in prime number list
#define PRIMENUMSIZE 25

//Find prime number bigger or equal to twice the size
unsigned int hashTable::getPrime(int size) {
	for(int i = 0; i < PRIMENUMSIZE; ++i) {
		if (prime[i] >= 2*size) 
			return prime[i];
	}
	
	return 0;
}

//Constructor, make a hash table with a prime numbersize at least twice as big as given size,
//Loops through table to make sure all Occupied/Deleted flags are false at first, and pv points to NULL
hashTable::hashTable(int size) {
	unsigned int newsize = hashTable::getPrime(size);
	capacity = newsize;
	filled = 0;
	data.resize(newsize);
	for (int i = 0; i < newsize; ++i) {
		data[i].isOccupied = false;
		data[i].isDeleted = false;
		data[i].pv = NULL;
	}
}

//Inserts a string into hash table
int hashTable::insert(const std::string &key, void *pv) {
	
	unsigned int tablesize = (unsigned int) data.size();
	
	//If the table is half-filled with Occupied/Deleted entries, the table must be rehashed
	//If this fails, it returns a 2
	if (capacity <= tablesize/2) {
		if (rehash() == false)
			return 2;
		else {
			tablesize = (unsigned int) data.size();
		}
	}
	
	int index = hash(key);
	
	//Loops through table to find an empty space
	//POTENTIALLY INFINITE, THOUGH SHOULDN'T BE IN THIS CASE
	//Returns 1 in key already exists
	while (1) {
		if (data[index].isOccupied == false) {
			
			//decreases capacity if slot had a deleted value before
			if (data[index].isDeleted == false) {
				--capacity; }
			else {
				data[index].isDeleted = false; }
			
			++filled;
			data[index].key = key;
			data[index].isOccupied = true;
			data[index].pv = pv;
			return 0;
		}
		
		//If the key already exists, it returns a 1
		else if (key == data[index].key) {
			return 1;
		}
		
		else {
			//Increments by 1, Wraps around if at end of hash table
			index = (index + 1) % tablesize;
		}
	}
}

//Searches hash table if string is in there
//Returns true if found, else returns false
bool hashTable::contains(const std::string &key) {
	if (findPos(key) != -1)
		return true;
	else
		return false;
}

//Gets pointer variable in the node of the key given,
//returns NULL if unsuccesful
void* hashTable::getPointer(const std::string &key, bool *b) {
	int index = findPos(key);
	if (index == -1) {
		if (b != NULL)
			*b = false;
		return NULL;
	}
	else {
		if (b != NULL)
			*b = true;
		return data[index].pv;
	}
}

//Sets the pointer variable in the node of the key given,
//Returns 0 on success, 1 if key is not in hash
int hashTable::setPointer(const std::string &key, void *pv) {
	int index = findPos(key);
	if (index == -1)
		return 1;
	else {
		data[index].pv = pv;
		return 0;
	}
}

//Removes key from hash table
//Sets Occupied to false, Deleted to true, and returns true if succesful
//Otherwise returns false if key is not in table
bool hashTable::remove(const std::string &key) {
	int index = findPos(key);
	if (index == -1)
		return false;
	else {
		data[index].isOccupied = false;
		data[index].isDeleted = true;
		--filled;
		return true;
	}
}

//Hash function, bit shifts characters by a 1-3 bytes to make
//32-bit value, modulates by tablesize
//TAKES ALL CHARACTERS OF THE STRING FOR NOW, IF SPEED IS NEEDED, CONSIDER
//USING ONLY THE FIRST COUPLE CHARACTERS
int hashTable::hash(const std::string &key) {
	unsigned int tablesize = (unsigned int) data.size();
	int strlen = key.size();
	unsigned int hashedkey = 0;
	
	for ( int i = 0; i < strlen; ++i) {
		hashedkey += key[i] << ((i % 4)*8);
	}
	
	return (hashedkey % tablesize);
}

//Finds and returns position in table of string
//Returns -1 if unsuccessful
int hashTable::findPos(const std::string &key) {
	int index = hash(key);
	
	unsigned int tablesize = (unsigned int) data.size();
	
	//Loops through entire table, returns -1 if table spots are all Occupied or Deleted
	//but string cannot be found
	for ( unsigned int i = 0; i < tablesize; ++i) {
		
		if (data[index].isOccupied == false && data[index].isDeleted == false)
			return -1;
		else if (data[index].isDeleted == true)
			;
		else if (key == data[index].key)
			return index;
			
		//Increments by 1, Wraps around if at end of hash table
		index = (index + 1) % tablesize;
	}
	
	return -1;
}

//Resizes the table to a prime number more than twice as big as the hash table
//Returns true is succesful, else returns false
bool hashTable::rehash() {
	unsigned int size = (unsigned int)data.size();
	
	for(int i = 0; i < PRIMENUMSIZE; ++i) {
		
		//Makes a temporary hash table that's bigger, copies values from old hash into temp hash,
		//then copies back to original hash
		if (prime[i] >= 2*size) {
			hashTable temp(prime[i]);
			for (int j = 0; j < size; ++j) {
				if (data[j].isOccupied == 1)
					temp.insert(data[j].key, data[j].pv);
			}
			
			data = temp.data;
			capacity = temp.capacity;
			filled = temp.filled;
			
			return true;
		}
	}
	
	return false;
}
