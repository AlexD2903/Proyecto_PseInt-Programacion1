#include "utils_vAmpliada.h"
#include <windows.h>

#define OPC_MIN 1
#define OPC_MAX 5
#define CANT_USERS 4
#define VACIO 0
#define OPCION_SALIDA 0
#define MIN_CLAVE 1000
#define MAX_CLAVE 9999

/////////////////// FUNCIONES DEL CLIENTE ///////////////////

void menuVistaCliente();
void menuOpcCliente(int *saldo);
void consultarSaldo(int *saldoConsultado);

/////////////////// FUNCIONES DEL ADMINISTRADOR ///////////////////

void menuVistaAdmin();
void menuOpcAdmin(char vecUser[], int vecClaves[], int *cantUsuarios);
void menuGestionarPersonal2(char vecUser[], int vecClaves[], int *cantUsuarios);
void vistaGestionPersonal();
void enEspera();

/////////////////// ALTA PERSONAL ///////////////////
void altaPersonal(char vecUser[], int vecClaves[], int *cantUsuarios);
void agregarUsuario(char vecUser[], int vecClaves[], int *cantUsuarios, char usuario, int clave);
char nuevoUsuario(char vecUser[], int cantUsuarios);
char validarUsuario();
int validarPass();

/////////////////// CONSULTAR O MODIFICAR ///////////////////
void listaUsuarios(char vecUser[], int *cantUsuarios);
int buscarUsuarios(char vecUser[], int *cantUsuarios);

void modificarUsuario(char vecUser[], int vecClave[], int *cantUsuarios);

void cambiarNombreDeUsuario(char vecUser[], int posicion, int *cant);
void cambiarClaveDeUsuario(int vecClave[], int posicion);

int main()
{

    char listaUser[CANT_USERS] = {0};
    int vecClaves[CANT_USERS] = {0};
    int cantUsuarios = 0;

    menuOpcAdmin(listaUser, vecClaves, &cantUsuarios);

    return 0;
}

void menuOpcAdmin(char vecUser[], int vecClaves[], int *cantUsuarios)
{
    int opcion = 0;

    menuVistaAdmin();
    opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");

    while (opcion != OPC_MAX)
    {
        switch (opcion)
        {
        case 1:
            menuGestionarPersonal2(vecUser, vecClaves, cantUsuarios);
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
        Sleep(3000);
        system("cls");
        menuVistaAdmin();
        opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    }
}

void menuGestionarPersonal2(char vecUser[], int vecClaves[], int *cantUsuarios)
{
    int opcion = 0;

    vistaGestionPersonal();
    opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    while (opcion != OPC_MAX)
    {
        switch (opcion)
        {
        case 1: // C (Create / Crear)
            altaPersonal(vecUser, vecClaves, cantUsuarios);
            break;
        case 2: // R (Read / Leer o Consultar)
            listaUsuarios(vecUser, cantUsuarios);
            break;
        case 3: // U (Update / Actulizar o Modificar)
            modificarUsuario(vecUser, vecClaves, cantUsuarios);
            break;
        case 4: // D (Delete / Borrar o Eliminar)
            break;
        }
        Sleep(3000);
        system("cls");
        vistaGestionPersonal();
        opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    }
}
// CASO 3

void modificarUsuario(char vecUser[], int vecClave[], int *cantUsuarios)
{
    int posUsuario = buscarUsuarios(vecUser, cantUsuarios);
    int opc = 0;

    printf("\nUsuario '%c' encontrado.\n\n", vecUser[posUsuario]);
    printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre de Usuario\n2. Clave\n0. Volver\n");

    opc = leerEnteroEntre(0, 2, "Elija una opcion: ");
    while (opc != OPCION_SALIDA)
    {
        switch (opc)
        {
        case 1: // modifica el nombre de usuario
            cambiarNombreDeUsuario(vecUser, posUsuario, cantUsuarios);
            break;
        case 2: // modifica el la clave
            cambiarClaveDeUsuario(vecClave, posUsuario);
            break;
        }

        printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre de Usuario\n2. Clave\n0. Volver\n");
        opc = leerEnteroEntre(0, 2, "Elija una opcion: ");
    }
}

void cambiarNombreDeUsuario(char vecUser[], int posicion, int *cant)
{
    char nombreViejo = vecUser[posicion];
    char nombreNuevo = leerCaracter("Elija nuevo nombre de usuario:\n");

    while (siExisteEnVector(nombreNuevo, vecUser, cant) || (nombreViejo == nombreNuevo))
    {
        printf("Nombre ya existe, por favor elija otro:\n");
        nombreNuevo = leerCaracter("Elija nuevo nombre de usuario:\n");
    }
    vecUser[posicion] = nombreNuevo;
}

void cambiarClaveDeUsuario(int vecClave[], int posicion)
{
    char claveVieja = vecClave[posicion];
    char claveNueva = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "Elija nuevo nombre de usuario:\n");

    while (claveVieja == claveNueva)
    {
        printf("Nombre ya existe, por favor elija otro:\n");
        claveNueva = leerCaracter("Elija nuevo nombre de usuario:\n");
    }
    vecClave[posicion] = claveNueva;
}

