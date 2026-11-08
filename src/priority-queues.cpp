#include "../include/priority-queues.hpp"

MaxHeap::MaxHeap() {
    capacity = 10;
    currentSize = 0;
    heap = new Song[capacity];
}

MaxHeap::~MaxHeap() {
    delete[] heap;
}

void MaxHeap::heapifyUp(int i) {
    while(i>0 && heap[parent(i)].recommendationScore < heap[i].recommendationScore) {
        Song temp = heap[i];
        heap[i] = heap[parent(i)];
        heap[parent(i)] = temp;
        i = parent(i);
    }
}

void MaxHeap::heapifyDown(int i) {
    int maxIndex = i;
    int left = leftChild(i);
    int right = rightChild(i);

    if (left<currentSize && heap[left].recommendationScore > heap[maxIndex].recommendationScore) {
        maxIndex = left;
    }

    if(right<currentSize && heap[right].recommendationScore > heap[maxIndex].recommendationScore){
        maxIndex = right;
    }

    if(i!=maxIndex) {
        Song temp = heap[i];
        heap[i] = heap[maxIndex];
        heap[maxIndex] = temp;
        heapifyDown(maxIndex);
    }
}

void MaxHeap::insert(Song s) {
    if(currentSize==capacity) {
        capacity = capacity * 2;
        Song* newHeap = new Song[capacity];
        for(int i=0; i<currentSize; i++){
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
    }

    heap[currentSize] = s;
    heapifyUp(currentSize);
    currentSize++;
}

Song* MaxHeap::extractTopK(int k, int& actualK) {
    if(k > currentSize) {
        k = currentSize;
    }
    actualK = k;

    Song* topK = new Song[k];

    for(int i=0; i<k; i++){
        if(currentSize==0) break;

        topK[i] = heap[0];

        heap[0] = heap[currentSize - 1];
        currentSize--;

        if(currentSize > 0) {
            heapifyDown(0);
        }
    }

    return topK;
}