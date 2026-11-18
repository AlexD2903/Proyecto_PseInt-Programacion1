#include "utils.h"
#include <errno.h> //Para detecnar si el archivo no existe (ENOENT)
#include <direct.h>
#include <windows.h>

/* ========================================================================
 *  CONSTANTES Y CONFIGURACION DEL SISTEMA
 * ======================================================================== */

#define CANT_USERS 100
#define VACIO 0
#define USUARIO_INEXISTENTE -1
#define INTENTO_MAXIMO 3

// Opciones de menus
#define OPC_MODIFICAR_USU 4
#define OPC_M_GEST_PERS 6
#define OPC_M_GEST_CLIENT 6
#define CANTIDAD_DATOS 7

// Tipos de campo para modificacion
#define OPC_DNI 1
#define OPC_ALIAS 2
#define OPC_NOMBRE 3
#define OPC_APELLIDO 4

// Roles y credenciales por defecto
#define ADMINISTRADOR 1
#define CLIENTE 2
#define ADMIN "admin"

// Longitudes de cadenas
#define LONG_MIN_DNI 7
#define LONG_MAX_DNI 8
#define LONG_MIN_ALIAS 6
#define LONG_MAX_ALIAS 20
#define LONG_MIN_NOM 2
#define LONG_MAX_NOM 15
#define LONG_MIN_APE 2
#define LONG_MAX_APE 20
#define LONG_MIN_CLAVE 4
#define LONG_MAX_CLAVE 20

// Topes financieros
#define MONTO_RETIRO_MIN 1000.00
#define MONTO_INGRESO_MIN 1000.00
#define MONTO_INGRESO_MAX 1500000.00
#define LIMITE_EXTRACION 1000000.00
#define LIMITE_CERO 0.00
#define MIN_TRANSFERENCIA 1

// Rutas de persistencia
#define CARPETA_DATA "../data"
#define RUTA_USUARIOS "../data/Usuarios.dat"
#define RUTA_MOVIMIENTOS "../data/movimientos.dat"
#define RUTA_BAJAS "../data/bajas.dat"
#define RUTA_REPORTES_CLIENTE "../data/reportesCliente.txt"
#define RUTA_REPORTE_SISTEMA "../data/reportesAdmin.txt"
#define RUTA_REPORTE_MOVIMIENTOS "../data/misTransferencias.txt"

// Constantes de matriz y operaciones
#define SALDO 1
#define DEPOSITOS 2
#define RETIROS 3
#define CANT_MESES 12
#define CANT_TIPOS_MOVIMIENTO 4
#define ENERO 1
#define DICIEMBRE 12

/* ========================================================================
 *  ESTRUCTURAS DE DATOS Y PUNTEROS A FUNCION
 * ======================================================================== */

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

typedef struct
{
    cadena alias,
        aliasDestino;
    int mes;
    float monto;
} Movimientos;

// FORMATO: typedef  <tipo de retorno>  (*<NombreDelTipo>)  (<lista de parámetros>);
typedef bool (*ReglaFormato)(cadena);
typedef const char *(*ObtenerCampo)(const Cliente *);

/* ========================================================================
 *  PROTOTIPOS DE FUNCIONES
 * ======================================================================== */

// 1. Vistas, Menus y Sesion
void cabecera(void);
void vistaGestionPersonal(void);
void menuVistaCliente(Cliente datos[], int posUsuario);
void iniciarSesion(Cliente listUser[], int *cantUsuarios);
void menuGestionDeCliente(Cliente listClientes[], int posicionActual, int cantUsers);
void menuGestionarPersonal(Cliente list[], int *cantUsuarios);

// 2. Operaciones Bancarias del cliente
void consultarSaldo(Cliente cliente);
void ingresarDinero(Cliente *clienteActual, int mesSimulado);
void retirarDinero(Cliente *clienteActual, int mesSimulado);
void confirmarExtraccion(Cliente *clienteActual, float montoRetirado, int mesSimulado);
void mensajeErrorExtraccion(Cliente *clienteActual);
void opcionTransferencia(Cliente listUser[], int cantUsuarios, int posUser, int mes);
bool realizarTransferencia(Cliente *origen, Cliente *destino, float monto, int mes);
Movimientos crearMovimientos(Cliente cliente, float monto, int mes);
float cuantoTransferir(Cliente usuario);
void verMovimientos(Cliente *cliente);
void consultarDatosPersonales(Cliente clienteActual);

// 3. Gestion de Usuarios (CRUD Administrador)
void altaPersonal(Cliente baseDatos[CANT_USERS], int *cantUsuarios);
Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios, int *opciones);
bool leerCampoValido(cadena mensaje, cadena nombreCampo, cadena dato);
void agregarUsuario(Cliente nuevo, Cliente baseDatos[CANT_USERS], int *cantUsuarios);
void listaUsuarios(Cliente baseDatos[], int *cantUsuarios);
void mostrarListadUsuarios(Cliente baseDatos[CANT_USERS], int cant);
void modificarUsuario(Cliente listUser[CANT_USERS], int *cantUsuarios);
void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[], int tipoDato, int cantidad);
void bajaUsuario(Cliente listUser[], int *cantUsuarios);
void confirmarEliminacion(Cliente listUser[], int posUsuario, int *cantUsuarios);
void eliminarUsuario(Cliente listUser[], int posUser, int *cantUser);

// 4. Busqueda, Ordenamiento y Validaciones
int posicionPorDNI(Cliente listUser[], int cantUsuarios);
int buscarDato(Cliente datos[], int cantidad, const cadena dato, ObtenerCampo campoBuscado);
void verificarDatoExistente(cadena datoBuscado, cadena nomDato, Cliente baseDatos[CANT_USERS], int cantUsuarios, ObtenerCampo campoBuscado, ReglaFormato funcionCondicional, int longMin, int longMax);
bool verificacionClave(Cliente usuario);
bool esClientePyme(bool *pSalida);
bool verificarCadena(cadena texto);
bool verificarAlias(cadena alias);
bool verificarNombre(cadena texto);
bool verificarDNI(cadena dni);
bool verificarCaracter(char string, char cInicial, char cFinal);
void ordenarUsuarios(Cliente baseDatos[], int cantUsuarios);

