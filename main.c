#include "utils.h"
#include <windows.h>

#define OPC_MIN 1
#define OPC_MAX 5
#define CANT_USERS 6
#define VACIO 0
#define OPCION_SALIDA 0
#define USUARIO_INEXISTENTE -1
#define OPC_DNI 1
#define OPC_ALIAS 2
#define OPC_NOMBRE 3
#define OPC_APELLIDO 4
#define INTENTO_MAXIMO 3
#define ADMINISTRADOR 1
#define CLIENTE 2

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

Cliente listUser[CANT_USERS] = {
    {{"Alex", "Ramos", "12121212"}, {"alex123", "alex.r", 340.4, ADMINISTRADOR, false}},
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
void msjDeConfirmacion(cadena nombreDeDato, int dato);
Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios);
void verificarDatoExistente(cadena texto, cadena datoNew, Cliente baseDatos[CANT_USERS], int cantUsuarios);

/////////////////// CONSULTAR O MODIFICAR ///////////////////

void listaUsuarios(Cliente baseDatos[], int *cantUsuarios);
void mostrarListadUsuarios(Cliente baseDatos[CANT_USERS], int cant);
void modificarUsuario(Cliente listUser[CANT_USERS], int *cantUsuarios);
void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[], int tipoDato, int cantidad);

void verificarExistenciaDNI(cadena nomCampo, cadena nomDato, Cliente usuario[], int posUsuario, int cantidad, const char *(*obtenerCampo)(const Cliente *));

/////////////////// BAJA ///////////////////

void bajaUsuario(Cliente listUser[], int *cantUsuarios);
void eliminarUsuario(Cliente listUser[], int posUser, int *cantUser);
void confirmarEliminacion(Cliente listUser[], int posUsuario, int *cantUsuarios);

/////////////////// BÚSQUEDA Y VALIDACIÓN ///////////////////

int posicionPorDNI(Cliente listUser[], int cantUsuarios);
bool verificarDNI(cadena dni);
bool esClientePyme(void);
bool verificacionClave(Cliente usuario);
int buscarDato(Cliente datos[], int cantidad, const cadena dato, const char *(*obtenerCampo)(const Cliente *));
// De prueba
const char *obtenerAlias(const Cliente *c);
const char *obtenerDNI(const Cliente *c);
const char *obtenerNombre(const Cliente *c);
const char *obtenerApellido(const Cliente *c);

// --------------MAIN
int main()
{

    int cantUsuarios = 4;
    menuGestionarPersonal(listUser,&cantUsuarios);

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
    printf("  |             BANCO INSPT                  |  \n");
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

    opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
    while (opcion != OPC_MAX)
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
        system("pause");
        system("cls");
        fflush(stdin);
        vistaGestionPersonal();
        opcion = leerEnteroEntre(OPC_MIN, OPC_MAX, "Elija una opcion del menu: ");
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

Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios)
{
    Cliente nuevo;

    leerCadena("Ingresar nombre: ", nuevo.datosPersonales.nombre);
    leerCadena("Ingresar apellido: ", nuevo.datosPersonales.apellido);

    verificarDatoExistente("alias", nuevo.datosBanco.alias, baseDatos, cantUsuarios);
    verificarDatoExistente("DNI", nuevo.datosPersonales.DNI, baseDatos, cantUsuarios);
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

void verificarDatoExistente(cadena texto, cadena datoNew, Cliente baseDatos[CANT_USERS], int cantUsuarios)
{
    int existe;

    do
    {
        printf("Ingresar %s",texto);
        leerCadena(": ", datoNew);
        existe = buscarDato(baseDatos, cantUsuarios, datoNew, obtenerDNI);
        if (existe == USUARIO_INEXISTENTE)
        {
            existe = buscarDato(baseDatos, cantUsuarios, datoNew, obtenerAlias);
        }

        msjDeConfirmacion(texto, existe);

    } while (existe != USUARIO_INEXISTENTE);
}

void msjDeConfirmacion(cadena nombreDeDato, int dato)
{
    if (dato != USUARIO_INEXISTENTE)
    {
        printf("El %s ya existe. Elija otro.\n", nombreDeDato);
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
        printf("CLIENTE %s %s\n", listUser[posUsuario].datosPersonales.nombre, listUser[posUsuario].datosPersonales.apellido);
        // nombre, apellido, dni, espyme
        printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre\n2. Apellido\n3. DNI\n4. Tipo de cliente\n0. Volver\n");

        opc = leerEnteroEntre(0, 3, "Elija una opcion: ");
        while (opc != OPCION_SALIDA)
        {
            switch (opc)
            {
            case 1:
                cambiarDatoDeUsuario("Ingresar nombre: ", posUsuario, listUser, OPC_NOMBRE, *cantUsuarios);
                break;
            case 2:
                cambiarDatoDeUsuario("Ingresar apellido: ", posUsuario, listUser, OPC_APELLIDO, *cantUsuarios);
                break;
            case 3:
                cambiarDatoDeUsuario("Ingresar DNI: ", posUsuario, listUser, OPC_DNI, *cantUsuarios);
                break;
            
            }
             printf("OPCIONES PARA MODIFICAR USUARIO:\n1. Nombre\n2. Apellido\n3. DNI\n4. Tipo de cliente\n0. Volver\n");
            opc = leerEnteroEntre(0, 3, "Elija una opcion: ");
        }
    }
    else
    {
        printf("\nNo existe ese usuario\n");
    }
}

void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[posUsuario], int tipodDato, int cantidad)
{
    cadena nomDato;

    leerCadena(texto, nomDato);
    switch (tipodDato)
    {
    case OPC_NOMBRE:
        strcpy(usuario[posUsuario].datosPersonales.nombre, nomDato);
        break;
    case OPC_APELLIDO:
        strcpy(usuario[posUsuario].datosPersonales.apellido, nomDato);
        break;
    case OPC_DNI:
        verificarExistenciaDNI("DNI", nomDato, usuario, posUsuario, cantidad, obtenerDNI);
        strcpy(usuario[posUsuario].datosPersonales.DNI, nomDato);
        break;
    }
}

