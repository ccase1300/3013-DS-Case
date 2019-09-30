/*
* Name: Carter Case
* Due Date: 9/30/19
* Course: CMPS 3013
* Program: A03
* Description:
* This program will read in the information of certain
* animals from a json file, and then put each animal 
* into an array of heaps based on said information. 
* The program will then sort each individual heap in 
* order of priority, and then print the top five animals
* in each heap. 
*/


#include "json_helper.cpp"
#include <fstream>
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;

const static double EarthRadiusKm = 6372.8;

inline double DegreeToRadian(double angle) {
	return M_PI * angle / 180.0;
}

class Coordinate {
public:
	Coordinate(double latitude, double longitude) : myLatitude(latitude), myLongitude(longitude) {}

	double Latitude() const {
		return myLatitude;
	}

	double Longitude() const {
		return myLongitude;
	}

private:
	double myLatitude;
	double myLongitude;
};

double HaversineDistance(const Coordinate &p1, const Coordinate &p2) {
	double latRad1 = DegreeToRadian(p1.Latitude());
	double latRad2 = DegreeToRadian(p2.Latitude());
	double lonRad1 = DegreeToRadian(p1.Longitude());
	double lonRad2 = DegreeToRadian(p2.Longitude());

	double diffLa = latRad2 - latRad1;
	double doffLo = lonRad2 - lonRad1;

	double computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
	return 2 * EarthRadiusKm * computation;
}

struct Animal {
	string animal_name;
	long date;
	double latitude;
	double longitude;
	float priority;
	bool validated;
	string version;

	Animal() {
		animal_name = "";
		date = 0;
		latitude = 0.0;
		longitude = 0.0;
		priority = 0.0;
		validated = 0;
		version = "";
	}

	Animal(string name, long _date, double lat, double lon, float pri, bool val, string ver) {
		animal_name = name;
		date = _date;
		latitude = lat;
		longitude = lon;
		priority = pri;
		validated = val;
		version = ver;
	}

	Animal(json j) {
		animal_name = j["animal_name"];
		date = j["date"];
		latitude = j["latitude"];
		longitude = j["longitude"];
		priority = j["priority"];
		validated = j["validated"];
		version = j["version"];
	}
};


// heap class
class Heap {
private:
	vector<Animal> bucket;
	int Next=1;

	/**
	*Function: BubUp
    *
    *Input: index# of item to be increased
    *
    *Process: swaps parent and child
	*
	*/
	void BubUp(int i) {
		int p = Parent(i);
		while (p > 0 && bucket[i].priority > bucket[p].priority) {
			Swap(i, p);
			i = p;
			p = Parent(i);
		}
	}

	/**
	*Function: BubDown
    *
    *Input: index# of item to be decreased
    *
    *Process: swaps parent and child
	*
	*/
	void BubDown(int i) {
		int c = PickChild(i);

		while (c > 0) {
			if (bucket[i].priority < bucket[c].priority) {
				Swap(i, c);
				i = c;
				c = PickChild(i);
			}
			else {
				c = -1;
			}
		}
	}

	/**
	*Function: Swap
    *
    *Input: the index#'s of the animals to swap
    *
    *Process: Swaps two elements in a vector of Animals
	*
	*/
	void Swap(int p, int i) {
		Animal temp = bucket[p];
		bucket[p] = bucket[i];
		bucket[i] = temp;
	}

	/**
	*Function: Parent
    *
    *Input: child index#
    *
    *Process: Returns parent index# of a given child
	*
	*Output: parent index#
	*/
	int Parent(int i) {
		return int(i / 2);
	}

    
    /**
	*Function: RChild
    *
    *Input: parent index#
    *
    *Process: Returns right child index#
	*
	*Output: right child index#
	* 
	*/
	int RChild(int i) {
		return i * 2 + 1;
	}
    
