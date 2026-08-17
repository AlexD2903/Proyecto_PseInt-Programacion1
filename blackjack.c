#include <stdio.h>
#include "utils_v1.2.h"
#include <stdlib.h>

#define LIMITE_PUNTAJE 21
#define VALOR_FIGURAS 10
#define MIN_FIGURA 11
#define MAX_CARTA 13
#define LIMITE_CRUPIER 17
#define TOTAL_CARTAS 52

int pedirCarta();
char pedirOpcionJugador();
int jugarTurnoCrupier(int puntajeActual);
int jugarBlacjack(int *saldoCuentaBancaria)
{
    int puntajeJugador = 0,
        puntajeCrupier = 0,
        apuesta = 0;
    char opcion = ' '; // Para guardar 'P' (Pedir) o 'M' (Mantenerse/Plantarse)

    printf("\n--- BIENVENIDO AL BLACKJACK ---\n");
    printf("Tu saldo actual para jugar es: $%d\n", *saldoCuentaBancaria);
    apuesta = leerEntero("Cuanto queres apostar? :$");

    // 1. Repartir cartas iniciales
    puntajeJugador = pedirCarta() + pedirCarta();
    puntajeCrupier = pedirCarta() + pedirCarta();

    printf("Tus cartas suman: %d\n", puntajeJugador);
    printf("El crupier muestra que tiene al menos un: %d\n", puntajeCrupier / 2);

    // 2. Turno del Jugador
    do
    {
        if (puntajeJugador < LIMITE_PUNTAJE)
        {
            opcion = pedirOpcionJugador();
            if (opcion == 'P')
            {
                puntajeJugador += pedirCarta();
                printf("Tu puntaje ahora es: %d\n", puntajeJugador);
            }
        }

    } while (opcion == 'P' && puntajeJugador < LIMITE_PUNTAJE);

    // 3. Evaluar si el jugador "se paso de 21" (perdió automáticamente)
    if (puntajeJugador > LIMITE_PUNTAJE)
    {
        printf("Te pasaste de %d. ¡Gana el Crupier!\n", LIMITE_PUNTAJE);
        // Aca no hace falta que juegue el crupier, el jugador ya perdio.

        *saldoCuentaBancaria -= apuesta;
    }
    else
    {
        // 4. Turno del Crupier (solo juega si el jugador no se pasó)
        printf("\nTurno del Crupier...\n");
        puntajeCrupier = jugarTurnoCrupier(puntajeCrupier);

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

    return 0;
}

int pedirCarta()
{
    int numeroGenerado = (rand() % MAX_CARTA) + 1, // Genera un número aleatorio entre 1 y 13
        valorCarta;

    if (numeroGenerado >= MIN_FIGURA && numeroGenerado <= MAX_CARTA) // Comprueba si salio una figura(jota, reina o rey)
    {
        valorCarta = VALOR_FIGURAS; // Si sale su valor es de 10
    }
    else
    {
        valorCarta = numeroGenerado;
    }

    return valorCarta;
}

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

int jugarTurnoCrupier(int puntajeActual)
{
    int cartaNueva = 0;

    while (puntajeActual < LIMITE_CRUPIER)
    {
        cartaNueva = pedirCarta();
        puntajeActual += cartaNueva;

        printf("El crupier saca un %d. Su puntaje es: %d\n", cartaNueva, puntajeActual);
    }

    return puntajeActual;
}