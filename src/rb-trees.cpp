#include "../include/rb-trees.hpp"
//
// Created by sidha on 11/1/2026.
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "CSV_parser.h"
#include "Song.h"
#include "RBT.h"
#include <unordered_map>

RBT::RBT() {
    root = nullptr;
}

RBT::Node* RBT::insert_helper(Node* h, const Song& song) {
    if (h == nullptr) {
        Node* new_input = new Node;
        new_input->song = song;
        new_input->color = RED;
        new_input->right = nullptr;
        new_input->left = nullptr;

        return new_input;
    }

    if (song.similarityScore > h->song.similarityScore ) {
        h->left = insert_helper(h->left, song);
    }else {
        h->right = insert_helper(h->right, song);
    }

    return balance(h);

}

void RBT::insert(const Song& song) {
    root = insert_helper(root,song);

    if (root->color != BLACK) {
        root->color = BLACK;
    }
}

bool RBT::isRed(Node* node) {
    if (node == nullptr) {
        return false;
    }
    else {
        return node->color;
    }

}

RBT::Node* RBT::balance(Node* h) {
    if (!isRed(h) && isRed(h->left) && isRed(h->right)) {
        if (isRed(h->left->left) || isRed(h->left->right) ||
            isRed(h->right->left) || isRed(h->right->right)) {
            h->color = RED;
            h->left->color = BLACK;
            h->right->color = BLACK;
            return h;
            }
    }

    // This is the left left rotation
    if (!isRed(h) && isRed(h->left) && isRed(h->left->left) && !isRed(h->right)) {
        Node* child = h->left;
        Node* grandchild = h->left->left;

        h->left = child->right;
        child->right = h;

        child->color = BLACK;
        grandchild->color = RED;
        h->color = RED;


        return child;
    }

    // This is the right right rotation
    if (!isRed(h) && isRed(h->right) && isRed(h->right->right) && !isRed(h->left)) {
        Node* child = h->right;
        Node* grandchild = h->right->right;

        h->right = child->left;
        child->left = h;

        child->color = BLACK;
        grandchild->color = RED;
        h->color = RED;


        return child;
    }

    // This is the left right rotation
    if (!isRed(h) && isRed(h->left) && isRed(h->left->right)) {
        Node* child = h->left;
        Node* grandchild = h->left->right;

        child->right = grandchild->left;
        h->left = grandchild->right;

        grandchild->left = child;
        grandchild->right = h;

        grandchild->color = BLACK;
        child->color = RED;
        h->color = RED;

        return grandchild;


    }

    // This is the right left rotation
    if (!isRed(h) && isRed(h->right) && isRed(h->right->left)) {
        Node* child = h->right;
        Node* grandchild = h->right->left;

        child->left = grandchild->right;
        h->right = grandchild->left;

        grandchild->right = child;
        grandchild->left = h;

        grandchild->color = BLACK;
        child->color = RED;
        h->color = RED;

        return grandchild;


    }


    return h;
}

void RBT::destroy_helper(Node* node) {

    if (node != nullptr) {
        destroy_helper(node->left);
        destroy_helper(node->right);

        delete node;
    }

}

void RBT::top_n_helper(Node* node, int n, vector<Song>& results) {

    if (node == nullptr || results.size() >= n) {
        return;
    }


    top_n_helper(node->left, n, results);

    if (results.size() < n) {
        results.push_back(node->song);
    }

    top_n_helper(node->right, n, results);
}

void RBT::top_n(int n) {
    vector<Song> results;

    top_n_helper(root, n, results);

    for (const Song& song : results) {
        cout << song.name << " - "
             << song.similarityScore << endl;
    }
}

RBT::~RBT() {
    destroy_helper(root);
}
