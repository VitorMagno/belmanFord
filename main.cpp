#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<queue>
#include<vector>
#include<climits>

using namespace std;
void relaxamento(int* dist, int* pais, int* grafo[1000][1000], int i, int j){
    dist[j] = dist[i] + *grafo[i][j];
    pais[j] = i;
    return;
}

bool belmanFord(int* grafo[1000][1000], int vertInicial, int vertFinal, int tamanhoGrafo, int ordemGrafo, int* dist, int *pais){
    int visitados[ordemGrafo+1][ordemGrafo+1] = {};
    /*for (int i  = 0; i < ordemGrafo + 1; i++){
        for (int j = 0; j < ordemGrafo + 1; j++){
            visitados[i][j] = 0;
        }
    }*/
    dist[vertInicial] = 0;
    pais[vertInicial] = vertInicial;
    for (int i = 1; i < ordemGrafo+1; i++){
        for (int j = 1; j < ordemGrafo+1; j++){
            if (grafo[i][j] != 0){
                if(dist[j] > dist[i] + *grafo[i][j]){
                    if(dist[j] == 100000) dist[j] = 0;
                    visitados[i][j] = 1;
                    relaxamento(dist, pais, grafo, i, j); // i = vertice origem do arco, j = vertice destino do arco
                }
            }
        }
    }
    for (int i = 1; i < ordemGrafo+1; i++){
        for (int j = 1; j < ordemGrafo+1; j++){
            if (*grafo[i][j] != INT_MAX){
                if(dist[j] > dist[i] + *grafo[i][j]){
                    return false;
                }
            }
        }
    }
    return true;
}

void solutions (int* grafo[1000][1000], bool write, string outputFile, bool orderSolution, int vertInicial, int vertFinal, int tamanhoGrafo, int ordemGrafo){
    int dist[ordemGrafo+1];
    int pais[ordemGrafo+1];
    for (int i = 0; i < ordemGrafo+1; i++){
        dist[i] = 100000;
        pais[i] = -1;
    }
    bool respo = belmanFord(grafo, vertInicial, vertFinal, tamanhoGrafo, ordemGrafo, &dist[0], &pais[0]);
    if(respo == false){
        cout <<"grafo possui ciclo negativo"<<endl;
    }
    
    if(write && orderSolution){
        ofstream writeFile;
        writeFile.open(outputFile, ofstream::out);
        for(int i= 1; i < ordemGrafo+1; i++){
            writeFile << i ;
            for (int j = 1; j < ordemGrafo + 1; j++){
                if(dist[j] == 100000){
                    continue;
                }else{
                    for(int k = j; k != vertInicial; k = pais[k]){
                        writeFile << dist[k] << " ";
                    }
                }
            }
            writeFile << endl;
        }
        writeFile.close();
        return;
    }
    if(write){
        ofstream writeFile;
        writeFile.open(outputFile, ofstream::out);
        int sumCaminho=0;

        for(int i= 1; i < ordemGrafo+1; i++){
            for (int j = 1; j < ordemGrafo + 1; j++){
                if(dist[j] == 100000){
                    continue;
                }else{
                    for(int k = j; k != vertInicial; k = pais[k]){
                        sumCaminho += dist[k];
                    }
                }
            }
        }

        writeFile << "peso do menor caminho: "<< sumCaminho << endl;
        writeFile.close();
        return;
    }
    if(orderSolution){
        // desenvolver
        for(int i= 1; i < ordemGrafo+1; i++){
            cout << i ;
            for (int j = 1; j < ordemGrafo + 1; j++){
                if(dist[j] == 100000){
                    continue;
                }else{
                    for(int k = j; k != vertInicial; k = pais[k]){
                        cout << dist[k] << " ";
                    }
                }
            }
            cout << endl;
        }
        return;
    }

    int sumCaminho=0;
    for(int i= 1; i < ordemGrafo+1; i++){
        for (int j = 1; j < ordemGrafo + 1; j++){
            if(dist[j] == 100000){
                continue;
            }else{
                for(int k = j; k != vertInicial; k = pais[k]){
                    sumCaminho += dist[k];
                }
            }
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
    int grafo[1000][1000] = {};
    int *ptr = &grafo[0][0];
    const int BUFFER_SIZE = 8;
    char buffer[BUFFER_SIZE];
    
    string opReadFile = "-f";
    string opWriteFile = "-o";
    string opOrderSolution = "-s";
    string opStart = "-i";
    string opEnd = "-l";
    string outputFile = "saida";
    string opHelp = "-h";
    
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


            indiceFinal = ordemGrafo+1;

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
        if(argv[i] == opHelp){
            cout << "Algoritmo de bellmanFord" << endl;
            cout << "use -f <arquivo> para entrada de um grafo utilizando uma matriz de incidencia"<< endl;
            cout << "use -o <arquivo> para redirecionar saida para \"arquivo\""<< endl;
            cout << "use -s para exibir solucao ordenada"<< endl;
            cout << "use -i <vertInicial> apos -o ou apos <arquivo> para indicar vertice inicial"<< endl;
            cout << "use -l <vertFinal> apos -o ou apos <arquivo> para indicar vertice final"<< endl;
            return 0;
        }
    }
    solutions(ptr, write, outputFile, orderSolution, indiceInicial, indiceFinal, tamanhoGrafo, ordemGrafo);
    return 0;
    
}