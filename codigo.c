#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Este número pues es el tamaño del archivo a leer
#define TAMANO_BUFFER 35670

//Para tokenizar, que no haya que poner el "|" cada vez
const char *DELIMITADOR = "|";

/*
 * Variable global, almacena el tipo de ordenamiento que se quiere hacer
 * 1 si es por título, 2 si es por por tamaño de título,
 * 3 si es por nombre del archivo, 4 si es por definir
 */
int eleccion = 0;

// Variable global para numerar la impresión de los artículos
int num_Impreso = 1;


// Pide la confirmación del usuario de que el programa puede seguir
void tiempoFuera(){
	printf("\nPresione ENTER para continuar\n");
	while(getchar() != '\n'){}
}


typedef struct monticulo{
    char **array;
    int tamano;      //Elementos actuales
    int capacidad;   //Elementos máximos
} monticulo;


monticulo *crear_monticulo(int capacidad){
    //Devuelve un puntero a un espacio de memoria tipo monticulo
    monticulo *arbol = (monticulo *)malloc(sizeof(monticulo));
    
    if(!arbol){
        return NULL;
    }
    
    arbol -> array = (char **)malloc(capacidad * sizeof(char *));
    arbol -> tamano = 0;
    arbol -> capacidad = capacidad;

    if(!arbol->array){
        free(arbol);
        return NULL;
    }
    return arbol;
}


void intercambiar(char **a, char **b){
    char *temporal = *a;
    *a = *b;
    *b = temporal;
}

/*
 * Esto guarda el tipo de ordenamiento a realizar en una variable \
 * global "eleccion"
 */
int escoger_orden(){
    printf("¿Qué tipo de ordenamiento quiere hacer?\n");
    printf("1. Por título\n2. Por tamaño del título\n%s%s",
           "3. Por nombre del archivo\n4. Por antigüedad\n",
           "5. Por contemporaneidad\n");
           
    scanf("%d",&eleccion);
    getchar();
    if(eleccion >= 1 && eleccion <= 5){
        return eleccion;
        
    } else{
        printf("Esa no es una opción\n");
        return escoger_orden();
    }
}


//basado en https://www.sanfoundry.com/c-program-count-words-in-sentence/

int contar_palabras(const char * string){
    int contador = 0;
    char s[200];
    strcpy(s, string);
    
    if(strlen(string) == 0){
        return 0;
    }
    for (int i = 0; s[i] != '\0'; i++){
        if (s[i] == ' ' && s[i+1] != ' ')
        contador++;
    }
    contador++;
    return contador;
}

