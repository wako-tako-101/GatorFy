
#ifndef UNTITLED2_SONG_H
#define UNTITLED2_SONG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct Song {
    string id;
    string name;
    string album;
    string artists;

    double tempo = 0.0;
    double loudness = 0.0;

    int key = -1;
    int mode = -1;
    bool explicitContent = false;
    int timeSignature = 0;
    int year = 0;

    double similarityScore = 0.0;
};

#endif

