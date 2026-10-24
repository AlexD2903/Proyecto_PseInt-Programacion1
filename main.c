#include "utils.h"

#define CANT_USERS 6
#define VACIO 0
#define OPC_SALIDA 0
#define OPC_MODIFICAR_USU 4
#define OPC_M_GEST_PERS 5
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
    float saldo;
    int rol;
    bool esPyme;
} DatosBancarios;

typedef struct
{
    DatosPersonales datosPersonales;
    DatosBancarios datosBanco;
} Cliente;

// FORMATO: typedef  <tipo de retorno>  (*<NombreDelTipo>)  (<lista de parámetros>);
typedef const char *(*ObtenerCampo)(const Cliente *);
typedef bool (*ReglaFormato)(cadena);



Cliente listUser[CANT_USERS] = {
    {{"Charly", "Cimino", "11111111"}, {"alex123", "alex.r", 0.0, ADMINISTRADOR, false}},
    {{"Alex", "Ramos", "12121212"}, {"alex123", "alex.r", 340.4, CLIENTE, false}},
    {{"Santiago", "Gutierrez", "13131313"}, {"Santi2", "santi.g", 10000.0, CLIENTE, false}},
    {{"Juan", "Britez", "14141414"}, {"Juan.Britez", "Juam1989", 4303.56, CLIENTE, true}},
    {{"Esteban", "Quinteros", "15151515"}, {"123123", "esteban.q", 10204.43, CLIENTE, false}}};

/////////////////// FUNCIONES DEL CLIENTE ///////////////////

void menuVistaCliente(Cliente datos[], int posUsuario);
void menuOpcCliente(float *saldo);
void consultarSaldo(float saldoConsultado);

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
<<<<<<< HEAD
void listaUsuarios(cadena matrizUser[][DATOS_USERS], int *cantUsuarios);
void mostrarDatosDeMatriz(int COLUMNAS, cadena matriz[][COLUMNAS], int *cant);
int buscarUsuarios(cadena matrizUser[][DATOS_USERS], int *cantUsuarios, cadena aliasBuscado, int posicionColumna);
=======
>>>>>>> origin/ramaAlex

void listaUsuarios(Cliente baseDatos[], int *cantUsuarios);
void mostrarListadUsuarios(Cliente baseDatos[CANT_USERS], int cant);
void modificarUsuario(Cliente listUser[CANT_USERS], int *cantUsuarios);
void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[posUsuario], int tipoDato, int cantidad);

<<<<<<< HEAD
void cambiarAliasDeUsuario(cadena matrizUser[][DATOS_USERS], int *cant, cadena aliasNuevo);
void cambiarClaveDeUsuario(int vecClave[], cadena matrizUser[][DATOS_USERS], int *cantUsuarios, cadena aliasBuscado, int posicionEnColumna);
int verificacionClave(int *contador, int vecClave[], int posicion, cadena msj1, cadena msj2);
void bajaUsuario(cadena matrizUser[][DATOS_USERS], int vecClave[], int *cantUsuarios, cadena aliasBuscado);
=======
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

//////////////////////OTRAS FUCIONES////////////////////////
void limpiarPantalla();
>>>>>>> origin/ramaAlex

// --------------MAIN
int main()
{

    int cantUsuarios = 4;
    menuGestionarPersonal(listUser, &cantUsuarios);

    // bool accesoConcedido = loginUsuarios(listUser, CANT_USERS, &cantUsuarios);
    // inicioSesion(listUser, cantUsuarios, accesoConcedido);

    return 0;
}

/* ========================================================================
 *  1. VISTAS (solo imprimen por pantalla)
 * ======================================================================== */

void cabecera()
{
<<<<<<< HEAD
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
=======
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
>>>>>>> origin/ramaAlex
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

void menuVistaCliente(Cliente datos[], int posUsuario)
{
    // Vista Cliente
    printf("\n");
    printf("==================================================\n");
    printf("                 BANCO INSPT                 \n");
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
    printf("  5. Solicitar prestamo\n");
    printf("  6. Ver movimientos\n");
    printf("  7. Consultar CBU y alias\n");
    printf("  8. Ver beneficios\n");
    printf("  0. Cerrar sesion\n");

    printf("\n==================================================\n");
    printf("Seleccione una operacion: ");
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
    leerCadena("Ingresar apellido: ", nuevo.datosPersonales.apellido);

    leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, "Ingresar DNI: ", nuevo.datosPersonales.DNI);
    verificarDatoExistente(nuevo.datosPersonales.DNI, "DNI", baseDatos, cantUsuarios, obtenerDNI, verificarDNI, LONG_MIN_DNI, LONG_MAX_DNI);

    leerCadenaEntre(LONG_MIN_ALIAS, LONG_MAX_ALIAS, "Ingresar alias: ", nuevo.datosBanco.alias);
    verificarDatoExistente(nuevo.datosBanco.alias, "alias", baseDatos, cantUsuarios, obtenerAlias, verificarAlias, LONG_MIN_ALIAS, LONG_MAX_ALIAS);

    leerCadena("Ingrese clave: ", nuevo.datosBanco.clave);
    nuevo.datosBanco.esPyme = esClientePyme();
    nuevo.datosBanco.rol = CLIENTE;
    nuevo.datosBanco.saldo = 0;

    return nuevo;
}

void agregarUsuario(Cliente new, Cliente baseDatos[CANT_USERS], int *cantUsuarios)
{
    strcpy(baseDatos[*cantUsuarios].datosPersonales.nombre, new.datosPersonales.nombre);
    strcpy(baseDatos[*cantUsuarios].datosPersonales.apellido, new.datosPersonales.apellido);
    strcpy(baseDatos[*cantUsuarios].datosPersonales.DNI, new.datosPersonales.DNI);
    strcpy(baseDatos[*cantUsuarios].datosBanco.alias, new.datosBanco.alias);
    strcpy(baseDatos[*cantUsuarios].datosBanco.clave, new.datosBanco.clave);
    baseDatos[*cantUsuarios].datosBanco.saldo = new.datosBanco.saldo;
    baseDatos[*cantUsuarios].datosBanco.esPyme = new.datosBanco.esPyme;
    baseDatos[*cantUsuarios].datosBanco.rol = new.datosBanco.rol;

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
} // stb

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
    // PENDIENTE QUE ALEX LO MODULARIZE
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
        strcpy(listUser[i].datosPersonales.nombre, listUser[i + 1].datosPersonales.nombre);
        strcpy(listUser[i].datosPersonales.apellido, listUser[i + 1].datosPersonales.apellido);
        strcpy(listUser[i].datosPersonales.DNI, listUser[i + 1].datosPersonales.DNI);
        strcpy(listUser[i].datosBanco.clave, listUser[i + 1].datosBanco.clave);
        strcpy(listUser[i].datosBanco.alias, listUser[i + 1].datosBanco.alias);
        listUser[i].datosBanco.saldo = listUser[i + 1].datosBanco.saldo;
        listUser[i].datosBanco.esPyme = listUser[i + 1].datosBanco.esPyme;
        listUser[i].datosBanco.rol = listUser[i + 1].datosBanco.rol;
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

//dejalo asi, no saques el fflush
void limpiarPantalla(){
    fflush(stdin);
    system("pause");
    system("cls");
}