int ordenador(char *texto1, char *texto2){
    //Puse todo esto afuera porque sino había que copiar y pegar
    //todo en cada if, no conviene

    //strtok modifica string original, entonces se crean copias
    char título1[TAMANO_BUFFER];
    char título2[TAMANO_BUFFER];
    strcpy(título1,texto1);
    strcpy(título2,texto2);

    //Hay que tokenizar la string
    //https://www.geeksforgeeks.org/cpp/string-tokenization-in-c/
    //https://systems-encyclopedia.cs.illinois.edu/articles/c-strtok/
    //Revisar el link de arriba, el de illinois, ese explica cómo sirve
    //Pero básicamente, reemplaza | por un nulo y guarda puntero
    //al inicio de cada parte, entonces token[0] es el nombre,
    //[1] apellidos y así


    //6 porque son 6 campos
    char *token1[6] = {0};
    char *token2[6] = {0};


    //Es esencial que esto esté en este orden, lo
    //aprendí por las malas
    token1[0] = strtok(título1,DELIMITADOR);
    for(int i = 1; i < 6; i++){
        token1[i] = strtok(NULL,DELIMITADOR);
    }
    token2[0] = strtok(título2,DELIMITADOR);
    for(int i = 1; i < 6; i++){
        token2[i] = strtok(NULL,DELIMITADOR);
    }

    //la ubicación del archivo
    char *archivo1 = token1[3];
    char *archivo2 = token2[3];
    
    //Ordenar por título alfabéticamente
    if(eleccion == 1){
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
    else if(eleccion == 2){
        //Caso tamaño del título
        int tamano1 = contar_palabras(token1[2]);
        int tamano2 = contar_palabras(token2[2]);
        //se ordena el menor título primero, devuelve
        //negativo si el primero es más pequeño
        return tamano1-tamano2;
    }
    else if(eleccion == 3){
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
        
    } else if(eleccion == 4){
        //Caso año
        //Los años de los artículos están en [4]
        //Función atoi(str) es el equivalente a int(str) en Python
        int anho1 = atoi(token1[4]);
        int anho2 = atoi(token2[4]);
        return anho1-anho2;
        
    } else if(eleccion == 5){
        //Caso año
        //Los años de los artículos están en [4]
        //Función atoi(str) es el equivalente a int(str) en Python
        int anho1 = atoi(token1[4]);
        int anho2 = atoi(token2[4]);
        return anho2-anho1;
        
    } else{
        printf("Error: Caso de ordenamiento inválido, elección = %d\n",
               eleccion);
        return 0;
    }
}

//Esto añade la nueva línea como el árbol heap solicita, que es buscando al padre y
//poniéndose en la posición. (i-1)/2, es la posición del padre
void meter_arriba(monticulo *arbol, int indice){
    /*arbol->tamano++;
    imprimir_arbol(arbol);
    arbol->tamano--;
    printf("\n\033[31mNO HUBO AÚN INTERCAMBIO\033[0m\n");
    tiempoFuera();*/
    int padre = floor((indice-1)/2);
    
    if(padre < 0){
        padre = 0;
    }

    /*
    La función ordenador devuelve negativo si el primero es
    menor, por ende, como es una min heap, se intercambia
    */
    int comprobar_intercambiar = ordenador(arbol->array[indice],arbol->array[padre]);
        //printf("\033[34m%d - %d - %d\033[0m\n", comprobar_intercambiar, indice, padre);
        //printf("%s\n%s\n",arbol->array[indice],arbol->array[padre]);
        
    while(indice > 0 && comprobar_intercambiar < 0){
        //printf("\033[31m%d - %d - %d\033[0m\n", comprobar_intercambiar, indice, padre);
        //se intercambian si el que se acaba de insertar va de primero
        intercambiar(&arbol->array[indice],&arbol->array[padre]);
        //printf("%s\n%s\n",arbol->array[indice],arbol->array[padre]);

        indice = padre;
        padre = floor((indice-1)/2);
        comprobar_intercambiar = ordenador(arbol->array[indice],arbol->array[padre]);
        
        /*printf("---\n%s\n%s\n",arbol->array[indice],arbol->array[padre]);
        arbol->tamano++;
        imprimir_arbol(arbol);
        arbol->tamano--;
        printf("\n\033[34mHUBO INTERCAMBIO\033[0m\n");
        tiempoFuera();*/
    }
    
    if(indice == 0 && comprobar_intercambiar < 0){
        //se intercambian si el que se acaba de insertar va de primero
        intercambiar(&arbol->array[indice], &arbol->array[padre]);
    }
}

void insertar_en_monticulo(monticulo *arbol, char *linea_nueva){
    if(arbol -> tamano >= arbol -> capacidad){
        printf("El árbol ya está lleno\n");
        free(linea_nueva);
        return;
    }
    
    arbol -> array[arbol->tamano] = linea_nueva;
    //printf("\n\033[32mSE AÑADIÓ UN ARTÍCULO\033[0m\n");
    meter_arriba(arbol, arbol->tamano);
    arbol->tamano++;
    
}

int crear_linea(monticulo *arbol){
    //Basado en https://www.youtube.com/watch?v=eIlcl-mt3tg
    FILE *archivo = fopen("archivo.txt","r");

    //Solo el archivo de texto de nosotros ya tiene más de 5000 caracteres
    //incluyendo los otros, se va a hacer muy extenso, por eso 32 MB
    char buffer[TAMANO_BUFFER] = {0};


    if(archivo == NULL){
        printf("Error al abrir el archivo\n");
        return -1;
    }

    //fgets lee los caracteres en una línea
    while(fgets(buffer, TAMANO_BUFFER, archivo) != NULL){
        int largo = strlen(buffer);
        //Esto cambia saltos de línea por nulo, para poder usar funciones de strings
        //similar a lo de ensamblador del billonario
        if(largo > 0 && buffer[largo-1] == '\n'){
            buffer[largo-1] = '\0';
        }
        //Se crea el espacio para la línea nueva del tamaño del buffer +1 por el \0
        char *nueva_linea = (char *)malloc(largo+1);


        strcpy(nueva_linea,buffer);
        insertar_en_monticulo(arbol, nueva_linea);

    }
    fclose(archivo);
    return 0;
}


//se usa const para evitar modificaciones
void imprimir_linea(const char *linea){
    //printf("\n\033[34m%d)\033[0m %s\n",num_Impreso,linea);

    char copia_string[TAMANO_BUFFER];
    strcpy(copia_string,linea);

    char *nombre = strtok(copia_string,DELIMITADOR);
    char *apellidos = strtok(NULL, DELIMITADOR);
    char *título = strtok(NULL,DELIMITADOR);
    char *archivo = strtok(NULL, DELIMITADOR);
    char *anho = strtok(NULL,DELIMITADOR);
    //acá se puede cambiar formato según la opción escogida
    //if(eleccion blabla)

    printf("\n\033[34m%d)\033[0m %s, %s  |  %s  |  %s\n",
           num_Impreso,apellidos, nombre, título, anho);
    num_Impreso++;
}


void reordenar_arbol(monticulo *arbol, int act){
    int izq = 2*act + 1;
    int der = 2*act + 2;
    int menor = act;

    if (izq < arbol->tamano &&
        ordenador(arbol->array[izq], arbol->array[menor]) < 0) {
        menor = izq;
    }

    if (der < arbol->tamano &&
        ordenador(arbol->array[der], arbol->array[menor]) < 0) {
        menor = der;
    }

    // ya está ordenado
    if (menor == act)
        return;

    intercambiar(&arbol->array[act], &arbol->array[menor]);
    reordenar_arbol(arbol, menor);
}


void imprimir_arbol(monticulo *arbol){
    monticulo *arbol_copia = arbol;

    while(arbol_copia->tamano > 0){
        imprimir_linea(arbol_copia->array[0]);
        free(arbol_copia->array[0]);
        arbol_copia->array[0] = arbol_copia->array[arbol_copia->tamano - 1];
        reordenar_arbol(arbol_copia, 0);
        arbol_copia->tamano--;
    }
    num_Impreso = 1;
}


// Función main que corre el programa
void main(){
    monticulo *arbolito = crear_monticulo(200);
    int eleccion = escoger_orden();
    crear_linea(arbolito);
    
    imprimir_arbol(arbolito);
}
