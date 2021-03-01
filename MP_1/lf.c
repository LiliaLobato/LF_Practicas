#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void states(char aut[64][64]);
void finalNode(char aut[64][64]);
int alfabeto(int matrix[2][32]);
int nodeTransition(int matrix[2][32], char key);

int main(int argv, char* args[]){

    //Variables
    int size=0;
    int node, index=0;
    int letters[2][32]={};
    char automat[64][64]={};
    char cadena[90]={};
    char number[5];
    char *helper;
    
    //Instrucciones
    printf("Se va a avaluar la cadena asignada al primer argumento\nPara separar utilice ; y evite el uso de espacios\nAl terminar los estados, presiona enter para comenzar la evaluación del caso\n");
    
    //Recolectamos el caso a evaluar
    char testCase[65]={};
    printf("Cadena a Evaluar: ");
    fgets(testCase, 64, stdin);

    //Obtenemos el alfabeto
    char alfSize=alfabeto(letters);
    
    //Decidimos cual es el nodo inicial
    printf("Nodo incial: ");
    fgets(number, 5, stdin);
    node=atoi(number);
    
    //Recibimos los nodos finales
    finalNode(automat);
    
    //Recibimos las transiciones de cada nodo
    printf("\t\t");
    int num_cols = sizeof(letters[0]) / sizeof(letters[0][0]);
    for (int alf_i = 0; alf_i <= (num_cols)-1; alf_i++) {
        if(letters[1][alf_i] != NULL){printf("%c;",letters[1][alf_i]);}
    }
    printf("\n------------------------\n");
    states(automat);
    

    snprintf(cadena, sizeof(cadena)-1, "%d",node);
    for(int c=0; c<strlen(testCase)-1; c++){
        node=automat[node][nodeTransition(letters, testCase[c])];
        helper=strdup(cadena);
        snprintf(cadena, sizeof(cadena)-1, "%s/%d", helper, node);
        free(helper);
    }
    
    if(automat[node][64]>0){
        printf("Aceptada\n");
    }else{
        printf("rechazada\n");
    }
    
    printf("%s\n", cadena);
}

int nodeTransition(int matrix[2][32], char key){
    for(int a=0; a<32; a++){
        if(matrix[1][a]==key){
            return matrix[0][a];
        }
    }
    return -1;
}

int alfabeto(int matrix[2][32]){
    for(int iter=0; iter<32; iter++){
        matrix[0][iter]=iter;
    }
    
    char dirty[65];
    printf("Alfabeto: ");
    fgets(dirty, 64, stdin);
    int iter;
    for(iter=0; iter<65; iter+=2){
        if(dirty[iter]=='\0'||dirty[iter]=='\n')break;
        matrix[1][iter/2]=dirty[iter];
    }
    return iter/2;
    
}


void finalNode(char aut[64][64]){
    char dirty[129];
    printf("Nodo final: ");
    fgets(dirty, 128, stdin);
    int iter;
    for(iter=0; iter<129; iter+=2){
        if(dirty[iter]=='\0'||dirty[iter]=='\n')break; 
        aut[atoi(&dirty[iter])][63]=1;
    }
}

void states(char aut[64][64]){
    char dirty[129];
    
    for(int node=0; node<64; node++){
        memset(dirty,'\0',129);
        
        printf("Estado %d:\t", node);
        fgets(dirty, 128, stdin);
        
        if(dirty[0]=='\n')break;
        int iter;
        
        for(iter=0; iter<129; iter+=2){
            if(dirty[iter]=='\0')break;
            aut[node][iter/2]=atoi(&dirty[iter]);
            
        }
    }
}


