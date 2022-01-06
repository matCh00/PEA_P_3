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
    vector<time_t> timeGA = {60};
    vector<int> population = {10, 50, 100, 150};
    vector<bool> mutationType = {0, 1};
    vector<float> mutationProbability1 = {0.01};
    vector<float> mutationProbability2 = {0.01, 0.05, 0.10};
    vector<bool> crossType = {0, 1};
    vector<float> crossProbability1 = {0.8};
    vector<float> crossProbability2 = {0.5, 0.7, 0.9};

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

                    for (int l = 0; l < mutationProbability1.size(); ++l) {

                        for (int n = 0; n < crossType.size(); ++n) {

                            for (int m = 0; m < crossProbability2.size(); ++m) {

                                ga->settingsGeneticAlgorithm(timeGA[i], population[j], mutationType[k], mutationProbability1[l], crossType[n], crossProbability2[m]);
                                exeTime = ga->algorithmGeneticAlgorithm(graph->getMatrix(), path, cost);

                                file << "SA:  rozmiar: " << graph->getSize() << " koszt: " << cost << " czas wykonania: " << exeTime << endl;
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < timeGA.size(); ++i) {

            for (int j = 0; j < population.size(); ++j) {

                for (int k = 0; k < mutationType.size(); ++k) {

                    for (int l = 0; l < mutationProbability2.size(); ++l) {

                        for (int n = 0; n < crossType.size(); ++n) {

                            for (int m = 0; m < crossProbability1.size(); ++m) {

                                ga->settingsGeneticAlgorithm(timeGA[i], population[j], mutationType[k], mutationProbability2[l], crossType[n], crossProbability1[m]);
                                exeTime = ga->algorithmGeneticAlgorithm(graph->getMatrix(), path, cost);

                                file << "SA:  rozmiar: " << graph->getSize() << " koszt: " << cost << " czas wykonania: " << exeTime << endl;
                            }
                        }
                    }
                }
            }
        }
    }
}