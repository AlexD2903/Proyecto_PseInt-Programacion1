#include "utils.h"
#include <errno.h> //Para detecnar si el archivo no existe (ENOENT)
#include <direct.h>

#define CANT_USERS 100
#define MAX_BAJAS 100

#define VACIO 0
#define OPC_SALIDA 0
#define OPC_MODIFICAR_USU 4
#define OPC_M_GEST_PERS 5
#define OPC_M_GEST_CLIENT 6
#define USUARIO_INEXISTENTE -1
#define OPC_DNI 1
#define OPC_ALIAS 2
#define OPC_NOMBRE 3
#define OPC_APELLIDO 4
#define INTENTO_MAXIMO 3
#define ADMINISTRADOR 1
#define CLIENTE 2
#define LONG_MIN_DNI 7
#define LONG_MAX_DNI 8
#define LONG_MIN_ALIAS 6
#define LONG_MAX_ALIAS 20

#define MONTO_RETIRO_MIN 1000.00
#define MONTO_INGRESO_MIN 1000.00
#define MONTO_INGRESO_MAX 1500000.00
#define LIMITE_EXTRACION 1000000.00
#define LIMITE_CERO 0.00

#define CARPETA_DATA "data"
#define RUTA_USUARIOS "data/listaUsuarios.dat"

typedef struct
{
    cadena nombre,
        apellido,
        DNI;
    // telefono;
} DatosPersonales;

typedef struct
{
    cadena clave,
        alias;
    float saldo,
        limiteExtraccion;
    int rol;
    bool esPyme;
} DatosBancarios;

typedef struct
{
    DatosPersonales datosPersonales;
    DatosBancarios datosBanco;
} Cliente;

// FORMATO: typedef  <tipo de retorno>  (*<NombreDelTipo>)  (<lista de parámetros>);
typedef bool (*ReglaFormato)(cadena);
typedef const char *(*ObtenerCampo)(const Cliente *);
/////////////////// FUNCIONES DEL CLIENTE ///////////////////

void menuVistaCliente(Cliente datos[], int posUsuario);
void menuOpcCliente(float *saldo);
void menuGestionDeCliente(Cliente listClientes[], int posicionActual, int *cantUsers);
void ingresarDinero(Cliente *clienteActual);
void retirarDinero(Cliente *clienteActual);
void confirmarExtraccion(Cliente *clienteActual, float montoRetirado);
void evaluarExtraccion(Cliente *clienteActual, float *montoRetirado);
void mensajeErrorExtraccion(Cliente *clienteActual);
void consultarDatosPersonales(Cliente clienteActual);

/////////////////// FUNCIONES DEL ADMINISTRADOR ///////////////////

void menuVistaAdmin(void);
void menuGestionarPersonal(Cliente list[], int *cantUsuarios);
void vistaGestionPersonal(void);
void enEspera(void);
void cabecera();

/////////////////// ALTA PERSONAL ///////////////////

void altaPersonal(Cliente baseDatos[CANT_USERS], int *cantUsuarios);
void agregarUsuario(Cliente nuevo, Cliente baseDatos[CANT_USERS], int *cantUsuarios);
Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios);
void verificarDatoExistente(cadena datoBuscado, cadena nomDato, Cliente baseDatos[CANT_USERS], int cantUsuarios, ObtenerCampo campoBuscado, ReglaFormato funcionCondicional, int longMin, int longMax);

/////////////////// CONSULTAR O MODIFICAR ///////////////////

void listaUsuarios(Cliente baseDatos[], int *cantUsuarios);
void mostrarListadUsuarios(Cliente baseDatos[CANT_USERS], int cant);
void modificarUsuario(Cliente listUser[CANT_USERS], int *cantUsuarios);
void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[posUsuario], int tipoDato, int cantidad);
void consultarSaldo(Cliente cliente);

/////////////////// BAJA ///////////////////

void bajaUsuario(Cliente listUser[], int *cantUsuarios);
void eliminarUsuario(Cliente listUser[], int posUser, int *cantUser);
void confirmarEliminacion(Cliente listUser[], int posUsuario, int *cantUsuarios);

/////////////////// BÚSQUEDA Y VALIDACIÓN ///////////////////

