#include "utils_vAmpliada.h"
#include <windows.h>

#define OPC_MIN 1
#define OPC_MAX 5
#define CANT_USERS 2
#define DATOS_USERS 3 //nombre, apellido, alias 
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
void menuOpcAdmin(cadena matrizUser[][], int vecClaves[], int *cantUsuarios);
void menuGestionarPersonal(cadena matrizUser[][], int vecClaves[], int *cantUsuarios);
void vistaGestionPersonal();
void enEspera();

/////////////////// ALTA PERSONAL ///////////////////
void altaPersonal(cadena matrizUser[][], int vecClaves[], int *cantUsuarios);
void agregarUsuario(cadena matrizUser[][], int vecClaves[], int *cantUsuarios, char usuario, int clave);
char nuevoUsuario(cadena matrizUser[][], int cantUsuarios);
int validarPass();

/////////////////// CONSULTAR O MODIFICAR ///////////////////
void listaUsuarios(cadena matrizUser[][], int *cantUsuarios);
int buscarUsuarios(cadena matrizUser[][], int *cantUsuarios);

void modificarUsuario(cadena matrizUser[][], int vecClave[], int *cantUsuarios);

void cambiarNombreDeUsuario(cadena matrizUser[][], int posicion, int *cant);
void cambiarClaveDeUsuario(int vecClave[], int posicion);


// --------------MAIN
int main()
{

    char listaUser[CANT_USERS][DATOS_USERS];
    int vecClaves[CANT_USERS] = {0};
    int cantUsuarios = 0;

    menuOpcAdmin(listaUser, vecClaves, &cantUsuarios);

    return 0;
}

void menuOpcAdmin(cadena matrizUser[][], int vecClaves[], int *cantUsuarios)
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
        Sleep(3000);
        system("cls");
        menuVistaAdmin();
        opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    }
}

void menuGestionarPersonal(cadena matrizUser[][], int vecClaves[], int *cantUsuarios)
{
    int opcion = 0;

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
            //listaUsuariosmatrizcUser, cantUsuarios);
            break;
        case 3: // U (Update / Actulizar o Modificar)
            //modificarUsuario(matrizUser, vecClaves, cantUsuarios);
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

void modificarUsuario(cadena matrizUser[][], int vecClave[], int *cantUsuarios)
{
    int posUsuario = buscarUsuarios(matrizUser, cantUsuarios);
    int opc = 0;

    printf("\nUsuario '%c' encontrado.\n\n", matrizUser[posUsuario]);
    printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre de Usuario\n2. Clave\n0. Volver\n");

    opc = leerEnteroEntre(0, 2, "Elija una opcion: ");
    while (opc != OPCION_SALIDA)
    {
        switch (opc)
        {
        case 1: // modifica el nombre de usuario
            cambiarNombreDeUsuario(matrizUser, posUsuario, cantUsuarios);
            break;
        case 2: // modifica el la clave
            cambiarClaveDeUsuario(vecClave, posUsuario);
            break;
        }

        printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre de Usuario\n2. Clave\n0. Volver\n");
        opc = leerEnteroEntre(0, 2, "Elija una opcion: ");
    }
}

void cambiarNombreDeUsuario(cadena matrizUser[][], int posicion, int *cant)
{
    char nombreViejo = matrizUser[posicion];
    char nombreNuevo = leerCaracter("Elija nuevo nombre de usuario:\n");

    while (siExisteEnVector(nombreNuevo, matrizUser, cant) || (nombreViejo == nombreNuevo))
    {
        printf("Nombre ya existe, por favor elija otro:\n");
        nombreNuevo = leerCaracter("Elija nuevo nombre de usuario:\n");
    }
    matrizUser[posicion] = nombreNuevo;
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

int buscarUsuarios(cadena matrizUser[][], int *cantUsuarios)
{
    char usuario = leerCaracter("Ingresar usuario: ");
    int posicion;
    if (siExisteEnVector(usuario, matrizUser, cantUsuarios))
    {
        posicion = posicionDeBuscadoEnVector(usuario, matrizUser);
    }
    else
    {
        printf("No se encontro el usuario");
    }
    return posicion;
}
// --------------------CONSULTAR   CASO 2

void listaUsuarios(cadena matrizUser[][], int *cantUsuarios)
{

    if (*cantUsuarios > VACIO)
    {
        printf("\nLista de usuarios:\n");
        mostrarVectorDeCaracteres(matrizUser, *cantUsuarios);
    }
    else
    {
        printf("---- No hay usuarios registrados ----\n");
    }
    system("pause");
}

// --------------------ALTA PERSONAL CASO 1
void altaPersonal(cadena matrizUser[][], int vecClaves[], int *cantUsuarios)
{
    char usuNuevo;
    int claveNueva;

    if (*cantUsuarios < CANT_USERS)
    {

        printf("========== ALTA DE PERSONAL ==========\n");
        usuNuevo = nuevoUsuario(matrizUser, *cantUsuarios);
        claveNueva = validarPass();
        agregarUsuario(matrizUser, vecClaves, cantUsuarios, usuNuevo, claveNueva);

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

void agregarUsuario(cadena matrizUser[][], int vecClaves[], int *cantUsuarios, char usuario, int clave)
{

    // meter el strcpy para copiar el string cadena a la posicion de la matriz

    //strcpy(en donde quiero, que cosa quiero guardar);
    matrizUser[*cantUsuarios] = usuario;
    vecClaves[*cantUsuarios] = clave;
    (*cantUsuarios)++;
}

void nuevoUsuario(cadena matrizUser[][], int cantUsuarios)
{
    cadena nombre, apellido, alias; //cadena = char nombreDeLaVariable[CANTIDAD]
    bool usuExistente = false;

    leerCadena("Ingresar nombre: ", nombre);
    leerCadena("Ingresar apellido: ", apellido);

    do
    {
        //leerCadena("Ingresar DNI: ", DNI); //buscar una solucion
        leerCadena("Ingresar alias: ", alias);
        usuExistente = siExisteEnVector(alias, matrizUser, &cantUsuarios);
        if (usuExistente)
        {
            printf("El usuario ya existe elija otro.");
        }
    } while (usuExistente);

}

int validarPass()
{
    return leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "Ingresar clave nueva: ");
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
