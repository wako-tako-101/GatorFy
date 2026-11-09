#include <chrono>
#include <cstdlib>
#include "priority-queues.hpp"
#include "RBT.h"
#include "Song.h"
#include "CSV_parser.h"
#include <limits>
#include <iomanip>


using namespace std;
using namespace std::chrono;

//for titles
void equalsGenerator(const int& n){
	for(int i = 0; i < n; i++) cout << '=';
}

void whitespaceGenerator(const int& n){
	for(int i = 0; i < n; i++) cout << ' ';
}

//for searches
vector<const Song*> searchSongs(const vector<Song>& songs, const string& songName, int numRecs){
	vector<const Song*> matches;

	for(const Song& song : songs){
		if(song.name == songName) matches.push_back(&song);
		if(matches.size() >= numRecs) break;
	}
	return matches;
}

//for showing song info
void songInfo(const Song& song){
	cout << "Song: " << song.name << "\n";
	cout << "Artist(s): " << song.artists << "\n";
	cout << "Album: " << song.album << "\n";
	cout << "ID: " << song.id << "\n";

	cout << "Song Details:\n";
	cout << "Tempo: " << song.tempo << "\n";
	cout << "Loudness: " << song.loudness << "\n";
	cout << "Key: " << song.key << "\n";
	cout << "Mode: " << (song.mode == 1 ? "Major" : song.mode == 0 ? "Minor" : "Unknown") << "\n";
	cout << "Explicit Content: " << (song.explicitContent ? "Yes" : "No") << "\n";
	cout << "Time Signature: " << song.timeSignature << "\n";
	cout << "Release Year: " << song.year << "\n";
}

//MAIN MENU INPUT
void printMenu(){

	equalsGenerator(10);
	whitespaceGenerator(5);
	cout << "GatorFy: Music Recommendation System";
	whitespaceGenerator(5);
	equalsGenerator(10);
	cout << "\n";

	//menu options
	cout << "Please enter your option:\n";
	cout 
	<< "1. Song Search\n"
	<< "2. Song Recommendations\n"
	<< "3. Song Information\n"
	<< "4. Data Structure Comparison\n"
	<< "5. Exit\n" << endl;
}