int buscarUsuarios(char vecUser[], int *cantUsuarios)
{
    char usuario = leerCaracter("Ingresar usuario: ");
    int posicion;
    if (siExisteEnVector(usuario, vecUser, cantUsuarios))
    {
        posicion = posicionDeBuscadoEnVector(usuario, vecUser);
    }
    else
    {
        printf("No se encontro el usuario");
    }
    return posicion;
}
// --------------------CONSULTAR   CASO 2

void listaUsuarios(char vecUser[], int *cantUsuarios)
{

    if (*cantUsuarios > VACIO)
    {
        printf("\nLista de usuarios:\n");
        mostrarVectorDeCaracteres(vecUser, *cantUsuarios);
    }
    else
    {
        printf("---- No hay usuarios registrados ----\n");
    }
    system("pause");
}

// --------------------ALTA PERSONAL CASO 1
void altaPersonal(char vecUser[], int vecClaves[], int *cantUsuarios)
{
    char usuNuevo;
    int claveNueva;

    if (*cantUsuarios < CANT_USERS)
    {

        printf("========== ALTA DE PERSONAL ==========\n");
        usuNuevo = nuevoUsuario(vecUser, *cantUsuarios);
        claveNueva = validarPass();
        agregarUsuario(vecUser, vecClaves, cantUsuarios, usuNuevo, claveNueva);

        printf("\nUsuario registrado correctamente.");
        printf("\nUsuario: %c", usuNuevo);
        printf("\nClave: %d\n", claveNueva);
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

void agregarUsuario(char vecUser[], int vecClaves[], int *cantUsuarios, char usuario, int clave)
{
    vecUser[*cantUsuarios] = usuario;
    vecClaves[*cantUsuarios] = clave;
    (*cantUsuarios)++;
}

char nuevoUsuario(char vecUser[], int cantUsuarios)
{
    char usuNuevo;
    bool usuExistente = false;

    do
    {
        usuNuevo = validarUsuario();
        usuExistente = siExisteEnVector(usuNuevo, vecUser, &cantUsuarios);
        if (usuExistente)
        {
            printf("El usuario ya existe elija otro.");
        }
    } while (usuExistente);

    return usuNuevo;
}

int validarPass()
{
    return leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "Ingresar clave nueva: ");
}
// modificar
char validarUsuario()
{
    return leerCaracter("Ingresar Usu: ");
}
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
void menuVistaCliente()
{
    // Vista de usuario
    printf("HOME BANKING\n\n");
    printf("1. Consultar Saldo\n");
    printf("2. Ralizar deposito\n");
    printf("3. Realizar retiro\n");
    printf("4. Transferir\n"); // Vamos viendo
    printf("5. Solicitar prestamos\n");
    printf("6. Ver movimientos\n"); // vamos viendo
    printf("7. CBU\n");             // no olvidarse de agregar Alias
    printf("8. Ver beneficios\n");
}
void menuOpcCliente(int *saldo)
{

    switch (leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: "))
    {
    case 1:
        consultarSaldo(saldo);
        break;
    case 2:

        break;
    case 3:

        break;
    case 4:

        break;
    case 5:

        break;
    case 6:

        break;
    case 7:

        break;
    case 8:

        break;
    default:
        printf("La opcion ingresada es invalida. Volve a intentar.");
        break;
    }
}

void consultarSaldo(int *saldoConsultado)
{
    printf("El saldo actual es %d", *saldoConsultado);
}
