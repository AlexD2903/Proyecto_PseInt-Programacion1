#include "utils.h"
#include <windows.h>

#define OPC_MIN 1
#define OPC_MAX 5
#define CANT_USERS 6
#define DATOS_USERS 3 // nombre, apellido, alias
#define VACIO 0
#define OPCION_SALIDA 0
#define MIN_CLAVE 1000
#define MAX_CLAVE 9999
#define INDICE_NOMBRE 0
#define INDICE_APELLIDO 1
#define INDICE_ALIAS 2
#define USUARIO_INEXISTENTE -1

typedef struct
{
    cadena nombre,
        apellido,
        DNI,
        clave,
        alias;
    float saldo;
    bool esPyme;
} Cliente;

Cliente listUser[CANT_USERS] = {
    {"Alex", "Ramos", "12121212", "Alexkpo3", "alex.r", 340.4, false},
    {"Santiago", "Gutierrez", "13131313", "Santi2", "santi.g", 10000.0, false},
    {"Juan", "Britez", "14141414", "Juan89", "juans.mp", 4303.56, true},
    {"Esteban", "Quinteros", "37541469", "123123", "esteban.q", 103204.43, false}};
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
void altaPersonal(Cliente baseDatos[CANT_USERS], int *cantUsuarios);
void agregarUsuario(Cliente new, Cliente baseDatos[CANT_USERS], int* cantUsuarios);
void msjDeConfirmacion(cadena nombreDeDato, int dato);

// Con struc
Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios);
void verificarYRegistrar(cadena texto, cadena pCampo, Cliente baseDatos[CANT_USERS], int cantUsuarios);
int busquedaLineal(Cliente datos[],int cantidad, cadena valor);
/////////////////// CONSULTAR O MODIFICAR ///////////////////
void listaUsuarios(cadena matrizUser[][DATOS_USERS], int *cantUsuarios);
void mostrarDatosDeMatriz(int COLUMNAS, cadena matriz[][COLUMNAS], int *cant);
int buscarUsuarios(cadena matrizUser[][DATOS_USERS], int *cantUsuarios);

void modificarUsuario(cadena matrizUser[][DATOS_USERS], int vecClave[], int *cantUsuarios);

void cambiarAliasDeUsuario(cadena matrizUser[][DATOS_USERS], int posicion, int *cant);
void cambiarClaveDeUsuario(int vecClave[], int posicion);
int verificacionClave(int *contador, int vecClave[], int posicion, cadena msj1, cadena msj2);
void bajaUsuario(cadena matrizUser[][DATOS_USERS], int vecClave[], int *cantUsuarios);


//deprueba
bool verificarDNI(cadena dni);



// --------------MAIN
int main()
{
    // Cliente clientes[CANT_USERS];
    // cadena listaUser[CANT_USERS][DATOS_USERS];
    // int vecClaves[CANT_USERS] = {0};
    int cantUsuarios = 4;
    altaPersonal(listUser, &cantUsuarios);

    // menuGestionarPersonal(listaUser, vecClaves, &cantUsuarios);

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
        Sleep(3000);
        system("cls");
        menuVistaAdmin();
        opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    }
}

void menuGestionarPersonal(cadena matrizUser[][DATOS_USERS], int vecClaves[], int *cantUsuarios)
{
    int opcion = 0;

    vistaGestionPersonal();
    opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    while (opcion != OPC_MAX)
    {
        switch (opcion)
        {
        case 1: // C (Create / Crear)
            //altaPersonal();
            break;
        case 2: // R (Read / Leer o Consultar)
            listaUsuarios(matrizUser, cantUsuarios);
            break;
        case 3: // U (Update / Actulizar o Modificar)
            modificarUsuario(matrizUser, vecClaves, cantUsuarios);
            break;
        case 4: // D (Delete / Borrar o Eliminar)
            bajaUsuario(matrizUser, vecClaves, cantUsuarios);
            break;
        }
        Sleep(3000);
        system("cls");
        vistaGestionPersonal();
        opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    }
}

void modificarUsuario(cadena matrizUser[][DATOS_USERS], int vecClave[], int *cantUsuarios)
{
    int opc = 0;
    int posUsuario = buscarUsuarios(matrizUser, cantUsuarios);

    if (posUsuario != -1)
    {
        printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre de Usuario\n2. Clave\n0. Volver\n");

        opc = leerEnteroEntre(0, 2, "Elija una opcion: ");
        while (opc != OPCION_SALIDA)
        {
            switch (opc)
            {
            case 1:
                cambiarAliasDeUsuario(matrizUser, posUsuario, cantUsuarios);
                break;
            case 2:
                cambiarClaveDeUsuario(vecClave, posUsuario);
                break;
            }

            printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre de Usuario\n2. Clave\n0. Volver\n");
            opc = leerEnteroEntre(0, 2, "Elija una opcion: ");
        }
    }
    else
    {
        printf("\nNo existe ese usuario\n");
    }
}

void cambiarAliasDeUsuario(cadena matrizUser[][DATOS_USERS], int posicion, int *cant)
{
    cadena aliasNuevo;
    leerCadena("Elija un nuevo alias: ", aliasNuevo);
    while (strcmp(aliasNuevo, matrizUser[posicion][INDICE_ALIAS]) == 0 || siExisteCadenaEnMatriz(aliasNuevo, INDICE_ALIAS, matrizUser, cant))
    {
        printf("El Alias ya existe, por favor elija otro:\n");
        leerCadena("Elija un nuevo alias: ", aliasNuevo);
    }
    strcpy(matrizUser[posicion][INDICE_ALIAS], aliasNuevo);
}

