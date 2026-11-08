#pragma once
#include <iostream>
#include <string>

using namespace std;

struct Song {
    string title;
    string primaryArtist;
    string featuredArtist;
    double tempo;
    double loudness;
    int harmonicKey;
    int scaleMode;
    int explicitStatus;
    int timeSignature;
    int releaseYear;
    int recommendationScore = 0;
};

class MaxHeap {
private:
    Song* heap;
    int capacity;
    int currentSize;

    int parent(int i) { return (i-1)/2; }
    int leftChild(int i) { return (2*i)+1; }
    int rightChild(int i) { return (2*i)+2; }

    void heapifyUp(int i);
    void heapifyDown(int i);

public:
    MaxHeap();
    ~MaxHeap();
    void insert(Song s);
    Song* extractTopK(int k, int& actualK);
    int size() { return currentSize; }
};