// 5. Getters
const char *obtenerAlias(const Cliente *c);
const char *obtenerDNI(const Cliente *c);

// 6. Persistencia y Archivos
int cargarUsuarios(Cliente listUser[]);
void guardarUsuarios(Cliente listUser[], int cant);
bool registrarMovimientos(Movimientos reg[], int cant);
bool registrarBaja(Cliente *eliminado);
void listaBajas(void);
void generarReporteSistema(Cliente listUser[], int cantUsuarios);
void generarReporteCliente(Cliente cliente);
void imprimirMovimiento(FILE *txt, Cliente cliente, Movimientos reg);

// 7. Utilidades
void limpiarPantalla(void);

/* ========================================================================
 *  FUNCION PRINCIPAL (CAJA LIMPIA)
 * ======================================================================== */


void reiniciarLimiteExtracciones(Cliente listUser[], int cant){
    for (int i = 0; i < cant; i++)
    {
        listUser[i].datosBanco.limiteExtraccion = LIMITE_EXTRACION;
    }
}


int main(void)
{
    _mkdir(CARPETA_DATA);

    Cliente listUser[CANT_USERS];

    int cantClientes = 0;
    int cantidadLeida = cargarUsuarios(listUser);
    reiniciarLimiteExtracciones(listUser, cantidadLeida);

    if (cantidadLeida >= 0)
    {
        cantClientes = cantidadLeida;
        limpiarPantalla();
    }
    else
    {
        puts("Sistema inicializado sin registros previos (0 usuarios).");
        limpiarPantalla();
    }

    bool cerrarApp = false;
    while (!cerrarApp)
    {
        system("cls");
        iniciarSesion(listUser, &cantClientes);
        cerrarApp = confirmaUsuario("Cerrar app?");
        guardarUsuarios(listUser, cantClientes);
    }

    return 0;
}

/* ========================================================================
 *  1. VISTAS, MENUS Y SESION
 * ======================================================================== */

void cabecera(void)
{
    printf("\n");
    printf("  ==================================================\n");
    printf("  |                 BANCO ABRAXAS                  |\n");
    printf("  |               PLATAFORMA DIGITAL               |\n");
    printf("  ==================================================\n");
    printf("                   INICIO DE SESION                 \n");
    printf("               Acceso seguro al sistema             \n\n");
}

void vistaGestionPersonal(void)
{
    system("cls");
    printf("\n==================================================\n");
    printf("         BANCO ABRAXAS - PANEL ADMINISTRADOR      \n");
    printf("==================================================\n\n");
    printf("  1. Alta de nuevo usuario\n");
    printf("  2. Ver lista de usuarios activos\n");
    printf("  3. Modificar datos de usuario\n");
    printf("  4. Baja de usuario\n");
    printf("  5. Ver historial de bajas\n");
    printf("  6. Exportar reporte del sistema (Archivo TXT)\n");
    printf("  0. Cerrar sesion de Administrador\n\n");
    printf("==================================================\n");
}

void menuVistaCliente(Cliente datos[], int posUsuario)
{
    printf("\n==================================================\n");
    printf("            BANCO ABRAXAS - HOME BANKING          \n");
    printf("==================================================\n");

    printf("Cliente: %s %s | Cuenta: %s\n",
           datos[posUsuario].datosPersonales.nombre,
           datos[posUsuario].datosPersonales.apellido,
           datos[posUsuario].datosBanco.esPyme ? "Pyme" : "Individuo");

    printf("--------------------------------------------------\n\n");

    printf("  1. Consultar saldo disponible\n");
    printf("  2. Realizar deposito\n");
    printf("  3. Realizar retiro\n");
    printf("  4. Realizar transferencia\n");
    printf("  5. Ver movimientos y exportar comprobantes\n");
    printf("  6. Consultar datos personales y alias\n");
    printf("  0. Cerrar sesion\n\n");
    printf("==================================================\n");
}

///////////////////////INICIAR SESION////////////////////////////

void iniciarSesion(Cliente listUser[], int *cantUsuarios)
{

    int posActual, intentos;
    cadena usuario, clave;
    bool usuarioValido, esAdmin = false, claveCorrecta = false;

    do
    {
        cabecera();
        intentos = 0;
        usuarioValido = false;

        do
        {
            leerCadena("-> Usuario / Alias: ", usuario);
            esAdmin = (strcmp(usuario, ADMIN) == 0);

            posActual = esAdmin ? USUARIO_INEXISTENTE : buscarDato(listUser, *cantUsuarios, usuario, obtenerAlias);

            if (esAdmin || posActual != USUARIO_INEXISTENTE)
            {
                usuarioValido = true;
            }
            else
            {
                puts("[!] Usuario inexistente. Intente nuevamente.\n");
            }

        } while (!usuarioValido);

        if (esAdmin)
        {
            do
            {
                leerCadena("-> Clave de Administrador:", clave);
                claveCorrecta = (strcmp(clave, ADMIN) == 0);
                if (!claveCorrecta)
                {
                    intentos++;
                    printf("  [!] Clave incorrecta (%d/%d intentos).\n", intentos, INTENTO_MAXIMO);
                }
            } while (!claveCorrecta && intentos < INTENTO_MAXIMO);

            if (claveCorrecta)
            {
                menuGestionarPersonal(listUser, cantUsuarios);
            }
        }
        else
        {
            claveCorrecta = verificacionClave(listUser[posActual]);
            if (claveCorrecta)
            {
                if (listUser[posActual].datosBanco.rol == CLIENTE)
                {
                    menuGestionDeCliente(listUser, posActual, *cantUsuarios);
                }
                else
                {
                    menuGestionarPersonal(listUser, cantUsuarios);
                }
            }
        }

        if (!claveCorrecta)
        {
            puts("\n  [ALERTA] Limite de intentos alcanzado. Espere 3 segundos...");
            Sleep(3000);
            system("cls");
        }
    } while (!claveCorrecta);
}

