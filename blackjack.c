#include <stdio.h>
#include "utils_v1.2.h"
#include <stdlib.h>
#include <time.h>

#define LIMITE_PUNTAJE 21
#define VALOR_FIGURAS 10
#define MIN_FIGURA 11
#define MAX_CARTA 13
#define LIMITE_CRUPIER 17
#define TOTAL_CARTAS 52

int pedirCarta(int mazo[], int *cartaActual);
char pedirOpcionJugador();
int jugarTurnoCrupier(int puntajeActual, int mazo[], int *cartaActual);
void inicializarMazo(int mazo[]);
void mezclarMazo(int mazo[]);
int repartirCartas(int mazo[], int *cartaActual);
void jugarBlackjack(int *saldoCuentaBancaria)
{
    int puntajeJugador = 0,
        puntajeCrupier = 0,
        apuesta = 0,
        mazo[TOTAL_CARTAS],
        cartaActual = 0;
    char opcion = ' '; // Para guardar 'P' (Pedir) o 'M' (Mantenerse/Plantarse)

    // Se prepara el mazo antes de jugar
    inicializarMazo(mazo);
    mezclarMazo(mazo);

    printf("\n--- BIENVENIDO AL BLACKJACK ---\n");
    printf("Tu saldo actual para jugar es: $%d\n", *saldoCuentaBancaria);

    // Ciclo para hacer una apuesta valida
    do
    {
        apuesta = leerEntero("Cuanto queres apostar? $");

        if (apuesta > *saldoCuentaBancaria)
        {
            printf("ERROR: Fondos insuficientes. Tu saldo disponible es de $%d\n", *saldoCuentaBancaria);
        }
        else if (apuesta <= 0)
        {
            printf("ERROR: La apuesta debe ser de al menos $1\n");
        }

    } while (apuesta > *saldoCuentaBancaria || apuesta <= 0);

    // 1. Repartir cartas iniciales

    // Se dan 2 cartas al jugador
    puntajeJugador = repartirCartas(mazo, &cartaActual);

    // Se dan tambien 2 cartas pero una no la muestra todavia
    int cartaVisible = pedirCarta(mazo, &cartaActual);
    int cartaOculta = pedirCarta(mazo, &cartaActual);
    puntajeCrupier = cartaVisible + cartaOculta;

    printf("Tus cartas suman: %d\n", puntajeJugador);
    printf("El crupier muestra un %d\n", cartaVisible);

    // 2. Turno del Jugador
    // Se juega si el jugador no saca 21 desde el principio (en el paso 1)
    if (puntajeJugador < LIMITE_PUNTAJE)
    {
        do
        {
            opcion = pedirOpcionJugador(); // Se valida que ingrese P o M
            if (opcion == 'P')
            {
                // Se pasa la cartaActual por puntero para que avance el mazo
                puntajeJugador += pedirCarta(mazo, &cartaActual);
                printf("Tu puntaje ahora es: %d\n", puntajeJugador);
            }
        } while (opcion == 'P' && puntajeJugador < LIMITE_PUNTAJE);
    }

    // 3. Evaluar si el jugador "se paso de 21" (perdió automáticamente)
    if (puntajeJugador > LIMITE_PUNTAJE)
    {
        printf("Te pasaste de %d. ¡Gana el Crupier!\n", LIMITE_PUNTAJE);
        // Aca no hace falta que juegue el crupier, el jugador ya perdio.

        *saldoCuentaBancaria -= apuesta; // Se resta la plata directamente del banco
    }
    else
    {
        // 4. Turno del Crupier (solo juega si el jugador no se pasó)
        printf("\nTurno del Crupier...\n");
        printf("El crupier da vuelta su carta oculta. Su puntaje arranca en: %d\n", puntajeCrupier);

        // El crupier juega automaticamente con las reglas del casino
        puntajeCrupier = jugarTurnoCrupier(puntajeCrupier, mazo, &cartaActual);

        // 5. DETERMINAR EL GANADOR
        if (puntajeCrupier > LIMITE_PUNTAJE)
        {
            printf("El crupier se paso, Ganaste!");

            *saldoCuentaBancaria += apuesta;
        }
        else if (puntajeCrupier < puntajeJugador)
        {
            printf("Tenes mas puntos que el crupier, Ganaste!");
            *saldoCuentaBancaria += apuesta;
        }
        else if (puntajeCrupier > puntajeJugador)
        {
            printf("Tenes menos puntos que el crupier, Gana el Crupier!");
            *saldoCuentaBancaria -= apuesta;
        }
        else
        {
            printf("Empate, ambos tienen los mismos puntos");
        }
    }
}