int posicionPorDNI(Cliente listUser[], int cantUsuarios);
bool verificarDNI(cadena dni);
bool verificaCaracter(char string, char cInicial, char cFinal);
bool verificarAlias(cadena alias);
bool esClientePyme(void);
bool verificacionClave(Cliente usuario);
int buscarDato(Cliente datos[], int cantidad, const cadena dato, ObtenerCampo campoBuscado);

const char *obtenerAlias(const Cliente *c);
const char *obtenerDNI(const Cliente *c);
const char *obtenerNombre(const Cliente *c);
const char *obtenerApellido(const Cliente *c);
void ordenarVectorDeCadenasAscendente(Cliente vec[], int cant);

////////////////////////    ARCHIVOS    //////////////////////////
void cargarUsuarios(Cliente listUser[]);
void guardarUsuarios(Cliente listUser[], int cantUsuarios);
int cargarUsuarios2(Cliente listUser[]);
void guardarUsuarios2(Cliente listUser[], int cantUsuarios);
void mostrarListadUsuarios2(Cliente baseDatos[CANT_USERS], int cant);

// de prueba
void listaUsuariosDos();
void mostrarUsuarioDos(Cliente c, int numero);

//////////////////////OTRAS FUCIONES////////////////////////
void limpiarPantalla();

// --------------MAIN
int main()
{
    Cliente listUser[CANT_USERS] = {
        {{"Charly", "Cimino", "11111111"}, {"charly.c", "terminator2", 0.0, LIMITE_EXTRACION, ADMINISTRADOR, false}},
        {{"Alex", "Ramos", "12121212"}, {"alex123", "alex.r", 340.4, LIMITE_EXTRACION, CLIENTE, false}},
        {{"Santiago", "Gutierrez", "13131313"}, {"Santi2", "santi.g", 10000.0, LIMITE_EXTRACION, CLIENTE, false}},
        {{"Juan", "Britez", "14141414"}, {"Juan.Britez", "Juam1989", 4303.56, LIMITE_EXTRACION, CLIENTE, true}},
        {{"Esteban", "Quinteros", "15151515"}, {"123123", "esteban.q", 10204.43, LIMITE_EXTRACION, CLIENTE, false}}};

    int cantUsuarios = 5;
    // menuGestionarPersonal(listUser, &cantUsuarios);
    //  bool accesoConcedido = loginUsuarios(listUser, CANT_USERS, &cantUsuarios);
    //  inicioSesion(listUser, cantUsuarios, accesoConcedido);

    //guardarUsuarios(listUser, cantUsuarios);
    //cargarUsuarios(listUser);
    //mostrarListadUsuarios(listUser, cantUsuarios);
    cantUsuarios = cargarUsuarios2(listUser);
    mostrarListadUsuarios2(listUser, cantUsuarios);
    guardarUsuarios2(listUser, cantUsuarios);

    // int posUsuario = 3;
    // int cantClientes = 4;
    //menuGestionDeCliente(listUser, posUsuario, &cantClientes);

    //listaUsuariosDos();

    return 0;
}

void cargarUsuarios(Cliente listUser[])
{
    // 1 Abrimos en modo rb  //read binary // leer binario

    // FILE viene con stdio
    int cantidad = 0;

    FILE *f = fopen(RUTA_USUARIOS, "rb");

    if (f != NULL)
    {
        // primero leeemos cuantos usuarios hay
        /*
        if( fread(cantUser, sizeof(int), 1, f) == 1  ){
            if(*cantUser > 0){
                fread(listUser, sizeof(Cliente), *cantUser, f);
            }
        }
        fclose(f);
        */
        while (fread(&listUser[cantidad], sizeof(Cliente), 1, f) == 1)
        {
            cantidad++;
        }

        fclose(f);
        printf("[SISTEMA] Base de datos cargada correctamente (%d usuarios).\n", cantidad);
    }
    else if (errno == ENOENT)
    {

        puts("Cargando desde del backup");
    }

    puts("Sistema iniciado");

}

// rb leer //este es para cargar, porque LEE
// wb sobrescribir O CREAARR C-R-E-A-R
// ab para agregar a lo ultimo

// fseek   para buscar, ni idea como se usaba, pero lo usamos

