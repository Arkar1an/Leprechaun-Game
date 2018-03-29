// leprechauns.cpp : Defines the entry point for the console application.
// @author Russell "The Boss" Richardson Arkar1an@csu.fullerton.edu
// @teamname FOXHOUND

//#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <random>
#include <map>
#include <vector>
#include <list>
#include <iomanip>

using namespace std;

struct Leprechaun {

	Leprechaun(double location, double gold, bool state) {
		
		this->location = location;
		this->gold = gold;
		isInTheGame = state;
	}

	double location;
	double gold;
	bool isInTheGame;
};

class RandomNumberGenerator {
public:
	RandomNumberGenerator(int x) :generator(x) {};
	// return a double between -1 and 1
	double randomBetween1and2() {
		return (2.0*generator()) / generator.max() - 1.0;
	}
private:
	minstd_rand0 generator;
};

int N;
// Use a constant random number seed so behavior is consistent from run to run.
int RANDOM_SEED;
const double STARTING_GOLD = 1000000.0;

int main()
{

	cout << "Enter seed for random number generator: ";
	cin >> RANDOM_SEED;
	RandomNumberGenerator rng(RANDOM_SEED);

	cout << "Enter number of leprechauns: ";
	cin >> N;

	long playtime;
	cout << "Enter game play time (seconds): ";
	cin >> playtime;
	playtime = playtime * 1000; // convert to milliseconds

	double score = 0;
	int nTrapped = 0;
	int currentLeps = N;
	//
	// CODE FOR INITIALIZING DATA STRUCTURES GOES HERE
	map<double, Leprechaun*>::iterator itEnd, itBegin, itLeft, itRight, existingLepLoc;
	vector<Leprechaun*> leprechaunIndex;
	map<double, Leprechaun*> leprechaunMap;
	pair<map<double, Leprechaun*>::iterator, bool> currentLepLoc;
	for (int i = 0; i < N; i++) {
		Leprechaun* temp = new Leprechaun(i * 1000, STARTING_GOLD, true);
		leprechaunIndex.push_back(temp);
		leprechaunMap.insert(pair<double, Leprechaun*>(i * 1000, temp));
	}
	// CODE FOR INITIALIZING DATA STRUCTURES GOES HERE
	//

	int t = 0; // keep track of number of iterations
	auto start_time0 = chrono::high_resolution_clock::now();
	auto timeSinceStartMS = 0;
	do {

		//
		// CODE FOR A SINGLE ITERATION GOES HERE
		for (int i = 0; i < N; i++) { // this is taking O(n) time

			// This checks whether a Leprechaun will be participating within an iteration
			if (leprechaunIndex[i]->isInTheGame == true) {

				double r = rng.randomBetween1and2();
				double oldLocation = leprechaunIndex[i]->location;
				leprechaunIndex[i]->location += (r * (leprechaunIndex[i]->gold));

				//  This determines if a Leprechaun jumped into a pit
				if ((leprechaunIndex[i]->location >= -1000) && (leprechaunIndex[i]->location <= 1000)) {
					score += leprechaunIndex[i]->gold;
					leprechaunIndex[i]->isInTheGame = false;
					leprechaunMap.erase(oldLocation); //this is taking O(log n) time
					nTrapped++;
					cout << setprecision(20) <<"t = " << t << ": Caught a Leprechaun!! Score = " << (long)score << endl;
				}

				// Leprechaun now steals from his left and right neighbors
				else {

					leprechaunMap.erase(oldLocation);//this is taking O(log n) time
					currentLepLoc = leprechaunMap.insert(pair<double, Leprechaun*>(leprechaunIndex[i]->location, leprechaunIndex[i])); //this is taking O(log n) time

					// This determines if a Leprachaun has landed on another Leprechaun
					// This is the most costly scenario, but extremely low chance this occurs
					if (currentLepLoc.second == false) { 

						leprechaunIndex[i]->gold += currentLepLoc.first->second->gold; 
						currentLepLoc.first->second->isInTheGame = false; 
						leprechaunMap.erase(currentLepLoc.first);
						cout << "Landed on a Leprechaun!! Gold stolen !!" << endl;
						currentLepLoc = leprechaunMap.insert(pair<double, Leprechaun*>(leprechaunIndex[i]->location, leprechaunIndex[i])); //this is taking O(log n) time
					}

					itEnd = leprechaunMap.end();
					itBegin = leprechaunMap.begin();
					itLeft = currentLepLoc.first; 
					itRight = currentLepLoc.first; 
					--itEnd, --itLeft, ++itRight; 

					//Checks if Leprechaun has no neighbor to the left
					if (itBegin->first == leprechaunIndex[i]->location) {
						leprechaunIndex[i]->gold += itRight->second->gold / 2;
						itRight->second->gold /= 2;
					}
					// Checks if Leprechaun has no neighbor to the right
					else if (itEnd->first == leprechaunIndex[i]->location) {
						leprechaunIndex[i]->gold += itLeft->second->gold / 2;
						itLeft->second->gold /= 2;
					}

					// Leprechaun must have a neighbor to the left and right
					else { 
						leprechaunIndex[i]->gold += itLeft->second->gold / 2;
						itLeft->second->gold /= 2;
						leprechaunIndex[i]->gold += itRight->second->gold / 2;
						itRight->second->gold /= 2;
					}
				}
			}
		}
		// CODE FOR A SINGLE ITERATION GOES HERE
		//

		// code to measure run time
		t++;
		auto end_time = std::chrono::high_resolution_clock::now();
		auto timeSinceStart = end_time - start_time0;
		timeSinceStartMS = chrono::duration_cast<chrono::milliseconds>(timeSinceStart).count();
	} while (timeSinceStartMS < playtime);
	
	// Destroy them.  Destroy all Leprechauns.
	for (int i = 0; i < N; i++) {
		Leprechaun* temp = leprechaunIndex[i];
		delete temp;
	}

	cout << "Number of iterations = " << t << endl;
	cout << "Number of trapped leprechauns = " << nTrapped << endl;
	cout << "Score = " << (long)score << endl;
	return 0;
}


// Thoughts on this project:

// I have no idea how your sample (seed = 123, num = 1000, time = 5) has been generating TWICE as many iterations and Leppers 
// caught than in my code. I have cut down the O(log n) function calls to either one or two: if a leprechaun is caught
// then the map function erase() is called, and if no leprechauns are caught then the map function insert() must be called
// twice. Which means my if statement structure has to be what is slowing me down, checking every scenario n times when most leprechauns 
// fall into my final "else" scenario of stealing from left and right partners. I will NEED an email after this explaining how 
// this can possibly be more efficient while maintaining the strict processing order. It has been absolutely stumping me as to 
// how you are able to perform a full iteration in only 21.186 milliseconds on average for the aforementioned sample. Given the
// iterations and run time, my code averages 48.544 milliseconds per iteration on my machine... 