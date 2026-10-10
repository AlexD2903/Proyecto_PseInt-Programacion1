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
#define ADMINISTRADOR 1
#define CLIENTE 2
#define OPC_DNI 1
#define OPC_ALIAS 2
#define OPC_NOMBRE 3
#define OPC_APELLIDO 4
#define INTENTOS_MAX 3
typedef struct
{
    cadena nombre,
        apellido,
        DNI,
        clave,
        alias;
    float saldo;
    int rol;
    bool Pyme;
} Cliente;

Cliente listUser[CANT_USERS] = {
    {"Alex", "Ramos", "12121212", "Alexkpo3", "alex.r", 340.4, ADMINISTRADOR, false},
    {"Santiago", "Gutierrez", "13131313", "Santi2", "santi.g", 10000.0, CLIENTE, false},
    {"Juan", "Britez", "34554543", "Juams89", "juams89.mp", 4303.56, CLIENTE, true},
    {"Esteban", "Quinteros", "37541469", "123123", "esteban.q", 103204.43, CLIENTE, false}};

int buscarDato(Cliente datos[], int cantidad, cadena valor, int tipodDato);
int busquedaLinealUsuario(Cliente datos[], int cantidad, cadena valor);
bool loginUsuarios(Cliente datos[], int cantidad, int *posUsuario);
void cabecera();
int verficarUsuario(Cliente datos[], int cantidad);
bool verificacionClave(Cliente datos[], int posicion, cadena msjPedido, cadena msjError);
bool esPyme();
void inicioSesion(Cliente datos[], int posUsuario, bool accesoConcedido);
void menuVistaAdmin(Cliente datos[], int posUsuario);
void menuVistaCliente(Cliente datos[], int posUsuario);
void menuVistaClientePYME(Cliente datos[], int posUsuario);
bool esMenor(int num, int num1);
bool esIgual(int num, int num1);
bool esClaveCorrecta(cadena claveIngresada, cadena claveGuardada);
void convertirCadenaAMayuscula(cadena frase);
int busquedaLinealDNI(Cliente datos[], int cantidad, cadena valorBuscado);
int busquedaLinealALIAS(Cliente datos[], int cantidad, cadena valorBuscado);

