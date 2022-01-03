#include "GeneticAlgorithm.h"



GeneticAlgorithm::GeneticAlgorithm() {

}


GeneticAlgorithm::~GeneticAlgorithm() {

}


/*
 * Algorytm genetyczny
 */
// TODO opisać algorytm i dodać komentarze do kodu, duże wyniki - są mniejsze po czasie ale i tak za duże, zwrócić optymalną ścieżkę
void GeneticAlgorithm::settingsGeneticAlgorithm(time_t executionTime, int population, bool mutationType, float mutationProbability, bool crossType, float crossProbability) {

    this->executionTime = executionTime;
    this->population = population;
    this->mutationType = mutationType;
    this->mutationProbability = mutationProbability;
    this->crossType = crossType;
    this->crossProbability = crossProbability;
}



double GeneticAlgorithm::algorithmGeneticAlgorithm(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost) {

    Timer timer;

    // rozpoczęcie mierzenia czasu
    timer.start();

    matrix = originalMatrix;
    matrixSize = originalMatrix.size();


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



void GeneticAlgorithm::generateInitialPopulation(vector<vector<int>> &pop) {

    for (int i = 1; i < population; i++) {
        vector<int> route;
        route.push_back(getInitialGreedyAndRandom(route)); // (n-2)/2 osobnikow losowo zachlannym
        pop.push_back(route);
    }

    vector<int> route;

    route.clear();
    route.push_back(getInitialGreedy(route)); // 1 osobnik zachlannym
    pop.push_back(route);

    sortVector(pop);
}



int GeneticAlgorithm::getInitialGreedy(vector<int> &bestTab) {
    int localMin = 0;
    int bestMin, tempBest = 0, oldTempBest = 0;
    int *visitedTab = new int[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        visitedTab[i] = 0;
    }

    bool ifVisited;
    for (int i = 0; i < matrixSize; i++) {
        bestMin = INT_MAX;
        oldTempBest = tempBest;
        for (int j = 0; j < matrixSize; j++) {
            ifVisited = true;
            if (j != oldTempBest) {
                for (int k = 0; k <= i; k++) {
                    if (j == visitedTab[k]) {
                        ifVisited = false;
                    }
                }
                if (matrix[oldTempBest][j] < bestMin && ifVisited == true) {
                    bestMin = matrix[oldTempBest][j];
                    tempBest = j;
                }
            }
        }
        if (i < matrixSize - 1)
            localMin = localMin + bestMin;
        else
            localMin = localMin + matrix[oldTempBest][0];

        bestTab.push_back(oldTempBest);
        visitedTab[i] = tempBest;
    }
    bestTab.push_back(0);

    delete[]visitedTab;

    return localMin;

}



int GeneticAlgorithm::getInitialGreedyAndRandom(vector<int> &bestTab) {

    random_device randomSrc;
    default_random_engine randomGen(randomSrc());
    uniform_int_distribution<> nodeRand(0, matrixSize - 1);

    int bestMin, tempBest = 0, oldTempBest = 0;
    int localMin = 0;
    int *visitedTab = new int[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        visitedTab[i] = 0;
    }

    bool ifVisited;
    int randomNode;
    int remainingNodes = 3;

    for (int i = 0; i < matrixSize; i++) {
        bestMin = INT_MAX;
        oldTempBest = tempBest;
        if (remainingNodes != 0) {
            ifVisited = false;
            while (ifVisited == false) {
                randomNode = nodeRand(randomGen);
                ifVisited = true;
                for (int k = 0; k <= i; k++) {
                    if (randomNode == visitedTab[k]) {
                        ifVisited = false;
                    }
                }
            }
            tempBest = randomNode;
            bestMin = matrix[oldTempBest][randomNode];
            remainingNodes--;

        } else {
            for (int j = 0; j < matrixSize; j++) {
                ifVisited = true;
                if (j != oldTempBest) {
                    for (int k = 0; k <= i; k++) {
                        if (j == visitedTab[k]) {
                            ifVisited = false;
                        }
                    }
                    if (matrix[oldTempBest][j] < bestMin && ifVisited == true) {
                        bestMin = matrix[oldTempBest][j];
                        tempBest = j;
                    }
                }
            }
        }
        if (i < matrixSize - 1)
            localMin = localMin + bestMin;
        else
            localMin = localMin + matrix[oldTempBest][0];

        bestTab.push_back(oldTempBest);
        visitedTab[i] = tempBest;
    }
    bestTab.push_back(0);

    return localMin;
}
