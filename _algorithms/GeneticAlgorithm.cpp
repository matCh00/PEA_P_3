#include "GeneticAlgorithm.h"



GeneticAlgorithm::GeneticAlgorithm() {

}


GeneticAlgorithm::~GeneticAlgorithm() {

}


/*
 * Algorytm genetyczny
 */
// TODO opisać algorytm i dodać komentarze do kodu
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
    float probability;

    for (size_t i = 0; i < population; i++) {

        // prawdopodobieństwo krzyżowania
        probability = static_cast <float> ((rand()) / (RAND_MAX));

        if (probability < crossProbability) {
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

        // dodawanie dziecka do nowej populacji
        childrenPopulation.push_back(child);
    }

    perm.resize(population + childrenPopulation.size());

    // nowa populacja
    for (size_t i = 0; i < childrenPopulation.size(); i++) {
        perm[i + population] = childrenPopulation[i];
    }
}



void GeneticAlgorithm::mutationChromosomes() {

    float probability;

    for (int i = 0; i < population; i++) {

        // prawdopodobieństwo mutacji
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



void GeneticAlgorithm::reducePopulation() {

    vector<int> temp(population);

    for (size_t i = 0; i < (int)perm.size()-1; i++) {

        for (size_t j = 0; j < perm.size() - 1; j++) {

            if (costs[j] > costs[j + 1]) {

                swap(costs[j], costs[j + 1]);
                swap(perm[j], perm[j + 1]);
            }
        }
    }

    if(perm.size() > population) {

        perm.erase(perm.begin() + population, perm.end());
        costs.erase(costs.begin() + population, costs.end());
    }
}



double GeneticAlgorithm::algorithmGeneticAlgorithm(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost) {

    Timer timer;

    // rozpoczęcie mierzenia czasu
    timer.start();

    matrix = originalMatrix;
    matrixSize = originalMatrix.size();

    costs.resize(population);
    perm.resize(population);

    // początkowa populacja chromosomów
    this->firstPopulation();

    // wykonywanie przez określony czas
    while (timer.stop() < executionTime) {

        // krzyżowanie chromosomów
        this->crossingChromosomes();

        // mutacja
        this->mutationChromosomes();

        // ocena przystosowania chromosomów
        this->fitnessAssessment();

        // utworzenie nowej populacji
        this->reducePopulation();
    }

    // najlepsze znalezione rozwiązanie
    bestCost = costs[0];
    //bestPath = ;

    return timer.stop();
}