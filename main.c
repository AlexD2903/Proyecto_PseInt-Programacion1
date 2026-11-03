#include "utils.h"
#include <errno.h> //Para detecnar si el archivo no existe (ENOENT)
#include <direct.h>
#include <time.h>

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

#define ADMIN "admin"

#define SALDO 1
#define DEPOSITOS 2
#define RETIROS 3

#define CANT_MESES 12
#define CANT_TIPOS_MOVIMIENTO 4
#define ENERO 0
#define FEBRERO 1
#define MARZO 2
#define ABRIL 3
#define MAYO 4
#define JUNIO 5
#define JULIO 6
#define AGOSTO 7
#define SEPTIEMBRE 8
#define OCTUBRE 9
#define NOVIEMBRE 10
#define DICIEMBRE 11

typedef struct
{
    cadena nombre,
        apellido,
        DNI;
} DatosPersonales;

typedef struct
{
    cadena clave,
        alias;
    float saldo,
        limiteExtraccion;
    int rol;
    bool esPyme;
    float movMensuales[CANT_MESES][CANT_TIPOS_MOVIMIENTO];
} DatosBancarios;

typedef struct
{
    DatosPersonales datosPersonales;
    DatosBancarios datosBanco;
} Cliente;

// FORMATO: typedef  <tipo de retorno>  (*<NombreDelTipo>)  (<lista de parámetros>);
typedef bool (*ReglaFormato)(cadena);
typedef const char *(*ObtenerCampo)(const Cliente *);

/////////////////// PROTOTIPOS ///////////////////

/////////////////// MENUS Y SESION ///////////////////
void menuVistaCliente(Cliente datos[], int posUsuario);
void menuGestionDeCliente(Cliente listClientes[], int posicionActual);
void iniciarSesion(Cliente listUser[], int *cantUsuarios);
void cabecera(void);
void vistaGestionPersonal(void);
void menuGestionarPersonal(Cliente list[], int *cantUsuarios);
void enEspera(void);

/////////////////// OPERACIONES DEL CLIENTE ///////////////////
void ingresarDinero(Cliente *clienteActual, int mesSimulado);
void retirarDinero(Cliente *clienteActual, int mesSimulado);
void confirmarExtraccion(Cliente *clienteActual, float montoRetirado, int mesSimulado);
void evaluarExtraccion(Cliente *clienteActual, float *montoRetirado);
void mensajeErrorExtraccion(Cliente *clienteActual);
void consultarDatosPersonales(Cliente clienteActual);
void consultarSaldo(Cliente cliente);

/////////////////// GESTION DE USUARIOS ///////////////////
void altaPersonal(Cliente baseDatos[CANT_USERS], int *cantUsuarios);
void agregarUsuario(Cliente nuevo, Cliente baseDatos[CANT_USERS], int *cantUsuarios);
Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios);
void verificarDatoExistente(cadena datoBuscado, cadena nomDato, Cliente baseDatos[CANT_USERS], int cantUsuarios, ObtenerCampo campoBuscado, ReglaFormato funcionCondicional, int longMin, int longMax);
void listaUsuarios(Cliente baseDatos[], int *cantUsuarios);
void mostrarListadUsuarios(Cliente baseDatos[CANT_USERS], int cant);
void modificarUsuario(Cliente listUser[CANT_USERS], int *cantUsuarios);
void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[], int tipoDato, int cantidad);
void bajaUsuario(Cliente listUser[], int *cantUsuarios);
void eliminarUsuario(Cliente listUser[], int posUser, int *cantUser);
void confirmarEliminacion(Cliente listUser[], int posUsuario, int *cantUsuarios);

/////////////////// BUSQUEDA Y VALIDACION ///////////////////
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
const char *obtenerClave(const Cliente *c);
void ordenarVectorDeCadenasAscendente(Cliente vec[], int cant);

/////////////////// ARCHIVOS ///////////////////
void guardarUsuariosVC(Cliente listUser[], int cant);
int cargarUsuariosVC(Cliente listUser[]);

/////////////////// UTILIDADES ///////////////////
void limpiarPantalla(void);

int main(void)
{
    Cliente listUser[CANT_USERS] /* = {
        {{"Charly", "Cimino", "11111111"}, {"charly.c", "terminator2", 0.0, LIMITE_EXTRACION, ADMINISTRADOR, false, {{0}}} },
        {{"Alex", "Ramos", "12121212"}, {"alex123", "alex.r", 340.4, LIMITE_EXTRACION, CLIENTE, false, {{0}}} },
        {{"Santiago", "Gutierrez", "13131313"}, {"Santi2", "santi.g", 10000.0, LIMITE_EXTRACION, CLIENTE, false, {{0}}} },
        {{"Juan", "Britez", "14141414"}, {"Juan.Britez", "Juam1989", 4303.56, LIMITE_EXTRACION, CLIENTE, true, {{0}}} },
        {{"Esteban", "Quinteros", "15151515"}, {"123123", "esteban.q", 10204.43, LIMITE_EXTRACION, CLIENTE, false, {{0}}} }
    } */;

    int cantClientes = 0;
    int cantidadLeida = cargarUsuariosVC(listUser);

    if (cantidadLeida >= 0)
    {
        cantClientes = cantidadLeida;
    }
    else
    {
        puts("No se pudieron cargar los usuarios.");
    }
    iniciarSesion(listUser, &cantClientes);

    //  bool accesoConcedido = loginUsuarios(listUser, CANT_USERS, &cantUsuarios);
    //  inicioSesion(listUser, cantUsuarios, accesoConcedido);

    return 0;
}

