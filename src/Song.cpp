#include "Song.h"

double calculateSimilarity(const Song &song1, const Song &song2)
{
    double score = 0.0;

    if(song1.artists == song2.artists){
        score += 20.0;
    }

    //1. Artists match
    for(int i = 0; i < song1.artists.size(); i++){
        if(song1.artists[i] == song2.artists[i]){
            score += 10.0;
            break;
        }
    }


    //2. Tempo proximity
    double tempoDifference = std::abs(song1.tempo - song2.tempo);
    if(tempoDifference <= 5.0) score += 15.0;
    else if(tempoDifference <= 12.0) score += 10.0;
    else if(tempoDifference <= 25.0) score += 5.0;
    

    //3 loudness proximity
    double loudnessDiff = std::abs(song1.loudness - song2.loudness);
    if (loudnessDiff <= 1.5) score += 15.0;
    else if (loudnessDiff <= 3.5) score += 10.0;
    else if (loudnessDiff <= 6.0) score += 5.0;

    //4. harmonic key compatibility
    if (song1.key >= 0 && song2.key >= 0) {
        int keyDiff = std::abs(song1.key - song2.key);

        if (keyDiff == 0) score += 15.0;
        else if (keyDiff == 7 || keyDiff == (5 % 12)) score += 8.0;
    }

    //5. scale mode
    if(song1.mode == song2.mode && (song1.mode >= 0 && song2.mode >= 0)){
        score += 10.0;
    }

    //6. explicit content
    if(song1.explicitContent == song2.explicitContent){
        score += 10.0;
    }

    //7. time signature
    if(song1.timeSignature > 0 && song1.timeSignature == song2.timeSignature){
        score += 10.0;
    }

    //8. release era/chronological proximity
    if(song1.year > 0 && song2.year > 0){
        int yearDiff = std::abs(song1.year - song2.year);


        if(yearDiff <= 3) score += 5.0;
        else if(yearDiff <= 7) score += 3.0;
    }
    
    return score;
}