void menuGestionDeCliente(Cliente listClientes[], int posicionActual, int cantUsers)
{
    printf("\n");

    int mesSimulado = leerEnteroEntre(ENERO, DICIEMBRE, "  -> Ingresar mes de operacion [1-12]:");
    system("cls");
    menuVistaCliente(listClientes, posicionActual);
    int opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_CLIENT, "  Seleccione una opcion: ");

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
            opcionTransferencia(listClientes, cantUsers, posicionActual, mesSimulado);
            break;
        case 5:
            verMovimientos(&listClientes[posicionActual]);
            break;
        case 6:
            consultarDatosPersonales(listClientes[posicionActual]);
            break;

        default:
            puts("Gracias vuelvas prontos");
            break;
        }
        limpiarPantalla();
        menuVistaCliente(listClientes, posicionActual);
        opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_CLIENT, "  Seleccione una opcion: ");
    }
}

void menuGestionarPersonal(Cliente list[], int *cantUsuarios)
{
    int opcion = 0;

    vistaGestionPersonal();
    opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_CLIENT, "  Seleccione una opcion: ");
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
        case 5:
            listaBajas();
            break;
        case 6:
            generarReporteSistema(list, *cantUsuarios);
            break;
        }

        limpiarPantalla();
        vistaGestionPersonal();
        opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_PERS, "  Seleccione una opcion: ");
    }
}

void consultarSaldo(Cliente clienteActual)
{
    printf("\n  --------------------------------------------------\n");
    printf("  Saldo disponible en cuenta: $%.2f\n", clienteActual.datosBanco.saldo);
    printf("  Limite diario de extraccion restante: $%.2f\n", clienteActual.datosBanco.limiteExtraccion);
    printf("  --------------------------------------------------\n");
}

void ingresarDinero(Cliente *clienteActual, int mesSimulado)
{
    system("cls");
    printf("\n================ DEPOSITO DE DINERO ================\n");
    printf("  Monto minimo: $%.2f | Monto maximo: $%.2f\n\n", MONTO_INGRESO_MIN, MONTO_INGRESO_MAX);

    float montoIngresado = leerFloatEntre(MONTO_INGRESO_MIN,
                                          MONTO_INGRESO_MAX,
                                          "  -> Monto a depositar: $",
                                          "  [!] No alcanza el monto minimo de deposito.",
                                          "  [!] Supera el monto maximo permitido por operacion.");

    if (confirmaUsuario("Desea confirmar el deposito? [Si/No]:"))
    {
        (*clienteActual).datosBanco.saldo += montoIngresado;
        clienteActual->datosBanco.movMensuales[mesSimulado - 1][DEPOSITOS] += montoIngresado;
        puts("\n  [OK] Deposito acreditado exitosamente.");
        consultarSaldo(*clienteActual);
    }
    else
    {
        puts("\n  [INFO] Operacion cancelada por el usuario.");
    }
}

void retirarDinero(Cliente *clienteActual, int mesSimulado)
{
    system("cls");
    printf("\n=============== EXTRACCION DE DINERO ===============\n");

    float maximoRetiro = clienteActual->datosBanco.saldo;
    
    if (clienteActual->datosBanco.limiteExtraccion < maximoRetiro)
    {
        maximoRetiro = clienteActual->datosBanco.limiteExtraccion;
    }

    if (maximoRetiro < MONTO_RETIRO_MIN)
    {
        mensajeErrorExtraccion(clienteActual);
    }
    else
    {
        printf("  Saldo actual: $%.2f | Limite diario disponible: $%.2f\n\n",
               clienteActual->datosBanco.saldo, clienteActual->datosBanco.limiteExtraccion);

        float montoRetirado = leerFloatEntre(MONTO_RETIRO_MIN, maximoRetiro,
        "  Ingresar 0 para cancelar -> Monto a retirar: $",
        "  [!] El monto no alcanza el minimo de extraccion ($1000).",
        "  [!] El monto supera su saldo disponible o su limite diario.");
        
        if (montoRetirado != OPC_SALIDA)
        {
            confirmarExtraccion(clienteActual, montoRetirado, mesSimulado);
        }
        
    }
}

void mensajeErrorExtraccion(Cliente *clienteActual)
{
    if (clienteActual->datosBanco.saldo < MONTO_RETIRO_MIN)
    {
        puts("  [!] Saldo insuficiente para realizar una extraccion minima.");
        consultarSaldo(*clienteActual);
        printf("  Monto minimo requerido: $%.2f\n", MONTO_RETIRO_MIN);
    }
    else if (clienteActual->datosBanco.limiteExtraccion < MONTO_RETIRO_MIN)
    {
        puts("  [!] Limite de extraccion diario insuficiente o agotado.");
    }
}

void confirmarExtraccion(Cliente *clienteActual, float montoRetirado, int mesSimulado)
{
    if (confirmaUsuario("Desea confirmar la extraccion? [Si/No]:"))
    {
        (*clienteActual).datosBanco.saldo -= montoRetirado;
        (*clienteActual).datosBanco.limiteExtraccion -= montoRetirado;
        clienteActual->datosBanco.movMensuales[mesSimulado - 1][RETIROS] += montoRetirado;
        puts("\n  [OK] Extraccion realizada con exito.");
        consultarSaldo(*clienteActual);
    }
    else
    {
        puts("\n  [INFO] Operacion cancelada por el usuario.");
    }
}

