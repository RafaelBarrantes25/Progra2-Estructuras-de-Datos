#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Es como EQU en NASM, 32 MB porque solo este ya consume 5
//Cuando se añadan los demás .txt va a pesar más
#define TAMAÑO_BUFFER 32768


typedef struct montículo{
    char **array;
    int tamaño;      //Elementos actuales
    int capacidad;   //Elementos máximos
}montículo;

montículo *crear_montículo(int capacidad){
    //Devuelve un puntero a un espacio de memoria tipo montículo
    montículo *árbol = (montículo *)malloc(sizeof(montículo));
    
    if(!árbol){
        return NULL;
    }
    árbol -> tamaño = 0;
    árbol -> capacidad = capacidad;

    árbol -> array = (char **)malloc(sizeof(char *));
    
    if(!árbol->array){
        free(árbol);
        return NULL;
    }
    return árbol;
}


void intercambiar(char **a, char **b){
    char *temporal = *a;
    *a = *b;
    *b = temporal;
}

int insertar_en_montículo(montículo *árbol){
    //Basado en https://www.youtube.com/watch?v=eIlcl-mt3tg
    FILE *archivo = fopen("archivo.txt","r");

    //Solo el archivo de texto de nosotros ya tiene más de 5000 caracteres
    //incluyendo los otros, se va a hacer muy extenso, por eso 32 MB
    char buffer[TAMAÑO_BUFFER] = {0};


    if(archivo == NULL){
        printf("Error al abrir el archivo\n");
        return 1;
    }

    //fgets lee los caracteres en una línea
    while(fgets(buffer, TAMAÑO_BUFFER,archivo) != NULL){
        int largo = strlen(buffer);
        //Esto cambia saltos de línea por nulo, para poder usar funciones de strings
        //similar a lo de ensamblador del billonario
        if(largo > 0 && buffer[largo-1] == '\n'){
            buffer[largo-1] = '\0';
        }
        //Se crea el espacio para la línea nueva del tamaño del buffer +1 por el \0
        char *nueva_línea = (char *)malloc(strlen(buffer)+1);

        strcpy(nueva_línea,buffer);
    }
    fclose(archivo);
    return 0;
}

void ordenar_alfabético(montículo *árbol, int i){
    
}

void main(){
    montículo *arbolito = crear_montículo(200);
}