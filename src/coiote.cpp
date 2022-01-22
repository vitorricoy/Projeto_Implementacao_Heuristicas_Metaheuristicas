#include <bits/stdc++.h>
#include "coiote.h"
#include "savings.h"
#include "entrada.h"

using namespace std;

AlgoritmoCoiote::AlgoritmoCoiote(int r1, int r2, int numeroGeracoes, int numeroCoiotes, int numeroMatilhas, Entrada entrada)
{
    this->demanda = entrada.demanda;
    this->pesos = entrada.pesos;
    this->capacidade = entrada.capacidade;
    this->numeroVeiculos = entrada.numeroVeiculos;
    this->numeroClientes = entrada.numeroClientes;

    this->r1 = r1;
    this->r2 = r2;
    this->numeroGeracoes = numeroGeracoes;
    this->numeroCoiotes = numeroCoiotes;
    this->numeroMatilhas = numeroMatilhas;

    this->gerarPopulacaoInicial(entrada);
}

void AlgoritmoCoiote::transformarEmSatisfazivel(Coiote *coiote)
{
    // Tem como agora com mais veiculos
    // Para cada veículo percorre o caminho do caminhão. Se exceder o limite volta pro deposito.
    // Tem que criar a aresta pro deposito e remover qualquer outra > 1.
    // No fim se algum vértice não foi visitado, cria uma nova rota com os vertices não visitados usando a heuristica construtiva
    coiote->adaptabilidade = -1;
    set<int> visitados;
    vector<int> veiculosDisponiveis;
    int totalCost = 0;
    for (int veiculo = 0; veiculo < this->numeroVeiculos; veiculo++)
    {
        int noAtual = 0;
        int demandaAtendida = 0;
        int nosVisitados = 0;
        while (true)
        {
            vector<int> possiveisCaminhos;
            double valorMaiorCaminho = 0;
            int caminhoEscolhido = -1;
            visitados.insert(noAtual);
            for (int cliente = 0; cliente < this->numeroClientes; cliente++)
            {
                if (coiote->solucao[veiculo][noAtual][cliente] >= 1)
                {
                    if (valorMaiorCaminho < coiote->solucao[veiculo][noAtual][cliente] && (cliente == 0 || visitados.find(cliente) == visitados.end()))
                    {
                        valorMaiorCaminho = coiote->solucao[veiculo][noAtual][cliente];
                        caminhoEscolhido = cliente;
                    }
                    possiveisCaminhos.push_back(cliente);
                }
            }
            if (possiveisCaminhos.size() > 1)
            {
                for (int i = 0; i < (int)possiveisCaminhos.size(); i++)
                {
                    if (possiveisCaminhos[i] != caminhoEscolhido)
                    {
                        coiote->solucao[veiculo][noAtual][possiveisCaminhos[i]] = 0.99999;
                    }
                }
            }
            if (caminhoEscolhido == 0)
            {
                totalCost += this->pesos[noAtual][0];
                break;
            }
            if (caminhoEscolhido == -1)
            {
                coiote->solucao[veiculo][noAtual][0] = 1;
                totalCost += this->pesos[noAtual][0];
                break;
            }
            demandaAtendida += this->demanda[caminhoEscolhido];
            if (demandaAtendida > this->capacidade)
            {
                coiote->solucao[veiculo][noAtual][caminhoEscolhido] = 0.99999;
                coiote->solucao[veiculo][noAtual][0] = 1;
                totalCost += this->pesos[noAtual][0];
                break;
            }
            totalCost += this->pesos[noAtual][caminhoEscolhido];
            noAtual = caminhoEscolhido;
            nosVisitados++;
        }
        if (nosVisitados == 0)
        {
            veiculosDisponiveis.push_back(veiculo);
        }
    }
    if ((int)visitados.size() != numeroClientes)
    {
        vector<int> naoVisitados;
        naoVisitados.push_back(0);
        for (int i = 0; i < numeroClientes; i++)
        {
            if (visitados.find(i) == visitados.end())
            {
                naoVisitados.push_back(i);
            }
        }
        vector<int> novaDemanda(naoVisitados.size());
        vector<vector<int>> novaDistancia(naoVisitados.size(), vector<int>(naoVisitados.size()));
        for (int i = 0; i < (int)naoVisitados.size(); i++)
        {
            novaDemanda[i] = this->demanda[naoVisitados[i]];
            for (int j = 0; j < (int)naoVisitados.size(); j++)
            {
                novaDistancia[i][j] = this->pesos[naoVisitados[i]][naoVisitados[j]];
            }
        }
        Entrada entradaFake;
        entradaFake.capacidade = this->capacidade;
        entradaFake.demanda = novaDemanda;
        entradaFake.numeroClientes = naoVisitados.size();
        entradaFake.numeroVeiculos = naoVisitados.size() - 1;
        entradaFake.pesos = novaDistancia;
        SavingsAlgorithm savings(entradaFake);
        vector<pair<int, vector<int>>> resposta = savings.buildSolution();
        for (pair<int, vector<int>> p : resposta)
        {
            if (p.first != 0)
            {
                if (veiculosDisponiveis.empty())
                {
                    coiote->invalido = true;
                    return;
                }
                int proximoVeiculo = veiculosDisponiveis.back();
                vector<vector<double>> solution(this->numeroClientes, vector<double>(this->numeroClientes, 0));
                if (p.first == 0)
                {
                    continue;
                }
                int previousPoint = 0;
                for (int point : p.second)
                {
                    solution[naoVisitados[previousPoint]][naoVisitados[point]] = 1;
                    previousPoint = point;
                    totalCost += novaDistancia[previousPoint][point];
                }
                solution[naoVisitados[previousPoint]][0] = 1;
                totalCost += novaDistancia[previousPoint][0];
                coiote->solucao[proximoVeiculo] = solution;
            }
        }
    }
    coiote->adaptabilidade = totalCost;
    coiote->invalido = false;
}

