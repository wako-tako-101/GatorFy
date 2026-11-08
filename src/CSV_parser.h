//
// Created by sidha on 11/1/2026.
//

#ifndef GATORFY_CSV_PARSER_H
#define GATORFY_CSV_PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include "Song.h"
using namespace std;

vector<string> parseCSVLine (const string &line);
vector<Song> loadSongs(const string &filename);


#endif //GATORFY_CSV_PARSER_H