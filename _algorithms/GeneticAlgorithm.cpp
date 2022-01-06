#include "GeneticAlgorithm.h"



GeneticAlgorithm::GeneticAlgorithm() {

}


GeneticAlgorithm::~GeneticAlgorithm() {

}


/*
 * Algorytm genetyczny jest heurystyką przeszukującą przestrzeń alternatywnych rozwiązań bazującą na zjawisku
 * ewolucji biologicznej. Zadaniem algorytmu jest symulowanie populacji danego rozwiązania dążącego do uzyskania
 * jak najlepszego rozwiązania badanego problemu. Wykorzystując naturalne mechanizmy takie jak rozmnażanie czy mutację
 * osobników staramy się przystosować kolejne pokolenia rozwiązań by coraz bardziej zbliżać się do optymalnego
 * rozwiązania (bez gwarancji na jego odnalezienie).
 */
// TODO dodać komentarze do ESCX, można coś pozmieniać
void GeneticAlgorithm::settingsGeneticAlgorithm(time_t executionTime, int populationSize, bool mutationType, float mutationProbability, bool crossType, float crossProbability) {

    this->executionTime = executionTime;
    this->populationSize = populationSize;
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

    // najlepsza ścieżka, ostatnim elementem jest jej koszt
    vector<int> best(matrixSize + 2);

    // populacja
    vector<vector<int>> population;

    // rodzice
    vector<int> parent1;
    vector<int> parent2;

    // potomstwo
    vector<int> offspring1(matrixSize + 2, 0);
    vector<int> offspring2(matrixSize + 2, 0);

    // generowanie początkowej populacji
    generateFirstPopulation(population);

    time_t seconds = 0;

    // wykonywanie przez określony czas
    while (timer.stop() < executionTime) {

        // nowa populacja
        vector<vector<int>> newPopulation;

        while (newPopulation.size() != populationSize) {

            // selekcja turniejowa dwóch rodziców
            parent1 = tournamentSelection(population);
            parent2 = tournamentSelection(population);

            // krzyżowanie rodziców z ustalonym prawdopodobieństwem
            if (static_cast<float>(rand()) / RAND_MAX < crossProbability) {
                crossover(parent1, parent2, offspring1, offspring2);
            }

            // mutacja pierwszego potomka z ustalonym prawdopodobieństwem
            if (static_cast<float>(rand()) / RAND_MAX < mutationProbability) {
                mutation(offspring1);
                newPopulation.push_back(offspring1);
                mutation(offspring2);
                newPopulation.push_back(offspring2);
            }
        }

        // sortowanie nowej populacji
        sortPopulation(newPopulation);

        // nadpisanie populacji
        overwritePopulation(population, newPopulation);

        // sortowanie populacji
        sortPopulation(population);

        // najlepsze dotychczasowe rozwiązanie
        best = population.at(0);

        // zwalnianie pamięci
        newPopulation.clear();

        if (timer.stop() > seconds) {
            cout << timer.stop() << "s " << best.at(matrixSize + 1) << endl;
            seconds += 5;
        }
    }

    // najlepszy znaleziony koszt
    bestCost = best.at(matrixSize + 1);

    // najlepsza znaleziona ścieżka
    best.erase(best.end() - 1);
    bestPath = best;

    // zwalnianie pamięci
    best.clear();
    population.clear();
    parent1.clear();
    parent2.clear();
    offspring1.clear();
    offspring2.clear();

    // czas wykonania
    return timer.stop();
}



void GeneticAlgorithm::generateFirstPopulation(vector<vector<int>> &firstPopulation) {

    for (int i = 0; i < populationSize; i++) {
        vector<int> route;

        // algorytm losowo zachłanny
        route.push_back(randomGreedyAlgorithm(route));
        firstPopulation.push_back(route);
    }

    // sortowanie populacji
    sortPopulation(firstPopulation);
}



