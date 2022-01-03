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



vector<int> GeneticAlgorithm::tournamentSelection(vector<vector<int>> pop) {
    vector<int> best;
    vector<int> ind;

    random_device randomSrc;
    default_random_engine randomGen(randomSrc());
    uniform_int_distribution<> indRand(0, pop.size() - 1);

    int k = 2;
    for (int i = 1; i <= k; i++) {
        if (i == 1)
            best = pop.at(indRand(randomGen));

        else {
            ind = pop.at(indRand(randomGen));
            if (ind.at(matrixSize + 1) < best.at(matrixSize + 1))
                best = ind;
        }
    }

    return best;
}



void GeneticAlgorithm::doCrossover(vector<int> parent1, vector<int> parent2, vector<int> &offspring1, vector<int> &offspring2) {

    switch (crossType) {
        case 2:
            OrderCO(parent1, parent2, offspring1, offspring2); // jest gituwa
            break;

            case 7:
                EnhancedSequentialCO(parent1, parent2, offspring1); //jest gituwa
                break;
    }
}



void GeneticAlgorithm::OrderCO(vector<int> parent1, vector<int> parent2, vector<int> &offspring1, vector<int> &offspring2) {

    random_device randomSrc;
    default_random_engine randomGen(randomSrc());
    uniform_int_distribution<> nodeRand(1, matrixSize - 1);

    vector<int> visitedOffspring1(matrixSize, 0);
    vector<int> visitedOffspring2(matrixSize, 0);


    int a, b;

    do {
        a = nodeRand(randomGen);
        b = nodeRand(randomGen);
    } while (a == b || a > b);


    for (int i = a; i < b; i++) {
        offspring1.at(i) = parent1.at(i);
        offspring2.at(i) = parent2.at(i);
        visitedOffspring1.at(parent1.at(i)) = 1;
        visitedOffspring2.at(parent2.at(i)) = 1;
    }


    int omitted = 0, omitted2 = 0;

    for (int i = b; i < matrixSize; i++) {
        if (visitedOffspring1.at(parent2.at(i)) != 1) {
            offspring1.at(i - omitted) = parent2.at(i);
            visitedOffspring1.at(parent2.at(i)) = 1;
        } else
            omitted++;

        if (visitedOffspring2.at(parent1.at(i)) != 1) {
            offspring2.at(i - omitted2) = parent1.at(i);
            visitedOffspring2.at(parent1.at(i)) = 1;
        } else
            omitted2++;

    }

    int helpOmitted1 = omitted, helpOmitted2 = omitted2;

    for (int i = 1; i < b; i++) {

        if (visitedOffspring1.at(parent2.at(i)) != 1) {
            if (omitted != 0) {
                offspring1.at(matrixSize - omitted) = parent2.at(i);
                visitedOffspring1.at(parent2.at(i)) = 1;
                omitted--;
            } else {
                offspring1.at(i - helpOmitted1) = parent2.at(i);
                visitedOffspring1.at(parent2.at(i)) = 1;
            }
        } else {
            helpOmitted1++;
        }


        if (visitedOffspring2.at(parent1.at(i)) != 1) {
            if (omitted2 != 0) {
                offspring2.at(matrixSize - omitted2) = parent1.at(i);
                visitedOffspring2.at(parent1.at(i)) = 1;
                omitted2--;
            } else {
                offspring2.at(i - helpOmitted2) = parent1.at(i);
                visitedOffspring2.at(parent1.at(i)) = 1;
            }
        } else {
            helpOmitted2++;
        }

    }

    offspring1.at(matrixSize + 1) = calculateCost(offspring1);
    offspring2.at(matrixSize + 1) = calculateCost(offspring2);

}