int main() {
	// MaxHeap heap;
	// int numSongs=100000;

	// auto start1 = high_resolution_clock::now();

	// for(int i=0; i<numSongs; i++){
	// 	Song temp;
	// 	temp.name="Song " + to_string(i);
	// 	temp.similarityScore=rand()%101;
	// 	heap.insert(temp);
	// }

	// auto stop1=high_resolution_clock::now();
	// auto dur1 = duration_cast<milliseconds>(stop1 - start1);

	// cout<<"Insert time: "<<dur1.count()<<" ms\n";

	// int k=10;
	// int actualK=0;

	// auto start2=high_resolution_clock::now();
	// Song* topRecs=heap.extractTopK(k, actualK);
	// auto stop2 = high_resolution_clock::now();

	// auto dur2=duration_cast<milliseconds>(stop2 - start2);

	// cout<<"Extract time: "<<dur2.count()<<" ms\n";

	// for(int i=0;i<actualK;i++){
	// 	cout<<i+1<<". "<<topRecs[i].name<<" - "<<topRecs[i].similarityScore<<"\n";
	// }

	// delete[] topRecs;

	
	//loading data from CSV file
	ifstream file("tracks_features.csv");
	if(!file.is_open()){
		cerr << "Error opening CSV file." << endl;
		return 1;
	}
	
	auto start = high_resolution_clock::now();
	const vector<Song>& songs = loadSongs("tracks_features.csv");

	if(songs.empty()){
		cerr << "No songs loaded from CSV file." << endl;
		return 1;
	}

	cout << "Loaded " << songs.size() << " songs from CSV file." << endl;
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Loading time: " << duration.count() << " ms" << " (" << duration.count() / 1000.0 << " seconds)" << endl;
	file.close();


	//USER INPUT
	while(true){
		printMenu();
		int userOption;

		//if invalid input, close program
		if(!(cin >> userOption)){
			if(cin.eof()) break;

			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cerr << "Invalid input. Please enter a number." << endl;
			continue;
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		string songName;

		//options
		switch(userOption){
			case 1:{
				// Song Search
				cout << "Enter song name: ";
				getline(cin, songName);

				cout << "How many recommendations? (Between 1-100)\n";
				int numRecs;
					
				if (!(cin >> numRecs)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Please enter a valid number.\n";
					break;
				}
				
				if(numRecs < 1 || numRecs > 100){
					cerr << "Invalid number of recommendations." << endl;
					break;
				}

				const vector<const Song*> matches = searchSongs(songs, songName, numRecs);
				if(matches.empty()){
					cerr << "No matches found for: " << songName << endl;
					break;
				}

				cout << "Showing " << matches.size() << " matches for: " << songName << endl;
				for (size_t i = 0; i < matches.size(); i++) {
					cout << i + 1 << ". " 
					<< matches[i]->name << ": " << matches[i]->artists 
					<< " - Album: " << matches[i]->album << "\n";
				}

				break;
			}
			case 2: {
					// Song Recommendations
					cout << "Enter song name: ";
					getline(cin, songName);

					cout << "How many recommendations? (Between 1-100)\n";
					int numRecs;
					
				if (!(cin >> numRecs)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Please enter a valid number.\n";
					break;
				}
				
				if(numRecs < 1 || numRecs > 100){
					cerr << "Invalid number of recommendations." << endl;
					break;
				}


				const vector<const Song*> matches = searchSongs(songs, songName, numRecs);

				if(matches.empty()){
					cerr << "No recommendations found for: " << songName << endl;
					break;
				}

				cout << "Showing " << numRecs << " recommendations for: " << songName << endl;

				const Song& seed = *matches[0];

				//use rbts and max heap to efficiently find similar songs
				MaxHeap heap;
				RBT tree;

				for(const Song& song : songs){
					//skip the original song
					if(&song == &seed || song.id == seed.id) continue;

					//find feasible recs
					Song candidate = song;
					candidate.similarityScore = calculateSimilarity(seed, candidate);

					//insert into both structs
					heap.insert(candidate);
					tree.insert(candidate);
				}

				//extract top numRecs similar songs from the heap
				int k = 0;
				Song* topRecs = heap.extractTopK(numRecs, k);

				//print top recommendations (Max Heap)
				cout << "Max Heap Recommendations:" << endl;

				for (int i = 0; i < k; i++) {
					cout << i + 1 << ". " << topRecs[i].name << ": " << topRecs[i].artists << " - Similarity Score: " << topRecs[i].similarityScore << "/100\n";
				}

				delete[] topRecs;

				//print top recommendations (Red-Black Tree)
				cout << "Red-Black Tree Recommendations:" << endl;
				tree.top_n(numRecs);
				break;
			}
			case 3:{
				// Song Information
				cout << "Enter song name: ";
				getline(cin, songName);

				cout << "How many matching songs to display? (Between 1-100)\n";
				int numRecs;
					
				if (!(cin >> numRecs)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Please enter a valid number.\n";
					break;
				}
				
				if(numRecs < 1 || numRecs > 100){
					cerr << "Invalid number of matching songs to display." << endl;
					break;
				}

				 const vector<const Song*> matches = searchSongs(songs, songName, numRecs);

				if(matches.empty()){
					cerr << "No matches found for: " << songName << endl;
					break;
				}
				else if(matches.size() == 1){

					songInfo(*matches[0]);
					break;
				}

				for(size_t i = 0; i < matches.size(); i++){
					cout << i + 1 << ". ";
					songInfo(*matches[i]);
					cout << endl;
				}
				break;
			}
			case 4:{
				// Data Structure Comparison
				cout << "Comparing Max Heap and Red-Black Tree for top recommendations..." << endl;
				cout << "Enter seed song name: ";
				getline(cin, songName);

				vector<const Song*> matches = searchSongs(songs, songName, 1);

				if (matches.empty()) {
					cerr << "Song not found!\n";
					break;
				}

				const Song& seed = *matches[0];

				cout << "How many Recommendations to display? (Between 1-100)\n";
				int numRecs;
				if (!(cin >> numRecs)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Please enter a valid number.\n";
					break;
				}
				if(numRecs < 1 || numRecs > 100){
					cerr << "Invalid number of recommendations to display." << endl;
					break;
				}

				vector<Song> candidates;
				for (const Song& song : songs){
					if(song.id == seed.id) continue;

					Song candidate = song;
					candidate.similarityScore = calculateSimilarity(seed, candidate);
					candidates.push_back(candidate);
				}

				cout << "\nScored Candidates: " << candidates.size() << endl;	

				// Compare Max Heap and Red-Black Tree
				MaxHeap heap;
				RBT tree;

				for (const Song& candidate : candidates) {
					heap.insert(candidate);
					tree.insert(candidate);
				}

				//benchmarking
				//max heap
				auto heapStart = chrono::steady_clock::now();

				int actualK = 0;
				Song* heapResults = heap.extractTopK(numRecs, actualK);

				auto heapEnd = chrono::steady_clock::now();

				//rbt
				auto treeStart = chrono::steady_clock::now();

				vector<Song> treeResults = tree.getTopN(numRecs);

				auto treeEnd = chrono::steady_clock::now();

				double heapDuration = chrono::duration<double, nano>(heapEnd - heapStart).count();
				double treeDuration = chrono::duration<double, nano>(treeEnd - treeStart).count();

				// Display results
				cout << fixed << setprecision(3);

				cout << "\nBenchmarks:\n";
				cout << "Heap Duration: " << heapDuration << " ns (" << heapDuration / 1000000.0 << " ms)\n";

				cout << "Red-Black Tree Duration: " << treeDuration	<< " ns (" << treeDuration / 1000000.0 << " ms)\n";

				cout << "\nHeap Results: " << actualK << endl;
				cout << "Tree Results: " << treeResults.size() << endl;

				// Display a sample result so computation is observable
				if (actualK > 0) {
					cout << "Heap Top Song: " << heapResults[0].name << " - Score: " << heapResults[0].similarityScore	<< endl;
				}

				if (!treeResults.empty()) {
					cout << "RBT Top Song: " << treeResults[0].name	<< " - Score: " << treeResults[0].similarityScore << endl;
				}
				delete[] heapResults;
			}
			case 5:{
				// Exit
				cout << "Closing GatorFy..." << endl;	
				return 0;
			}
			default:
				// Invalid option
				cerr << "Invalid option. Please try again." << endl;
				break;
		}
	}

	return 0;
}