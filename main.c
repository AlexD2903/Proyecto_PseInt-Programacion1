#include "utils.h"
#include <windows.h>

#define OPC_MIN 1
#define OPC_MAX 5
#define CANT_USERS 5
#define DATOS_USERS 3 // nombre, apellido, alias
#define VACIO 0
#define OPCION_SALIDA 0
#define MIN_CLAVE 1000
#define MAX_CLAVE 9999
#define INDICE_NOMBRE 0
#define INDICE_APELLIDO 1
#define INDICE_ALIAS 2

/////////////////// FUNCIONES DEL CLIENTE ///////////////////

void menuVistaCliente();
void menuOpcCliente(int *saldo);
void consultarSaldo(int *saldoConsultado);

/////////////////// FUNCIONES DEL ADMINISTRADOR ///////////////////

void menuVistaAdmin();
void menuOpcAdmin(cadena matrizUser[][DATOS_USERS], int vecClaves[], int *cantUsuarios);
void menuGestionarPersonal(cadena matrizUser[][DATOS_USERS], int vecClaves[], int *cantUsuarios);
void vistaGestionPersonal();
void enEspera();

/////////////////// ALTA PERSONAL ///////////////////
void altaPersonal(cadena matrizUser[][DATOS_USERS], int vecClaves[], int *cantUsuarios);
void agregarUsuario(cadena matrizUser[][DATOS_USERS], int vecClaves[], int *cantUsuarios, cadena nombre, cadena apellido, cadena alias, int clave);
void nuevoUsuario(cadena nombre, cadena apellido, cadena alias, cadena matrizUser[][DATOS_USERS], int cantUsuarios);

/////////////////// CONSULTAR O MODIFICAR ///////////////////
void listaUsuarios(cadena matrizUser[][DATOS_USERS], int *cantUsuarios);
void mostrarDatosDeMatriz(int COLUMNAS, cadena matriz[][COLUMNAS], int *cant);
int buscarUsuarios(cadena matrizUser[][DATOS_USERS], int *cantUsuarios, cadena aliasBuscado, int posicionColumna);

void modificarUsuario(cadena matrizUser[][DATOS_USERS], int vecClave[], int *cantUsuarios);

void cambiarAliasDeUsuario(cadena matrizUser[][DATOS_USERS], int *cant, cadena aliasNuevo);
void cambiarClaveDeUsuario(int vecClave[], cadena matrizUser[][DATOS_USERS], int *cantUsuarios, cadena aliasBuscado, int posicionEnColumna);
int verificacionClave(int *contador, int vecClave[], int posicion, cadena msj1, cadena msj2);
void bajaUsuario(cadena matrizUser[][DATOS_USERS], int vecClave[], int *cantUsuarios, cadena aliasBuscado);

// --------------MAIN
int main()
{

    cadena listaUser[CANT_USERS][DATOS_USERS];
    int vecClaves[CANT_USERS] = {0};
    int cantUsuarios = 0;

    menuGestionarPersonal(listaUser, vecClaves, &cantUsuarios);

    return 0;
}

void menuOpcAdmin(cadena matrizUser[][DATOS_USERS], int vecClaves[], int *cantUsuarios)
{
    int opcion = 0;

    menuVistaAdmin();
    opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");

    while (opcion != OPC_MAX)
    {
        switch (opcion)
        {
        case 1:
            menuGestionarPersonal(matrizUser, vecClaves, cantUsuarios);
            break;
        case 2:
            enEspera();
            break;
        case 3:
            enEspera();
            break;
        case 4:
            enEspera();
            break;
        case 5:
            printf("Cerrando sesion");
            break;
        }
        Sleep(1000);
        system("cls");
        menuVistaAdmin();
        opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    }
}

