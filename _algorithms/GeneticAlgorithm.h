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

    // liczebność populacji
    int populationSize;

    // typ mutacji
    bool mutationType;

    // prawdopodobieństwo mutacji
    float mutationProbability;

    // typ krzyżowania
    bool crossType;

    // prawdopodobieństwo krzyżowania
    float crossProbability;

    // czas wykonania
    time_t executionTime;



    void generateInitialPopulation(vector<vector<int>> &pop);

    void sortVector(vector<vector<int>> &vect);

    void overwritePopulation(vector<vector<int>> &population, vector<vector<int>> popul);

    void mutation(vector<int> &ind);

    void doCrossover(vector<int> parent1, vector<int> parent2, vector<int> &offspring1, vector<int> &offspring2);


    void OrderCO(vector<int> parent1, vector<int> parent2, vector<int> &offspring1, vector<int> &offspring2);

    void EnhancedSequentialCO(vector<int> parent1, vector<int> parent2, vector<int> &offspring);

    vector<int> tournamentSelection(vector<vector<int>> pop);

    int calculateCost(vector<int> a);

    void reverseVector(int a, int b, vector<int> &currentRoute);

    void insertVector(int a, int b, vector<int> &currentRoute);

    void swapVector(int a, int b, vector<int> &currentRoute);

    void calculateInsert(int i, int j, int &balance, vector<int> currentRoute);

    void calculateReverse(int i, int j, int &balance, vector<int> currentRoute);

    void calculateSwap(int i, int j, int &balance, vector<int> currentRoute);

    int getInitialGreedyAndRandom(vector<int> &bestTab);

    int getInitialGreedy(vector<int> &bestTab);


public:

    // konstruktor
    GeneticAlgorithm();

    // destruktor
    ~GeneticAlgorithm();

    // algorytm genetyczny
    double algorithmGeneticAlgorithm(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost);

    // ustawienia Genetic Algorithm
    void settingsGeneticAlgorithm(time_t executionTime, int populationSize, bool mutationType, float mutationProbability, bool crossType, float crossProbability);
};


#endif
