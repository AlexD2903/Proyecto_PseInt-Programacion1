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
#define CANT_ADMINS 3

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
} DatosLogin;

typedef struct
{
    DatosLogin datosLogin;
    float saldo;
    // int rol;
    bool esPyme;
} DatosBancarios;

typedef struct
{
    DatosPersonales datosPersonales;
    DatosBancarios datosBanco;
} Cliente;

// Administrador
typedef struct
{
    DatosPersonales datosPersonales;
    DatosLogin datosLogin;
} Administradores;

// FORMATO: typedef  <tipo de retorno>  (*<NombreDelTipo>)  (<lista de parámetros>);
typedef const char *(*ObtenerCampo)(const Cliente *); 
typedef bool (*ReglaFormato)(cadena);

Administradores listAdmins[CANT_ADMINS] = {
    {{"Charly", "Cimino", "11111111"}, {"charly.cimino", "terminator2"}},
    {{"Juan", "Messi", "16161616"}, {"juan.messi", "terminator3"}}
};

Cliente listUser[CANT_USERS] = {
    {{"Alex", "Ramos", "12121212"}, {{"alex123", "alex.r"}, 340.4, false}},
    {{"Santiago", "Gutierrez", "13131313"}, {{"Santi2", "santi.g"}, 10000.0, false}},
    {{"Juan", "Britez", "14141414"}, {{"Juan.Britez", "Juam1989"}, 4303.56, true}},
    {{"Esteban", "Quinteros", "15151515"}, {{"123123", "esteban.q"}, 10204.43, false}}
};

/////////////////// FUNCIONES DEL CLIENTE ///////////////////

void menuVistaCliente(Cliente datos[], int posUsuario);
void menuOpcCliente(float *saldo);
void consultarSaldo(float saldoConsultado);

/////////////////// FUNCIONES DEL ADMINISTRADOR ///////////////////

void menuVistaAdmin(void);
void menuGestionarPersonal(Cliente list[], int *cantUsuarios, Administradores listAdmins[], int *cantAdmins);
void vistaGestionPersonal(void);
void enEspera(void);
void cabecera();
Administradores nuevoAdministrador(Administradores baseDatos[CANT_ADMINS], int cantAdministradores);

/////////////////// ALTA PERSONAL ///////////////////

void altaPersonal(Cliente baseDatos[CANT_USERS], int *cantUsuarios, Administradores listAdmins[CANT_ADMINS], int *cantAdmins);
void agregarUsuario(Cliente nuevo, Cliente baseDatos[CANT_USERS], int *cantUsuarios);
Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios);
void verificarDatoExistente(cadena datoBuscado, cadena nomDato, Cliente baseDatos[CANT_USERS], int cantUsuarios, ObtenerCampo campoBuscado, ReglaFormato funcionCondicional, int longMin, int longMax);

/////////////////// CONSULTAR O MODIFICAR ///////////////////

void listaUsuarios(Cliente baseDatos[], int *cantUsuarios);
void mostrarListadUsuarios(Cliente baseDatos[CANT_USERS], int cant);
void modificarUsuario(Cliente listUser[CANT_USERS], int *cantUsuarios);
void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[posUsuario], int tipoDato, int cantidad);
void listaUsuarios(Cliente baseDatos[], int *cantUsuarios);
void mostrarListadAdministradores(Administradores baseDatos[CANT_USERS], int cant);
void listaAdministradores(Administradores baseDatos[], int *cantUsuarios);

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
const char *obtenerAliasAdmin(const Administradores *c);

//////////////////////OTRAS FUCIONES////////////////////////
void limpiarPantalla();

