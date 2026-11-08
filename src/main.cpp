#include <iostream>
#include <chrono>
#include <cstdlib>
#include "../include/priority-queues.hpp"

using namespace std;
using namespace std::chrono;

int main() {
	MaxHeap heap;
	int numSongs=100000;

	auto start1 = high_resolution_clock::now();

	for(int i=0; i<numSongs; i++){
		Song temp;
		temp.title="Song " + to_string(i);
		temp.recommendationScore=rand()%101;
		heap.insert(temp);
	}

	auto stop1=high_resolution_clock::now();
	auto dur1 = duration_cast<milliseconds>(stop1 - start1);

	cout<<"Insert time: "<<dur1.count()<<" ms\n";

	int k=10;
	int actualK=0;

	auto start2=high_resolution_clock::now();
	Song* topRecs=heap.extractTopK(k, actualK);
	auto stop2 = high_resolution_clock::now();

	auto dur2=duration_cast<milliseconds>(stop2 - start2);

	cout<<"Extract time: "<<dur2.count()<<" ms\n";

	for(int i=0;i<actualK;i++){
		cout<<i+1<<". "<<topRecs[i].title<<" - "<<topRecs[i].recommendationScore<<"\n";
	}

	delete[] topRecs;
	return 0;
}