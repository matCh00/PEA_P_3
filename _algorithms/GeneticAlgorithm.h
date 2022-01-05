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

    // generowanie początkowej populacji algorytmem losowo zachłannym
    void generateFirstPopulation(vector<vector<int>> &firstPopulation);

    // algorytm losowo zachłanny generowania ścieżki
    // część miast jest generowanych losowo, część zachłannie
    int randomGreedyAlgorithm(vector<int> &generatedPath);

    // sortowanie populacji
    // na indeksie 0 zawsze najlepsze rozwiązanie, na ostatniej pozycji najgorsze
    void sortPopulation(vector<vector<int>> &toSort);

    // nadpisywanie populacji
    // ELITARYZM -> kilka najlepszych osobników ze starej populacji + reszta z nowej populacji
    void overwritePopulation(vector<vector<int>> &oldPopulation, vector<vector<int>> newPopulation);

    // krzyżowanie rodziców
    void crossover(vector<int> parent1, vector<int> parent2, vector<int> &offspring1, vector<int> &offspring2);

    // metoda krzyżowania - Order Crossover
    void crossover_OX(vector<int> parent1, vector<int> parent2, vector<int> &offspring1, vector<int> &offspring2);

    // metoda krzyżowania - Enhanced Sequential Constructive Crossover
    void crossover_ESCX(vector<int> parent1, vector<int> parent2, vector<int> &offspring);

    // selekcja rodziców - Turniejowa
    // losuje 2 osobników i wybiera lepszego
    vector<int> tournamentSelection(vector<vector<int>> currentPopulation);

    // liczenie kosztu ścieżki
    int calculateCost(vector<int> path);

    // mutacja potomków
    void mutation(vector<int> &generation);

    // metoda mutacji - Reverse
    void reverseMutation(int a, int b, vector<int> &path);

    // zliczanie kosztu ścieżki po mutacji Reverse
    void calculateReverseMutation(int i, int j, int &balance, vector<int> path);

    // metoda mutacji - Insert
    void insertMutation(int a, int b, vector<int> &path);

    // zliczanie kosztu ścieżki po mutacji Insert
    void calculateInsertMutation(int i, int j, int &balance, vector<int> path);


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