int GeneticAlgorithm::randomGreedyAlgorithm(vector<int> &generatedPath) {

    Randomize r;

    // zmienne algorytmu
    int localMin = 0, bestMin, currentBest = 0, oldBest, randomNode;
    bool ifVisited;

    vector<int> visited(matrixSize, 0);

    // miasta wygenerowane losowo, reszta zachłannie
    int randomNodes = populationSize / 2;

    for (int i = 0; i < matrixSize; i++) {

        bestMin = INT_MAX;
        oldBest = currentBest;

        // miasto wybierane losowo
        if (randomNodes != 0) {

            ifVisited = false;

            while (ifVisited == false) {

                randomNode = r.random_mt19937(0, matrixSize - 1);
                ifVisited = true;

                for (int k = 0; k <= i; k++) {

                    if (randomNode == visited[k]) {
                        ifVisited = false;
                    }
                }
            }

            currentBest = randomNode;
            bestMin = matrix[oldBest][randomNode];
            randomNodes--;
        }

        // miasto wybierane zachłannie
        else {
            for (int j = 0; j < matrixSize; j++) {

                ifVisited = true;

                if (j != oldBest) {

                    for (int k = 0; k <= i; k++) {

                        if (j == visited[k]) {
                            ifVisited = false;
                        }
                    }

                    if (matrix[oldBest][j] < bestMin && ifVisited == true) {

                        bestMin = matrix[oldBest][j];
                        currentBest = j;
                    }
                }
            }
        }

        if (i < matrixSize - 1)
            localMin = localMin + bestMin;
        else
            localMin = localMin + matrix[oldBest][0];

        // dodanie miasta
        generatedPath.push_back(oldBest);
        visited[i] = currentBest;
    }

    // dodanie miasta początkowego
    generatedPath.push_back(0);

    return localMin;
}



vector<int> GeneticAlgorithm::tournamentSelection(vector<vector<int>> currentPopulation) {

    vector<int> best;
    vector<int> second;

    random_device randomSrc;
    default_random_engine randomGen(randomSrc());
    uniform_int_distribution<> indRand(0, currentPopulation.size() - 1);

    for (int i = 1; i <= 2; i++) {

        // najlepszy możliwy
        if (i == 1)
            best = currentPopulation.at(indRand(randomGen));

        else {
            second = currentPopulation.at(indRand(randomGen));

            // wybranie lepszego
            if (second.at(matrixSize + 1) < best.at(matrixSize + 1))
                best = second;
        }
    }

    return best;
}



void GeneticAlgorithm::crossover(vector<int> parent1, vector<int> parent2, vector<int> &offspring1, vector<int> &offspring2) {

    switch (crossType) {

        case 0:
            crossover_PMX(parent1, parent2, offspring1, offspring2);
            break;

        case 1:
            crossover_OX(parent1, parent2, offspring1, offspring2);
            break;
    }
}



void GeneticAlgorithm::crossover_PMX(vector<int> parent1, vector<int> parent2, vector<int> &offspring1, vector<int> &offspring2) {

    Randomize r;

    vector<bool> visitedOffspring1(matrixSize, 0);
    vector<bool> visitedOffspring2(matrixSize, 0);

    int a, b;

    // losujemy dwa różne indeksy
    do {
        a = r.random_engine(1, matrixSize - 1);
        b = r.random_engine(1, matrixSize - 1);
    } while (a == b || a > b);

    for (int i = a; i < b; i++) {
        offspring1.at(i)=parent2.at(i);
        offspring2.at(i)=parent1.at(i);
        visitedOffspring1.at(parent2.at(i)) = 1;
        visitedOffspring2.at(parent1.at(i)) = 1;
    }

    for (int i = a-1; i >=0; i--) {

        if (visitedOffspring1.at(parent1.at(i)) != 1) {
            offspring1.at(i)=parent1.at(i);
            if (parent1.at(i) != 0)
                visitedOffspring1.at(parent1.at(i)) = 1;
        }
        else
            offspring1.at(i) = 1000;

        if (visitedOffspring2.at(parent2.at(i)) != 1) {
            offspring2.at(i) = parent2.at(i);
            if (parent2.at(i) != 0)
                visitedOffspring2.at(parent2.at(i)) = 1;
        }
        else
            offspring2.at(i) = 1000;
    }


    for (int i = b; i <= matrixSize; i++) {

        if (visitedOffspring1.at(parent1.at(i)) != 1) {
            offspring1.at(i)=parent1.at(i);
            visitedOffspring1.at(parent1.at(i)) = 1;
        }
        else
            offspring1.at(i)=1000;

        if (visitedOffspring2.at(parent2.at(i))!= 1) {
            offspring2.at(i) = parent2.at(i);
            visitedOffspring2.at(parent2.at(i)) = 1;
        }
        else
            offspring2.at(i) = 1000;
    }

    int temp;
    bool continueAlg = true;

    for (int i = 0; i < matrixSize; i++) {

        if (offspring1.at(i) == 1000) {
            temp = i;

            while (continueAlg == true) {

                for (int j=0; j < matrixSize; j++) {

                    if (parent2.at(j) == parent1.at(temp)) {

                        if (visitedOffspring1.at(parent1.at(j)) != 1) {

                            offspring1.at(i) = parent1.at(j);
                            continueAlg = false;
                            break;
                        }
                        else {
                            temp = j;
                        }
                    }
                }
            }
            continueAlg = true;
        }

        if (offspring2.at(i) == 1000) {

            temp = i;

            while (continueAlg == true) {

                for (int j = 0; j < matrixSize; j++) {

                    if (parent1.at(j) == parent2.at(temp)) {

                        if (visitedOffspring2.at(parent2.at(j)) != 1) {

                            offspring2.at(i) = parent2.at(j);
                            continueAlg = false;
                            break;
                        }
                        else {
                            temp = j;
                        }
                    }
                }
            }
            continueAlg = true;
        }
    }

    // ostatnim elementem jest koszt ścieżki
    offspring1.at(matrixSize + 1) = calculateCost(offspring1);
    offspring2.at(matrixSize + 1) = calculateCost(offspring2);

    // zwolnienie pamięci
    visitedOffspring1.clear();
    visitedOffspring2.clear();
}



