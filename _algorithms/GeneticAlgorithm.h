/*
 * Algorytm genetyczny jest heurystyką przeszukującą przestrzeń alternatywnych rozwiązań bazującą na zjawisku
 * ewolucji biologicznej. Zadaniem algorytmu jest symulowanie populacji danego rozwiązania dążącego do uzyskania
 * jak najlepszego rozwiązania badanego problemu. Wykorzystując naturalne mechanizmy takie jak rozmnażanie czy mutację
 * osobników staramy się przystosować kolejne pokolenia rozwiązań by coraz bardziej zbliżać się do optymalnego
 * rozwiązania (bez gwarancji na jego odnalezienie).
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
    void overwritePopulation(vector<vector<int>> &oldPopulation, vector<vector<int>> &newPopulation);

    // krzyżowanie rodziców
    void crossover(const vector<int> &parent1, const vector<int> &parent2, vector<int> &offspring1, vector<int> &offspring2);

    // operator krzyżowania - Partially Matched Crossover
    // http://aragorn.pb.bialystok.pl/~wkwedlo/EA5.pdf
    void crossover_PMX(const vector<int> &parent1, const vector<int> &parent2, vector<int> &offspring1, vector<int> &offspring2);

    // operator krzyżowania - Order Crossover
    // http://aragorn.pb.bialystok.pl/~wkwedlo/EA5.pdf
    void crossover_OX(const vector<int> &parent1, const vector<int> &parent2, vector<int> &offspring1, vector<int> &offspring2);

    // selekcja rodziców - Turniejowa
    // https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_parent_selection.htm
    vector<int> tournamentSelection(const vector<vector<int>> &currentPopulation);

    // liczenie kosztu ścieżki
    int calculateCost(const vector<int> &path);

    // mutacja potomków
    void mutation(vector<int> &generation);

    // metoda mutacji - Reverse
    void reverseMutation(int a, int b, vector<int> &path);

    // zliczanie kosztu ścieżki po mutacji Reverse
    void calculateReverseMutation(int i, int j, int &balance, const vector<int> &path);

    // metoda mutacji - Insert
    void insertMutation(int a, int b, vector<int> &path);

    // zliczanie kosztu ścieżki po mutacji Insert
    void calculateInsertMutation(int i, int j, int &balance, const vector<int> &path);


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
