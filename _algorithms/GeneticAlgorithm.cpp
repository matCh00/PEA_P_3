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



double GeneticAlgorithm::algorithmGeneticAlgorithm(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost) {

    return 0;
}