// --------------MAIN
int main()
{

    int cantUsuarios = 4;
    int cantAdmins = 2;
    menuGestionarPersonal(listUser, &cantUsuarios, listAdmins, &cantAdmins);

    // bool accesoConcedido = loginUsuarios(listUser, CANT_USERS, &cantUsuarios);
    // inicioSesion(listUser, cantUsuarios, accesoConcedido);

    return 0;
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
    printf("GESTION PERSONAL\n\n");
    printf("1. Alta de nuevo usuario\n");
    printf("2. Ver lista de clientes\n");
    printf("3. Ver lista de administradores\n");
    printf("4. Modificar usuario\n");
    printf("5. Baja de usuario\n");
    printf("6. Volver\n\n");
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

void menuGestionarPersonal(Cliente list[], int *cantUsuarios, Administradores listAdmins[], int *cantAdmins)
{
    int opcion = 0;

    vistaGestionPersonal();

    opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_PERS, "Elija una opcion del menu: ");
    while (opcion != OPC_SALIDA)
    {
        switch (opcion)
        {
        case 1:
            altaPersonal(list, cantUsuarios, listAdmins, cantAdmins);
            break;
        case 2:
            listaUsuarios(list, cantUsuarios);
            break;
        case 3:
            listaAdministradores(listAdmins, cantAdmins);
            break;
        case 4:
            modificarUsuario(list, cantUsuarios);
            break;
        case 5:
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

void altaPersonal(Cliente baseDatos[CANT_USERS], int *cantUsuarios, Administradores listAdmins[CANT_ADMINS], int *cantAdmins)
{
    Cliente newCliente;
    Administradores newAdmin;

    printf("Es cliente o administrador?\n1. Cliente\n2. Administrador\n");
    int opcion = leerEnteroEntre(1, 2, "Elija una opcion: ");

    switch (opcion)
    {
    case 1:
        if (*cantUsuarios < CANT_USERS)
        { 

            printf("========== ALTA DE CLIENTE ==========\n");
            newCliente = nuevoUsuario(baseDatos, *cantUsuarios);
            agregarUsuario(newCliente, baseDatos, cantUsuarios);

            printf("\nUsuario registrado correctamente.");
            printf("\nNombre y apellido: %s %s", newCliente.datosPersonales.nombre, newCliente.datosPersonales.apellido);
            printf("\nAlias: %s\n", newCliente.datosBanco.datosLogin.alias);
        }
        else
        {
            printf("No se puede registrar el usuario.\n");
            printf("No hay  mas espacio\n");
            printf("Se alcanzo el limite maximo.\n");
        }
    case 2:
        if (*cantAdmins < CANT_ADMINS)
        {
            printf("========== ALTA DE ADMINISTRADOR ==========\n");
            newAdmin = nuevoAdministrador(listAdmins, *cantAdmins);
        }
        else
        {
            printf("No se pueden registrar mas administradores\n");
        }
    }
}

Administradores nuevoAdministrador(Administradores baseDatos[CANT_ADMINS], int cantAdministradores)
{
    Administradores nuevo;

    leerCadena("Ingresar nombre: ", nuevo.datosPersonales.nombre);
    leerCadena("Ingresar apellido: ", nuevo.datosPersonales.apellido);

    leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, "Ingresar DNI: ", nuevo.datosPersonales.DNI);
    verificarDatoExistente(nuevo.datosPersonales.DNI, "DNI", baseDatos, cantAdministradores, obtenerDNI, verificarDNI, LONG_MIN_DNI, LONG_MAX_DNI);

    leerCadenaEntre(LONG_MIN_ALIAS, LONG_MAX_ALIAS, "Ingresar nombre de Usuario: ", nuevo.datosLogin.alias);
    verificarDatoExistente(nuevo.datosLogin.alias, "Nombre Usuario", baseDatos, cantAdministradores, obtenerAlias, verificarAlias, LONG_MIN_ALIAS, LONG_MAX_ALIAS);

    return nuevo;
}

Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios)
{
    Cliente nuevo;

    leerCadena("Ingresar nombre: ", nuevo.datosPersonales.nombre);
    leerCadena("Ingresar apellido: ", nuevo.datosPersonales.apellido);

    leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, "Ingresar DNI: ", nuevo.datosPersonales.DNI);
    verificarDatoExistente(nuevo.datosPersonales.DNI, "DNI", baseDatos, cantUsuarios, obtenerDNI, verificarDNI, LONG_MIN_DNI, LONG_MAX_DNI);

    leerCadenaEntre(LONG_MIN_ALIAS, LONG_MAX_ALIAS, "Ingresar alias: ", nuevo.datosBanco.datosLogin.alias);
    verificarDatoExistente(nuevo.datosBanco.datosLogin.alias, "alias", baseDatos, cantUsuarios, obtenerAlias, verificarAlias, LONG_MIN_ALIAS, LONG_MAX_ALIAS);

    leerCadena("Ingrese clave: ", nuevo.datosBanco.datosLogin.clave);
    nuevo.datosBanco.esPyme = esClientePyme();
    // nuevo.datosBanco.rol = CLIENTE;
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

/// LISTA CLIENTEES
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
}

// LISTA DE ADMINISTRADORES
void listaAdministradores(Administradores baseDatos[], int *cantUsuarios)
{
    printf("\nLista de Administradores:\n");
    mostrarListadAdministradores(baseDatos, *cantUsuarios);
}

void mostrarListadAdministradores(Administradores baseDatos[CANT_USERS], int cant)
{
    for (int i = 0; i < cant; i++)
    {
        printf("%d. Nombre: %s %s,\nDNI: %s\n=======================\n", i + 1, baseDatos[i].datosPersonales.nombre, baseDatos[i].datosPersonales.apellido, baseDatos[i].datosPersonales.DNI);
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
        respuesta = strcmp(usuario.datosBanco.datosLogin.clave, claveAux) == 0;

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
    return c->datosBanco.datosLogin.alias;
}

const char *obtenerAliasAdmin(const Administradores *c)
{
    return c->datosLogin.alias;
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
//     verificarDatoExistente("DNI o alias ", aliasOdni.datosBanco.datosLogin.alias, datos, posUsuario);
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