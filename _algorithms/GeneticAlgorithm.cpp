#include "GeneticAlgorithm.h"



GeneticAlgorithm::GeneticAlgorithm() {

}


GeneticAlgorithm::~GeneticAlgorithm() {

}


/*
 * Algorytm genetyczny
 */

void GeneticAlgorithm::settingsGeneticAlgorithm(time_t executionTime, int population, bool mutationType, float mutationProbability, float crossProbability) {

    this->executionTime = executionTime;
    this->population = population;
    this->mutationType = mutationType;
    this->mutationProbability = mutationProbability;
    this->crossProbability = crossProbability;
}



void GeneticAlgorithm::firstPopulation() {

    vector<int> cities;

    cities.resize(matrixSize);

    // wypełnienie wektora
    iota(begin(cities), end(cities), 0);

    for (int i = 0; i < population; i++) {

        // sposób losowy
        random_shuffle(cities.begin() + 1, cities.end());

        perm[i] = cities;
    }
}



void GeneticAlgorithm::fitnessAssessment() {

    int thisCost;
    costs.resize(perm.size());

    for (size_t i = 0; i < perm.size(); i++) {

        thisCost = 0;

        // zliczanie kosztu
        for (size_t j = 0; j < perm[i].size() - 1; j++) {
            thisCost += matrix[perm[i][j]][perm[i][j + 1]];
        }
        thisCost += matrix[perm[i][matrixSize - 1]][perm[i][0]];

        costs[i] = thisCost;
    }
}



void GeneticAlgorithm::crossingChromosomes() {

}



void GeneticAlgorithm::mutationChromosomes() {

}



double GeneticAlgorithm::algorithmGeneticAlgorithm(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost) {

    costs.resize(population);
    perm.resize(population);

    return 0;
}