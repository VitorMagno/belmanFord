#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<queue>
#include<vector>
#include<climits>

using namespace std;

void belmanFord(int** grafo, int vertInicial, int vertFinal, int tamanhoGrafo, int* dist, int *pais){
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    dist[vertInicial] = 0;
    pq.push({dist[vertInicial],vertInicial});
    while(!pq.empty()){
        pair<int,int> vprioridade = pq.top(); //peso = .first, vertice = .second
        pq.pop();
        if (vprioridade.second == vertFinal){
            break;
        }
        for(int i  = 1; i < tamanhoGrafo; i++){
            if(grafo[vprioridade.second][i] != INT_MAX){
                if(dist[i]> vprioridade.first + grafo[vprioridade.second][i]){ // distancia do vizinho do vertice atual > dist ate o atual + dist pro vizinho no grafo
                    dist[i] = vprioridade.first + grafo[vprioridade.second][i];
                    pais[i] = i;
                    pq.push({dist[i],i});
                }
            }
        }
    }
    return;
}

void solutions (int** grafo, bool write, string outputFile, bool orderSolution, int vertInicial, int vertFinal, int tamanhoGrafo){
    int dist[tamanhoGrafo];
    int pais[tamanhoGrafo];

    for (int i = 0; i < tamanhoGrafo; i++){
        dist[i] = 100000;
        pais[i] = -1;
    }

    belmanFord(grafo, vertInicial, vertFinal, tamanhoGrafo, &dist[0], &pais[0]);
    
    if(write && orderSolution){
        ofstream writeFile;
        writeFile.open(outputFile, ofstream::out);
        writeFile.close();
        return;
    }
    if(write){
        ofstream writeFile;
        writeFile.open(outputFile, ofstream::out);
        int sumCaminho=0;

        for(int i = vertInicial; i < vertFinal; i++){
            if(dist[i] == 100000){
                continue;
            }else{
                sumCaminho += dist[i];
            }
        }

        writeFile << "peso do menor caminho: "<< sumCaminho << endl;
        writeFile.close();
        return;
    }
    if(orderSolution){
        //desenvolver
        return;
    }

    int sumCaminho=0;
    for(int i = vertInicial; i < vertFinal; i++){
        if(dist[i] == 100000){
            continue;
        }else{
            sumCaminho += dist[i];
        }
    }

    cout << "peso do menor caminho: "<< sumCaminho << endl;
    return;
}

int main(int argc, char const *argv[]){
    int indiceInicial = 1;
    int indiceFinal = 999;
    int tamanhoGrafo = 0;
    int ordemGrafo = 0;
    int** grafo;
    const int BUFFER_SIZE = 8;
    char buffer[BUFFER_SIZE];
    
    string opReadFile = "-f";
    string opWriteFile = "-o";
    string opOrderSolution = "-s";
    string opStart = "-i";
    string opEnd = "-l";
    string outputFile;
    
    bool read = false;
    bool write = false;
    bool orderSolution = false;
    bool start = false;
    bool end = false;
    
    for(int i = 0; i < argc; i++){
        if(argv[i] == opReadFile){
            read = true;
            
            ifstream fileInput;
            fileInput = ifstream(argv[i+1], ifstream::binary);

            if(!fileInput.is_open()){
                cout << "Erro ao abrir o arquivo de entrada" << endl;
                return 1;
            }
            
            fileInput.getline(buffer, BUFFER_SIZE);
            ordemGrafo = atoi(&buffer[0]); // qtd vertices
            tamanhoGrafo = atoi(&buffer[2]); // qtd arestas
            
            //int grafo[tamanhoGrafo + 1][tamanhoGrafo+1]; 
            //ptrgrafo = &grafo[0][0];
            grafo = new int* [tamanhoGrafo+1];
            for(int i = 0; i < tamanhoGrafo+1; i++){
                grafo[i] = new int [tamanhoGrafo+1];
            };

            indiceFinal = ordemGrafo+1;
            
            for(int i = 0; i < tamanhoGrafo+1; i++){
                for(int j = 0; j < tamanhoGrafo+1; j++){
                    grafo[i][j] = INT_MAX;
                }
            }
            while (!fileInput.fail()){
                fileInput.getline(buffer, BUFFER_SIZE);
                
                if(fileInput.fail()){
                    break;
                }
                
                int vertOrigem = atoi(&buffer[0]);
                int vertDestino = atoi(&buffer[2]);
                int peso = atoi(&buffer[4]);
                grafo[vertOrigem][vertDestino] = peso;
            }
        }
        if(argv[i] == opStart){
            start = true;
            indiceInicial = atoi(argv[i+1]);
        }
        if(argv[i] == opEnd){
            end = true;
            indiceFinal = atoi(argv[i+1]);
        }
        if(argv[i] == opWriteFile){
            write = true;
            outputFile = argv[i+1];
        }
        if(argv[i] == opOrderSolution){
            orderSolution = true;
        }
    }
    solutions(grafo, write, outputFile, orderSolution, indiceInicial, indiceFinal, tamanhoGrafo);
    free(grafo);
    return 0;
    
}