void opcionTransferencia(Cliente listUser[], int cantUsuarios, int posUser, int mes)
{
    cadena aliasDestinatario;
    float saldo;
    int posDestino;

    system("cls");
    printf("\n============== TRANSFERENCIA BANCARIA ==============\n");
    printf("  Saldo disponible: $%.2f\n\n", listUser[posUser].datosBanco.saldo);

    if (listUser[posUser].datosBanco.saldo >= MIN_TRANSFERENCIA)
    {
        leerCadena("  -> Ingresar Alias del destinatario: ", aliasDestinatario);
        posDestino = buscarDato(listUser, cantUsuarios, aliasDestinatario, obtenerAlias);

        if (posDestino != USUARIO_INEXISTENTE)
        {
            if (posDestino == posUser)
            {
                puts("\n  [!] Error: No puede transferir dinero a su propia cuenta.");
            }
            else
            {
                printf("  Destinatario: %s %s\n",
                       listUser[posDestino].datosPersonales.nombre,
                       listUser[posDestino].datosPersonales.apellido);
                saldo = cuantoTransferir(listUser[posUser]);

                if (confirmaUsuario("Confirmar transferencia bancaria? [Si/No]:"))
                {
                    if (realizarTransferencia(&listUser[posUser], &listUser[posDestino], saldo, mes))
                    {
                        puts("\n  [OK] Transferencia enviada y registrada con exito.");
                    }
                    else
                    {
                        puts("\n  [ERROR] No se pudo procesar el registro de la transferencia.");
                    }
                }
                else
                {
                    puts("\n  [INFO] Transferencia cancelada.");
                }
                consultarSaldo(listUser[posUser]);
            }
        }
        else
        {
            puts("\n  [!] El alias ingresado no pertenece a ningun cliente activo.");
        }
    }
    else
    {
        puts("  [!] Saldo insuficiente para iniciar una transferencia.");
        consultarSaldo(listUser[posUser]);
    }
}

bool realizarTransferencia(Cliente *origen, Cliente *destino, float monto, int mes)
{
    bool todoOk = false;
    Movimientos reg[2];

    reg[0] = crearMovimientos(*origen, monto, mes);
    strcpy(reg[0].aliasDestino, destino->datosBanco.alias);

    reg[1] = crearMovimientos(*destino, monto, mes);
    strcpy(reg[1].aliasDestino, origen->datosBanco.alias);

    todoOk = registrarMovimientos(reg, 2);
    if (todoOk)
    {
        (*origen).datosBanco.saldo -= monto;
        (*destino).datosBanco.saldo += monto;

        origen->datosBanco.movMensuales[mes - 1][RETIROS] += monto;
        destino->datosBanco.movMensuales[mes - 1][DEPOSITOS] += monto;
    }

    return todoOk;
}

Movimientos crearMovimientos(Cliente cliente, float monto, int mes)
{
    Movimientos reg;

    strcpy(reg.alias, cliente.datosBanco.alias);
    reg.monto = monto;
    reg.mes = mes;

    return reg;
}

float cuantoTransferir(Cliente usuario)
{
    return leerFloatEntre(MIN_TRANSFERENCIA, usuario.datosBanco.saldo,
                          "  -> Monto a transferir: $",
                          "  [!] Monto invalido. No alcanza el minimo ($1.00).",
                          "  [!] Saldo insuficiente para cubrir ese monto.");
}

void verMovimientos(Cliente *cliente)
{
    cadena nombresMeses[CANT_MESES] = {
        "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    system("cls");
    printf("\n============= CONSULTA DE MOVIMIENTOS ==============\n\n");
    printf("  1. Consultar total de Depositos por mes\n");
    printf("  2. Consultar total de Retiros/Transferencias enviadas por mes\n");
    printf("  3. Exportar comprobante de transferencias (TXT)\n\n");

    int opcion = leerEnteroEntre(1, 3, "  Seleccione una opcion: ");

    if (opcion == 3)
    {
        generarReporteCliente(*cliente);
    }
    else
    {
        int mes = leerEnteroEntre(ENERO, DICIEMBRE, "  -> Ingrese el mes a consultar [1-12]: ") - 1;
        printf("\n  --------------------------------------------------\n");

        switch (opcion + 1)
        {
        case DEPOSITOS:
            printf("  Total depositado en %s: +$%.2f\n",
                   nombresMeses[mes],
                   cliente->datosBanco.movMensuales[mes][DEPOSITOS]);
            break;
        case RETIROS:
            printf("  Total retirado/transferido en %s: -$%.2f\n",
                   nombresMeses[mes],
                   cliente->datosBanco.movMensuales[mes][RETIROS]);
            break;
        default:
            break;
        }
        printf("  --------------------------------------------------\n");
    }
}

void consultarDatosPersonales(Cliente clienteActual)
{
    system("cls");
    printf("\n================ FICHA DEL CLIENTE =================\n");
    printf("  Nombre completo : %s %s\n",
           clienteActual.datosPersonales.nombre,
           clienteActual.datosPersonales.apellido);
    printf("  Documento (DNI) : %s\n", clienteActual.datosPersonales.DNI);
    printf("  Alias bancario  : %s\n", clienteActual.datosBanco.alias);
    printf("  Segmento        : %s\n", clienteActual.datosBanco.esPyme ? "Cuenta Pyme" : "Cuenta Individuo");
    printf("====================================================\n");
}

/* ========================================================================
 *  3. GESTION DE USUARIOS (CRUD ADMINISTRADOR)
 * ======================================================================== */

void altaPersonal(Cliente baseDatos[CANT_USERS], int *cantUsuarios)
{
    system("cls");
    Cliente newCliente;
    int opciones = 1;

    if (*cantUsuarios < CANT_USERS)
    {
        printf("========== ALTA DE PERSONAL ==========\n");
        newCliente = nuevoUsuario(baseDatos, *cantUsuarios, &opciones);
        if (opciones >= CANTIDAD_DATOS)
        {
            agregarUsuario(newCliente, baseDatos, cantUsuarios);
            printf("\nUsuario registrado correctamente.");
            printf("\nNombre y apellido: %s %s", newCliente.datosPersonales.nombre,
                   newCliente.datosPersonales.apellido);
            printf("\nAlias: %s\n", newCliente.datosBanco.alias);
        }
        else
        {
            system("cls");
            puts("Operacion cancelada.");
        }
    }
    else
    {
        printf("No se puede registrar el usuario.\n");
        printf("No hay mas espacio\n");
        printf("Se alcanzo el limite maximo.\n");
    }
}

Cliente nuevoUsuario(Cliente baseDatos[CANT_USERS], int cantUsuarios, int *opciones)
{
    Cliente nuevo = {0};
    bool correcto = false;

    while (OPC_SALIDA != *opciones && *opciones <= CANTIDAD_DATOS)
    {
        switch (*opciones)
        {
        case 1:
            correcto = leerCampoValido("Para cancelar ingresar 0.\n\nIngresar nombre: ", "nombre", nuevo.datosPersonales.nombre);
            break;
        case 2:
            correcto = leerCampoValido("Ingresar Apellido: ", "apellido", nuevo.datosPersonales.apellido);
            break;
        case 3:
            leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, "Ingresar DNI: ", nuevo.datosPersonales.DNI);
            if (strcmp(nuevo.datosPersonales.DNI, "0") == 0)
            {
                correcto = false;
            }
            else
            {
                verificarDatoExistente(nuevo.datosPersonales.DNI, "DNI", baseDatos, cantUsuarios, obtenerDNI, verificarDNI, LONG_MIN_DNI, LONG_MAX_DNI);
                correcto = true;
            }
            break;
        case 4:
            leerCadenaEntre(LONG_MIN_ALIAS, LONG_MAX_ALIAS, "Ingresar alias: ", nuevo.datosBanco.alias);
            if (strcmp(nuevo.datosBanco.alias, "0") == 0)
            {
                correcto = false;
            }
            else
            {
                verificarDatoExistente(nuevo.datosBanco.alias, "alias", baseDatos, cantUsuarios, obtenerAlias, verificarAlias, LONG_MIN_ALIAS, LONG_MAX_ALIAS);
                correcto = true;
            }
            break;
        case 5:
            leerCadenaEntre(LONG_MIN_CLAVE, LONG_MAX_CLAVE, "Ingrese clave: ", nuevo.datosBanco.clave);
            correcto = strcmp(nuevo.datosBanco.clave, "0") != 0;
            break;
        case 6:
            correcto = esClientePyme(&nuevo.datosBanco.esPyme);
            break;
        case 7:
            nuevo.datosBanco.rol = CLIENTE;
            nuevo.datosBanco.limiteExtraccion = LIMITE_EXTRACION;
            nuevo.datosBanco.saldo = 0;
            correcto = true;
            break;
        }
        correcto ? (*opciones)++ : ((*opciones) = OPC_SALIDA);
    }

    return nuevo;
}