void AlgoritmoCoiote::gerarPopulacaoInicial(Entrada entrada)
{
    this->matilha = vector<vector<Coiote *>>(this->numeroMatilhas, vector<Coiote *>(this->numeroCoiotes));

    for (int i = 0; i < this->numeroMatilhas; i++)
    {
        for (int j = 0; j < this->numeroCoiotes; j++)
        {
            SavingsAlgorithm savings(entrada);
            pair<int, vector<vector<vector<double>>>> solucao = savings.converterParaSolucao(savings.buildSolution());
            this->matilha[i][j] = new Coiote();
            this->matilha[i][j]->adaptabilidade = solucao.first;
            this->matilha[i][j]->idade = 0;
            this->matilha[i][j]->invalido = false;
            this->matilha[i][j]->solucao = solucao.second;
        }
    }
}

vector<vector<int>> AlgoritmoCoiote::getCaminhos(Coiote *coiote)
{
    set<int> visitados;
    vector<vector<int>> caminhos(this->numeroVeiculos);
    for (int veiculo = 0; veiculo < this->numeroVeiculos; veiculo++)
    {
        int noAtual = 0;
        while (true)
        {
            caminhos[veiculo].push_back(noAtual);
            vector<int> possiveisCaminhos;
            double valorMaiorCaminho = 0;
            int caminhoEscolhido = 1;
            visitados.insert(noAtual);
            for (int cliente = 0; cliente < this->numeroClientes; cliente++)
            {
                if (coiote->solucao[veiculo][noAtual][cliente] >= 1)
                {
                    if (valorMaiorCaminho < coiote->solucao[veiculo][noAtual][cliente])
                    {
                        valorMaiorCaminho = coiote->solucao[veiculo][noAtual][cliente];
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

Coiote *AlgoritmoCoiote::novoCoiote(vector<Coiote *> &matilha)
{
    Coiote *novo = new Coiote();
    novo->idade = 0;
    novo->solucao = vector<vector<vector<double>>>(this->numeroVeiculos, vector<vector<double>>(this->numeroClientes, vector<double>(this->numeroClientes)));
    int pai = rand() % matilha.size();
    int mae = rand() % matilha.size();
    int j1 = rand() % (this->numeroVeiculos * this->numeroClientes * this->numeroClientes);
    int j2 = rand() % (this->numeroVeiculos * this->numeroClientes * this->numeroClientes);
    while (j1 == j2)
    {
        j2 = rand() % (this->numeroVeiculos * this->numeroClientes * this->numeroClientes);
    }
    double ps = 1.0 / (double)(this->numeroVeiculos * this->numeroClientes * this->numeroClientes);
    double pa = (1.0 - ps) / 2.0;
    for (int veiculo = 0; veiculo < this->numeroVeiculos; veiculo++)
    {
        for (int cliente1 = 0; cliente1 < this->numeroClientes; cliente1++)
        {
            for (int cliente2 = 0; cliente2 < this->numeroClientes; cliente2++)
            {
                double aleatorio = ((double)rand() / (RAND_MAX));
                if (veiculo * (this->numeroClientes * this->numeroClientes) + cliente1 * this->numeroClientes + cliente2 == j1 || aleatorio < ps)
                {
                    //pai
                    novo->solucao[veiculo][cliente1][cliente2] = matilha[pai]->solucao[veiculo][cliente1][cliente2];
                }
                else
                {
                    if (veiculo * (this->numeroClientes * this->numeroClientes) + cliente1 * this->numeroClientes + cliente2 == j2 || aleatorio < ps + pa)
                    {
                        //mae
                        novo->solucao[veiculo][cliente1][cliente2] = matilha[mae]->solucao[veiculo][cliente1][cliente2];
                    }
                    else
                    {
                        novo->solucao[veiculo][cliente1][cliente2] = ((double)rand() / (RAND_MAX));
                    }
                }
            }
        }
    }
    transformarEmSatisfazivel(novo);
    return novo;
}

pair<int, vector<vector<int>>> AlgoritmoCoiote::executarAlgoritmo()
{
    for (int geracao = 0; geracao < this->numeroGeracoes; geracao++)
    {
        cout << "Rodando geração " << geracao << endl;
        for (int idtMatilha = 0; idtMatilha < this->numeroMatilhas; idtMatilha++)
        {
            int maiorAdaptabilidade = 0;
            int menorAdaptabilidade = INT32_MAX;
            int idadeMaisVelho = -1;
            int posMaisVelho;
            Coiote *alfa;
            int pos = 0;
            for (Coiote *coiote : this->matilha[idtMatilha])
            {
                if (coiote->adaptabilidade > maiorAdaptabilidade)
                {
                    maiorAdaptabilidade = coiote->adaptabilidade;
                }

                if (coiote->adaptabilidade < menorAdaptabilidade)
                {
                    menorAdaptabilidade = coiote->adaptabilidade;
                    alfa = coiote;
                }

                if (idadeMaisVelho < coiote->idade && coiote->invalido)
                {
                    posMaisVelho = pos;
                    // Elimina antes os insatisfaziveis
                    idadeMaisVelho = (coiote->invalido ? INT_MAX : coiote->idade);
                }
                pos += 1;
            }

            // Calcula tendencia da matilha
            vector<vector<vector<double>>> tendenciaSocial(numeroVeiculos, vector<vector<double>>(numeroClientes, vector<double>(numeroClientes)));
            for (int veiculo = 0; veiculo < this->numeroVeiculos; veiculo++)
            {
                for (int cliente1 = 0; cliente1 < this->numeroClientes; cliente1++)
                {
                    for (int cliente2 = 0; cliente2 < this->numeroClientes; cliente2++)
                    {
                        int somaAdaptabilidade = 0;
                        double somaValores = 0;
                        for (Coiote *coiote : matilha[idtMatilha])
                        {
                            somaValores += (maiorAdaptabilidade - (double)coiote->adaptabilidade) * coiote->solucao[veiculo][cliente1][cliente2];
                            somaAdaptabilidade += (maiorAdaptabilidade - (double)coiote->adaptabilidade);
                        }
                        tendenciaSocial[veiculo][cliente1][cliente2] = somaValores / somaAdaptabilidade;
                    }
                }
            }

            for (Coiote *coiote : matilha[idtMatilha])
            {
                Coiote *novoCoiote = new Coiote();
                novoCoiote->adaptabilidade = coiote->adaptabilidade;
                novoCoiote->idade = coiote->idade;
                novoCoiote->solucao = coiote->solucao;
                novoCoiote->invalido = coiote->invalido;

                // Atualiza condição social
                for (int veiculo = 0; veiculo < this->numeroVeiculos; veiculo++)
                {
                    for (int cliente1 = 0; cliente1 < this->numeroClientes; cliente1++)
                    {
                        for (int cliente2 = 0; cliente2 < this->numeroClientes; cliente2++)
                        {
                            double c1 = alfa->solucao[veiculo][cliente1][cliente2] - coiote->solucao[veiculo][cliente1][cliente2];
                            double c2 = tendenciaSocial[veiculo][cliente1][cliente2] - coiote->solucao[veiculo][cliente1][cliente2];
                            novoCoiote->solucao[veiculo][cliente1][cliente2] += this->r1 * c1 + this->r2 * c2;
                        }
                    }
                }

                // Avalia a nova condição social do coiote
                transformarEmSatisfazivel(novoCoiote);
                // Realiza a adaptação do coiote
                if (novoCoiote->adaptabilidade < coiote->adaptabilidade)
                {
                    Coiote *temp = coiote;
                    coiote = novoCoiote;
                    free(temp);
                }
                else
                {
                    free(novoCoiote);
                }
            }
            // Processa nascimentos e mortes
            Coiote *temp = matilha[idtMatilha][posMaisVelho];
            matilha[idtMatilha][posMaisVelho] = novoCoiote(matilha[idtMatilha]);
            free(temp);
        }
        // Processa a transição de coiotes
        for (int idtMatilha = 0; idtMatilha < this->numeroMatilhas; idtMatilha++)
        {
            double aleatorio = ((double)rand() / (RAND_MAX));
            double tamanhoMatilha = this->matilha[idtMatilha].size();
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
        for (int idtMatilha = 0; idtMatilha < this->numeroMatilhas; idtMatilha++)
        {
            for (Coiote *coiote : this->matilha[idtMatilha])
            {
                coiote->idade++;
            }
        }
    }
    int menorAdaptabilidade = INT32_MAX;
    Coiote *maisAdaptado;
    for (int idtMatilha = 0; idtMatilha < this->numeroMatilhas; idtMatilha++)
    {
        for (Coiote *coiote : this->matilha[idtMatilha])
        {
            if (coiote->adaptabilidade < menorAdaptabilidade)
            {
                menorAdaptabilidade = coiote->adaptabilidade;
                maisAdaptado = coiote;
            }
        }
    }
    vector<vector<int>> caminhosMelhor = getCaminhos(maisAdaptado);
    for (int idtMatilha = 0; idtMatilha < this->numeroMatilhas; idtMatilha++)
    {
        for (Coiote *coiote : this->matilha[idtMatilha])
        {
            free(coiote);
        }
    }
    return {menorAdaptabilidade, caminhosMelhor};
}