void GeneticAlgorithm::EnhancedSequentialCO(vector<int> parent1, vector<int> parent2, vector<int> &offspring) {

    int omitted = 0, inserted = 0;
    vector<int> visitedOffspring(matrixSize, 0);
    vector<int> indexParent1(matrixSize, 0);
    vector<int> indexParent2(matrixSize, 0);

    for (int i = 0; i < matrixSize; i++) {
        indexParent1.at(parent1.at(i)) = i;
        indexParent2.at(parent2.at(i)) = i;
    }

    offspring.at(0) = 0;

    int node1, node2, minimum1 = INT_MAX, minimum2 = INT_MAX;

    for (int i = 0; i < matrixSize; i++) {
        if (visitedOffspring.at(i) != 1 && matrix[parent1.at(1)][i] < minimum1)
            minimum1 = matrix[parent1.at(1)][i];

        if (visitedOffspring.at(i) != 1 && matrix[parent2.at(1)][i] < minimum2)
            minimum2 = matrix[parent2.at(1)][i];
    }

    if (matrix[0][parent1.at(1)] + minimum1 < matrix[0][parent2.at(1)] + minimum2) {
        offspring.at(1) = parent1.at(1);
        visitedOffspring.at(parent1.at(1)) = 1;
    } else {
        offspring.at(1) = parent2.at(1);
        visitedOffspring.at(parent2.at(1)) = 1;
    }

    for (int i = 2; i < matrixSize; i++) {
        if (indexParent1.at(offspring.at(i - 1)) + 1 <= matrixSize - 1 &&
        visitedOffspring.at(parent1.at(indexParent1.at(offspring.at(i - 1)) + 1)) != 1) {
            node1 = parent1.at(indexParent1.at(offspring.at(i - 1)) + 1);
        } else {
            for (int j = 1; j < matrixSize; j++)
                if (visitedOffspring.at(j) != 1) {
                    node1 = j;
                    break;
                }
        }
        if (indexParent2.at(offspring.at(i - 1)) + 1 <= matrixSize - 1 &&
        visitedOffspring.at(parent2.at(indexParent2.at(offspring.at(i - 1)) + 1)) != 1) {
            node2 = parent2.at(indexParent2.at(offspring.at(i - 1)) + 1);
        } else {
            for (int j = 1; j < matrixSize; j++)
                if (visitedOffspring.at(j) != 1) {
                    node2 = j;
                    break;
                }
        }

        minimum1 = INT_MAX, minimum2 = INT_MAX;

        for (int j = 0; j < matrixSize; j++) {
            if (visitedOffspring.at(j) != 1 && matrix[parent1.at(1)][j] < minimum1)
                minimum1 = matrix[node1][j];

            if (visitedOffspring.at(j) != 1 && matrix[parent2.at(1)][j] < minimum2)
                minimum2 = matrix[node2][j];
        }

        if (matrix[offspring.at(i - 1)][node1] + minimum1 < matrix[offspring.at(i - 1)][node2] + minimum2) {
            offspring.at(i) = node1;
            visitedOffspring.at(node1) = 1;
        } else {
            offspring.at(i) = node2;
            visitedOffspring.at(node2) = 1;
        }
    }

    offspring.at(matrixSize + 1) = calculateCost(offspring);

    //showVector(offspring);
}



void GeneticAlgorithm::mutation(vector<int> &ind) {
    random_device randomSrc;
    default_random_engine randomGen(randomSrc());
    uniform_int_distribution<> nodeRand(1, matrixSize - 1);

    int bestI = 0;
    int bestJ = 0;
    int bestBalance = INT_MAX;

    int i, j, balance = 0;


    if (mutationType == 0) {
        for (int k = 0; k < 2; k++) { //po 2 losowania, wybierane lepsze
            do {
                i = nodeRand(randomGen);
                j = nodeRand(randomGen);
            } while (i == j || j < i);

            calculateSwap(i, j, balance, ind);
            if (balance < bestBalance) {
                bestI = i;
                bestJ = j;
                bestBalance = balance;
            }
        }
        swapVector(bestI, bestJ, ind);
    } else if (mutationType == 2) {
        for (int k = 0; k < 2; k++) {
            do {
                i = nodeRand(randomGen);
                j = nodeRand(randomGen);
            } while (i == j || j < i);

            calculateReverse(i, j, balance, ind);
            if (balance < bestBalance) {
                bestI = i;
                bestJ = j;
                bestBalance = balance;
            }
        }

        reverseVector(bestI, bestJ, ind);
    } else if (mutationType == 1) {
        for (int k = 0; k < 2; k++) {
            do {
                i = nodeRand(randomGen);
                j = nodeRand(randomGen);
            } while (i == j - 1 || i == j || i == j + 1);

            calculateInsert(i, j, balance, ind);
            if (balance < bestBalance) {
                bestI = i;
                bestJ = j;
                bestBalance = balance;
            }
        }
        insertVector(bestI, bestJ, ind);
    }

    ind.at(matrixSize + 1) += bestBalance;
}