void menuGestionarPersonal(cadena matrizUser[][DATOS_USERS], int vecClaves[], int *cantUsuarios)
{
    int opcion = 0;
    cadena aliasAuxiliar;

    vistaGestionPersonal();
    opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    while (opcion != OPC_MAX)
    {
        switch (opcion)
        {
        case 1: // C (Create / Crear)
            altaPersonal(matrizUser, vecClaves, cantUsuarios);
            break;
        case 2: // R (Read / Leer o Consultar)
            listaUsuarios(matrizUser, cantUsuarios);
            break;
        case 3: // U (Update / Actulizar o Modificar)
            modificarUsuario(matrizUser, vecClaves, cantUsuarios);
            break;
        case 4: // D (Delete / Borrar o Eliminar)
            leerCadena("Ingrese el alias del usuario a dar de baja", aliasAuxiliar);
            bajaUsuario(matrizUser, vecClaves, cantUsuarios, aliasAuxiliar);
            break;
        }
        Sleep(1000);
        system("cls");
        vistaGestionPersonal();
        opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    }
}

void modificarUsuario(cadena matrizUser[][DATOS_USERS], int vecClave[], int *cantUsuarios)
{
    int opc = 0;
    cadena aliasAuxiliar;

    printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre de Usuario\n2. Clave\n0. Volver\n");

    opc = leerEnteroEntre(0, 2, "Elija una opcion: ");
    while (opc != OPCION_SALIDA)
    {
        switch (opc)
        {
        case 1:
            leerCadena("Ingrese el alias del usuario que desea modificar: ", aliasAuxiliar);
            cambiarAliasDeUsuario(matrizUser, cantUsuarios, aliasAuxiliar);
            break;
        case 2:
            leerCadena("Ingrese el alias del usuario que desea modificar: ", aliasAuxiliar);
            cambiarClaveDeUsuario(vecClave, matrizUser, cantUsuarios, aliasAuxiliar, INDICE_ALIAS);
            break;
        }

        printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre de Usuario\n2. Clave\n0. Volver\n");
        opc = leerEnteroEntre(0, 2, "Elija una opcion: ");
    }
}

void cambiarAliasDeUsuario(cadena matrizUser[][DATOS_USERS], int *cant, cadena aliasBuscado)
{
    int posicion = buscarUsuarios(matrizUser, cant, aliasBuscado, INDICE_ALIAS);

    if (posicion != -1)
    {
        cadena aliasNuevo;
        leerCadena("Elija un nuevo alias", aliasNuevo);

        while (buscarUsuarios(matrizUser, cant, aliasNuevo, INDICE_ALIAS) != -1)
        {
            printf("El Alias ya existe, por favor elija otro:\n");
            leerCadena("Elija un nuevo alias: ", aliasNuevo);
        }
        strcpy(matrizUser[posicion][INDICE_ALIAS], aliasNuevo);
        printf("Se cambio el alias con exito\n");
    }
    else
    {
        printf("Usuario no encontrado\n");
    }
}

void cambiarClaveDeUsuario(int vecClave[], cadena matrizUser[][DATOS_USERS], int *cantUsuarios, cadena aliasBuscado, int posicionEnColumna)
{
    int contadorIntentos = 0;

    int posicion = buscarUsuarios(matrizUser, cantUsuarios, aliasBuscado, posicionEnColumna);

    if (posicion != -1)
    {
        int claveActual = verificacionClave(&contadorIntentos, vecClave, posicion, "Ingresar clave de seguridad: ", "\nClave Incorrecta, intentalo de nuevo\n");

        if (contadorIntentos < 3 && vecClave[posicion] == claveActual)
        {
            claveActual = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "\nIngresar nueva clave: \n");

            while (vecClave[posicion] == claveActual)
            {
                printf("La clave no puede ser igual a la actual\n");
                claveActual = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "Ingresar nueva clave: ");
            }

            vecClave[posicion] = claveActual;
            printf("Se cambio la clave con exito\n");
        }
        else
        {
            printf("\nYA SUPERASTE LA CANTIDAD MAXIMA DE INTENTOS\n");
        }
    }
    else
    {
        printf("Usuario no encontrado.\n");
    }
}
int verificacionClave(int *contador, int vecClave[], int posicion, cadena msj1, cadena msj2)
{
    int claveActual = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, msj1);

    while (claveActual != vecClave[posicion] && *contador < 3)
    {
        printf("%s", msj2);
        // printf("\nClave Incorrecta, intentalo de nuevo\n");
        // claveActual = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "Ingresa clave de seguridad: ");
        claveActual = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, msj1);
        (*contador)++;
    }
    return claveActual;
}

