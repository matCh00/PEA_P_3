/*
 * Algorytm genetyczny
 */

#ifndef PEA_P_3_GENETICALGORITHM_H
#define PEA_P_3_GENETICALGORITHM_H

#include "../_structures/Graph.h"

using namespace std;


class GeneticAlgorithm {

private:

    // macierz
    vector<vector<int>> matrix;

    // liczba miast
    int matrixSize;

    // optymalny koszt
    int cost;

    // optymalna ścieżka
    vector<int> path;

    // populacja
    int population;

    // typ mutacji
    bool mutationType;

    // prawdopodobieństwo mutacji
    float mutationProbability;

    // prawdopodobieństwo krzyżowania
    float crossProbability;

    // czas wykonania
    time_t executionTime;


public:

    // konstruktor
    GeneticAlgorithm();

    // destruktor
    ~GeneticAlgorithm();

    // algorytm genetyczny
    double algorithmGeneticAlgorithm(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost);

    // ustawienia Genetic Algorithm
    void settingsGeneticAlgorithm(time_t executionTime, int population, bool mutationType, float mutationProbability, float crossProbability);
};


#endif
