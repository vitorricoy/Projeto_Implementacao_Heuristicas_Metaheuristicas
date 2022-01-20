#include <bits/stdc++.h>

using namespace std;

struct Coiote
{
    int id;
    int adaptabilidade;
    vector<vector<vector<double>>> solucao;
    int idade;
};

vector<vector<Coiote>> matilha;

vector<int> demanda;
vector<vector<int>> pesos;
int capacidade;

Coiote transformarEmSatisfazivel(Coiote coiote, int numeroClientes, int numeroVeiculos)
{
    // Tem como agora com mais veiculos
    // Para cada veículo percorre o caminho do caminhão. Se exceder o limite volta pro deposito.
    // Tem que criar a aresta pro deposito e remover qualquer outra > 1.
    // No fim se algum vértice não foi visitado, cria uma nova rota com os vertices não visitados usando a heuristica construtiva
}

vector<vector<int>> getCaminhos(Coiote coiote, int numeroClientes, int numeroVeiculos)
{
    set<int> visitados;
    vector<vector<int>> caminhos(numeroVeiculos);
    for (int veiculo = 0; veiculo < numeroVeiculos; veiculo++)
    {
        int noAtual = 0;
        while (true)
        {
            caminhos[veiculo].push_back(noAtual);
            vector<int> possiveisCaminhos;
            double valorMaiorCaminho = 0;
            int caminhoEscolhido = 1;
            visitados.insert(noAtual);
            for (int cliente = 0; cliente < numeroClientes; cliente++)
            {
                if (coiote.solucao[veiculo][noAtual][cliente] >= 1)
                {
                    if (valorMaiorCaminho < coiote.solucao[veiculo][noAtual][cliente])
                    {
                        valorMaiorCaminho = coiote.solucao[veiculo][noAtual][cliente];
                        caminhoEscolhido = cliente;
                    }
                    possiveisCaminhos.push_back(cliente);
                }
            }
            if (caminhoEscolhido == 0)
            {
                caminhos[veiculo].push_back(0);
                break;
            }
        }
    }
    return caminhos;
}

Coiote novoCoiote(vector<Coiote> &matilha, int numeroClientes, int numeroVeiculos)
{
    Coiote novo;
    novo.idade = 0;
    novo.solucao = vector<vector<vector<double>>>(numeroVeiculos, vector<vector<double>>(numeroClientes, vector<double>(numeroClientes)));
    int pai = rand() % matilha.size();
    int mae = rand() % matilha.size();
    int j1 = rand() % (numeroVeiculos * numeroClientes * numeroClientes);
    int j2 = rand() % (numeroVeiculos * numeroClientes * numeroClientes);
    while (j1 == j2)
    {
        j2 = rand() % (numeroVeiculos * numeroClientes * numeroClientes);
    }
    double ps = 1.0 / (double)(numeroVeiculos * numeroClientes * numeroClientes);
    double pa = (1.0 - ps) / 2.0;
    for (int veiculo = 0; veiculo < numeroVeiculos; veiculo++)
    {
        for (int cliente1 = 0; cliente1 < numeroClientes; cliente1++)
        {
            for (int cliente2 = 0; cliente2 < numeroClientes; cliente2++)
            {
                double aleatorio = ((double)rand() / (RAND_MAX));
                if (veiculo * (numeroClientes * numeroClientes) + cliente1 * numeroClientes + cliente2 == j1 || aleatorio < ps)
                {
                    //pai
                    novo.solucao[veiculo][cliente1][cliente2] = matilha[pai].solucao[veiculo][cliente1][cliente2];
                }
                else
                {
                    if (veiculo * (numeroClientes * numeroClientes) + cliente1 * numeroClientes + cliente2 == j2 || aleatorio < ps + pa)
                    {
                        //mae
                        novo.solucao[veiculo][cliente1][cliente2] = matilha[mae].solucao[veiculo][cliente1][cliente2];
                    }
                    else
                    {
                        novo.solucao[veiculo][cliente1][cliente2] = ((double)rand() / (RAND_MAX));
                    }
                }
            }
        }
    }
    return transformarEmSatisfazivel(novo, numeroClientes, numeroVeiculos);
}