void GeneticAlgorithm::crossover_OX(vector<int> parent1, vector<int> parent2, vector<int> &offspring1, vector<int> &offspring2) {

    Randomize r;

    vector<bool> visitedOffspring1(matrixSize, 0);
    vector<bool> visitedOffspring2(matrixSize, 0);

    int a, b;

    // losujemy dwa różne indeksy
    do {
        a = r.random_mt19937(1, matrixSize - 1);
        b = r.random_mt19937(1, matrixSize - 1);
    } while (a == b || a > b);

    // nowa sekwencja wierzchołków, p1-o1, p2-o2
    for (int i = a; i < b; i++) {
        offspring1.at(i) = parent1.at(i);
        offspring2.at(i) = parent2.at(i);
        visitedOffspring1.at(parent1.at(i)) = 1;
        visitedOffspring2.at(parent2.at(i)) = 1;
    }

    // pominięte
    int omitted = 0, omitted2 = 0;

    // wstawianie sekwencji zapobiegając kolizji, p1-o2, p2-o1
    for (int i = b; i < matrixSize; i++) {

        if (visitedOffspring1.at(parent2.at(i)) != 1) {
            offspring1.at(i - omitted) = parent2.at(i);
            visitedOffspring1.at(parent2.at(i)) = 1;
        }
        else
            omitted++;

        if (visitedOffspring2.at(parent1.at(i)) != 1) {
            offspring2.at(i - omitted2) = parent1.at(i);
            visitedOffspring2.at(parent1.at(i)) = 1;
        }
        else
            omitted2++;
    }

    // pomocnicze
    int tempOmitted1 = omitted, tempOmitted2 = omitted2;

    // wstawienie pozostałych miast
    for (int i = 1; i < b; i++) {

        if (visitedOffspring1.at(parent2.at(i)) != 1) {

            if (omitted != 0) {
                offspring1.at(matrixSize - omitted) = parent2.at(i);
                visitedOffspring1.at(parent2.at(i)) = 1;
                omitted--;
            }
            else {
                offspring1.at(i - tempOmitted1) = parent2.at(i);
                visitedOffspring1.at(parent2.at(i)) = 1;
            }
        } else {
            tempOmitted1++;
        }

        if (visitedOffspring2.at(parent1.at(i)) != 1) {
            if (omitted2 != 0) {
                offspring2.at(matrixSize - omitted2) = parent1.at(i);
                visitedOffspring2.at(parent1.at(i)) = 1;
                omitted2--;
            }
            else {
                offspring2.at(i - tempOmitted2) = parent1.at(i);
                visitedOffspring2.at(parent1.at(i)) = 1;
            }
        } else {
            tempOmitted2++;
        }
    }

    // ostatnim elementem jest koszt ścieżki
    offspring1.at(matrixSize + 1) = calculateCost(offspring1);
    offspring2.at(matrixSize + 1) = calculateCost(offspring2);

    // zwolnienie pamięci
    visitedOffspring1.clear();
    visitedOffspring2.clear();
}