//--------->     ACA SE VA A CREEAAAARRRRR    <--------------------
void guardarUsuarios(Cliente listUser[], int cantUsuarios)
{

    _mkdir(CARPETA_DATA);
    FILE *archivo = fopen(RUTA_USUARIOS, "wb");

    if (archivo != NULL)
    {

        int escritos = -1;

        if (fwrite(&cantUsuarios, sizeof(int), 1, archivo) == 1)
        {
            escritos = fwrite(listUser, sizeof(Cliente), cantUsuarios, archivo);
        }
        fclose(archivo);

        if (escritos == cantUsuarios)
        {
            printf("Datos guardados correctamente (%d usuarios).\n", cantUsuarios);
        }
        else
        {
            puts("Error: no se pudieron escribir todos los datos.");
        }
    }
    else
    {
        puts("Error: No se pudo guardar la informacion en el disco.");
    }
}

/* ========================================================================
 *  1. VISTAS (solo imprimen por pantalla)
 * ======================================================================== */

void cabecera()
{
    printf("\n");
    printf("   __________________________________________   \n");
    printf("  |                                          |  \n");
    printf("  |             BANCO ABRAXAS                |  \n");
    printf("  |          ------------------              |  \n");
    printf("  |          PLATAFORMA DIGITAL              |  \n");
    printf("  |__________________________________________|  \n");
    printf("\n");
    printf("              INICIO DE SESION                 \n");
    printf("          Acceso seguro al sistema              \n\n");
}

void vistaGestionPersonal()
{
    // Vista de usuario
    printf("GESTION PERSONAL\n\n");
    printf("1. Alta de nuevo usuario\n"); // C
    printf("2. Ver lista de usuarios\n"); // R
    printf("3. Modificar usuario\n");     // U
    printf("4. Baja de usuario\n");       // D
    printf("0. Volver\n\n");
}

void menuVistaCliente(Cliente datos[], int posUsuario)
{
    // Vista Cliente
    printf("\n");
    printf("==================================================\n");
    printf("                BANCO ABRAXAS                 \n");
    printf("                HOME BANKING                   \n");
    printf("==================================================\n\n");

    printf("Bienvenido/a, %s %s\n\n",
           datos[posUsuario].datosPersonales.nombre,
           datos[posUsuario].datosPersonales.apellido);

    printf("--------------- MENU PRINCIPAL ------------------\n\n");

    printf("  1. Consultar saldo\n");
    printf("  2. Realizar deposito\n");
    printf("  3. Realizar retiro\n");
    printf("  4. Realizar transferencia\n");
    printf("  5. Ver movimientos\n");
    printf("  6. Consultar CBU y alias\n");
    // printf("  5. Solicitar prestamo\n");
    printf("  0. Cerrar sesion\n");

    printf("\n==================================================\n");
}

///////////////////////INICIAR SESION////////////////////////////

// void iniciarSesion(){

//     printf("Ingrese su usuario");

//     printf("Ingrese su clave");
// }

void menuGestionDeCliente(Cliente listClientes[], int posicionActual, int *cantUsers)
{
    
menuVistaCliente(listClientes, posicionActual);
int opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_CLIENT, "Elija una opcion del menu: ");

while (opcion != OPC_SALIDA)
{
    switch (opcion)
    {
        case 1:
        consultarSaldo(listClientes[posicionActual]);
        break;
        case 2:
        ingresarDinero(&listClientes[posicionActual]);
        break;
        case 3:
        retirarDinero(&listClientes[posicionActual]);
        break;
        case 4:
        // realizarTransferencia();
        break;
        case 5:
        // verMovimientos();
        break;
        case 6:
        consultarDatosPersonales(listClientes[posicionActual]);
        break;
        // case 7:
        // PRESTAMOOOOSSSS!!!!!!
        default:
        puts("Gracias vuelvas prontos");
        break;
    }
    limpiarPantalla();
    menuVistaCliente(listClientes, posicionActual);
    opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_CLIENT, "\nElija una opcion del menu: ");
}
}

void consultarDatosPersonales(Cliente clienteActual)
{
    system("cls");
    printf("Datos personales:\n");
    printf("Nombre y Apellido: %s %s\n", clienteActual.datosPersonales.nombre, clienteActual.datosPersonales.apellido);
    printf("DNI: %s\n", clienteActual.datosPersonales.DNI);
    printf("Alias: %s\n", clienteActual.datosBanco.alias);
}

void consultarSaldo(Cliente clienteActual)
{
    printf("Saldo actual: %.2f\n", clienteActual.datosBanco.saldo);
}

