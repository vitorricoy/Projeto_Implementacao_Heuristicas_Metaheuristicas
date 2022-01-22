#include <bits/stdc++.h>
#include "savings.h"
#include "entrada.h"

using namespace std;

#define DEPOT 0

SavingsAlgorithm::SavingsAlgorithm(Entrada entrada)
{
    this->demand = entrada.demanda;
    this->distanceMatrix = entrada.pesos;
    this->routes = vector<pair<int, vector<int>>>(entrada.numeroVeiculos);
    this->numeroVeiculos = entrada.numeroVeiculos;
    this->numeroClientes = entrada.numeroClientes;
    this->capacidade = entrada.capacidade;
    this->point_route.clear();
    this->savingsList.clear();
    this->weight.clear();
    this->assigned.clear();
}

class RandomPickVector
{
public:
    int n;
    vector<int> v;
    RandomPickVector(vector<int> w)
    {
        n = 0;
        for (int i = 0; i < (int)w.size(); i++)
        {
            n += w[i];
            v.push_back(n);
        }
    }

    void setWeight(vector<int> w)
    {
        n = 0;
        v.clear();
        for (int i = 0; i < (int)w.size(); i++)
        {
            n += w[i];
            v.push_back(n);
        }
    }

    int pickIndex()
    {
        return upper_bound(v.begin(), v.end(), rand() % v.back()) - v.begin();
    }
};

int SavingsAlgorithm::distance(int i, int j)
{ // consulta na matriz de distância a distância entre dois pontos i e j
    return distanceMatrix[i][j];
}

void SavingsAlgorithm::savings(int lin, int col)
{
    int saving;
    int minSaving = 0;
    for (int i = 1; i < lin; i++)
    {
        for (int j = i+1; j < col; j++)
        {
            saving = distance(DEPOT, i) + distance(DEPOT, j) - distance(i, j);
            minSaving = min(saving, minSaving);
            weight.push_back(saving);
            savingsList.push_back(make_pair(saving, make_pair(i, j)));
        }
    }

    for (int &p : weight)
    {
        p -= minSaving - 1;
    }
}

bool SavingsAlgorithm::isAssigned(int point)
{
    return !(assigned.find(point) == assigned.end());
}

int SavingsAlgorithm::notInternal(int point)
{ // retorna 1 se primeira posição, retorna 2 se final, retorna 0 se no meio
    vector<int> rota = routes[point_route[point]].second;

    if (rota.front() == point)
    {
        return 1;
    }

    if (rota.back() == point)
    {
        return 2;
    }

    return 0;
}

void SavingsAlgorithm::insertRoute(int index, int point, int pos)
{
    if (pos == 1)
    {
        routes[index].second.insert(routes[index].second.begin(), point);
    }
    else
    {
        routes[index].second.push_back(point);
    }
    routes[index].first += demand[point];
}

void SavingsAlgorithm::merge(int index_i, int pos_i, int index_j, int pos_j)
{

    if (pos_i == 1 && pos_j == 2)
    {
        //Coloca a rota contendo i no final da rota contendo j
        routes[index_j].second.insert(routes[index_j].second.end(), routes[index_i].second.begin(), routes[index_i].second.end());

        for (int i : routes[index_i].second)
        {
            point_route[i] = index_j;
        }
        routes[index_i].first = 0;
        routes[index_i].second.clear();
    }

    if (pos_i == 2 && pos_j == 1)
    {
        //Coloca a rota contendo j no final da rota contendo i
        routes[index_i].second.insert(routes[index_i].second.end(), routes[index_j].second.begin(), routes[index_j].second.end());

        for (int j : routes[index_j].second)
        {
            point_route[j] = index_i;
        }
        routes[index_j].first = 0;
        routes[index_j].second.clear();
    }

    if (pos_i == 1 && pos_j == 1)
    {
        //Coloca a rota contendo j no final da inversão da rota contendo i
        // [i,1] [j,2] => [1,i] + [j,2]
        reverse(routes[index_i].second.begin(), routes[index_i].second.end());

        routes[index_i].second.insert(routes[index_i].second.end(), routes[index_j].second.begin(), routes[index_j].second.end());

        for (int j : routes[index_j].second)
        {
            point_route[j] = index_i;
        }
        routes[index_j].first = 0;
        routes[index_j].second.clear();
    }

    if (pos_i == 2 && pos_j == 2)
    {
        //Coloca a inversão da rota contendo j no final da rota contendo i
        // [1,i] [2,j] => [1,i] + [j,2]
        routes[index_i].second.insert(routes[index_i].second.end(), routes[index_j].second.rbegin(), routes[index_j].second.rend());

        for (int j : routes[index_j].second)
        {
            point_route[j] = index_i;
        }
        routes[index_j].first = 0;
        routes[index_j].second.clear();
    }
}

