//
// Created by sidha on 11/1/2026.
#include <string>
#include <vector>
#include <fstream>
#include "CSV_parser.h"
#include "Song.h"
#include <unordered_map>
using namespace std;

vector<string> parseCSVLine(const string& line) {
    vector<string> fields;

    string current_field = "";
    bool insideQuotes = false;

    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];

        if (c == '"') {
            if (insideQuotes && i+1 < line.length() && line[i+1]== '"') {
                current_field += '"';
                i++;
            }else {
                insideQuotes = !insideQuotes;
            }


        } else if (c == ',' &&  insideQuotes == false) {
            fields.push_back(current_field);
            current_field.clear();

        }
        else {
            current_field +=c;
        }

    }
    fields.push_back(current_field);

    return fields;

}

vector<Song> loadSongs(const string &filename) {
    ifstream file(filename);
    vector<Song> songs;

    if (!file.is_open()) {
        cerr << "Error!: Can't OPEN file" << endl;
        return {};
    }

    string line;

    if (!getline(file,line)) {
        cerr << "CSV file is empty!" << endl;
        return {};
    }

    vector<string> headers = parseCSVLine(line);
    vector<string> required_columns = { "name", "year", "tempo", "id", "album", "artists","loudness" , "key", "mode","explicit", "time_signature"};




    unordered_map<string,size_t> columnIndex;

    for (size_t i = 0; i < headers.size(); i++) {
        columnIndex[headers[i]] = i;
    }


    for (const string& column: required_columns) {
        if (columnIndex.find(column) == columnIndex.end()) {
            return {};
        }
    }

    while (getline(file, line)) {
        vector<string> fields = parseCSVLine(line);

        if (fields.size() != headers.size()) {
            continue;
        }

        try {
            Song song;

            song.name = fields[columnIndex.at("name")];
            song.year = stoi(fields[columnIndex.at("year")]);
            song.tempo = stod(fields[columnIndex.at("tempo")]);
            if (fields[columnIndex.at("explicit")] == "True") {
                song.explicitContent = true;
            }else {
                song.explicitContent = false;
            }
            song.key = stoi(fields[columnIndex.at("key")]);
            song.id = fields[columnIndex.at("id")];
            song.album = fields[columnIndex.at("album")];
            song.artists = fields[columnIndex.at("artists")];
            song.loudness = stod(fields[columnIndex.at("loudness")]);
            song.mode = stoi(fields[columnIndex.at("mode")]);
            song.timeSignature = stoi(fields[columnIndex.at("time_signature")]);

            songs.push_back(song);

        } catch (const exception& e) {
            cout << "Invalid song data: " << e.what() << endl;
            continue;
        }
    }

    return songs;




}