void bajaUsuario(cadena matrizUser[][DATOS_USERS], int vecClave[], int *cantUsuarios, cadena aliasBuscado)
{
    int opc;
    int posUsuario = buscarUsuarios(matrizUser, cantUsuarios, aliasBuscado, INDICE_ALIAS);

    if (posUsuario != -1)
    {
        int contadorIntentos = 0;
        int ingresarClave = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "\nIngrese la clave:\n");

        while (ingresarClave != vecClave[posUsuario] && contadorIntentos < 3)
        {
            printf("\nClave incorrecta, ingrese nuevamente: ");
            ingresarClave = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "\nIngrese la clave:\n");
            contadorIntentos++;
        }

        if (ingresarClave == vecClave[posUsuario])
        {

            printf("\nUsuario encontrado:\n Nombre y apellido %s %s\nAlias: %s\n\n", matrizUser[posUsuario][INDICE_NOMBRE], matrizUser[posUsuario][INDICE_APELLIDO], matrizUser[posUsuario][INDICE_ALIAS]);
            opc = leerEnteroEntre(1, 2, "Estas seguro de eliminar este usuario. 1: SI. 2: NO\nOpc: ");
            if (opc == 1)
            {

                for (int i = posUsuario; i < (*cantUsuarios - 1); i++)
                {
                    strcpy(matrizUser[i][INDICE_NOMBRE], matrizUser[i + 1][INDICE_NOMBRE]);
                    strcpy(matrizUser[i][INDICE_APELLIDO], matrizUser[i + 1][INDICE_APELLIDO]);
                    strcpy(matrizUser[i][INDICE_ALIAS], matrizUser[i + 1][INDICE_ALIAS]);
                    vecClave[i] = vecClave[i + 1];
                }

                (*cantUsuarios)--;

                printf("Usuario eliminado con exito");
            }
            else
            {
                printf("Operacion Cancelada");
            }
        }
        else
        {
            printf("CANTIDAD MAXIMA SUPERADA");
        }
    }
    else
    {
        printf("Usuario no encontrado.\n");
    }
}

int buscarUsuarios(cadena matrizUser[][DATOS_USERS], int *cantUsuarios, cadena aliasBuscado, int posicionColumna)
{
    int posicion = -1,
        i = 0;
    bool encontrado = false;

    while (i < *cantUsuarios && encontrado == false)
    {
        if (strcmp(matrizUser[i][posicionColumna], aliasBuscado) == 0)
        {
            encontrado = true;
            posicion = i;
        }
        else
        {
            i++;
        }
    }

    return posicion;
}
// --------------------CONSULTAR   CASO 2

void listaUsuarios(cadena matrizUser[][DATOS_USERS], int *cantUsuarios)
{
    if (*cantUsuarios > VACIO)
    {
        printf("\nLista de usuarios:\n");
        mostrarDatosDeMatriz(DATOS_USERS, matrizUser, cantUsuarios);
    }
    else
    {
        printf("---- No hay usuarios registrados ----\n");
    }
    system("pause");
}
void mostrarDatosDeMatriz(int COLUMNAS, cadena matriz[][COLUMNAS], int *cant)
{

    for (int i = 0; i < *cant; i++)
    {
        printf("%d. Cliente:%s %s, Alias: %s\n", i + 1, matriz[i][INDICE_NOMBRE], matriz[i][INDICE_APELLIDO], matriz[i][INDICE_ALIAS]);
    }
}
// --------------------ALTA PERSONAL CASO 1
void altaPersonal(cadena matrizUser[][DATOS_USERS], int vecClaves[], int *cantUsuarios)
{
    cadena nombre, apellido, alias;
    int claveNueva;

    if (*cantUsuarios < CANT_USERS)
    {

        printf("========== ALTA DE PERSONAL ==========\n");
        nuevoUsuario(nombre, apellido, alias, matrizUser, *cantUsuarios);
        claveNueva = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "Ingresar clave nueva: ");
        agregarUsuario(matrizUser, vecClaves, cantUsuarios, nombre, apellido, alias, claveNueva);

        printf("\nUsuario registrado correctamente.");
        printf("\nNombre y apellido: %s %s", nombre, apellido);
        printf("\nAlias: %s\n", alias);
        system("pause");
    }
    else
    {
        printf("No se puede registrar el usuario.\n");
        printf("No hay  mas espacio\n");
        printf("Se alcanzo el limite maximo.\n");
        system("pause");
    }
}

