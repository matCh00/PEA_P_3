// menu projektu

#ifndef PEA_P_1_MENU_H
#define PEA_P_1_MENU_H

#include "../_efficiency/Tests.h"

using namespace std;



class Menu {

private:

    // graf
    Graph* graph;

    // czas wykonywania
    time_t timeGA;

    // populacja
    int populationSize;

    // typ mutacji
    bool mutationType;

    // prawdopodobieństwo mutacji
    float mutationProbability;

    // typ krzyżowania
    bool crossType;

    // prawdopodobieństwo krzyżowania
    float crossProbability;


public:

    // konstruktor
    Menu();

    // destruktor
    ~Menu();

    // start
    void startMenu();
};


#endif