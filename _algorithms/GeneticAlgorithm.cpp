#include "GeneticAlgorithm.h"



GeneticAlgorithm::GeneticAlgorithm() {

}


GeneticAlgorithm::~GeneticAlgorithm() {

}


/*
 * Algorytm genetyczny
 */
// TODO opisać algorytm i dodać komentarze do kodu, duże wyniki - są mniejsze po czasie ale i tak za duże, zwrócić optymalną ścieżkę
void GeneticAlgorithm::settingsGeneticAlgorithm(time_t executionTime, int population, bool mutationType, float mutationProbability, float crossProbability) {

    this->executionTime = executionTime;
    this->population = population;
    this->mutationType = mutationType;
    this->mutationProbability = mutationProbability;
    this->crossProbability = crossProbability;
}



vector<int> GeneticAlgorithm::findInitialSolution() {

    vector<int> route;
    route.resize(matrixSize);

    int localMinimum = 0;
    int bestMinimum, tempBest = 0, oldTempBest;

    // tablica odwiedzonych wierzchołków
    vector<int> visitedTab(matrixSize, 0);

    bool ifVisited;

    // dla każdego wierzchołka
    for (int i = 0; i < matrixSize; i++) {

        bestMinimum = INT_MAX;
        oldTempBest = tempBest;

        // szukamy najkorzystniejsze rozwiązanie (dla niesprawdzonych wierzchołków)
        for (int j = 0; j < matrixSize; j++) {

            ifVisited = true;

            if (j != oldTempBest) {

                for (int k = 0; k <= i; k++) {

                    if (j == visitedTab[k]) {
                        ifVisited = false;
                    }
                }

                // znalezienie lokalnego minimum dla danego wierzchołka
                if (matrix[oldTempBest][j] < bestMinimum && ifVisited == true) {
                    bestMinimum = matrix[oldTempBest][j];
                    tempBest = j;
                }
            }
        }
        if (i < matrixSize - 1) {

            // dodajemy optymalny koszt dla danego wierzchołka
            localMinimum = localMinimum + bestMinimum;
        }
        else {

            // dodajemy koszt z ostatniego wierzchołka do pierwszego
            localMinimum = localMinimum + matrix[oldTempBest][0];
        }

        // dodajemy do ścieżki optymalny wierzchołek
        route.push_back(oldTempBest);
        visitedTab[i] = tempBest;
    }

    // dodajemy na koniec ścieżki wierzchołek startowy
    route.push_back(0);

    visitedTab.clear();

    return route;
}



void GeneticAlgorithm::firstPopulation() {

    vector<int> cities;

    cities.resize(matrixSize);


// SPOSÓB LOSOWY

    // wypełnienie wektora
    iota(begin(cities), end(cities), 0);

    for (int i = 0; i < population; i++) {

        random_shuffle(cities.begin() + 1, cities.end());
        pop[i] = cities;
    }


// SPOSÓB ZACHŁANNY

//    for (int i = 0; i < population; i++) {
//
//        pop[i] = findInitialSolution();
//    }
}



void GeneticAlgorithm::fitnessAssessment() {

    int thisCost;
    costs.resize(pop.size());

    for (size_t i = 0; i < pop.size(); i++) {

        thisCost = 0;

        // zliczanie kosztu
        for (size_t j = 0; j < pop[i].size() - 1; j++) {
            thisCost += matrix[pop[i][j]][pop[i][j + 1]];
        }

        // przejście z ostatniego miasta do pierwszego
        thisCost += matrix[pop[i][matrixSize - 1]][pop[i][0]];

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
        int half = (int)pop[father].size() / 2;
        vector<int> temp(matrixSize / 2);
        vector<int> child(matrixSize);

        if (matrixSize % 2 == 0) {

            for (int j = 0; j < half; j++) {
                child[j] = pop[crossed[i].first][j];
            }

            vector<int> temp2(half);
            for (int j = 0; j < half; j++) {
                for (int k = 0; k < matrixSize; k++) {

                    if (pop[father][j + half] == pop[mother][k]) {
                        temp2[j] = k;
                    }
                }
            }

            sort(temp2.begin(), temp2.end());

            for (int j = 0; j < matrixSize / 2; j++) {
                child[j + matrixSize / 2] = pop[crossed[i].second][temp2[j]];
            }
        }

        else {

            for (int j = 0; j < half + 1; j++) {
                child[j] = pop[crossed[i].first][j];
            }

            vector<int> temp2(half);
            for (int j = 0; j < half; j++) {
                for (int k = 0; k < matrixSize; k++) {
                    if (pop[father][j + half + 1] == pop[mother][k]) {
                        temp2[j] = k;
                    }
                }
            }

            sort(temp2.begin(), temp2.end());

            for (int j = 0; j < matrixSize / 2; j++) {
                child[j + matrixSize / 2 + 1] = pop[crossed[i].second][temp2[j]];
            }
        }

        // dodawanie dziecka do nowej populacji
        childrenPopulation.push_back(child);
    }

    pop.resize(population + childrenPopulation.size());

    // nowa populacja
    for (size_t i = 0; i < childrenPopulation.size(); i++) {
        pop[i + population] = childrenPopulation[i];
    }
}



void GeneticAlgorithm::mutationChromosomes() {

    float probability;

    for (int i = 0; i < population; i++) {

        // prawdopodobieństwo mutacji
        probability = static_cast <float> (rand()) / (RAND_MAX);

        if (probability < mutationProbability) {

            // pierwsza metoda mutacji
            if (mutationType == 0) {
                mutationTransposition(i);
            }

            // druga metoda mutacji
            else if (mutationType == 1) {
                mutationInversion(i);
            }
        }
    }
}



void GeneticAlgorithm::mutationTransposition(int chap) {

    int first = rand() % (matrixSize - 1) + 1;
    int second = 0;

    do {
        second = rand() % (matrixSize - 1) + 1;
    } while (first == second);

    swap(pop[chap][first], pop[chap][second]);
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

    reverse(pop[chap].begin() + first, pop[chap].begin() + second);
}



void GeneticAlgorithm::reducePopulation() {

    vector<int> temp(population);

    for (size_t i = 0; i < (int)pop.size() - 1; i++) {

        for (size_t j = 0; j < pop.size() - 1; j++) {

            if (costs[j] > costs[j + 1]) {

                swap(costs[j], costs[j + 1]);
                swap(pop[j], pop[j + 1]);
            }
        }
    }

    if(pop.size() > population) {

        pop.erase(pop.begin() + population, pop.end());
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
    pop.resize(population);

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