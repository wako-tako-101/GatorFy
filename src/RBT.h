//
// Created by sidha on 11/4/2026.
//

#ifndef GATORFY_RBT_H
#define GATORFY_RBT_H
//
// Created by sidha on 11/4/2026.
//

#include <vector>
#include <fstream>
#include "CSV_parser.h"
#include "Song.h"
#include <unordered_map>
#pragma once
#include <string>
using namespace std;



#define RED true
#define BLACK false

class RBT {

    struct Node{
        Song song;
        bool color;
        Node* left;
        Node* right;
    };

    Node* root;
    bool isRed(Node* node);
    Node* balance(Node* h);
    Node* insert_helper(Node *h, const Song& song);
    void destroy_helper(Node* node);

    void top_n_helper(Node* node, int n, vector<Song>& results);


public:
    RBT();
    ~RBT();

    void insert(const Song& song);
    void top_n(int n);
    vector<Song> getTopN(int n);
};

#endif //GATORFY_RBT_H