void ingresarDinero(Cliente *clienteActual)
{
    system("cls");
    printf("Indique la cantidad que desea ingresar, puede ingresar hasta $%.2f\n", MONTO_INGRESO_MAX);
    float montoIngresado = leerFloatEntre(MONTO_INGRESO_MIN, MONTO_INGRESO_MAX, "Monto: ");

    if (confirmaUsuario("Desea confirmar la operacion?"))
    {
        (*clienteActual).datosBanco.saldo += montoIngresado;
        printf("Opercion exitosa\n"), consultarSaldo(*clienteActual);
    }
    else
    {
        puts("Operacion cancelada.\n");
    }
}

void retirarDinero(Cliente *clienteActual)
{
    system("cls");
    if (clienteActual->datosBanco.saldo >= MONTO_RETIRO_MIN && clienteActual->datosBanco.limiteExtraccion != LIMITE_CERO)
    {
        printf("Indique la cantidad que desea retirar, puede retirar un minimo de $%.2f\n", MONTO_RETIRO_MIN);
        printf("Limite de extraccion por dia $%.2f\n", (*clienteActual).datosBanco.limiteExtraccion);
        float montoRetirado = leerFloatEntre(MONTO_RETIRO_MIN, clienteActual->datosBanco.limiteExtraccion, "Cuanto desea retirar?\n$");

        evaluarExtraccion(clienteActual, &montoRetirado);
        confirmarExtraccion(clienteActual, montoRetirado);
    }
    else
    {
        mensajeErrorExtraccion(clienteActual);
    }
}

void mensajeErrorExtraccion(Cliente *clienteActual)
{
    if (clienteActual->datosBanco.saldo <= MONTO_RETIRO_MIN)
    {
        puts("Saldo insifuciente para realizar una extraccion minima.\n");
        consultarSaldo(*clienteActual);
        printf("Minimo para retirar es: %.2f\n", MONTO_RETIRO_MIN);
    }
    else if (clienteActual->datosBanco.limiteExtraccion <= LIMITE_CERO)
    {
        puts("Limite de extracion superado.");
    }
}

void confirmarExtraccion(Cliente *clienteActual, float montoRetirado)
{
    if (confirmaUsuario("Desea confirmar la operacion?"))
    {
        (*clienteActual).datosBanco.saldo -= montoRetirado;
        (*clienteActual).datosBanco.limiteExtraccion -= montoRetirado;
        puts("Operacion exitosa!\n");
        consultarSaldo(*clienteActual);
    }
    else
    {
        puts("Operacion cancelada.\n");
    }
}

void evaluarExtraccion(Cliente *clienteActual, float *montoRetirado)
{
    while (*montoRetirado > clienteActual->datosBanco.limiteExtraccion || *montoRetirado > clienteActual->datosBanco.saldo)
    {
        if (*montoRetirado >= clienteActual->datosBanco.limiteExtraccion)
        {
            printf("Error. El limite de extraccion es $%.2f", clienteActual->datosBanco.limiteExtraccion);
        }
        else
        {
            printf("El monto supera el saldo actual");
        }
        *montoRetirado = leerFloatEntre(MONTO_RETIRO_MIN, clienteActual->datosBanco.saldo, "\nCuanto desea retirar?");
    }
}

/* ========================================================================
 *  2. MENU DE GESTION DE PERSONAL (ADMINISTRADOR)
 * ======================================================================== */

void menuGestionarPersonal(Cliente list[], int *cantUsuarios)
{
    int opcion = 0;

    vistaGestionPersonal();

    opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_PERS, "Elija una opcion del menu: ");
    while (opcion != OPC_SALIDA)
    {
        switch (opcion)
        {
        case 1: // C (Create / Crear)
            altaPersonal(list, cantUsuarios);
            break;
        case 2: // R (Read / Leer o Consultar)
            listaUsuarios(list, cantUsuarios);
            break;
        case 3: // U (Update / Actulizar o Modificar)
            modificarUsuario(list, cantUsuarios);
            break;
        case 4: // D (Delete / Borrar o Eliminar)
            bajaUsuario(list, cantUsuarios);
            break;
        // case 5: // Genera Reporte
        // break;
        }

        limpiarPantalla();
        vistaGestionPersonal();
        opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_PERS, "Elija una opcion del menu: ");
    }
}

/* ========================================================================
 *  3. ALTA DE USUARIO (Create)
 * ======================================================================== */

