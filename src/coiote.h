#include "entrada.h"

struct Coiote
{
    int adaptabilidade;
    bool invalido;
    vector<vector<vector<double>>> solucao;
    int idade;
    Coiote() : adaptabilidade(0), invalido(true), solucao(vector<vector<vector<double>>>()), idade(0) {}
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
    vector<vector<Coiote *>> matilha;
    int numeroCoiotes;
    vector<int> demanda;
    vector<vector<int>> pesos;
    int capacidade;

    Coiote *novoCoiote(vector<Coiote *> &matilha);
    vector<vector<int>> getCaminhos(Coiote *coiote);
    void transformarEmSatisfazivel(Coiote *coiote);
    void gerarPopulacaoInicial(Entrada entrada);

public:
    pair<int, vector<vector<int>>> executarAlgoritmo();
    AlgoritmoCoiote(int r1, int r2, int numeroGeracoes, int numeroCoiotes, int numeroMatilhas, Entrada entrada);
};