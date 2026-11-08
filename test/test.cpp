#include <catch2/catch_test_macros.hpp>
#include "../include/priority-queues.hpp"
#include <string>

TEST_CASE("MaxHeap correctly tracks size when inserting", "[MaxHeap]") {
    MaxHeap heap;
    REQUIRE(heap.size()==0);

    Song s1;
    s1.title="Song 1";
    s1.recommendationScore=50;

    Song s2;
    s2.title="Song 2";
    s2.recommendationScore=80;

    heap.insert(s1);
    REQUIRE(heap.size()==1);

    heap.insert(s2);
    REQUIRE(heap.size()==2);
}

TEST_CASE("MaxHeap extracts the top K songs in the correct order", "[MaxHeap]") {
    MaxHeap heap;

    Song s1; s1.title="Low Score Song"; s1.recommendationScore=40;
    Song s2; s2.title="Highest Score Song"; s2.recommendationScore=95;
    Song s3; s3.title="Middle Score Song"; s3.recommendationScore=75;
    Song s4; s4.title="Another Low Song"; s4.recommendationScore=20;

    heap.insert(s1);
    heap.insert(s2);
    heap.insert(s3);
    heap.insert(s4);

    int actualK=0;
    Song* topSongs = heap.extractTopK(2, actualK);

    REQUIRE(actualK==2);
    REQUIRE(topSongs[0].title=="Highest Score Song");
    REQUIRE(topSongs[1].title=="Middle Score Song");

    delete[] topSongs;
}

TEST_CASE("MaxHeap handles extracting more songs than it contains", "[MaxHeap]") {
    MaxHeap heap;

    Song s1;
    s1.title="Only Song";
    s1.recommendationScore=88;

    heap.insert(s1);

    int actualK=0;
    Song* topSongs=heap.extractTopK(5, actualK);

    REQUIRE(actualK==1);
    REQUIRE(topSongs[0].title=="Only Song");

    delete[] topSongs;
}