vector<pair<int, vector<int>>> SavingsAlgorithm::buildSolution()
{
    this->savings(this->numeroClientes, this->numeroClientes);

    RandomPickVector weightedSavings(weight);
    while (weight.size() > 0)
    {
        weightedSavings.setWeight(weight);
        // escolhe um elemento aleatório da lista
        int index = weightedSavings.pickIndex();

        pair<int, int> route; // rota entre dois pontos
        route = savingsList[index].second;

        if (isAssigned(route.first))
        {
            if (!isAssigned(route.second))
            {
                int pos = notInternal(route.first);
                if (pos != 0 && routes[point_route[route.first]].first + demand[route.second] <= this->capacidade)
                {
                    insertRoute(point_route[route.first], route.second, pos);
                    assigned.insert(route.second);
                } // adiciona j a rota que contem i
            }
        }

        if (isAssigned(route.second))
        {
            if (!isAssigned(route.first))
            {
                int pos = notInternal(route.first);
                if (pos != 0 && routes[point_route[route.second]].first + demand[route.first] <= this->capacidade)
                {
                    insertRoute(point_route[route.second], route.first, pos);
                    assigned.insert(route.first);
                } // adiciona i a rota que contem j
            }
        }

        if (!isAssigned(route.first) && !isAssigned(route.second))
        {
            for (int i = 0; i < this->numeroVeiculos; i++)
            {
                if (routes[i].first == 0 &&  demand[route.first] + demand[route.second] <= this->capacidade)
                {
                    routes[i].first += demand[route.first] + demand[route.second];
                    routes[i].second.push_back(route.first);
                    routes[i].second.push_back(route.second);
                    point_route[route.first] = i;
                    point_route[route.second] = i;
                    assigned.insert(route.first);
                    assigned.insert(route.second);
                    break;
                }
            }
        }

        if (isAssigned(route.first) && isAssigned(route.second))
        {
            int rota1 = point_route[route.first];
            int rota2 = point_route[route.second];

            if (rota1 != rota2)
            {
                int pos_i = notInternal(route.first);
                int pos_j = notInternal(route.second);
                if (!pos_i && !pos_j && routes[rota1].first + routes[rota2].first <= this->capacidade)
                {
                    merge(point_route[route.first], pos_i, point_route[route.second], pos_j);
                }
            }
        }
        // remove os elementos escolhidos das listas
        savingsList.erase(savingsList.begin() + index);
        weight.erase(weight.begin() + index);
    }

    for (int i = 1; i < this->numeroClientes; i++) {
        if (assigned.find(i) == assigned.end()) {
            for (int j = 0; j < this->numeroVeiculos; j++)
            {
                if (routes[j].first == 0)
                {
                    routes[j].first += demand[i];
                    routes[j].second.push_back(i);
                    break;
                }
            }
        }
    }
    // routes:  vetor contendo rotas finais e custo de cada rota
    return this->routes;
}

pair<int, vector<vector<vector<double>>>> SavingsAlgorithm::converterParaSolucao(vector<pair<int, vector<int>>> rotas)
{
    vector<vector<vector<double>>> solution(this->numeroVeiculos, vector<vector<double>>(this->numeroClientes, vector<double>(this->numeroClientes, 0)));
    int previousPoint;
    int totalCost = 0;
    for (int truck = 0; truck < this->numeroVeiculos; truck++)
    {
        pair<int, vector<int>> route = rotas[truck];
        if (route.first == 0)
        {
            continue;
        }
        previousPoint = 0;
        for (int point : route.second)
        {
            solution[truck][previousPoint][point] = 1;
            totalCost += distance(previousPoint, point);
            previousPoint = point;
        }
        solution[truck][previousPoint][0] = 1;
        totalCost += distance(previousPoint, 0);
    }
    return make_pair(totalCost, solution);
}