// rb leer //este es para cargar, porque LEE
// wb sobrescribir O CREAARR C-R-E-A-R
// ab para agregar a lo ultimo

// fseek   para buscar, ni idea como se usaba, pero lo usamos

/* ========================================================================
 *  1. VISTAS (solo imprimen por pantalla)
 * ======================================================================== */

void cabecera(void)
{
    printf("\n");
    printf("   __________________________________________   \n");
    printf("  |                                          |  \n");
    printf("  |            BANCO ABRAXAS                 |  \n");
    printf("  |          ------------------              |  \n");
    printf("  |          PLATAFORMA DIGITAL              |  \n");
    printf("  |__________________________________________|  \n");
    printf("\n");
    printf("              INICIO DE SESION                 \n");
    printf("          Acceso seguro al sistema              \n\n");
}

void vistaGestionPersonal(void)
{
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

void iniciarSesion(Cliente listUser[], int *cantUsuarios)
{
    int posActual;
    cadena usuario,
        clave;

    cabecera();

    leerCadena("Ingrese usuario: ", usuario);
    leerCadena("Ingrese la clave", clave);

    // while ()
    // {
    //     /* code */
    // }
    

    if (strcmp(usuario, ADMIN) == 0 && strcmp(clave, ADMIN) == 0)
    {
        menuGestionarPersonal(listUser, cantUsuarios);
    }
    else
    {

        posActual = buscarDato(listUser, *cantUsuarios, usuario, obtenerAlias);

        if (posActual != USUARIO_INEXISTENTE)
        {
            if (strcmp(clave, listUser[posActual].datosBanco.clave) == 0)
            {
                listUser[posActual].datosBanco.rol == CLIENTE ? menuGestionDeCliente(listUser, posActual) : menuGestionarPersonal(listUser, cantUsuarios);
            }
            else
            {
                puts("Clave incorrecta guachin");
            }
        }
        else
        {
            puts("Usuario incorrecto.");
        }
    }
}

void menuGestionDeCliente(Cliente listClientes[], int posicionActual)
{
    int mesSimulado = leerEnteroEntre(ENERO + 1, DICIEMBRE + 1, "Ingresar mes simulado [1,12]: ");
    system("cls");
    menuVistaCliente(listClientes, posicionActual);
    int opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_CLIENT, "Elija una opcion del menu: ");

    while (opcion != OPC_SALIDA)
    {
        switch (opcion)
        {
        case SALDO:
            consultarSaldo(listClientes[posicionActual]);
            break;
        case DEPOSITOS:
            ingresarDinero(&listClientes[posicionActual], mesSimulado);
            break;
        case RETIROS:
            retirarDinero(&listClientes[posicionActual], mesSimulado);
            break;
        case 4:
            // realizarTransferencia();
            break;
        case 5:
            verMovimientos(&listClientes[posicionActual]);
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
void verMovimientos(Cliente cliente[])
{
    cadena nombresMeses[CANT_MESES] = {
        "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };

    int mes = leerEnteroEntre(ENERO+1, DICIEMBRE+1, "Ingrese el mes que quiere consultar (1-12): ");

    int opcion = leerEnteroEntre(DEPOSITOS, RETIROS, "1. Depositos\n2. Retiros\nTipo de movimiento: ");
    int columna;

    switch (opcion-1)
    {
    case DEPOSITOS:
        printf("Total del mes seleccionado: $%.2f\n",
           cliente->datosBanco.movMensuales[mes - 1][columna]);
        break;
    case RETIROS:
        columna = RETIROS;
        break;
    }

    float total = 0;

    

    printf("Total del tipo seleccionado: $%.2f\n", total);
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

void ingresarDinero(Cliente *clienteActual, int mesSimulado)
{
    system("cls");
    printf("Indique la cantidad que desea ingresar, puede ingresar hasta $%.2f\n", MONTO_INGRESO_MAX);
    float montoIngresado = leerFloatEntre(MONTO_INGRESO_MIN, MONTO_INGRESO_MAX, "Monto: ");

    if (confirmaUsuario("Desea confirmar la operacion?"))
    {
        (*clienteActual).datosBanco.saldo += montoIngresado;
        clienteActual->datosBanco.movMensuales[mesSimulado - 1][DEPOSITOS] += montoIngresado;
        puts("Opercion exitosa.");
        consultarSaldo(*clienteActual);
    }
    else
    {
        puts("Operacion cancelada.\n");
    }
}

void retirarDinero(Cliente *clienteActual, int mesSimulado)
{
    system("cls");
    if (clienteActual->datosBanco.saldo >= MONTO_RETIRO_MIN && clienteActual->datosBanco.limiteExtraccion != LIMITE_CERO)
    {
        printf("Indique la cantidad que desea retirar, puede retirar un minimo de $%.2f\n", MONTO_RETIRO_MIN);
        printf("Limite de extraccion por dia $%.2f\n", (*clienteActual).datosBanco.limiteExtraccion);
        float montoRetirado = leerFloatEntre(MONTO_RETIRO_MIN, clienteActual->datosBanco.limiteExtraccion, "Cuanto desea retirar?\n$");

        evaluarExtraccion(clienteActual, &montoRetirado);
        confirmarExtraccion(clienteActual, montoRetirado, mesSimulado);
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

void confirmarExtraccion(Cliente *clienteActual, float montoRetirado, int mesSimulado)
{
    if (confirmaUsuario("Desea confirmar la operacion?"))
    {
        (*clienteActual).datosBanco.saldo -= montoRetirado;
        (*clienteActual).datosBanco.limiteExtraccion -= montoRetirado;
        clienteActual->datosBanco.movMensuales[mesSimulado - 1][RETIROS] -= montoRetirado;
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
        case 5: // Genera Reporte

            break;
        }

        limpiarPantalla();
        vistaGestionPersonal();
        opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_PERS, "Elija una opcion del menu: ");
    }
    puts("////////fwrite////////");
    guardarUsuariosVC(list, *cantUsuarios);
}
int cargarUsuariosVC(Cliente listUser[]) // la cantidad se lee desde el archivo, no se necesita pasar por parametro
{
    int cantidad = -1;
    FILE *archivo = fopen("../ListUsers/Usuarios.dat", "rb");

    if (archivo != NULL)
    {
        puts("El archivo se abrio correctamente.");
        if (fread(&cantidad, sizeof(int), 1, archivo) == 1)
        {
            printf("Cantidad de usuarios leida: %d\n", cantidad);

            if (cantidad >= 0 && cantidad <= CANT_USERS) // verifica que la cantidad no sea negativa y supere la cantidad de usuarios
            {
                if (fread(listUser, sizeof(Cliente), cantidad, archivo) == (size_t)cantidad)
                {
                    puts("Usuarios leidos correctamente.");
                }
                else
                {
                    puts("No se pudieron leer todos los usuarios.");
                    cantidad = -1;
                }
            }
            else
            {
                puts("La cantidad guardada en el archivo no es valida.");
                cantidad = -1;
            }
        }
        else
        {
            puts("No se pudo leer la cantidad de usuarios.");
        }

        fclose(archivo); // siempre va cuando el archivo se abre correctamente
    }
    else if (errno == ENOENT)
    {
        printf("Archivo inexistente : %s\n", strerror(errno));
    }
    else
    {
        puts("Hubo otro error.");
    }
    return cantidad;
}
void guardarUsuariosVC(Cliente listUser[], int cant)
{
    FILE *archivo;
    _mkdir("../ListUsers");
    archivo = fopen("../ListUsers/Usuarios.dat", "wb");

    if (archivo != NULL)
    {
        puts("Si se pudo abrir.");
        // sirve para escribir la cantidad de usuarios, si no, no se puede guardar el vector
        if (fwrite(&cant, sizeof(int), 1, archivo) == 1)
        {
            puts("Cantidad de usuarios guardada.");
            int escritos = fwrite(listUser, sizeof(Cliente), cant, archivo);

            if (escritos == cant)
            {
                puts("Usuarios guardados correctamente.");
            }
            else
            {
                puts("Error al guardar los usuarios.");
            }
        }
        else
        {
            puts("Error al guardar la cantidad de usuarios.");
        }
        fclose(archivo);
    }
    else
    {
        puts("No se pudo abrir el archivo.");
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
    Cliente nuevo = {0};

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

bool esClientePyme(void)
{
    cadena pyme;
    bool esuserpyme = false;

    do
    {
        leerCadena("Es pyme: ", pyme);
        cadenaAPascal(pyme);
        if (strcmp(pyme, "Si") == 0)
        {
            esuserpyme = true;
        }

    } while (strcmp(pyme, "Si") != 0 && strcmp(pyme, "No") != 0);

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
void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[], int tipodDato, int cantidad)
{
    cadena nomDato;

    switch (tipodDato)
    {
    case OPC_NOMBRE:
        leerCadena(texto, nomDato);
        cadenaAPascal(nomDato);
        confirmaUsuario("Estas seguro?") ? strcpy(usuario[posUsuario].datosPersonales.nombre, nomDato) : NULL;
        break;

    case OPC_APELLIDO:
        leerCadena(texto, nomDato);
        cadenaAPascal(nomDato);
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

const char *obtenerClave(const Cliente *c)
{
    return c->datosBanco.clave;
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
void limpiarPantalla(void)
{
    fflush(stdin);
    system("pause");
    system("cls");
}