// FUNCION NUEVA
void verificarExistenciaDNI(cadena nomCampo, cadena nomDato, Cliente usuario[], int posUsuario, int cantidad, const char *(*obtenerCampo)(const Cliente *))
{
    while (strcmp(nomDato, obtenerCampo(&usuario[posUsuario])) == 0 || buscarDato(usuario, cantidad, nomDato,obtenerCampo) != -1)
    {
        printf("El %s ya existe, por favor elija otro:\nElija un nuevo %s", nomCampo, nomCampo);
        leerCadena(": ", nomDato);
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
        if (verificacionClave(listUser[posUsuario])) // es un vector? NO ES UN VECTOR!
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
    leerCadena("Ingrese DNI de cliente: ", dniAux);
    return buscarDato(listUser, cantUsuarios, dniAux, obtenerDNI);
}

int buscarDato(Cliente datos[], int cantidad, const cadena dato, const char *(*obtenerCampo)(const Cliente *))
{
    int pos = cantidad - 1;
    while (pos >= 0 && strcmp(obtenerCampo(&datos[pos]), dato) != 0)
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

bool verificarDNI(cadena dni)
{
    bool valido = true;
    int posicion = 0;

    if ((strlen(dni) != 8) || (valido && (dni[0] == '0' && dni[1] == '0')))
    {
        valido = false;
    }
    while (valido && dni[posicion] != '\0')
    {
        if (dni[posicion] < '0' || dni[posicion] > '9')
        {
            valido = false;
        }
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

// ---------- Version anterior de cambiarDatoDeUsuario ----------

// void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[posUsuario], int tipodDato, int cantidad)
// {
//     cadena nomDato;

//     leerCadena(texto, nomDato);
//     switch (tipodDato)
//     {
//     case OPC_NOMBRE:
//         while (strcmp(nomDato, usuario[posUsuario].datosPersonales.nombre))
//         {
//             printf("El nombre ya existe, por favor elija otro:\n");
//             leerCadena("Elija un nuevo nombre: ", nomDato);
//         }
//         strcpy(usuario[posUsuario].datosPersonales.nombre, nomDato);
//         break;
//     case OPC_APELLIDO:
//         while (strcmp(nomDato, usuario[posUsuario].datosPersonales.apellido))
//         {
//             printf("El apellido ya existe, por favor elija otro:\n");
//             leerCadena("Elija un nuevo apellido: ", nomDato);
//         }
//         strcpy(usuario[posUsuario].datosPersonales.apellido, nomDato);
//         break;
//     case OPC_DNI:
//         while (strcmp(nomDato, usuario[posUsuario].datosPersonales.DNI) == 0 || buscarDNI(usuario, cantidad, nomDato) != -1)
//         {
//             printf("El DNI ya existe, por favor elija otro:\n");
//             leerCadena("Elija un nuevo DNI: ", nomDato);
//         }
//         strcpy(usuario[posUsuario].datosPersonales.DNI, nomDato);
//     }
// }

// ---------- Versiones anteriores de las funciones de busqueda ----------

// int buscarAlias(Cliente datos[], int cantidad, cadena dato)
// {
//     int pos = cantidad - 1;
//     while (pos >= 0 && strcmp(datos[pos].datosBanco.alias, dato) != 0)
//     {
//         pos--;
//     }
//     return pos;
// }

// Alex: hizo dos funciones para no matarnos con los parametros y se vuelve más cohesiva
// int buscarDato(Cliente datos[], int cantidad, cadena valor, int tipodDato)
// {
//     int pos = cantidad - 1;
//     switch (tipodDato)
//     {
//     case OPC_DNI:
//         while (pos >= 0 && strcmp(datos[pos].datosPersonales.DNI, valor) != 0)
//         {
//             pos--;
//         }
//         break;
//     case OPC_ALIAS:
//         while (pos >= 0 && strcmp(datos[pos].datosBanco.alias, valor) != 0)
//         {
//             pos--;
//         }
//         break;
//     }

//     return pos;
// }