bool leerCampoValido(cadena mensaje, cadena nombreCampo, cadena dato)
{
    bool correcto = false;
    bool regresar = true;

    do
    {
        leerCadena(mensaje, dato);
        if (strcmp(dato, "0") != 0)
        {
            correcto = verificarCadena(dato);
            if (!correcto)
            {
                printf("'%s' es un %s invalido, intente nuevamente.\n", dato, nombreCampo);
            }
        }
        else
        {
            regresar = OPC_SALIDA;
            correcto = true;
        }
    } while (!correcto);

    cadenaAPascal(dato);
    return regresar;
}

void agregarUsuario(Cliente new, Cliente baseDatos[CANT_USERS], int *cantUsuarios)
{
    baseDatos[*cantUsuarios] = new;

    (*cantUsuarios)++;
}

void listaUsuarios(Cliente baseDatos[], int *cantUsuarios)
{
    system("cls");
    printf("\n============== NOMINA DE CLIENTES ACTIVOS ==============\n\n");
    if (*cantUsuarios > VACIO)
    {
        ordenarUsuarios(baseDatos, *cantUsuarios);
        mostrarListadUsuarios(baseDatos, *cantUsuarios);
    }
    else
    {
        puts("  [INFO] No hay usuarios registrados en el sistema.");
    }
}

void mostrarListadUsuarios(Cliente baseDatos[CANT_USERS], int cant)
{
    for (int i = 0; i < cant; i++)
    {
        printf("  %2d. %-15s %-15s | DNI: %-8s | Alias: %-15s | $%.2f\n",
               i + 1,
               baseDatos[i].datosPersonales.apellido,
               baseDatos[i].datosPersonales.nombre,
               baseDatos[i].datosPersonales.DNI,
               baseDatos[i].datosBanco.alias,
               baseDatos[i].datosBanco.saldo);
        printf("----------------------------------------------------------------------------\n");
    }
}

void modificarUsuario(Cliente listUser[CANT_USERS], int *cantUsuarios)
{
    system("cls");
    printf("\n============= MODIFICACION DE CLIENTE ==============\n\n");
    if (*cantUsuarios > VACIO)
    {

        int opc = 0;
        int posUsuario = posicionPorDNI(listUser, *cantUsuarios);
        bool nuevoTipo;

        if (posUsuario != USUARIO_INEXISTENTE)
        {
            do
            {
                system("cls");
                printf("\n============= MODIFICANDO CLIENTE ==============\n");
                printf("  Titular : %s %s\n",
                       listUser[posUsuario].datosPersonales.nombre,
                       listUser[posUsuario].datosPersonales.apellido);
                printf("DNI     : %s | Tipo: %s\n",
                       listUser[posUsuario].datosPersonales.DNI,
                       listUser[posUsuario].datosBanco.esPyme ? "Pyme" : "Cliente Normal");
                printf("------------------------------------------------\n");
                printf("  1. Modificar Nombre\n");
                printf("  2. Modificar Apellido\n");
                printf("  3. Modificar DNI\n");
                printf("  4. Modificar Tipo de Cliente (Pyme / Normal)\n");
                printf("  0. Volver al menu anterior\n\n");

                opc = leerEnteroEntre(OPC_SALIDA, OPC_MODIFICAR_USU, "  Seleccione una opcion: ");

                switch (opc)
                {
                case 1:
                    cambiarDatoDeUsuario("  -> Ingresar nuevo nombre: ", posUsuario, listUser, OPC_NOMBRE, *cantUsuarios);
                    system("pause");
                    break;
                case 2:
                    cambiarDatoDeUsuario("  -> Ingresar nuevo apellido: ", posUsuario, listUser, OPC_APELLIDO, *cantUsuarios);
                    system("pause");
                    break;
                case 3:
                    cambiarDatoDeUsuario("  -> Ingresar nuevo DNI: ", posUsuario, listUser, OPC_DNI, *cantUsuarios);
                    system("pause");
                    break;
                case 4:
                    printf("Tipo actual: %s\n", listUser[posUsuario].datosBanco.esPyme ? "Pyme" : "Normal");
                    nuevoTipo = listUser[posUsuario].datosBanco.esPyme;
                    esClientePyme(&nuevoTipo);

                    if (confirmaUsuario("Desea aplicar el cambio de categoria? [Si/No]:"))
                    {
                        listUser[posUsuario].datosBanco.esPyme = nuevoTipo;
                        puts("\n  [OK] Tipo de cliente actualizado exitosamente.");
                    }
                    else
                    {
                        puts("\n  [INFO] Cambio de tipo cancelado.");
                    }
                    system("pause");
                    break;
                }

            } while (opc != OPC_SALIDA);
        }
        else
        {
            printf("\n  [!] No existe ningun cliente registrado con ese DNI.");
        }
    }
    else
    {
        puts("  [INFO] No hay usuarios registrados en el sistema.");
    }
}