int main(int argc, char **argv)
{
    int numeroGeracoes = atoi(argv[1]);
    int numeroMatilhas = atoi(argv[2]);
    int numeroCoiotes = atoi(argv[3]);
    int r1 = atoi(argv[4]);
    int r2 = atoi(argv[5]);
    int numeroVeiculos = -1; // Le do arquivo
    int numeroClientes = -1; // Le do arquivo

    for (int geracao = 0; geracao < numeroGeracoes; geracao++)
    {
        for (int idtMatilha = 0; idtMatilha < numeroMatilhas; idtMatilha++)
        {
            int maiorAdaptabilidade = 0;
            int menorAdaptabilidade = INT32_MAX;
            int idadeMaisVelho = 0;
            int posMaisVelho;
            Coiote alfa;
            int pos = 0;
            for (Coiote coiote : matilha[idtMatilha])
            {
                if (coiote.adaptabilidade > maiorAdaptabilidade)
                {
                    maiorAdaptabilidade = coiote.adaptabilidade;
                }

                if (coiote.adaptabilidade < menorAdaptabilidade)
                {
                    menorAdaptabilidade = coiote.adaptabilidade;
                    alfa = coiote;
                }

                if (idadeMaisVelho < coiote.idade && coiote.adaptabilidade == INT_MAX)
                {
                    posMaisVelho = pos;
                    // Elimina antes os insatisfaziveis
                    idadeMaisVelho = (coiote.adaptabilidade == INT_MAX ? INT_MAX : coiote.idade);
                }
                pos += 1;
            }
            // Calcula o coiote alfa
            Coiote alfa = *matilha[idtMatilha].begin();

            int maiorAdaptabilidade = matilha[idtMatilha].rbegin()->adaptabilidade;

            // Calcula tendencia da matilha
            vector<vector<vector<double>>> tendenciaSocial;
            for (int veiculo = 0; veiculo < numeroVeiculos; veiculo++)
            {
                for (int cliente1 = 0; cliente1 < numeroClientes; cliente1++)
                {
                    for (int cliente2 = 0; cliente2 < numeroClientes; cliente2++)
                    {
                        int somaAdaptabilidade = 0;
                        double somaValores = 0;
                        for (Coiote coiote : matilha[idtMatilha])
                        {
                            somaValores += (maiorAdaptabilidade - coiote.adaptabilidade) * coiote.solucao[veiculo][cliente1][cliente2];
                            somaAdaptabilidade += (maiorAdaptabilidade - coiote.adaptabilidade);
                        }
                        tendenciaSocial[veiculo][cliente1][cliente2] = somaValores / somaAdaptabilidade;
                    }
                }
            }

            for (Coiote &coiote : matilha[idtMatilha])
            {
                Coiote novoCoiote = coiote;
                // Atualiza condição social
                for (int veiculo = 0; veiculo < numeroVeiculos; veiculo++)
                {
                    for (int cliente1 = 0; cliente1 < numeroClientes; cliente1++)
                    {
                        for (int cliente2 = 0; cliente2 < numeroClientes; cliente2++)
                        {
                            double c1 = alfa.solucao[veiculo][cliente1][cliente2] - coiote.solucao[veiculo][cliente1][cliente2];
                            double c2 = tendenciaSocial[veiculo][cliente1][cliente2] - coiote.solucao[veiculo][cliente1][cliente2];
                            novoCoiote.solucao[veiculo][cliente1][cliente2] += c1 + c2;
                        }
                    }
                }

                // Avalia a nova condição social do coiote
                novoCoiote = transformarEmSatisfazivel(novoCoiote, numeroClientes, numeroVeiculos);
                // Realiza a adaptação do coiote
                if (novoCoiote.adaptabilidade < coiote.adaptabilidade)
                {
                    coiote = novoCoiote;
                }
            }
            // Processa nascimentos e mortes
            matilha[idtMatilha][posMaisVelho] = novoCoiote(matilha[idtMatilha], numeroClientes, numeroVeiculos);
        }
        // Processa a transição de coiotes
        for (int idtMatilha = 0; idtMatilha < numeroMatilhas; idtMatilha++)
        {
            double aleatorio = ((double)rand() / (RAND_MAX));
            double tamanhoMatilha = matilha[idtMatilha].size();
            if (aleatorio > 0.005 * (tamanhoMatilha * tamanhoMatilha))
            {
                int vaiSair = rand() % matilha[idtMatilha].size();
                int vaiPara = rand() % matilha.size();
                while (vaiPara == idtMatilha)
                {
                    vaiPara = rand() % matilha.size();
                }
                matilha[vaiPara].push_back(matilha[idtMatilha][vaiSair]);
                matilha[idtMatilha].erase(matilha[idtMatilha].begin() + vaiSair);
            }
        }
        // Atualiza as idades dos coiotes
        for (int idtMatilha = 0; idtMatilha < numeroMatilhas; idtMatilha++)
        {
            for (Coiote &coiote : matilha[idtMatilha])
            {
                coiote.idade++;
            }
        }
    }
    int menorAdaptabilidade = INT32_MAX;
    Coiote maisAdaptado;
    for (int idtMatilha = 0; idtMatilha < numeroMatilhas; idtMatilha++)
    {
        for (Coiote coiote : matilha[idtMatilha])
        {
            if (coiote.adaptabilidade < menorAdaptabilidade)
            {
                menorAdaptabilidade = coiote.adaptabilidade;
                maisAdaptado = coiote;
            }
        }
    }
    vector<vector<int>> caminhosMelhor = getCaminhos(maisAdaptado, numeroClientes, numeroVeiculos);
}