    /**
	*Function: LChild
    *
    *Input: parent index#
    *
    *Process: Returns left child index#
	*
	*Output: left child index#
	* 
	*/
	int LChild(int i) {
		return i * 2;
	}

	/**
	*Function: PickChild
    *
    *Input: index# of parent
    *
    *Process: See if child should be swapped
	*
	*Output: index# to swap with
	*/
	int PickChild(int i) {
		if (RChild(i) >= Next) {    
			if (LChild(i) >= Next) { 
				return -1;
			}
			else { 
				return LChild(i);
			}
		}
		else {
			if (bucket[i * 2 + 1].priority > bucket[i * 2].priority) {
				return RChild(i);
			}
			else {
				return LChild(i);
			}
		}
	}

	/**
	*Function: PrintTop
    *
    *Input: The animal extracted
    *
    *Process: Prints the name and priority of the Animal 
	*from the front of the heap.
	*
	*/

	void PrintTop(Animal a) {
		cout << a.animal_name << " " << a.priority << endl;
	}

public:
	/**
	*Function: Heap
	*     
    *Process: heap constructor that makes a bucket
    *of animals. 
	*/

	Heap() {
		bucket.push_back(Animal{});
		Next = 1;
	}

	/**
	*Function Insert:
	*  
	*Input: an animal to be put into the heap
    *     
    *Process: put animal into heap
	*/
	void Insert(Animal x){
		if (x.validated == true) 
			x.priority = (1.0 / x.priority);
		
		bucket.push_back(x);
		BubUp(Next);
		Next++;
	}

	/**
	*Function: GetTop
    *
    *Process: Removes the top element from heap
    */
	void GetTop() {

		if (bucket.empty()) 
			return;
    
        Animal retval = bucket[1];
        
        bucket[1] = bucket[--Next];

		if (bucket.size() > 1)
            BubDown(1);
            
		PrintExtract(retval);
	}
};



class AnimalHelper {
private:
	Animal **Animals;
	JsonHelper *J;
	json obj;
	int size;

public:
	AnimalHelper(string filename) {
		J = new JsonHelper(filename);
		size = J->getSize();
		Animals = new Animal *[size];
		for (int i = 0; i < size; i++) {
			obj = J->getNext();
			Animals[i] = new Animal(obj);
		}
	}

	/**
	*Function: sort
    *		
    *Input:
    *The array of Heaps to hold the Animals
    *and the number of heaps
    *
    *Process: Sorts the Animals into an array of Heaps based
	*on the number of Heaps
    * 
	*/
	void sort(Heap B[], int n) {
		int index;
		Animal move;
		for (int i = 0; i < size; i++) {
			index = abs(Animals[i]->date) % n;
			move.animal_name = Animals[i]->animal_name;
			move.date = Animals[i]->date;
			move.latitude = Animals[i]->latitude;
			move.longitude = Animals[i]->longitude;
			move.priority = Animals[i]->priority;
			move.validated = Animals[i]->validated;
			move.version = Animals[i]->version;
			B[index].Insert(move);
		}
	}

	void PrintAnimals() {

		for (int i = 0; i < size; i++) {

			cout << Animals[i]->animal_name << " "
				<< Animals[i]->latitude << " "
				<< Animals[i]->longitude << " "
				<< Animals[i]->date << " "
				<< Animals[i]->priority << " "
				<< Animals[i]->validated << " "
				<< Animals[i]->version << endl;

		}
	}
};


int main(int argc, char **argv) {

	int NumberofBuckets;
	cin >> NumberofBuckets;
	Heap buckets[NumberofBuckets];
	AnimalHelper AH("animals.json");
    AH.sort(buckets, NumberofBuckets);
	
    //Gets top 5 animals of each heap
    for (int i = 0; i < NumberofBuckets; i++) {
		cout << endl << "Heap " << i << endl
			<< "========================" << endl;
		for (int j = 1; j <= 5; j++) {
			buckets[i].GetTop();
		}
	}
    
    return 0;
}
