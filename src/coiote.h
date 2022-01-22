#include "entrada.h"

struct Coiote
{
    int adaptabilidade;
    vector<vector<vector<double>>> solucao;
    int idade;
};

class AlgoritmoCoiote
{
private:
    int r1;
    int r2;
    int numeroGeracoes;
    int numeroMatilhas;
    int numeroVeiculos;
    int numeroClientes;
    vector<vector<Coiote>> matilha;
    int numeroCoiotes;
    vector<int> demanda;
    vector<vector<int>> pesos;
    int capacidade;

    Coiote novoCoiote(vector<Coiote> &matilha);
    vector<vector<int>> getCaminhos(Coiote coiote);
    Coiote transformarEmSatisfazivel(Coiote coiote);
    void gerarPopulacaoInicial();

public:
    pair<int, vector<vector<int>>> AlgoritmoCoiote::executarAlgoritmo();
    AlgoritmoCoiote(int r1, int r2, int numeroGeracoes, int numeroCoiotes, int numeroMatilhas, Entrada entrada);
};