void cambiarDatoDeUsuario(cadena texto, int posUsuario, Cliente usuario[], int tipodDato, int cantidad)
{
    cadena nomDato;
    bool formatoOk = false;

    switch (tipodDato)
    {
    case OPC_NOMBRE:
        do
        {
            leerCadenaEntre(LONG_MIN_NOM, LONG_MAX_NOM, texto, nomDato);
            formatoOk = verificarNombre(nomDato);
            if (!formatoOk)
            {
                puts("  [!] Nombre invalido. Solo se permiten letras.");
            }

        } while (!formatoOk);
        cadenaAPascal(nomDato);

        if (confirmaUsuario("Confirma la actualizacion del nombre? [Si/No]:"))
        {
            strcpy(usuario[posUsuario].datosPersonales.nombre, nomDato);
            puts("\n  [OK] Nombre actualizado correctamente.");
        }
        else
        {
            puts("\n  [INFO] Modificacion cancelada.");
        }
        break;

    case OPC_APELLIDO:
        do
        {
            leerCadenaEntre(LONG_MIN_APE, LONG_MAX_APE, texto, nomDato);
            formatoOk = verificarNombre(nomDato);
            if (!formatoOk)
            {
                puts("  [!] Apellido invalido. Solo se permiten letras.");
            }

        } while (!formatoOk);
        cadenaAPascal(nomDato);

        if (confirmaUsuario("Confirma la actualizacion del apellido? [Si/No]:"))
        {
            strcpy(usuario[posUsuario].datosPersonales.apellido, nomDato);
            puts("\n  [OK] Apellido actualizado correctamente.");
        }
        else
        {
            puts("\n  [INFO] Modificacion cancelada.");
        }
        break;

    case OPC_DNI:
        leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, texto, nomDato);
        verificarDatoExistente(nomDato, "DNI", usuario, cantidad, obtenerDNI, verificarDNI, LONG_MIN_DNI, LONG_MAX_DNI);
        if (confirmaUsuario("Confirma la actualizacion del DNI? [Si/No]:"))
        {
            strcpy(usuario[posUsuario].datosPersonales.DNI, nomDato);
            puts("\n  [OK] DNI actualizado correctamente.");
        }
        else
        {
            puts("\n  [INFO] Modificacion cancelada.");
        }
        break;
    }
}

void bajaUsuario(Cliente listUser[], int *cantUsuarios)
{
    system("cls");
    printf("\n================ BAJA DE CLIENTE ===================\n\n");
    if (*cantUsuarios > VACIO)
    {
        int posUsuario = posicionPorDNI(listUser, *cantUsuarios);

        if (posUsuario != USUARIO_INEXISTENTE)
        {
            printf("\n  Verificacion de seguridad para dar de baja a %s %s:",
                   listUser[posUsuario].datosPersonales.nombre,
                   listUser[posUsuario].datosPersonales.apellido);

            if (verificacionClave(listUser[posUsuario]))
            {
                confirmarEliminacion(listUser, posUsuario, cantUsuarios);
            }
            else
            {
                printf("\n  [!] Cantidad maxima de intentos superada. Baja cancelada.");
            }
        }
        else
        {

            printf("\n  [!] El DNI ingresado no corresponde a ningun usuario activo.");
        }
    }
    else
    {
        printf("  [INFO] No hay usuarios registrados en el sistema.");
    }
    system("pause");
}

