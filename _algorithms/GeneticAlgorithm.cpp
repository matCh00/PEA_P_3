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

    vector<vector<int>> childrenPopulation;
    vector<pair<int, int>> crossed;
    int temp = 0;
    vector<float> probability(population);

    for (size_t j = 0; j < probability.size(); j++) {
        probability[j] = static_cast <float> ((rand()) / (RAND_MAX));
    }

    for (size_t i = 0; i < probability.size(); i++) {
        if (probability[i] < crossProbability) {
            do {
                temp = rand() % population;
            } while (temp == i);
            crossed.push_back(make_pair(i, temp));
        }
    }

    crossed.shrink_to_fit();

    for (size_t i = 0; i < crossed.size(); i++) {

        int father = crossed[i].first;
        int mother = crossed[i].second;

        // krzyżowanie
        int half = (int)perm[father].size() / 2;
        vector<int> temp(matrixSize / 2);
        vector<int> child(matrixSize);

        if (matrixSize % 2 == 0){

            for (int j = 0; j < half; j++) {
                child[j] = perm[crossed[i].first][j];
            }

            vector<int> temp2(half);
            for (int j = 0; j < half; j++) {
                for (int k = 0; k < matrixSize; k++) {

                    if (perm[father][j + half] == perm[mother][k]) {
                        temp2[j] = k;
                    }
                }
            }

            sort(temp2.begin(), temp2.end());

            for (int j = 0; j < matrixSize / 2; j++) {
                child[j + matrixSize / 2] = perm[crossed[i].second][temp2[j]];
            }
        }

        else {

            for (int j = 0; j < half + 1; j++) {
                child[j] = perm[crossed[i].first][j];
            }

            vector<int> temp2(half);
            for (int j = 0; j < half; j++) {
                for (int k = 0; k < matrixSize; k++) {
                    if (perm[father][j + half + 1] == perm[mother][k]) {
                        temp2[j] = k;
                    }
                }
            }

            sort(temp2.begin(), temp2.end());

            for (int j = 0; j < matrixSize / 2; j++) {
                child[j + matrixSize / 2 + 1] = perm[crossed[i].second][temp2[j]];
            }
        }

        childrenPopulation.push_back(child);
    }

    perm.resize(population + childrenPopulation.size());

    for (size_t i = 0; i < childrenPopulation.size(); i++) {
        perm[i + population] = childrenPopulation[i];
    }
}



void GeneticAlgorithm::mutationChromosomes() {

    float probability;

    for (int i = 0; i < population; i++) {

        probability = static_cast <float> (rand()) / (RAND_MAX);

        if (probability < mutationProbability) {

            // pierwsza metoda mutacji
            if (mutationType == 1) {
                mutationInversion(i);
            }

            // druga metoda mutacji
            else if (mutationType == 2) {
                mutationInsertion(i);
            }
        }
    }
}



void GeneticAlgorithm::mutationInsertion(int chap) {

    auto first = perm[chap].begin() + 1 + (rand() % matrixSize - 1);
    auto last = perm[chap].begin() + 1 + (rand() % matrixSize - 1);

    perm[chap].insert(first, *last);
    perm[chap].erase(last);
}



void GeneticAlgorithm::mutationInversion(int chap) {

    int first = rand() % (matrixSize - 1) + 1;
    int second = 0;

    do {
        second = rand() % (matrixSize - 1) + 1;
    } while (first == second);

    if (second < first) {
        swap(first, second);
    }

    reverse(perm[chap].begin() + first, perm[chap].begin() + second);
}



double GeneticAlgorithm::algorithmGeneticAlgorithm(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost) {

    costs.resize(population);
    perm.resize(population);

    return 0;
}