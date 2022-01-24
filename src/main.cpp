#include <bits/stdc++.h>
#include "entrada.h"
#include "coiote.h"

using namespace std;

vector<vector<int>> readDistances(ifstream &inputFile, int nbClients)
{
    vector<vector<int>> distances(nbClients, vector<int>(nbClients));
    for (int i = 0; i < nbClients; i++)
    {
        for (int j = 0; j < i; j++)
        {
            inputFile >> distances[i][j];
            distances[j][i] = distances[i][j];
        }
    }
    return distances;
}

struct No
{
    int demanda;
    int coordX;
    int coordY;
    double anguloPolar;
    int id;
};

int positive_mod(int i)
{
    return (i % 65536 + 65536) % 65536;
}

Entrada lerArquivo(string pathToInstance)
{
    string content, content2, content3;
    vector<vector<int>> distances;
    ifstream inputFile(pathToInstance);
    int numeroClientes = -1;
    int numeroVeiculos = -1;
    int capacidade = -1;
    vector<No> nos;
    if (inputFile.is_open())
    {
        getline(inputFile, content);
        getline(inputFile, content);
        getline(inputFile, content);
        for (inputFile >> content; content != "NODE_COORD_SECTION"; inputFile >> content)
        {
            if (content == "DIMENSION")
                inputFile >> content2 >> numeroClientes;
            else if (content == "EDGE_WEIGHT_TYPE")
                inputFile >> content2 >> content3;
            else if (content == "EDGE_WEIGHT_FORMAT")
                inputFile >> content2 >> content3;
            else if (content == "DISPLAY_DATA_TYPE")
                inputFile >> content2 >> content3;
            else if (content == "CAPACITY")
                inputFile >> content2 >> capacidade;
            else if (content == "DISTANCE")
                inputFile >> content2 >> content3;
            else if (content == "EDGE_WEIGHT_SECTION")
                distances = readDistances(inputFile, numeroClientes);
            else if (content == "NODE_COORD_TYPE")
                inputFile >> content2 >> content3;
            else
                throw string("Entrada inesperada no arquivo: " + content);
        }
        numeroVeiculos = numeroClientes - 1;
        if (numeroClientes <= 0)
            throw string("Número de clientes indefinido");
        if (capacidade == 0)
            throw string("Capacidade de veículo indefinida");

        nos = vector<No>(numeroClientes);
        for (int i = 0; i < numeroClientes; i++)
        {
            inputFile >> nos[i].id >> nos[i].coordX >> nos[i].coordY;
            nos[i].id--;
            nos[i].anguloPolar = positive_mod(32768. * atan2(nos[i].coordY - nos[0].coordY, nos[i].coordX - nos[0].coordX) / (2 * acos(0.0)));
        }

        inputFile >> content;
        if (content != "DEMAND_SECTION")
            throw string("Entrada inesperada no arquivo: " + content);

        for (int i = 0; i < numeroClientes; i++)
        {
            inputFile >> content >> nos[i].demanda;
        }

        inputFile >> content >> content2 >> content3 >> content3;
        if (content != "DEPOT_SECTION")
            throw string("Entrada inesperada no arquivo: " + content);
        if (content2 != "1")
            throw string("Esperado depósito na posição 1 ao invés de " + content2);
        if (content3 != "EOF")
            throw string("Entrada inesperada no arquivo: " + content3);
    }
    else
        throw invalid_argument("Impossible to open instance file: " + pathToInstance);

    vector<vector<int>> timeCost(numeroClientes, vector<int>(numeroClientes));

    for (int i = 0; i < numeroClientes; i++)
    {
        for (int j = 0; j < numeroClientes; j++)
        {
            if (distances.empty())
            {
                double d = sqrt((nos[i].coordX - nos[j].coordX) * (nos[i].coordX - nos[j].coordX) + (nos[i].coordY - nos[j].coordY) * (nos[i].coordY - nos[j].coordY));
                d += 0.5;
                timeCost[i][j] = (int)d;
            }
            else
            {
                timeCost[i][j] = distances[i][j];
            }
        }
    }

    Entrada entrada;
    entrada.capacidade = capacidade;
    entrada.demanda.clear();
    for (int i = 0; i < numeroClientes; i++)
    {
        entrada.demanda.push_back(nos[i].demanda);
    }
    entrada.numeroClientes = numeroClientes;
    entrada.numeroVeiculos = numeroVeiculos;
    entrada.pesos = timeCost;
    return entrada;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    // ./prog <arquivo-entrada> <r1> <r2> <geracoes> <numero_coiotes> <numero_matilhas>
    Entrada entrada = lerArquivo(argv[1]);
    AlgoritmoCoiote coiote(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), entrada);
    pair<int, vector<vector<int>>> pares = coiote.executarAlgoritmo();
    cout << pares.first;
}