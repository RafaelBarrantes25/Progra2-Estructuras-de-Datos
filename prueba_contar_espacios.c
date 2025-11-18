#include <stdio.h>
#include <string.h>
//basado en https://www.sanfoundry.com/c-program-count-words-in-sentence/

int contar_espacios(char *string){
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



void main(){
    printf("%d\n",contar_espacios("hahaha"));
}