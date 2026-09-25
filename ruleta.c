#include "utils_vAmpliada.h"
#include <time.h>
#include <stdlib.h>

#define COLOR_VERDE 0
#define CANT_ROJOS 18
#define CANT_TOTAL 36

int numAleatorioEntre(int num1, int num2);
bool esColorRojo (int numero);
bool esPar(int numero);

int main(){
    srand(time(NULL));
    int numeroIngresado = 0;
    int numeroRuleta = 0;

    numeroIngresado = leerEnteroEntre(0, 36, "Ingresa numero: \n");

    if(esPar(numeroIngresado)){
        printf("Es par\n");
    }else{
        printf("Es inpar\n");
    }

    numeroRuleta = numAleatorioEntre(COLOR_VERDE, CANT_TOTAL);

    if (numeroRuleta == 0){
        printf("VERDE 0\n");
    }else if ( esColorRojo(numeroRuleta) ){
        printf("Colorado el %d\n", numeroRuleta);
    }else{
        printf("Negro el %d\n", numeroRuleta);
    }

    if (numeroRuleta == numeroIngresado ){
        printf("Ganaste");
    }else{
        printf("Perdiste");
    }

    return 0;
}

bool esPar(int numero){
    return numero % 2 == 0;
}

bool esColorRojo (int numeroBuscado){
    int grupoRojos[CANT_ROJOS] = {1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36};
    bool encontrado = false;
    int posicion = 0;

    while(posicion< CANT_ROJOS){
        if(grupoRojos[posicion]== numeroBuscado){
            encontrado = true;
        }
        posicion++;
    }

    return encontrado;
}

int numAleatorioEntre(int num1, int num2){
    int num = ( rand() % num2 ) + num1;
    return num;
}