void altaPersonal(Cliente baseDatos[CANT_USERS], int *cantUsuarios)
{
    Cliente newCliente;

    if (*cantUsuarios < CANT_USERS)
    {

        printf("========== ALTA DE PERSONAL ==========\n");
        newCliente = nuevoUsuario(baseDatos, *cantUsuarios);
        agregarUsuario(newCliente, baseDatos, cantUsuarios);

        printf("\nUsuario registrado correctamente.");
        printf("\nNombre y apellido: %s %s", newCliente.datosPersonales.nombre, newCliente.datosPersonales.apellido);
        printf("\nAlias: %s\n", newCliente.datosBanco.alias);
    }
    else
    {
        printf("No se puede registrar el usuario.\n");
        printf("No hay  mas espacio\n");
        printf("Se alcanzo el limite maximo.\n");
    }
}

Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios)
{
    Cliente nuevo;

    leerCadena("Ingresar nombre: ", nuevo.datosPersonales.nombre);
    cadenaAPascal(nuevo.datosPersonales.nombre);
    leerCadena("Ingresar apellido: ", nuevo.datosPersonales.apellido);
    cadenaAPascal(nuevo.datosPersonales.apellido);

    leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, "Ingresar DNI: ", nuevo.datosPersonales.DNI);
    verificarDatoExistente(nuevo.datosPersonales.DNI, "DNI", baseDatos, cantUsuarios, obtenerDNI, verificarDNI, LONG_MIN_DNI, LONG_MAX_DNI);

    leerCadenaEntre(LONG_MIN_ALIAS, LONG_MAX_ALIAS, "Ingresar alias: ", nuevo.datosBanco.alias);
    verificarDatoExistente(nuevo.datosBanco.alias, "alias", baseDatos, cantUsuarios, obtenerAlias, verificarAlias, LONG_MIN_ALIAS, LONG_MAX_ALIAS);

    leerCadena("Ingrese clave: ", nuevo.datosBanco.clave);
    nuevo.datosBanco.esPyme = esClientePyme();
    nuevo.datosBanco.rol = CLIENTE;
    nuevo.datosBanco.limiteExtraccion = LIMITE_EXTRACION;
    nuevo.datosBanco.saldo = 0;

    return nuevo;
}

void agregarUsuario(Cliente new, Cliente baseDatos[CANT_USERS], int *cantUsuarios)
{
    baseDatos[*cantUsuarios] = new;

    (*cantUsuarios)++;
}

bool esClientePyme()
{
    cadena pyme;
    bool esuserpyme = false;

    do
    {
        leerCadena("Es pyme: ", pyme);
        convertirCadenaAMayuscula(pyme);
        if (strcmp(pyme, "SI") == 0)
        {
            esuserpyme = true;
        }

    } while (strcmp(pyme, "SI") != 0 && strcmp(pyme, "NO") != 0);

    return esuserpyme;
}

void verificarDatoExistente(cadena datoBuscado, cadena nomDato, Cliente baseDatos[CANT_USERS], int cantUsuarios, ObtenerCampo campoBuscado, ReglaFormato funcionCondicional, int longMin, int longMax)
{
    bool valido = false;

    while (!valido)
    {
        if (!funcionCondicional(datoBuscado))
        {
            printf("El %s ingresado es invalido.\n", nomDato);
        }
        else if (buscarDato(baseDatos, cantUsuarios, datoBuscado, campoBuscado) != USUARIO_INEXISTENTE)
        {
            printf("El %s ya existe.\n", nomDato);
        }
        else
        {
            valido = true;
        }
        if (!valido)
        {
            printf("Ingresar %s", nomDato);
            leerCadenaEntre(longMin, longMax, ": ", datoBuscado);
        }
    }
}

/* ========================================================================
 *  4. CONSULTA DE USUARIOS (Read)
 * ======================================================================== */

void listaUsuarios(Cliente baseDatos[], int *cantUsuarios)
{
    if (*cantUsuarios > VACIO)
    {
        ordenarVectorDeCadenasAscendente(baseDatos, *cantUsuarios);
        printf("\nLista de usuarios:\n");
        mostrarListadUsuarios(baseDatos, *cantUsuarios);
    }
    else
    {
        printf("---- No hay usuarios registrados ----\n");
    }
}

