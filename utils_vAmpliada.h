#include <stdio.h>
#include <ctype.h>
#include <stdbool.h> // Para usar el tipo de dato bool
#include <string.h> // Este #include servirá para más adelante…
typedef char cadena[200]; // Para tratar a los arrays de chars como 'cadena'

///////////////////////////////////////////////////////////////////////////
////////////////////////////  PROTOTIPOS  /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje);
float leerFloat(cadena mensaje);
char leerCaracter(cadena mensaje);
int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje);
float leerFloatEntre(float valorMin, float valorMax, cadena mensaje);
bool confirmaUsuario(cadena mensaje);

//////////////////////  FUNCIONES EXTRAS  /////////////////////////////////////

int cantDivisores_NumeroEntero(int numero);
int Porcentaje(int cantEspecifica, int cantTotal);
float Promedio(float sumConjunto, int divCantTotal);
void imprimirSimbolo(int cantSimbolos, char simbolo);

//////////////////////  FUNCIONES CON VECTORES  ///////////////////////////

void leerVectorDeEnteros(cadena mensaje, int vec[], int cant);
void leerVectorDeFloats(cadena mensaje, float vec[], int cant);
void leerVectorDeCaracteres(cadena mensaje, char vec[], int cant);
void leerVectorDeEnterosEntre(int valorMin, int valorMax, cadena mensaje, int vec[], int cant);
void leerVectorDeFloatsEntre(float valorMin, float valorMax, cadena mensaje, float vec[], int cant);
void mostrarVectorDeEnteros(int vec[], int cant);
void mostrarVectorDeFloats(float vec[], int cant);
void mostrarVectorDeCaracteres(char vec[], int cant);
void ordenarVectorDeEnterosAscendente(int vec[], int cant);
void ordenarVectorDeEnterosDescendente(int vec[], int cant);
bool buscador(char buscado, char vector[], int* cant); //modificar segun strings

////////////////TERCERA VERSIÓN: Funciones para leer cadenas////////////////

void leerCadena(cadena mensaje, cadena target);
void leerCadenaEntre(int minLength, int maxLength, cadena mensaje, cadena target);


///////////////////////////////////////////////////////////////////////////
/////////////////////////  IMPLEMENTACIONES  //////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un entero y lo retorna */
    int numero;
    printf("%s", mensaje);
    scanf(" %d", &numero);
    return numero;
}
float leerFloat(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un float y lo retorna */
    float numero;
    printf("%s", mensaje);
    fflush(stdin); // No es obligatorio pero por las dudas...
    scanf("%f", &numero);
    return numero;
}
char leerCaracter(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un char y lo retorna */
    char letra;
    printf("%s", mensaje);
    scanf(" %c", &letra);
    return letra;
}
int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje) {
    /* Muestra el mensaje al usuario. Luego lee enteros mientras éstos estén
       fuera del rango [valorMin, valorMax] informando al usuario de
       su error. Cuando finalmente lea un entero válido, lo retorna */
    int numero;
    printf("%s", mensaje);
    scanf("%d", &numero);
    while (numero < valorMin || numero > valorMax)
    {
        printf("Error. El valor ingresado no esta dentro del rango de %d y %d\n", valorMin, valorMax);
        printf("%d %d %s",valorMin, valorMax, mensaje);
        scanf(" %d", &numero);
    }
    return numero;
}
float leerFloatEntre(float valorMin, float valorMax, cadena mensaje) {
    /* Muestra el mensaje al usuario. Luego, lee floats mientras éstos estén
       fuera del rango [valorMin, valorMax] informando al usuario de
       su error. Cuando finalmente lea un float válido, lo retorna */
    float numero;
    printf("%s", mensaje);
    scanf("%f", &numero);
    while (numero < valorMin || numero > valorMax)
    {
        printf("Error. El valor ingresado no esta dentro del rango de %.2f y %.2f\n", valorMin, valorMax);
        printf("%.2f %.2f %s", valorMin, valorMax, mensaje);
        scanf(" %f", &numero);
    }
    return numero;
}
bool confirmaUsuario(cadena mensaje) {
    /* Muestra el mensaje al usuario junto a la leyenda "[S/N]" que
       representa "Si" o "No". Luego, lee chars mientras éstos no sean los
       previstos ('S' o 'N', incluyendo minúsculas) informando al
       usuario de su error. Retorna si el usuario seleccionó que sí. */
    char respuesta;
    printf("%s [S/N]: ", mensaje);
    scanf(" %c", &respuesta);
    respuesta = toupper(respuesta);
    while (respuesta != 'S' && respuesta != 'N') {
        printf("Error. Respuesta no valida. Ingrese S o N: ");
        scanf(" %c", &respuesta);
        respuesta = toupper(respuesta);
    }
    return (respuesta == 'S'); 
} 

float Promedio(float sumConjunto, int divCantTotal)
{
    return (sumConjunto / divCantTotal);
}
int cantDivisores_NumeroEntero(int numero)
{

    int cantDiv = 0;

    for (int i = 1; i <= numero; i++)
    {
        if (numero % i == 0)
        {
            cantDiv++;
        }
    }

    return cantDiv;
}
int Porcentaje(int cantEspecifica, int cantTotal)
{
    return ((cantEspecifica * 100) / cantTotal);
}
void imprimirSimbolo(int cantSimbolos, char simbolo)
{
    for (int i = 0; i < cantSimbolos; i++)
    {
        printf("%c", simbolo);
    }
}

