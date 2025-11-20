#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Es como EQU en NASM, 32 MB porque solo este ya consume 5
//Cuando se añadan los demás .txt va a pesar más
#define TAMAÑO_BUFFER 32768

//Para tokenizar, que no haya que poner el "|" cada vez
const char *DELIMITADOR = "|";

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
int escoger_orden(){
    int elección = 0;
    printf("¿Qué tipo de ordenamiento quiere hacer?\n");
    printf("1. Por título\n2. Por tamaño del título\n3. Por nombre del archivo\n4. Por definir\n");
    scanf("%d",&elección);
    if(elección >= 1 || elección <= 4){
    return elección;
    }
    else{
        printf("Esa no es una opción\n");
        return escoger_orden();
    }
}


//basado en https://www.sanfoundry.com/c-program-count-words-in-sentence/

int contar_palabras(char * string){
    int contador = 0;
    char s[200];
    strcpy(s, string);
    
    if(strlen(string) == 0){
        return 0;
    }
    for (int i = 0;s[i] != '\0';i++){
        if (s[i] == ' ' && s[i+1] != ' ')
        contador++;
    }
    contador++;
    return contador;
}

int ordenador(char *texto1, char *texto2,int elección){
    //Puse todo esto afuera porque sino había que copiar y pegar
    //todo en cada if, no conviene

    //strtok modifica string original, entonces se crean copias
    char título1[2048];
    char título2[2048];
    strcpy(título1,texto1);
    strcpy(título2,texto2);

    //Hay que tokenizar la string 
    //https://www.geeksforgeeks.org/cpp/string-tokenization-in-c/
    //https://systems-encyclopedia.cs.illinois.edu/articles/c-strtok/
    char *token1[2048];
    char *token2[2048];

    //la ubicación del archivo
    char *archivo1 = token1[3];

    char *archivo2 = token2[3];
    //Revisar el link de arriba, el de illinois, ese explica cómo sirve
    //Pero básicamente, reemplaza | por un nulo y guarda puntero
    //al inicio de cada parte, entonces token[0] es el nombre,
    //[1] apellidos y así

    for(int i = 1; i < 2048; i++){
        token1[i] = strtok(NULL,DELIMITADOR);
    }
    for(int i = 1; i < 2048; i++){
        token2[i] = strtok(NULL,DELIMITADOR);
    }

    //Ordenar por título alfabéticamente
    if(elección == 1){
        /*
        strcmp devuelve un número negativo si la primera
        va primero y positivo si la segunda va primero,
        0 si son iguales
        Se va a usar esto para saber cuál va primero
        en la otra función que los ordena
        */

        //se usa [2] porque es el campo del título
        return strcmp(token1[2],token2[2]);
        
    }
    else if(elección == 2){
        //Caso tamaño del título
        int tamaño1 = contar_palabras(token1[2]);
        int tamaño2 = contar_palabras(token2[2]);
        //se ordena el menor título primero, devuelve
        //negativo si el primero es más pequeño
        return tamaño1-tamaño2;
    }
    else if(elección == 3){
        /*
        Para este caso, lo que se tiene es una ruta de archivo
        
        Se usa la función strrchr que localiza la última
        ocurrencia de un caracter, para buscar el último
        / ya que lo que sigue es el nombre del archivo
        */
        //https://www.ibm.com/docs/es/i/7.5.0?topic=functions-strrchr-locate-last-occurrence-character-in-string

        //se pone el puntero en la última /
        char *nombre_del_archivo_a_ordenar1 = strrchr(archivo1,'/');
        char *nombre_del_archivo_a_ordenar2 = strrchr(archivo2,'/');
        //negativo si primero va primero, positivo si no, 0 si iguales
        return strcmp(nombre_del_archivo_a_ordenar1,nombre_del_archivo_a_ordenar2);
    }
    else if(elección == 4){
        return 0;
    }
    else{
        return 0;
    }
}

//Esto añade la nueva línea como el árbol heap solicita, que es buscando al padre y
//poniéndose en la posición. (i-1)/2, es la posición del padre
void meter_arriba(montículo *árbol, int índice){
    int padre = (índice-1)/2;

    
}

void insertar_en_montículo(montículo *árbol, char *línea_nueva){
    if(árbol -> tamaño >= árbol -> capacidad){
        printf("El árbol ya está lleno\n");
        free(línea_nueva);
        return;
    }
    
    árbol -> array[árbol->tamaño] = línea_nueva;
    meter_arriba(árbol, árbol->tamaño);
    árbol->tamaño++;
    
}

int crear_línea(montículo *árbol){
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
        insertar_en_montículo(árbol, nueva_línea);

    }
    fclose(archivo);
    return 0;
}

void ordenar_alfabético(montículo *árbol, int i){
    
}

void main(){
    montículo *arbolito = crear_montículo(200);
    int elección = escoger_orden();
    ordenador("hola","hola",elección);
}