void mostrarListadUsuarios(Cliente baseDatos[CANT_USERS], int cant)
{
    for (int i = 0; i < cant; i++)
    {
        printf("%d. Cliente: %s %s,\nDNI: %s\n=======================\n", i + 1, baseDatos[i].datosPersonales.nombre, baseDatos[i].datosPersonales.apellido, baseDatos[i].datosPersonales.DNI);
    }
}

// void listaUsuariosDos()
// {
//     Cliente aux;
//     int cantidad = 0;
//     int i = 0;

//     FILE *f = fopen(RUTA_USUARIOS, "rb");

//     if (f != NULL)
//     {
//         fread(&cantidad, sizeof(int), 1, f);

//         // ---- DEBUG (se borra despues) ----
//         printf("DEBUG: sizeof(Cliente) = %zu bytes\n", sizeof(Cliente));
//         printf("DEBUG: cantidad leida del archivo = %d\n", cantidad);
//         // ----------------------------------

//         printf("\nLista de usuarios:\n");

//         while (i < cantidad && fread(&aux, sizeof(Cliente), 1, f) == 1)
//         {
//             mostrarUsuarioDos(aux, i + 1);
//             i++;
//         }
//         fclose(f);

//         // ---- DEBUG ----
//         printf("DEBUG: clientes impresos = %d\n", i);
//         // ---------------

//         if (i == 0)
//         {
//             puts("---- No hay usuarios registrados ----");
//         }
//     }
//     else
//     {
//         puts("ERROR, no se pudo abrir el archivo");
//     }
// }

// void mostrarUsuarioDos(Cliente c, int numero)
// {
//     printf("%d. Cliente: %s %s,\nDNI: %s\n=======================\n", numero, c.datosPersonales.nombre, c.datosPersonales.apellido, c.datosPersonales.DNI);
// }

/* ========================================================================
 *  5. MODIFICACION DE USUARIOS (Update)
 * ======================================================================== */

void modificarUsuario(Cliente listUser[CANT_USERS], int *cantUsuarios)
{
    int opc = 0;
    int posUsuario = posicionPorDNI(listUser, *cantUsuarios);

    if (posUsuario != -1)
    {
        system("cls");
        printf("CLIENTE %s %s\n", listUser[posUsuario].datosPersonales.nombre, listUser[posUsuario].datosPersonales.apellido);
        printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre\n2. Apellido\n3. DNI\n4. Tipo de cliente\n0. Volver\n");

        opc = leerEnteroEntre(OPC_SALIDA, OPC_MODIFICAR_USU, "Elija una opcion: ");
        while (opc != OPC_SALIDA)
        {
            switch (opc)
            {
            case 1:
                cambiarDatoDeUsuario("Ingresar nuevo nombre: ", posUsuario, listUser, OPC_NOMBRE, *cantUsuarios);
                break;
            case 2:
                cambiarDatoDeUsuario("Ingresar nuevo apellido: ", posUsuario, listUser, OPC_APELLIDO, *cantUsuarios);
                break;
            case 3:
                cambiarDatoDeUsuario("Ingresar nuevo DNI: ", posUsuario, listUser, OPC_DNI, *cantUsuarios);
                break;
            case 4:
                // Agregar tipo de cliente
                enEspera();
                break;
            }
            system("cls");
            printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre\n2. Apellido\n3. DNI\n4. Tipo de cliente\n0. Volver\n");
            opc = leerEnteroEntre(OPC_SALIDA, OPC_MODIFICAR_USU, "Elija una opcion: ");
        }
    }
    else
    {
        printf("\nNo existe ese usuario\n");
    }
}

// Esta la opc. de parametrizar la longitud de la cadena y llamar solo una vez leerCadenaEntre()
void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[posUsuario], int tipodDato, int cantidad)
{
    cadena nomDato;

    switch (tipodDato)
    {
    case OPC_NOMBRE:
        leerCadena(texto, nomDato);
        confirmaUsuario("Estas seguro?") ? strcpy(usuario[posUsuario].datosPersonales.nombre, nomDato) : NULL;
        break;

    case OPC_APELLIDO:
        leerCadena(texto, nomDato);
        confirmaUsuario("Estas seguro?") ? strcpy(usuario[posUsuario].datosPersonales.apellido, nomDato) : NULL;
        break;

    case OPC_DNI:
        leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, texto, nomDato);
        verificarDatoExistente(nomDato, "DNI", usuario, cantidad, obtenerDNI, verificarDNI, LONG_MIN_DNI, LONG_MAX_DNI);
        confirmaUsuario("Estas seguro?") ? strcpy(usuario[posUsuario].datosPersonales.DNI, nomDato) : NULL;

        break;
    }
}

