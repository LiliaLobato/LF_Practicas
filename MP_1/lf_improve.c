#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void joke();
void menu();
int file_up();

void states(char aut[64][64]);
void finalNode(char aut[64][64], char dirty[129]);
int alfabeto(int matrix[2][32], char dirty[65]);
int nodeTransition(int matrix[2][32], char key);
void states_file(char aut[64][64], int node, char dirty[129]);
void manually();

int main()
{
    menu();
    return 0;
}

void menu()
{
    int menu1choice;

    printf("'\n\nAFD Simulation\n");
    printf("\n------------------------\n");
    printf("1. Upload a .txt file\n");
    printf("2. Manually Upload each description\n");
    printf("ANY OTHER KEY = Exit\n");
    scanf("%d", &menu1choice);
    switch ( menu1choice ) {
        case 1: 
        {
            file_up();
            menu();
        }
        case 2: 
        {
            manually();
            menu();
        }
        default :
        {
            break;
        }

    }

}

int file_up() {
    char c[1000];
    char filename [100];
    int line = 0;
    
    //Variables
    int size=0;
    int node, node_c, index=0;
    int letters[2][32]={};
    char automat[64][64]={};
    char cadena[90]={};
    char number[5];
    char *helper;
    char testCase[65]={};
    
    fgets(filename, 64, stdin);
    printf("Se va a avaluar la cadena asignada al inicio del documento\nPara separar utilice ; y evite el uso de espacios\n");
    printf("File name: ");
    fgets(filename, 64, stdin);
    strtok(filename, "\n");
    
    FILE *fptr;
    if ((fptr = fopen(filename, "r")) == NULL) {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    while ( fgets( c, sizeof( c ), fptr ) != NULL ) {
        strtok(c, "\n");
        if (line==0) {
            //Recolectamos el caso a evaluar
            strcpy(testCase, c);
            printf("Cadena a Evaluar: %s\n", testCase);
        } else if (line == 1) {
            //Obtenemos el alfabeto
            char alfSize=alfabeto(letters,c);
            printf("Alfabeto: %s\n", c);
        } else if (line == 2) {
            //Decidimos cual es el nodo inicial
            strcpy(number, c);
            printf("Nodo incial: %s\n", number);
            node=atoi(number);
        } else if (line == 3) {
            printf("Nodo final: %s\n", c);
            finalNode(automat,c);
            printf("\n------------------------\n");
            printf("\t\t");
            int num_cols = sizeof(letters[0]) / sizeof(letters[0][0]);
            for (int alf_i = 0; alf_i <= (num_cols)-1; alf_i++) {
                if(letters[1][alf_i] != NULL){printf("%c;",letters[1][alf_i]);}
            }
            printf("\n------------------------\n");
        } else {
            printf("Estado %d:\t%s\n", node_c, c);
            states_file(automat, node_c,c);
            node_c++;
        }
        line++;
    }
    
    snprintf(cadena, sizeof(cadena)-1, "%d",node);
    for(int p=0; p<strlen(testCase)-1; p++){
        node=automat[node][nodeTransition(letters, testCase[p])];
        helper=strdup(cadena);
        snprintf(cadena, sizeof(cadena)-1, "%s/%d", helper, node);
        free(helper);
    }
    
    if(automat[node][64]>0){
        printf("ACEPTADA\n");
    }else{
        printf("RECHAZADA\n");
    }
    
    printf("%s\n", cadena);
    
    return 0;
}

void manually(){

    //Variables
    int size=0;
    int node, index=0;
    int letters[2][32]={};
    char automat[64][64]={};
    char cadena[90]={};
    char number[5];
    char dirty[65];
    char *helper;
    char dirty2[129];
    
    //Instrucciones
    printf("Se va a avaluar la cadena asignada al primer argumento\nPara separar utilice ; y evite el uso de espacios\nAl terminar los estados, presiona enter para comenzar la evaluación del caso\n");
    
    //Recolectamos el caso a evaluar
    char testCase[65]={};
    fgets(testCase, 64, stdin);
    printf("Cadena a Evaluar: ");
    fgets(testCase, 64, stdin);

    //Obtenemos el alfabeto
    printf("Alfabeto: ");
    fgets(dirty, 64, stdin);
    char alfSize=alfabeto(letters,dirty);
    
    //Decidimos cual es el nodo inicial
    printf("Nodo incial: ");
    fgets(number, 5, stdin);
    node=atoi(number);
    
    //Recibimos los nodos finales
    printf("Nodo final: ");
    fgets(dirty2, 128, stdin);
    finalNode(automat,dirty2);
    
    //Recibimos las transiciones de cada nodo
    
    printf("\n------------------------\n");
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
        printf("ACEPTADA\n");
    }else{
        printf("RECHAZADA\n");
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

int alfabeto(int matrix[2][32], char dirty[65]){
    for(int iter=0; iter<32; iter++){
        matrix[0][iter]=iter;
    }
    int iter;
    for(iter=0; iter<65; iter+=2){
        if(dirty[iter]=='\0'||dirty[iter]=='\n')break;
        matrix[1][iter/2]=dirty[iter];
    }
    return iter/2;
}

void finalNode(char aut[64][64], char dirty[129]){
    int iter;
    for(iter=0; iter<129; iter+=2){
        if(dirty[iter]=='\0'||dirty[iter]=='\n')break; 
        aut[atoi(&dirty[iter])][63]=1;
    }
}

void states_file(char aut[64][64], int node, char dirty[129]){
    if(node<64){
        int iter;
        if(dirty[0]!='\n') {
            for(iter=0; iter<129; iter+=2){
                if(dirty[iter]=='\0')break;
                aut[node][iter/2]=atoi(&dirty[iter]);
            }
        }
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
            printf("%d",aut[node][iter/2]);
            
        }
    }
}