int main()
{
    int posUsuario;
    // int cantUsuarios = 4;
    bool accesoConcedido = loginUsuarios(listUser, CANT_USERS, &posUsuario);
    inicioSesion(listUser, posUsuario, accesoConcedido);

    return 0;
}
int buscarDato(Cliente datos[], int cantidad, cadena valorBuscado, int tipodDato)
{
    int pos = USUARIO_INEXISTENTE;
    switch (tipodDato)
    {
    case OPC_DNI:
        pos = busquedaLinealDNI(datos, cantidad, valorBuscado);
        break;
    case OPC_ALIAS:
        pos = busquedaLinealALIAS(datos, cantidad, valorBuscado);
        break;
    }
    return pos;
}
int busquedaLinealUsuario(Cliente datos[], int cantidad, cadena valor)
{
    /*Usamos buscar datos para que busque mas limpio en el login sin tener que hacer la seleccion
    del tipo de dato*/
    int posicion;
    posicion = buscarDato(datos, cantidad, valor, OPC_DNI);
    if (posicion == USUARIO_INEXISTENTE)
    {
        posicion = buscarDato(datos, cantidad, valor, OPC_ALIAS);
    }

    return posicion;
}
bool loginUsuarios(Cliente datos[], int cantidad, int *posUsuario)
{

    bool accesoConcedido = false;
    cabecera();
    *posUsuario = verficarUsuario(datos, cantidad);
    if (!esIgual(*posUsuario,USUARIO_INEXISTENTE))
    {
        accesoConcedido = verificacionClave(datos, *posUsuario, "Ingrese contrasenia: ", "ERROR: Contrasenia incorrecta, intentalo de nuevo");
    }
    else
    {
        printf("ACCESO DENEGADO.\n");
    }
    
    return accesoConcedido;
}
bool verificacionClave(Cliente datos[], int posicion, cadena msjPedido, cadena msjError)
{
    cadena claveActual;
    int contador = 1;
    leerCadena(msjPedido, claveActual);
    // comparo como cadena claveIngresada y la clave guardada
    while (!esClaveCorrecta(claveActual, datos[posicion].clave) && esMenor(contador, INTENTOS_MAX))
    {
        printf("%s\n", msjError);
        // printf("\nClave Incorrecta, intentalo de nuevo\n");
        // claveActual = leerEnteroEntre(MIN_CLAVE, MAX_CLAVE, "Ingresa clave de seguridad: ");
        leerCadena(msjPedido, claveActual);
        contador++;
        if (esIgual(contador, INTENTOS_MAX))
        {
            puts("No es posible realizar mas intentos. Intente mas tarde");
        }
    }
    return esClaveCorrecta(claveActual, datos[posicion].clave);
}
void inicioSesion(Cliente datos[], int posUsuario, bool accesoConcedido)
{
    if (accesoConcedido)
    {
        switch (datos[posUsuario].rol)
        {
        case ADMINISTRADOR:
            menuVistaAdmin(datos, posUsuario);
            break;
        case CLIENTE:
            if (datos[posUsuario].Pyme == false)
            {
                menuVistaCliente(datos, posUsuario);
            }
            else
            {
                menuVistaClientePYME(datos, posUsuario);
            }

            break;
        }
    }
}
bool esPyme()
{
    cadena pyme;
    bool esPyme = false;

    leerCadena("Es pyme: ", pyme);
    convertirCadenaAMayuscula(pyme);

    do
    {
        if (strcmp(pyme, "SI") == 0)
        {
            esPyme = true;
        }

    } while (strcmp(pyme, "SI") != 0 && strcmp(pyme, "NO") != 0);

    return esPyme;
}
void menuVistaAdmin(Cliente datos[], int posUsuario)
{

    // Vista de administrador

    printf("\n");
    printf("==================================================\n");
    printf("                 BANCO INSPT                  \n");
    printf("             SISTEMA DE GESTION INTERNA           \n");
    printf("==================================================\n\n");

    printf("Administrador: %s %s\n\n",
           datos[posUsuario].nombre,
           datos[posUsuario].apellido);

    printf("------------ MENU DE ADMINISTRACION -------------\n\n");

    printf("  1. Gestionar personal\n");
    printf("  2. Gestionar agentes\n");
    printf("  3. Consultar clientes\n");
    printf("  4. Consultar balance\n");
    printf("  5. Cerrar sesion\n");

    printf("\n==================================================\n");
    printf("Seleccione una opcion: ");
}
void menuVistaCliente(Cliente datos[], int posUsuario)
{
    // Vista Cliente
    printf("\n");
    printf("==================================================\n");
    printf("                 BANCO INSPT                 \n");
    printf("                HOME BANKING                   \n");
    printf("==================================================\n\n");

    printf("Bienvenido/a, %s %s\n\n",
           datos[posUsuario].nombre,
           datos[posUsuario].apellido);

    printf("--------------- MENU PRINCIPAL ------------------\n\n");

    printf("  1. Consultar saldo\n");
    printf("  2. Realizar deposito\n");
    printf("  3. Realizar retiro\n");
    printf("  4. Realizar transferencia\n");
    printf("  5. Solicitar prestamo\n");
    printf("  6. Ver movimientos\n");
    printf("  7. Consultar CBU y alias\n");
    printf("  8. Ver beneficios\n");
    printf("  0. Cerrar sesion\n");

    printf("\n==================================================\n");
    printf("Seleccione una operacion: ");
}
void menuVistaClientePYME(Cliente datos[], int posUsuario)
{
    // vista pyme
    printf("\n");
    printf("==================================================\n");
    printf("                 BANCO INSPT                  \n");
    printf("              PLATAFORMA DIGITAL - PYME                \n");
    printf("==================================================\n");

    printf("\n");
    printf("  BIENVENIDO/A\n");
    printf("  Cliente: %s %s\n",
           datos[posUsuario].nombre,
           datos[posUsuario].apellido);

    printf("--------------------------------------------------\n");
    printf("              MENU PRINCIPAL                      \n");
    printf("--------------------------------------------------\n");

    printf("  [1]  Consultar saldo\n");
    printf("  [2]  Realizar deposito\n");
    printf("  [3]  Realizar retiro\n");
    printf("  [4]  Realizar transferencia\n");
    printf("  [5]  Solicitar prestamo\n");
    printf("  [6]  Consultar movimientos\n");
    printf("  [7]  Consultar CBU y alias\n");
    printf("  [8]  Consultar beneficios\n");

    printf("--------------------------------------------------\n");
    printf("  [0]  Cerrar sesion\n");
    printf("==================================================\n");
    printf("  Seleccione una operacion: ");
}
void cabecera()
{
    printf("\n");
    printf("   __________________________________________   \n");
    printf("  |                                          |  \n");
    printf("  |             BANCO INSPT                  |  \n");
    printf("  |          ------------------              |  \n");
    printf("  |          PLATAFORMA DIGITAL              |  \n");
    printf("  |__________________________________________|  \n");
    printf("\n");
    printf("              INICIO DE SESION                 \n");
    printf("          Acceso seguro al sistema              \n\n");
}
bool esMenor(int num, int num1)
{
    return num < num1;
}
bool esIgual(int num, int num1)
{
    return num == num1;
}
bool esClaveCorrecta(cadena claveIngresada, cadena claveGuardada)
{
    return strcmp(claveIngresada, claveGuardada) == 0;
}
void convertirCadenaAMayuscula(cadena frase)
{
    // cadena copia;
    int hasta;
    // strcpy(copia, frase); copia para no modificar la variable original
    hasta = strlen(frase);
    for (int i = 0; i < hasta; i++)
    {
        frase[i] = toupper(frase[i]);
    }
}
bool respestaEsPyme(bool respuesta)
{
    return respuesta == true;
}
int busquedaLinealDNI(Cliente datos[], int cantidad, cadena valorBuscado)
{
    int pos = cantidad - 1;
    while (pos >= 0 && strcmp(datos[pos].DNI, valorBuscado) != 0)
    {
        pos--;
    }
    return pos;
}
int busquedaLinealALIAS(Cliente datos[], int cantidad, cadena valorBuscado)
{
    int pos = cantidad - 1;
    while (pos >= 0 && strcmp(datos[pos].alias, valorBuscado) != 0)
    {
        pos--;
    }
    return pos;
}
int verficarUsuario(Cliente datos[], int cantidad)
{
    cadena usuario;
    int contador = 0;
    int posUsuario = USUARIO_INEXISTENTE;
    do
    {
        leerCadena("Ingresar DNI o alias: ", usuario);
        posUsuario = busquedaLinealUsuario(datos, cantidad, usuario);
        contador++;

        if (posUsuario == USUARIO_INEXISTENTE)
        {
            printf("Usuario inexistente.\n");
        }
        else
        {
            printf("Usuario encontrado.\n");
        }
    } while (posUsuario == USUARIO_INEXISTENTE && esMenor(contador, INTENTOS_MAX));
    return posUsuario;
}