/* ========================================================================
 *  6. BAJA DE USUARIOS (Delete)
 * ======================================================================== */

void bajaUsuario(Cliente listUser[], int *cantUsuarios)
{
    int posUsuario = posicionPorDNI(listUser, *cantUsuarios);

    if (posUsuario != USUARIO_INEXISTENTE)
    {
        if (verificacionClave(listUser[posUsuario]))
        {
            confirmarEliminacion(listUser, posUsuario, cantUsuarios);
        }
        else
        {
            printf("CANTIDAD MAXIMA SUPERADA");
        }
    }
    else
    {

        printf("\nEl usuario no existe\n");
    }
}

void confirmarEliminacion(Cliente listUser[], int posUsuario, int *cantUsuarios)
{
    printf("\nUsuario encontrado:\n%s %s\n=================\n", listUser[posUsuario].datosPersonales.nombre, listUser[posUsuario].datosPersonales.apellido);

    if (confirmaUsuario("Estas seguro de eliminar este usuario."))
    {
        eliminarUsuario(listUser, posUsuario, cantUsuarios);
        printf("Usuario eliminado con exito.\n");
    }
    else
    {
        printf("Operacion Cancelada");
    }
}

void eliminarUsuario(Cliente listUser[], int posUser, int *cantUser)
{
    for (int i = posUser; i < (*cantUser - 1); i++)
    {
        listUser[i] = listUser[i + 1];
    }

    (*cantUser)--;
}

/* ========================================================================
 *  7. BUSQUEDA Y VALIDACION
 * ======================================================================== */

int posicionPorDNI(Cliente listUser[], int cantUsuarios)
{
    cadena dniAux;
    leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, "Ingresar DNI: ", dniAux);
    return buscarDato(listUser, cantUsuarios, dniAux, obtenerDNI);
}

int buscarDato(Cliente datos[], int cantidad, const cadena dato, ObtenerCampo campoBuscado)
{
    int pos = cantidad - 1;
    while (pos >= 0 && strcmp(campoBuscado(&datos[pos]), dato) != 0)
    {
        pos--;
    }
    return pos;
}

bool verificacionClave(Cliente usuario)
{
    int contadorIntentos = 0;
    bool respuesta = false;
    cadena claveAux;

    do
    {
        leerCadena("\nIngrese la clave: ", claveAux);
        respuesta = strcmp(usuario.datosBanco.clave, claveAux) == 0;

        if (!respuesta)
        {
            printf("\nClave incorrecta, ingrese nuevamente.");
            contadorIntentos++;
        }
    } while (!respuesta && contadorIntentos < INTENTO_MAXIMO);

    return respuesta;
}

bool verificarAlias(cadena alias)
{
    bool valido = (alias[0] != '\0'); // el alias vacío no es válido
    int posicion = 0;

    while (valido && alias[posicion] != '\0')
    {
        bool numeros = verificaCaracter(alias[posicion], '0', '9');
        bool mayus = verificaCaracter(alias[posicion], 'A', 'Z');
        bool minus = verificaCaracter(alias[posicion], 'a', 'z');
        bool signos = verificaCaracter(alias[posicion], '-', '.');

        valido = numeros || mayus || minus || signos;

        posicion++;
    }
    return valido;
}
bool verificaCaracter(char string, char cInicial, char cFinal)
{
    return (string >= cInicial && string <= cFinal);
}

bool verificarDNI(cadena dni)
{
    bool valido = (dni[0] != '\0');
    int posicion = 0;

    if (valido && dni[0] == '0' && dni[1] == '0')
    {
        valido = false;
    }
    while (valido && dni[posicion] != '\0')
    {
        valido = verificaCaracter(dni[posicion], '0', '9');
        posicion++;
    }
    return valido;
}

void ordenarVectorDeCadenasAscendente(Cliente vec[], int cant)
{
    Cliente aux;
    for (int i = 0; i < (cant - 1); i++)
    {
        for (int j = i + 1; j < cant; j++)
        {
            if (strcmp(vec[j].datosPersonales.nombre, vec[i].datosPersonales.nombre) < 0)
            {
                aux = vec[j];
                vec[j] = vec[i];
                vec[i] = aux;
            }
        }
    }
}