void leerVectorDeEnteros(cadena mensaje, int vec[], int cant) {
    /* Lee 'cant' enteros, mostrando el mismo mensaje cada vez,
    y guarda los valores en cada posición de 'vec' */
    for (int i = 0; i < cant; i++)
    {
        printf("[%d] ", i);
        vec[i] = leerEntero(mensaje);
    }
}
void leerVectorDeFloats(cadena mensaje, float vec[], int cant) {
    /* Lee 'cant' floats, mostrando el mismo mensaje cada vez,
    y guarda los valores en cada posición de 'vec' */
    for (int i = 0; i < cant; i++) {
        printf("[%d] ", i); // Los corchetes se imprimen a modo ilustrativo
        vec[i] = leerFloat(mensaje); // Reaprovecho la función hecha antes
    }
}
void leerVectorDeCaracteres(cadena mensaje, char vec[], int cant) {
    /* Lee 'cant' chars, mostrando el mismo mensaje cada vez,
    y guarda los valores en cada posición de 'vec' */
    for (int i = 0; i < cant; i++)
    {
        printf("[%d] ", i); // Los corchetes se imprimen a modo ilustrativo
        vec[i] = leerCaracter(mensaje);
    }
}
void leerVectorDeEnterosEntre(int valorMin, int valorMax, cadena mensaje, int vec[], int cant) {
    /* Lee 'cant' enteros, dentro del rango [valorMin, valorMax],
    mostrando el mismo mensaje cada vez, y guarda los
    valores en cada posición de 'vec' */
    for (int i = 0; i < cant; i++)
    {
        printf("[%d] ", i);
        vec[i] = leerEnteroEntre(valorMin, valorMax, mensaje);
    }
}
void leerVectorDeFloatsEntre(float valorMin, float valorMax, cadena mensaje, float vec[], int cant) {
    /* Lee 'cant' floats, dentro del rango [valorMin, valorMax],
    mostrando el mismo mensaje cada vez, y guarda los
    valores en cada posición de 'vec' */
    for (int i = 0; i < cant; i++)
    {
        printf("[%d] ", i);
        vec[i] = leerFloatEntre(valorMin, valorMax, mensaje);
    }
}
void mostrarVectorDeEnteros(int vec[], int cant) {
    /* Muestra el contenido de 'vec' en renglones separados,
    Incluyendo también el número de posición (empezando desde cero) */
    for (int i = 0; i < cant; i++)
    {
        printf("[%d] => %d\n", i, vec[i]);
    }
}
void mostrarVectorDeFloats(float vec[], int cant) {
    for (int i = 0; i < cant; i++) {
        printf("[%d] => %f\n", i, vec[i]); // Los corchetes se imprimen a modo ilustrativo
    }
}
void mostrarVectorDeCaracteres(char vec[], int cant) {
    /* Muestra el contenido de 'vec' en renglones separados,
    Incluyendo también el número de posición (empezando desde cero) */
    for (int i = 0; i < cant; i++)
    {
        printf("[%d] => %c\n", i, vec[i]);
    }
}
void ordenarVectorDeEnterosAscendente(int vec[], int cant) {
    /* Ordena los valores de 'vec' de forma ascendente (menor a mayor),
    utilizando algún algoritmo de ordenamiento (investigar) */
    /*Método de búrbuja*/
    int aux;
    for (int i = 0; i < (cant - 1); i++)
    {
        for (int j = i + 1; j < cant; j++)
        {
            if (vec[j] < vec[i])
            {
                aux = vec[j];
                vec[j] = vec[i];
                vec[i] = aux;
            }
        }
    }
}
void ordenarVectorDeEnterosDescendente(int vec[], int cant) {
    /* Ordena los valores de 'vec' de forma descendente (mayor a menor),
    utilizando algún algoritmo de ordenamiento (investigar) distinto al
    anterior */
    /*Método de búrbuja*/
    int aux;
    for (int i = 0; i < (cant - 1); i++)
    {
        for (int j = i + 1; j < cant; j++)
        {
            if (vec[j] > vec[i])
            {
                aux = vec[j];
                vec[j] = vec[i];
                vec[i] = aux;
            }
        }
    }
}
bool buscador(char buscado, char vector[], int* cant){
    bool existe = false;
    for (int i = 0; i < *cant; i++)
    {
        if (buscado == vector[i])
        {
            existe = true;
        }
    }
    return existe;
}

void leerCadena(cadena mensaje, cadena target) {
    /* Muestra el mensaje al usuario y luego lee una
    cadena por referencia usando la función 'gets()' */
    printf("%s", mensaje);
    gets(target);
    //scanf("%s", target); // No va el &, porque una cadena ya es un puntero al primer elemento.
}
void leerCadenaEntre(int minLength, int maxLength, cadena mensaje, cadena target) {
/* Muestra el mensaje al usuario y luego lee una
cadena por referencia usando la función 'gets()' mientras
su longitud no esté dentro del rango [minLength, maxLength] */
    printf("%s \n", mensaje);
    gets(target);
<<<<<<< HEAD
    while ((int)strlen(target) < minLength || (int)strlen(target) > maxLength) //Se castea para que no haya errores si ingresa un numero negativo
=======
    while ((int) strlen(target) < minLength ||(int) strlen(target) > maxLength)
>>>>>>> bab03efebf1d2f8ab44b04c138a3ba9a2bd3081e
    {
        printf("Erorr. La cadena no cumple con un minimo de %d caracteres y de %d caracteres.", minLength, maxLength);
        gets(target);
    }
}