void cambiarClaveDeUsuario(int vecClave[], int posicion)
{
    int contadorIntentos = 0;

    int claveActual = verificacionClave(&contadorIntentos, vecClave, posicion, "Ingresar clave de seguridad: ", "\nClave Incorrecta, intentalo de nuevo\n");

    if (contadorIntentos < 3 && vecClave[posicion] == claveActual)
    {
        claveActual = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "\nIngresar nueva clave: \n");

        while (vecClave[posicion] == claveActual)
        {
            printf("La clave no puede ser igual a la actual\n");
            claveActual = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "Ingresar nueva clave: ");
        }
    }
    else
    {
        printf("\nYA SUPERASTE LA CANTIDAD MAXIMA DE INTENTOS\n");
    }

    vecClave[posicion] = claveActual;
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

void bajaUsuario(cadena matrizUser[][DATOS_USERS], int vecClave[], int *cantUsuarios)
{
    int opc;
    int posUsuario = buscarUsuarios(matrizUser, cantUsuarios);

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
}

int buscarUsuarios(cadena matrizUser[][DATOS_USERS], int *cantUsuarios)
{
    cadena aliasBuscado;
    int posicion = -1;

    leerCadena("Ingresar alias: ", aliasBuscado);
    for (int i = 0; i < *cantUsuarios; i++)
    {
        if (strcmp(aliasBuscado, matrizUser[i][INDICE_ALIAS]) == 0)
        {
            printf("Alias encontrado. \n");
            posicion = i;
        }
    }
    if (posicion == -1)
    {
        printf("\nNO EXISTE ALIAS.\n");
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
void altaPersonal(Cliente baseDatos[CANT_USERS], int *cantUsuarios)
{
    Cliente newCliente;

    if (*cantUsuarios < CANT_USERS)
    {

        printf("========== ALTA DE PERSONAL ==========\n");
        newCliente = nuevoUsuario(baseDatos, *cantUsuarios);
        agregarUsuario(newCliente, baseDatos, cantUsuarios);

        printf("\nUsuario registrado correctamente.");
        printf("\nNombre y apellido: %s %s", newCliente.nombre, newCliente.apellido);
        printf("\nAlias: %s", newCliente.alias);
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

void agregarUsuario(Cliente new, Cliente baseDatos[CANT_USERS], int* cantUsuarios)
{
    strcpy(baseDatos[*cantUsuarios].nombre, new.nombre);
    strcpy(baseDatos[*cantUsuarios].apellido, new.apellido);
    strcpy(baseDatos[*cantUsuarios].DNI, new.DNI);
    strcpy(baseDatos[*cantUsuarios].alias, new.alias);
    strcpy(baseDatos[*cantUsuarios].clave, new.clave);
    (*cantUsuarios)++;
}

Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios)
{
    Cliente nuevo;
    //cadena alias, dni;

    leerCadena("Ingresar nombre: ", nuevo.nombre);
    leerCadena("Ingresar apellido: ", nuevo.apellido);
    
    verificarYRegistrar("alias: ",nuevo.alias, baseDatos, cantUsuarios);
    verificarYRegistrar("dni: ",nuevo.DNI, baseDatos, cantUsuarios);
    leerCadena("Ingrese clave: ", nuevo.clave); 
    
    return nuevo;
}

//PRUEBA DE COPIA DE VECTOR
//
// void copiaDeVector(Cliente datos[], int cantidad, cadena vectorAux[], campo){
//     for(int i= 0; i < cantidad ; i++){
//         strcpy(vectorAux[i], datos[i].campo);
//     }
// }

void verificarYRegistrar(cadena texto, cadena columna, Cliente baseDatos[CANT_USERS], int cantUsuarios){
    int existe;
    // cadena vectorAux[CANT_USERS];
    do{
        printf("Ingresar ");
        leerCadena(texto, columna);
        strcpy()
        existe = busquedaLineal(baseDatos, cantUsuarios, columna);
        msjDeConfirmacion(texto, existe);
    
    }while(existe != USUARIO_INEXISTENTE);
    
}
int busquedaLineal(Cliente datos[],int cantidad, cadena valor)
{
    int pos = cantidad -1;
    while(pos >= 0 && strcmp(datos[pos].DNI, valor) != 0 && strcmp(datos[pos].alias, valor) != 0)
    {
        pos--;
    }
    return pos;
}

bool verificarDNI(cadena dni)
{
    bool valido = true;
    int posicion= 0;

    if((strlen(dni) != 8) || (valido && (dni[0]== '0' && dni[1]== '0'))){
    valido= false;
    }
    while(valido && dni[posicion] != '\0'){
        if(dni[posicion] < '0' || dni[posicion] > '9'){
            valido = false;
        }
        posicion++;
    }
    return valido;
}

void msjDeConfirmacion(cadena nombreDeDato, int dato)
{
    if (dato != USUARIO_INEXISTENTE)
    {
        printf("El %s ya existe. Elija otro.\n", nombreDeDato);
    }
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


//opcion a
// #define BUSCAR_ALIAS 0
// #define BUSCAR_DNI   1

// int busquedaLineal(Cliente datos[], int cantidad, cadena valor, int campo)
// {
//     int pos = cantidad - 1;

//     if (campo == BUSCAR_ALIAS)
//     {
//         while (pos >= 0 && strcmp(datos[pos].alias, valor) != 0)
//         {
//             pos--;
//         }
//     }
//     else if (campo == BUSCAR_DNI)
//     {
//         while (pos >= 0 && strcmp(datos[pos].DNI, valor) != 0)
//         {
//             pos--;
//         }
//     }

//     return pos;
// }