/* ========================================================================
 *  8. GETTERS (acceso a campos, usados como punteros a funcion)
 * ======================================================================== */

const char *obtenerNombre(const Cliente *c)
{
    return (*c).datosPersonales.nombre;
}

const char *obtenerApellido(const Cliente *c)
{
    return (*c).datosPersonales.apellido;
}

const char *obtenerDNI(const Cliente *c)
{
    return c->datosPersonales.DNI;
}

const char *obtenerAlias(const Cliente *c)
{
    return c->datosBanco.alias;
}

/* ========================================================================
 *  9. PENDIENTES (funciones sin implementar)
 * ======================================================================== */

// modificar
void enEspera()
{
    printf("Queda pendiente");
}

/* ========================================================================
 *  10. CODIGO COMENTADO (login / versiones anteriores)
 *      Lo dejo al final para que no moleste al leer el codigo activo.
 * ======================================================================== */

// ---------- Login e inicio de sesion (a futuro) ----------

// bool loginUsuarios(Cliente datos[], int cantidad, int *posUsuario)
// {
//     Cliente aliasOdni;
//     bool accesoConcedido = false;
//     cabecera();
//     verificarDatoExistente("DNI o alias ", aliasOdni.datosBanco.alias, datos, posUsuario);
//     *posUsuario =
//         if (!esIgual(*posUsuario, USUARIO_INEXISTENTE))
//     {
//         accesoConcedido = verificacionClave(datos, *posUsuario, "Ingrese contrasenia: ", "ERROR: Contrasenia incorrecta, intentalo de nuevo");
//     }
//     else
//     {
//         printf("ACCESO DENEGADO.\n");
//     }

//     return accesoConcedido;
// }

// void inicioSesion(Cliente datos[], int posUsuario, bool accesoConcedido)
// {
//     if (accesoConcedido)
//     {
//         switch (datos[posUsuario].rol)
//         {
//         case ADMINISTRADOR:
//             menuVistaAdmin(datos, posUsuario);
//             break;
//         case CLIENTE:
//             if (datos[posUsuario].Pyme == false)
//             {
//                 menuVistaCliente(datos, posUsuario);
//             }
//             else
//             {
//                 menuVistaClientePYME(datos, posUsuario);
//             }

//             break;
//         }
//     }
// }

// dejalo asi, no saques el fflush
void limpiarPantalla()
{
    fflush(stdin);
    system("pause");
    system("cls");
}

int cargarUsuarios2(Cliente listUser[])
{
    int cantidad = 0;
    FILE *f = fopen(RUTA_USUARIOS, "rb");

    if (f != NULL)
    {
        // 1. Leemos la cabecera: el numero exacto de clientes guardados
        if (fread(&cantidad, sizeof(int), 1, f) == 1)
        {
            // 2. Si hay al menos 1, leemos todos los clientes de un solo golpe
            if (cantidad > 0)
            {
                fread(listUser, sizeof(Cliente), cantidad, f);
            }
        }
        fclose(f);
        printf("[SISTEMA] Base de datos cargada correctamente (%d usuarios).\n", cantidad);
    }
    else
    {
        // Cumplimiento estricto: inicializar limpio si no existe el archivo.
        printf("\n[SISTEMA] No se encontro archivo de backup. Iniciando sistema vacio...\n");
    }

    return cantidad;
}

void guardarUsuarios2(Cliente listUser[], int cantUsuarios)
{
    _mkdir(CARPETA_DATA);
    FILE *archivo = fopen(RUTA_USUARIOS, "wb");

    if (archivo != NULL)
    {
        // Guardamos primero el numero (la cabecera)
        if (fwrite(&cantUsuarios, sizeof(int), 1, archivo) == 1)
        {
            // Luego guardamos los clientes
            if (cantUsuarios > 0)
            {
                fwrite(listUser, sizeof(Cliente), cantUsuarios, archivo);
            }
            printf("\n[SISTEMA] Datos guardados correctamente (%d usuarios).\n", cantUsuarios);
        }
        fclose(archivo);
    }
    else
    {
        puts("\n[ERROR] No se pudo guardar la informacion en el disco.");
    }
}