// Saca una carta del vector mazo ya mezclado, evalua si es figura y avanza el indice
int pedirCarta(int mazo[], int *cartaActual)
{
    int cartaOriginal = mazo[*cartaActual]; // Carta original (del 1 al 13)

    (*cartaActual)++; // Se avanza en el mazo
    int valorCarta;
    // En blackjack Jota(11), Reina(12) y Rey(13) valen 10 puntos
    if (cartaOriginal >= MIN_FIGURA && cartaOriginal <= MAX_CARTA)
    {
        valorCarta = VALOR_FIGURAS; // Si sale su valor es de 10
    }
    else
    {
        valorCarta = cartaOriginal; // Las demas valen su propio numero
    }

    return valorCarta;
}

// Pide y valida que el usuario ingrese solamente P o M
char pedirOpcionJugador()
{
    char opcionJugador;

    opcionJugador = leerCaracter("Queres pedir carta (P) o mantenerte (M)?");
    opcionJugador = toupper(opcionJugador);

    while (opcionJugador != 'P' && opcionJugador != 'M')
    {
        opcionJugador = leerCaracter("ERROR. Ingrese (P) para pedir o (M) para mantenerse");
        opcionJugador = toupper(opcionJugador);
    }

    return opcionJugador;
}

// El crupier pide cartas hasta que llegue a 17 o mas
int jugarTurnoCrupier(int puntajeActual, int mazo[], int *cartaActual)
{
    int cartaNueva = 0;

    while (puntajeActual < LIMITE_CRUPIER)
    {
        cartaNueva = pedirCarta(mazo, cartaActual);
        puntajeActual += cartaNueva;

        printf("El crupier saca un %d. Su puntaje es: %d\n", cartaNueva, puntajeActual);
    }

    return puntajeActual;
}

void inicializarMazo(int mazo[]) // Lograr que las 52 cartas tengan un valor
{
    int casillero = 0; // Variable para movernos en el mazo

    for (int palo = 0; palo < 4; palo++) // Recorre los palos (corazones, picas, diamantes, treboles)
    {
        for (int valor = 1; valor <= MAX_CARTA; valor++) // Genera los numeros del 1 al 13
        {
            mazo[casillero] = valor; // Guarda el numero de la carta en la posicion actual

            casillero++; // Avanza en el mazo
        }
    }
}

// Se hace un intercambio de cartas, entre la posicion actual y el numero al azar. Se usa para posicion y valor
void mezclarMazo(int mazo[])
{
    int posicionAlAzar;
    int auxiliar; // Variable temporal para no perder la carta en el intercambio

    for (int i = 0; i < TOTAL_CARTAS; i++) // se recorre todo el mazo
    {
        posicionAlAzar = rand() % TOTAL_CARTAS; // Se genera una posicion al azar

        auxiliar = mazo[i];              // Guardamos la carta actual
        mazo[i] = mazo[posicionAlAzar];  // Se guarda la carta al azar en la posicion actual
        mazo[posicionAlAzar] = auxiliar; // Se guarda la el valor que guardamos antes en el numero al azar
    }
}

// Funcion para repartir 2 cartas en el inicio sumando el puntaje automaticamente
int repartirCartas(int mazo[], int *cartaActual)
{
    int puntaje = 0;

    for (int i = 0; i < 2; i++)
    {
        puntaje += pedirCarta(mazo, cartaActual);
    }

    return puntaje;
}

// main para simular el juego
int main()
{

    srand(time(NULL));

    // Simulamos el saldo bancario real del cliente
    int saldoPrueba = 5000;

    // Llamamos a tu función pasándole la dirección de memoria de nuestro saldo
    jugarBlackjack(&saldoPrueba);

    printf("\n--- PRUEBA TERMINADA ---\n");
    printf("Tu saldo bancario final quedo en: $%d\n", saldoPrueba);

    return 0;
}