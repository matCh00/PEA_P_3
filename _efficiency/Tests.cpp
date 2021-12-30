#include "Tests.h"


ofstream file;


void Tests::startAutomaticTests() {

    file.open("C:/Users/matic/Desktop/current projects/PEA_P-3/resultsGA.txt");

    if(!file.is_open()) {

        cout << "plik nie jest otwarty";
    }

    GA();

    file.close();
}



void Tests::GA() {

    Graph *graph;
    vector<int> path;
    int cost;
    double exeTime;

    // parametry GA
    vector<time_t> timeGA = {5, 10};
    vector<int> population = {10};
    vector<bool> mutationType = {0, 1};
    vector<float> mutationProbability = {0.01};
    vector<float> crossProbability = {0.8};

    // GA
    GeneticAlgorithm *ga = new GeneticAlgorithm();

    for (int num = 0; num < 3; num++) {

        if (num == 0)
            graph = new Graph("C:/Users/matic/Desktop/PEA/sourcesAsymmetricTSP/br17.atsp");

        if (num == 1)
            graph = new Graph("C:/Users/matic/Desktop/PEA/sourcesAsymmetricTSP/ft70.atsp");

        if (num == 2)
            graph = new Graph("C:/Users/matic/Desktop/PEA/sourcesAsymmetricTSP/ftv170.atsp");


        path.resize(graph->getSize() + 1);

        for (int i = 0; i < timeGA.size(); ++i) {

            for (int j = 0; j < population.size(); ++j) {

                for (int k = 0; k < mutationType.size(); ++k) {

                    for (int l = 0; l < mutationProbability.size(); ++l) {

                        for (int m = 0; m < crossProbability.size(); ++m) {

                            ga->settingsGeneticAlgorithm(timeGA[i], population[j], mutationType[k], mutationProbability[l], crossProbability[m]);
                            exeTime = ga->algorithmGeneticAlgorithm(graph->getMatrix(), path, cost);

                            file << "SA:  rozmiar: " << graph->getSize() << " koszt: " << cost << " czas wykonania: " << exeTime << endl;
                        }
                    }
                }
            }
        }
    }
}