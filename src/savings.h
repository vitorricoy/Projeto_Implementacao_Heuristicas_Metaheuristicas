#include <bits/stdc++.h>
#include "entrada.h"

using namespace std;

class SavingsAlgorithm
{
private:
    vector<int> demand;
    vector<vector<int>> distanceMatrix;
    map<int, int> point_route;
    vector<pair<int, pair<int, int>>> savingsList;
    vector<pair<int, vector<int>>> routes;
    vector<int> weight;
    set<int> assigned;

    int numeroVeiculos;
    int numeroClientes;
    int capacidade;

    int distance(int i, int j);
    void savings(int lin, int col);
    bool isAssigned(int point);
    int notInternal(int point);
    void insertRoute(int index, int point, int pos);
    void merge(int index_i, int pos_i, int index_j, int pos_j);

public:
    vector<pair<int, vector<int>>> buildSolution();
    pair<int, vector<vector<vector<double>>>> converterParaSolucao(vector<pair<int, vector<int>>> rotas);
    SavingsAlgorithm(Entrada entrada);
};