void GeneticAlgorithm::mutation(vector<int> &generation) {

    Randomize r;

    int bestI = 0, bestJ = 0, bestBalance = INT_MAX, i, j, balance = 0;

    // mutacja REVERSE
    if (mutationType == 0) {

        // 5 prób na najlepszą mutację
        for (int k = 0; k < 5; k++) {

            // losujemy dwa różne wierzchołki
            do {
                i = r.random_engine(1, matrixSize - 1);
                j = r.random_engine(1, matrixSize - 1);
            } while (i == j || j < i);

            // obliczamy balans po mutacji
            calculateReverseMutation(i, j, balance, generation);

            // jeżeli jest to najlepsza dotychczas mutacja
            if (balance < bestBalance) {
                bestI = i;
                bestJ = j;
                bestBalance = balance;
            }
        }

        // mutacja na danej generacji (najlepsza spośród 5 prób)
        reverseMutation(bestI, bestJ, generation);
    }

    // mutacja INSERT
    else if (mutationType == 1) {

        // 5 prób na najlepszą mutację
        for (int k = 0; k < 5; k++) {

            // losujemy dwa różne wierzchołki
            do {
                i = r.random_engine(1, matrixSize - 1);
                j = r.random_engine(1, matrixSize - 1);
            } while (i == j - 1 || i == j || i == j + 1);

            // obliczamy balans po mutacji
            calculateInsertMutation(i, j, balance, generation);

            // jeżeli jest to najlepsza dotychczas mutacja
            if (balance < bestBalance) {
                bestI = i;
                bestJ = j;
                bestBalance = balance;
            }
        }

        // mutacja na danej generacji (najlepsza spośród 5 prób)
        insertMutation(bestI, bestJ, generation);
    }

    // aktualizacja kosztu danej generacji
    generation.at(matrixSize + 1) += bestBalance;
}



void GeneticAlgorithm::sortPopulation(vector<vector<int>> &toSort) {

    // sortujemy populację tak, aby na indeksie 0 było zawsze najlepsze rozwiązanie, a na ostatniej pozycji najgorsze

    sort(toSort.begin(), toSort.end(),[&](const vector<int> &a, const vector<int> &b) {

        return a.at(matrixSize + 1) < b.at(matrixSize + 1);
    });
}



void GeneticAlgorithm::overwritePopulation(vector<vector<int>> &oldPopulation, vector<vector<int>> newPopulation) {

    // elitaryzm -> tutaj wynosi 5
    int elitism = 5;

    for (int i = elitism; i < oldPopulation.size(); i++)

        // pozostawiamy 5 najlepszych generacji
        oldPopulation.at(i) = newPopulation.at(i - elitism);
}



int GeneticAlgorithm::calculateCost(vector<int> path) {

    int sum = 0, a, b;

    for (int i = 0; i < matrixSize; i++) {
        a = path.at(i);
        b = path.at(i + 1);
        sum += matrix[a][b];
    }
    return sum;
}



void GeneticAlgorithm::insertMutation(int a, int b, vector<int> &path) {

    // wstawianie elementu
    path.insert(path.begin() + b, path.at(a));

    // usuwanie nadmiarowego elementu
    if (b > a)
        path.erase(path.begin() + a);
    else
        path.erase(path.begin() + a + 1);
}



void GeneticAlgorithm::calculateInsertMutation(int i, int j, int &balance, vector<int> path) {

    balance = matrix[path.at(i)][path.at(j)] - matrix[path.at(i)][path.at(i + 1)];
    balance = balance - matrix[path.at(j - 1)][path.at(j)] - matrix[path.at(i - 1)][path.at(i)];
    balance = balance + matrix[path.at(i - 1)][path.at(i + 1)] + matrix[path.at(j - 1)][path.at(i)];
}



void GeneticAlgorithm::reverseMutation(int a, int b, vector<int> &path) {

    // odwrócenie podciągu pomiędzy dwoma elementami
    reverse(path.begin() + a, path.begin() + b + 1);
}



void GeneticAlgorithm::calculateReverseMutation(int i, int j, int &balance, vector<int> path) {

    balance = 0 - matrix[path.at(i - 1)][path.at(i)] - matrix[path.at(j)][path.at(j + 1)];
    balance = balance + matrix[path.at(i - 1)][path.at(j)] + matrix[path.at(i)][path.at(j + 1)];

    for (int k = i; k < j; k++)
        balance = balance - matrix[path.at(k)][path.at(k + 1)] + matrix[path.at(k + 1)][path.at(k)];
}
