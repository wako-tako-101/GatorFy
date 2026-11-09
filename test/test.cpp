#include <catch2/catch_test_macros.hpp>
#include "../src/priority-queues.hpp"
#include "../src/Song.h"
#include <string>

TEST_CASE("MaxHeap correctly tracks size when inserting", "[MaxHeap]") {
    MaxHeap heap;
    REQUIRE(heap.size()==0);

    Song s1;
    s1.name="Song 1";
    s1.similarityScore=50;

    Song s2;
    s2.name="Song 2";
    s2.similarityScore=80;

    heap.insert(s1);
    REQUIRE(heap.size()==1);

    heap.insert(s2);
    REQUIRE(heap.size()==2);
}

TEST_CASE("MaxHeap extracts the top K songs in the correct order", "[MaxHeap]") {
    MaxHeap heap;

    Song s1; s1.name="Low Score Song"; s1.similarityScore=40;
    Song s2; s2.name="Highest Score Song"; s2.similarityScore=95;
    Song s3; s3.name="Middle Score Song"; s3.similarityScore=75;
    Song s4; s4.name="Another Low Song"; s4.similarityScore=20;

    heap.insert(s1);
    heap.insert(s2);
    heap.insert(s3);
    heap.insert(s4);

    int actualK=0;
    Song* topSongs = heap.extractTopK(2, actualK);

    REQUIRE(actualK==2);
    REQUIRE(topSongs[0].name=="Highest Score Song");
    REQUIRE(topSongs[1].name=="Middle Score Song");

    delete[] topSongs;
}

TEST_CASE("MaxHeap handles extracting more songs than it contains", "[MaxHeap]") {
    MaxHeap heap;

    Song s1;
    s1.name="Only Song";
    s1.similarityScore=88;

    heap.insert(s1);

    int actualK=0;
    Song* topSongs=heap.extractTopK(5, actualK);

    REQUIRE(actualK==1);
    REQUIRE(topSongs[0].name=="Only Song");

    delete[] topSongs;
}