void agregarUsuario(cadena matrizUser[][DATOS_USERS], int vecClaves[], int *cantUsuarios, cadena nombre, cadena apellido, cadena alias, int clave)
{
    // strcpy(en donde quiero, que cosa quiero guardar);

    strcpy(matrizUser[*cantUsuarios][INDICE_NOMBRE], nombre);
    strcpy(matrizUser[*cantUsuarios][INDICE_APELLIDO], apellido);
    strcpy(matrizUser[*cantUsuarios][INDICE_ALIAS], alias);
    vecClaves[*cantUsuarios] = clave;
    (*cantUsuarios)++;
}

void nuevoUsuario(cadena nombre, cadena apellido, cadena alias, cadena matrizUser[][DATOS_USERS], int cantUsuarios)
{
    int posicionBuscada;

    leerCadena("Ingresar nombre: ", nombre);
    leerCadena("Ingresar apellido: ", apellido);

    do
    {
        // leerCadena("Ingresar DNI: ", DNI); //buscar una solucion
        leerCadena("Ingresar alias: ", alias);
        posicionBuscada = buscarUsuarios(matrizUser, &cantUsuarios, alias, INDICE_ALIAS);
        if (posicionBuscada != -1)
        {
            printf("El usuario ya existe elija otro.\n");
        }
    } while (posicionBuscada != -1);
}

// modificar
void enEspera()
{
    printf("Queda pendiente");
}

void vistaGestionPersonal()
{
    // Vista de usuario
    printf("GESTION PERSONAL\n\n");
    printf("1. Alta de nuevo usuario\n"); // C
    printf("2. Ver lista de usuarios\n"); // R
    printf("3. Modificar usuario\n");     // U
    printf("4. Baja de usuario\n");       // D
    printf("5. Volver\n\n");
}

void menuVistaAdmin()
{
    // Vista de usuario
    printf("VISTA ADMINISTRADOR\n\n");
    printf("1. Gestionar personal\n");
    printf("2. Gestionar agentes\n");
    printf("3. Consultar clientes\n");
    printf("4. Consultar balance\n");
    printf("5. Cerrar sesion\n\n");
}

//--------------------------------------------Vista cliente
// void menuVistaCliente()
// {
//     // Vista de usuario
//     printf("HOME BANKING\n\n");
//     printf("1. Consultar Saldo\n");
//     printf("2. Ralizar deposito\n");
//     printf("3. Realizar retiro\n");
//     printf("4. Transferir\n"); // Vamos viendo
//     printf("5. Solicitar prestamos\n");
//     printf("6. Ver movimientos\n"); // vamos viendo
//     printf("7. CBU\n");             // no olvidarse de agregar Alias
//     printf("8. Ver beneficios\n");
// }
// void menuOpcCliente(int *saldo)
// {

//     switch (leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: "))
//     {
//     case 1:
//         consultarSaldo(saldo);
//         break;
//     case 2:

//         break;
//     case 3:

//         break;
//     case 4:

//         break;
//     case 5:

//         break;
//     case 6:

//         break;
//     case 7:

//         break;
//     case 8:

//         break;
//     default:
//         printf("La opcion ingresada es invalida. Volve a intentar.");
//         break;
//     }
// }

// void consultarSaldo(int *saldoConsultado)
// {
//     printf("El saldo actual es %d", *saldoConsultado);
// }
