#include <stdio.h>
#include <string.h>

int orden_alfabetico(char *palabra1, char *palabra2){
    return strcmp(palabra1,palabra2);
}


void main(){
    printf("%d\n",orden_alfabetico("a","b"));
}