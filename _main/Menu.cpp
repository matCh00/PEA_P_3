#include "Menu.h"



Menu::Menu() {

}



Menu::~Menu() {

}



void Menu::startMenu() {

    // do menu
    int key;
    bool run = true;
    string name;

    // defaultowe ustawienia algorytmu
    timeGA = 30;
    populationSize = 10;
    mutationType = 0;
    mutationProbability = 0.01;
    crossType = 0;
    crossProbability = 0.8;

    // instancja grafu
    graph = new Graph();


    while (run) {

        cout << "\n [1] - wczytaj dane z pliku \n"
                " [2] - wyswietl dane \n"
                "  [3] - algorytm genetyczny \n"
                " [9] - pomiary automatyczne \n"
                " [0] - wyjscie \n";

        cin >> key;
        cout << endl;

        switch (key) {

            case 1:
                cout << "podaj nazwe pliku wraz z rozszerzeniem: ";
                cin >> name;
                graph = new Graph(name);
                break;

            case 2:
                cout << "wczytane dane: \n";
                graph->display();
                break;

            case 3:
                if (graph != nullptr) {

                    int choose;
                    bool loop = true;

                    while (loop) {

                        cout << " [1] - pokaz parametry\n"
                                " [2] - ustaw parametry\n"
                                " [3] - rozpocznij algorytm\n"
                                " [0] - wyjdz\n" << endl;
                        cin >> choose;

                        if (choose == 0) {

                            loop = false;
                        }

                        else if (choose == 1) {

                            cout << " czas [s]: " << timeGA << endl;
                            cout << " populacja: " << populationSize << endl;
                            cout << " typ mutacji: " << mutationType << endl;
                            cout << " P(mutacja): " << mutationProbability << endl;
                            cout << " typ krzyzowania: " << crossType << endl;
                            cout << " P(krzyzowanie): " << crossProbability << endl << endl;
                        }

                        else if (choose == 2) {

                            int x;
                            cout << "1) czas   2) populacja   3) typ mutacji   4) P(mutacja)   5) typ krzyzowania   6) P(krzyzowanie)  ";
                            cin >> x;

                            if (x == 1) {
                                cout << "czas [s]: "; cin >> timeGA;
                            }
                            else if (x == 2) {
                                cout << "populacja: "; cin >> populationSize;
                            }
                            else if (x == 3) {
                                cout << "typ mutacji: "; cin >> mutationType;
                            }
                            else if (x == 4) {
                                cout << "P(mutacja) <0,1>: "; cin >> mutationProbability;
                            }
                            else if (x == 5) {
                                cout << "typ krzyzowania: "; cin >> crossType;
                            }
                            else if (x == 6) {
                                cout << "P(krzyzowanie) <0,1>: "; cin >> crossProbability;
                            }

                            cout << endl;
                        }

                        else if (choose == 3) {

                            GeneticAlgorithm *ga = new GeneticAlgorithm();

                            vector<int> path(graph->getSize() + 1);
                            int cost;
                            double exeTime;

                            ga->settingsGeneticAlgorithm(timeGA, populationSize, mutationType, mutationProbability, crossType, crossProbability);
                            exeTime = ga->algorithmGeneticAlgorithm(graph->getMatrix(), path, cost);

                            cout << "\nCzas: " << exeTime << " s" << endl;
                            cout << "Koszt: " << cost << endl;
                            cout << "Sciezka: " ;
                            for (int i = 0; i < graph->getSize(); i++)
                                cout << path[i] << " -> ";
                            cout << path[graph->getSize()] << endl << endl;
                        }
                    }
                    break;
                }
                else {

                    cout << "macierz jest pusta \n";
                }
                break;

            case 9:

                system("PAUSE");
                Tests tests;
                tests.startAutomaticTests();
                break;

            case 0:
                run = false;
                break;

            default:
                break;

        }
    }
}