void confirmarEliminacion(Cliente listUser[], int posUsuario, int *cantUsuarios)
{
    printf("\n  Usuario seleccionado: %s %s (DNI: %s)\n",
           listUser[posUsuario].datosPersonales.nombre,
           listUser[posUsuario].datosPersonales.apellido,
           listUser[posUsuario].datosPersonales.DNI);

    if (confirmaUsuario("Esta seguro que desea eliminar este usuario? [Si/No]:"))
    {
        if (registrarBaja(&listUser[posUsuario]))
        {
            eliminarUsuario(listUser, posUsuario, cantUsuarios);
            puts("\n  [OK] Usuario dado de baja y archivado en bajas.dat con exito.");
        }
        else
        {
            puts("\n  [ERROR] No se pudo registrar la baja en el archivo historico.");
        }
    }
    else
    {
        printf("\n  [INFO] Operacion de baja cancelada.");
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
 *  4. BUSQUEDA, ORDENAMIENTO Y VALIDACIONES
 * ======================================================================== */

int posicionPorDNI(Cliente listUser[], int cantUsuarios)
{
    cadena dniAux;
    leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, "  -> Ingresar DNI del cliente: ", dniAux);
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

void verificarDatoExistente(cadena datoBuscado, cadena nomDato, Cliente baseDatos[CANT_USERS], int cantUsuarios, ObtenerCampo campoBuscado, ReglaFormato funcionCondicional, int longMin, int longMax)
{
    bool valido = false;

    while (!valido)
    {
        if (!funcionCondicional(datoBuscado))
        {
            printf("  [!] El %s ingresado tiene un formato invalido.\n", nomDato);
        }
        else if (buscarDato(baseDatos, cantUsuarios, datoBuscado, campoBuscado) != USUARIO_INEXISTENTE)
        {
            printf("  [!] El %s ingresado ya se encuentra registrado.\n", nomDato);
        }
        else
        {
            valido = true;
        }
        if (!valido)
        {
            printf("  -> Ingresar %s nuevamente", nomDato);
            leerCadenaEntre(longMin, longMax, ": ", datoBuscado);
        }
    }
}

bool verificacionClave(Cliente usuario)
{
    int contadorIntentos = 0;
    bool respuesta = false;
    cadena claveAux;

    do
    {
        leerCadena("\n  -> Ingrese la clave del cliente: ", claveAux);
        respuesta = strcmp(usuario.datosBanco.clave, claveAux) == 0;

        if (!respuesta)
        {
            contadorIntentos++;
            printf("  [!] Clave incorrecta (%d/%d intentos).\n", contadorIntentos, INTENTO_MAXIMO);
        }
    } while (!respuesta && contadorIntentos < INTENTO_MAXIMO);

    return respuesta;
}

bool esClientePyme(bool *esPyme)
{
    cadena pyme;
    bool respondio = true;

    leerCadena("Es pyme: ", pyme);
    cadenaAPascal(pyme);

    while (strcmp(pyme, "Si") != 0 && strcmp(pyme, "No") != 0 && strcmp(pyme, "0") != 0)
    {
        printf("Respuesta invalida. Ingrese Si o No (0 para volver).\n");
        leerCadena("Es pyme: ", pyme);
        cadenaAPascal(pyme);
    }

    if (strcmp(pyme, "0") == 0)
    {
        respondio = false;
    }
    else
    {
        *esPyme = (strcmp(pyme, "Si") == 0);
    }
    return respondio;
}

bool verificarCadena(cadena texto)
{
    int posicion = 0;
    bool valido = (texto[posicion] != '\0') && (texto[posicion] != ' ');

    while (valido && texto[posicion] != '\0')
    {
        if (isalpha((unsigned char)texto[posicion]))
        {
            valido = true;
        }
        else
        {
            valido = false;
        }
        if (texto[posicion] == ' ')
        {
            valido = (texto[posicion + 1] != ' ') && (texto[posicion + 1] != '\0');
        }

        posicion++;
    }
    return valido;
}

bool verificarAlias(cadena texto)
{
    int posicion = 0;
    bool valido = (texto[0] != '\0') && (texto[0] != ' ');

    while (valido && texto[posicion] != '\0')
    {
        valido = texto[posicion] == '.' || texto[posicion] == '-' || isalnum((unsigned char)texto[posicion]);
        posicion++;
    }
    return valido;
}

bool verificarNombre(cadena texto)
{
    return verificarCadena(texto);
}

bool verificarCaracter(char c, char cInicial, char cFinal)
{
    return (c >= cInicial && c <= cFinal);
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
        valido = verificarCaracter(dni[posicion], '0', '9');
        posicion++;
    }
    return valido;
}

void ordenarUsuarios(Cliente baseDatos[], int cantUsuarios)
{
    Cliente aux;

    for (int i = 0; i < cantUsuarios - 1; i++)
    {
        for (int j = i + 1; j < cantUsuarios; j++)
        {
            if (strcmp(baseDatos[i].datosPersonales.apellido, baseDatos[j].datosPersonales.apellido) > 0)
            {
                aux = baseDatos[i];
                baseDatos[i] = baseDatos[j];
                baseDatos[j] = aux;
            }
        }
    }
}

/* ========================================================================
 *  5. GETTERS (ACCESO A CAMPOS PARA PUNTEROS A FUNCION)
 * ======================================================================== */

const char *obtenerDNI(const Cliente *c)
{
    return c->datosPersonales.DNI;
}

const char *obtenerAlias(const Cliente *c)
{
    return c->datosBanco.alias;
}

/* ========================================================================
 *  6. PERSISTENCIA Y ARCHIVOS (.DAT Y .TXT)
 * ======================================================================== */

int cargarUsuarios(Cliente listUser[]) // la cantidad se lee desde el archivo, no se necesita pasar por parametro
{
    int cantidad = -1;
    FILE *archivo = fopen(RUTA_USUARIOS, "rb");

    if (archivo != NULL)
    {
        if (fread(&cantidad, sizeof(int), 1, archivo) == 1)
        {
            if (cantidad >= 0 && cantidad <= CANT_USERS) // verifica que la cantidad no sea negativa y supere la cantidad de usuarios
            {
                if (fread(listUser, sizeof(Cliente), cantidad, archivo) == (size_t)cantidad)
                {
                    printf("[SISTEMA] Base de datos cargada exitosamente (%d clientes activos).\n", cantidad);
                }
                else
                {
                    puts("[ERROR] Lectura incompleta de registros de clientes.");
                    cantidad = -1;
                }
            }
            else
            {
                puts("[ERROR] Cabecera de cantidad invalida en el archivo.");
                cantidad = -1;
            }
        }
        else
        {
            puts("[ERROR] No se pudo leer la cabecera del archivo de usuarios.");
        }
        fclose(archivo); // siempre va cuando el archivo se abre correctamente
    }
    else if (errno == ENOENT)
    {
        puts("[ERROR] Fallo del sistema al intentar abrir Usuarios.dat.");
    }

    return cantidad;
}

void guardarUsuarios(Cliente listUser[], int cant)
{
    FILE *archivo = fopen(RUTA_USUARIOS, "wb");

    if (archivo != NULL)
    {
        // sirve para escribir la cantidad de usuarios, si no, no se puede guardar el vector
        if (fwrite(&cant, sizeof(int), 1, archivo) == 1)
        {
            int escritos = fwrite(listUser, sizeof(Cliente), cant, archivo);
            if (escritos == cant)
            {
                puts("[SISTEMA] Cambios sincronizados en disco (Usuarios.dat).");
            }
            else
            {
                puts("[ERROR] Fallo al escribir el bloque de usuarios en disco.");
            }
        }
        else
        {
            puts("[ERROR] No se pudo escribir la cabecera de cantidad en disco.");
        }
        fclose(archivo);
    }
    else
    {
        puts("[ERROR] No se pudo abrir Usuarios.dat para escritura.");
    }
}

bool registrarMovimientos(Movimientos reg[], int cant)
{
    bool funciona = false;
    FILE *f = fopen(RUTA_MOVIMIENTOS, "ab");

    if (f != NULL)
    {
        funciona = (fwrite(reg, sizeof(Movimientos), cant, f) == (size_t)cant);
        fclose(f);
    }
    else
    {
        perror("[ERROR] No se pudo abrir movimientos.dat");
    }
    return funciona;
}

bool registrarBaja(Cliente *eliminado)
{
    bool todoOk = false;
    FILE *f = fopen(RUTA_BAJAS, "ab");

    if (f != NULL)
    {
        todoOk = (fwrite(eliminado, sizeof(Cliente), 1, f) == 1);
        fclose(f);
    }
    return todoOk;
}

void listaBajas(void)
{
    Cliente baja;
    int cont = 0;
    system("cls");
    printf("\n============== HISTORIAL DE BAJAS (ARCHIVO) ==============\n\n");

    FILE *f = fopen(RUTA_BAJAS, "rb");
    if (f != NULL)
    {
        while (fread(&baja, sizeof(Cliente), 1, f) == 1)
        {
            cont++;
            printf("%2d. Ex-Cliente: %-15s %-15s | DNI: %s\n",
                   cont,
                   baja.datosPersonales.apellido,
                   baja.datosPersonales.nombre,
                   baja.datosPersonales.DNI);
        }
        if (cont == 0)
        {
            puts("  [INFO] El archivo de bajas se encuentra vacio.");
        }
        fclose(f);
    }
    else
    {
        puts("  [INFO] Aun no existen bajas registradas en el sistema.");
    }
}

void generarReporteCliente(Cliente cliente)
{
    Movimientos reg;
    FILE *f, *txt;

    txt = fopen(RUTA_REPORTE_MOVIMIENTOS, "w");

    if (txt != NULL)
    {
        fprintf(txt, "========================================================\n");
        fprintf(txt, "       BANCO ABRAXAS - HISTORIAL DE TRANSFERENCIAS      \n");
        fprintf(txt, "========================================================\n");
        fprintf(txt, "Titular: %s %s | Alias: %s\n",
                cliente.datosPersonales.nombre,
                cliente.datosPersonales.apellido,
                cliente.datosBanco.alias);
        fprintf(txt, "--------------------------------------------------------\n\n");

        f = fopen(RUTA_MOVIMIENTOS, "rb");
        if (f != NULL)
        {
            while (fread(&reg, sizeof(Movimientos), 1, f) == 1)
            {
                imprimirMovimiento(txt, cliente, reg);
            }
            fclose(f);
        }
        fclose(txt);
        printf("\n  [OK] Comprobante exportado exitosamente en '%s'.\n", RUTA_REPORTE_MOVIMIENTOS);
    }
    else
    {
        puts("\n  [ERROR] No se pudo crear el archivo de reporte del cliente.");
    }
}

void imprimirMovimiento(FILE *txt, Cliente cliente, Movimientos reg)
{
    // Escribe solo los movimientos de este cliente.
    if (strcmp(reg.alias, cliente.datosBanco.alias) == 0)
    {
        fprintf(txt, "Cuenta Origen/Asociada : %s %s (%s)\n",
                cliente.datosPersonales.nombre,
                cliente.datosPersonales.apellido,
                reg.alias);
        fprintf(txt, "Alias Contraparte      : %s\n", reg.aliasDestino);
        fprintf(txt, "Mes de Operacion       : %d\n", reg.mes);
        fprintf(txt, "Monto Transferido      : $%.2f\n", reg.monto);
        fprintf(txt, "--------------------------------------------------------\n");
    }
}

void generarReporteSistema(Cliente listUser[], int cantUsuarios)
{
    FILE *f = fopen(RUTA_REPORTE_SISTEMA, "w");

    if (f != NULL)
    {
        ordenarUsuarios(listUser, cantUsuarios);

        fprintf(f, "========================================================\n");
        fprintf(f, "        REPORTE DE AUDITORIA - BANCO ABRAXAS            \n");
        fprintf(f, "========================================================\n");
        fprintf(f, "Total de clientes activos en el sistema: %d\n", cantUsuarios);
        fprintf(f, "--------------------------------------------------------\n\n");

        for (int i = 0; i < cantUsuarios; i++)
        {
            fprintf(f, "%d. %s, %s | DNI: %s | Alias: %s\n",
                    i + 1,
                    listUser[i].datosPersonales.apellido,
                    listUser[i].datosPersonales.nombre,
                    listUser[i].datosPersonales.DNI,
                    listUser[i].datosBanco.alias);
            fprintf(f, "   -> Saldo: $%.2f | Segmento: %s\n",
                    listUser[i].datosBanco.saldo,
                    listUser[i].datosBanco.esPyme ? "Pyme" : "Cliente normal");
            fprintf(f, "--------------------------------------------------------\n");
        }
        fclose(f);
        printf("\n  [OK] Reporte TXT generado exitosamente en '%s'.\n", RUTA_REPORTE_SISTEMA);
    }
    else
    {
        printf("\n  [ERROR] No se pudo crear el archivo de auditoria.\n");
    }
}

/* ========================================================================
 *  7. UTILIDADES
 * ======================================================================== */

void limpiarPantalla(void)
{
    system("pause");
    system("cls");
}