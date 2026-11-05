#include "utils.h"
#include <errno.h> //Para detecnar si el archivo no existe (ENOENT)
#include <direct.h>
#include <windows.h>

#define CANT_USERS 100
#define MAX_BAJAS 100

#define VACIO 0
#define OPC_SALIDA 0
#define OPC_MODIFICAR_USU 4
#define OPC_M_GEST_PERS 6
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
#define LONG_MIN_NOM 2
#define LONG_MAX_NOM 15
#define LONG_MIN_APE 2
#define LONG_MAX_APE 20
#define LONG_MIN_CLAVE 4
#define LONG_MAX_CLAVE 20

#define MONTO_RETIRO_MIN 1000.00
#define MONTO_INGRESO_MIN 1000.00
#define MONTO_INGRESO_MAX 1500000.00
#define LIMITE_EXTRACION 1000000.00
#define LIMITE_CERO 0.00
#define MIN_TRANSFERENCIA 1

#define CARPETA_DATA "../data"
#define RUTA_USUARIOS "../data/Usuarios.dat"
#define RUTA_MOVIMIENTOS "../data/movimientos.dat"
#define RUTA_BAJAS "../data/bajas.dat"
#define RUTA_REPORTES_CLIENTE "../data/reportesAdmin.txt"

#define ADMIN "admin"
#define SALDO 1
#define DEPOSITOS 2
#define RETIROS 3

#define CANT_MESES 12
#define CANT_TIPOS_MOVIMIENTO 4
#define ENERO 1
#define DICIEMBRE 12

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

/////////////////// PROTOTIPOS ///////////////////

/////////////////// MENUS Y SESION ///////////////////
void menuVistaCliente(Cliente datos[], int posUsuario);
void menuGestionDeCliente(Cliente listClientes[], int posicionActual, int cantUsers);
void iniciarSesion(Cliente listUser[], int *cantUsuarios);
void cabecera(void);
void vistaGestionPersonal(void);
void menuGestionarPersonal(Cliente list[], int *cantUsuarios);

/////////////////// OPERACIONES DEL CLIENTE ///////////////////
void ingresarDinero(Cliente *clienteActual, int mesSimulado);
void retirarDinero(Cliente *clienteActual, int mesSimulado);
void confirmarExtraccion(Cliente *clienteActual, float montoRetirado, int mesSimulado);
void evaluarExtraccion(Cliente *clienteActual, float *montoRetirado);
void mensajeErrorExtraccion(Cliente *clienteActual);
void consultarDatosPersonales(Cliente clienteActual);
void consultarSaldo(Cliente cliente);
void verMovimientos(Cliente *cliente);
bool realizarTransferencia(Cliente *origen, Cliente *destino, float monto, int mes);

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
float cuantoTransferir(Cliente usuario);

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
void guardarUsuarios(Cliente listUser[], int cant);
int cargarUsuarios(Cliente listUser[]);
bool registrarMovimientos(Movimientos reg[], int cant);
bool registrarBaja(Cliente *eliminado);
void listaBajas();
void generarReporteSistema(Cliente listUser[], int cantUsuarios);
void imprimirMovimiento(FILE *txt, Cliente cliente, Movimientos reg);

/////////////////// UTILIDADES ///////////////////
void limpiarPantalla(void);

///////////////////transferencias/////////////
void opcionTransferencia(Cliente listUser[], int cantUsuarios, int posUser, int mes);
Movimientos crearMovimientos(Cliente cliente, float monto, int mes);

int main(void)
{
    _mkdir(CARPETA_DATA);

    Cliente listUser[CANT_USERS];

    int cantClientes = 0;
    int cantidadLeida = cargarUsuarios(listUser);

    if (cantidadLeida >= 0)
    {
        cantClientes = cantidadLeida;
    }
    else
    {
        puts("No se pudieron cargar los usuarios.");
    }

    bool cerrarApp = false;
    system("pause");
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
    system("cls");
    printf("GESTION PERSONAL\n\n");
    printf("1. Alta de nuevo usuario\n"); // C
    printf("2. Ver lista de usuarios\n"); // R
    printf("3. Modificar usuario\n");     // U
    printf("4. Baja de usuario\n");       // D
    printf("5. Ver lista de bajas\n");
    puts("6. Generar reporte sistema (txt)");
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
    printf("  6. Consultar datos personales y alias\n");
    // printf("  5. Solicitar prestamo\n");
    printf("  0. Cerrar sesion\n");

    printf("\n==================================================\n");
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
            leerCadena("\nIngrese usuario: ", usuario);
            esAdmin = (strcmp(usuario, ADMIN) == 0);

            posActual = esAdmin ? USUARIO_INEXISTENTE : buscarDato(listUser, *cantUsuarios, usuario, obtenerAlias);

            usuarioValido = (esAdmin || posActual != USUARIO_INEXISTENTE) ? true : (puts("\nUsuario incorrecto, intente nuevamente"), false);

        } while (!usuarioValido);

        if (esAdmin)
        {
            do
            {
                leerCadena("Ingrese clave: ", clave);
                claveCorrecta = (strcmp(clave, ADMIN) == 0);
                if (!claveCorrecta)
                {
                    puts("\nClave incorrecta, ingrese nuevamente.");
                    intentos++;
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
                listUser[posActual].datosBanco.rol == CLIENTE ? menuGestionDeCliente(listUser, posActual, *cantUsuarios) : menuGestionarPersonal(listUser, cantUsuarios);
            }
        }

        if (!claveCorrecta)
        {
            puts("\nEspere 3 segundos para volver a intentar.");
            Sleep(3000);
            system("cls");
        }
    } while (!claveCorrecta);
}

void menuGestionDeCliente(Cliente listClientes[], int posicionActual, int cantUsers)
{
    int mesSimulado = leerEnteroEntre(ENERO, DICIEMBRE, "Ingresar mes simulado [1,12]: ");
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
        opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_CLIENT, "\nElija una opcion del menu: ");
    }
}

void opcionTransferencia(Cliente listUser[], int cantUsuarios, int posUser, int mes)
{

    cadena aliasDestinatario;
    float saldo;
    system("cls");
    int posDestino;

    printf("Saldo actual: $%.2f\n", listUser[posUser].datosBanco.saldo);

    if (listUser[posUser].datosBanco.saldo >= MIN_TRANSFERENCIA)
    {
        leerCadena("Ingresar Alias: ", aliasDestinatario);
        posDestino = buscarDato(listUser, cantUsuarios, aliasDestinatario, obtenerAlias);

        if (posDestino != USUARIO_INEXISTENTE)
        {
            if (posDestino == posUser)
            {
                puts("No podes transferirte a vos mismo.");
            }
            else
            {
                saldo = cuantoTransferir(listUser[posUser]);
                if (confirmaUsuario("Confirmar transferencia?:"))
                {
                    if (realizarTransferencia(&listUser[posUser], &listUser[posDestino], saldo, mes))
                    {
                        puts("Transferencia realizada con exito.");
                    }
                    else
                    {
                        puts("No se pudo.");
                    }
                }
                else
                {
                    puts("Transferencia cancelada.");
                }
                printf("Saldo actual: $%.2f\n", listUser[posUser].datosBanco.saldo);
            }
        }
        else
        {
            puts("El alias ingresado no existe.");
        }
    }
    else
    {
        puts("Saldo insuficiente.");
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
        perror("ERROR");
    }
    return funciona;
}

float cuantoTransferir(Cliente usuario)
{
    return leerFloatEntre(MIN_TRANSFERENCIA, usuario.datosBanco.saldo, "Monto de tranferecia: ", "Monto invalido. No alcanza el minimo", "Saldo insufiente.");
}

void verMovimientos(Cliente *cliente)
{
    cadena nombresMeses[CANT_MESES] = {
        "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    int mes = leerEnteroEntre(ENERO, DICIEMBRE, "Ingrese el mes que quiere consultar (1-12): ") - 1;

    int opcion = leerEnteroEntre(1, 2, "1. Depositos\n2. Retiros\nTipo de movimiento: ");

    switch (opcion + 1)
    {
    case DEPOSITOS:
        printf("Total de depositos de %s: $%.2f\n", nombresMeses[mes], cliente->datosBanco.movMensuales[mes][DEPOSITOS]);
        break;
    case RETIROS:
        printf("Total de retiros de %s: -$%.2f\n", nombresMeses[mes], cliente->datosBanco.movMensuales[mes][RETIROS]);
        break;
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
    printf("Saldo actual: $%.2f\n", clienteActual.datosBanco.saldo);
}

void ingresarDinero(Cliente *clienteActual, int mesSimulado)
{
    system("cls");
    printf("Indique la cantidad que desea ingresar, puede ingresar hasta $%.2f\n", MONTO_INGRESO_MAX);
    float montoIngresado = leerFloatEntre(MONTO_INGRESO_MIN, MONTO_INGRESO_MAX, "Monto: ", "No cumple con el monto minimo.", "Supera el monto maximo.");

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
        printf("Limite de extraccion por dia: $%.2f\n",
               clienteActual->datosBanco.limiteExtraccion);

        float montoRetirado = leerFloatEntre(
            MONTO_RETIRO_MIN,
            maximoRetiro,
            "Cuanto desea retirar?\n$",
            "No cumple con el minimo.",
            "Supera el saldo o el limite de extraccion.");

        confirmarExtraccion(clienteActual, montoRetirado, mesSimulado);
    }

}

void mensajeErrorExtraccion(Cliente *clienteActual)
{
    if (clienteActual->datosBanco.saldo < MONTO_RETIRO_MIN)
    {
        puts("Saldo insifuciente para realizar una extraccion minima.\n");
        consultarSaldo(*clienteActual);
        printf("Minimo para retirar es: %.2f\n", MONTO_RETIRO_MIN);
    }
    else if (clienteActual->datosBanco.limiteExtraccion < MONTO_RETIRO_MIN)
    {
        puts("Limite de extracion diario insuficiente o superado.");
    }
}

void confirmarExtraccion(Cliente *clienteActual, float montoRetirado, int mesSimulado)
{
    if (confirmaUsuario("Desea confirmar la operacion?"))
    {
        (*clienteActual).datosBanco.saldo -= montoRetirado;
        (*clienteActual).datosBanco.limiteExtraccion -= montoRetirado;
        clienteActual->datosBanco.movMensuales[mesSimulado - 1][RETIROS] += montoRetirado;
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
        *montoRetirado = leerFloatEntre(MONTO_RETIRO_MIN, clienteActual->datosBanco.saldo, "\nCuanto desea retirar?", "", "");
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
        case 5:
            listaBajas();
            break;
        case 6:
            generarReporteSistema(list, *cantUsuarios);
            break;
        }

        limpiarPantalla();
        vistaGestionPersonal();
        opcion = leerEnteroEntre(OPC_SALIDA, OPC_M_GEST_PERS, "Elija una opcion del menu: ");
    }
    guardarUsuarios(list, *cantUsuarios);
}

int cargarUsuarios(Cliente listUser[]) // la cantidad se lee desde el archivo, no se necesita pasar por parametro
{
    int cantidad = -1;
    FILE *archivo = fopen(RUTA_USUARIOS, "rb");

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
void guardarUsuarios(Cliente listUser[], int cant)
{
    FILE *archivo;

    archivo = fopen(RUTA_USUARIOS, "wb");

    if (archivo != NULL)
    {
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

    leerCadenaEntre(LONG_MIN_NOM, LONG_MAX_NOM, "Ingresar nombre: ", nuevo.datosPersonales.nombre);
    cadenaAPascal(nuevo.datosPersonales.nombre);
    bool letras = isalpha((unsigned char)alias[posicion]);
    leerCadenaEntre(LONG_MIN_APE, LONG_MAX_APE, "Ingrese apellido: ", nuevo.datosPersonales.apellido);
    cadenaAPascal(nuevo.datosPersonales.apellido);

    leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, "Ingresar DNI: ", nuevo.datosPersonales.DNI);
    verificarDatoExistente(nuevo.datosPersonales.DNI, "DNI", baseDatos, cantUsuarios, obtenerDNI, verificarDNI, LONG_MIN_DNI, LONG_MAX_DNI);

    leerCadenaEntre(LONG_MIN_ALIAS, LONG_MAX_ALIAS, "Ingresar alias: ", nuevo.datosBanco.alias);
    verificarDatoExistente(nuevo.datosBanco.alias, "alias", baseDatos, cantUsuarios, obtenerAlias, verificarAlias, LONG_MIN_ALIAS, LONG_MAX_ALIAS);

    leerCadenaEntre(LONG_MIN_CLAVE, LONG_MAX_CLAVE, "Ingrese clave: ", nuevo.datosBanco.clave);
    nuevo.datosBanco.esPyme = esClientePyme();
    nuevo.datosBanco.rol = CLIENTE;
    nuevo.datosBanco.limiteExtraccion = LIMITE_EXTRACION;
    nuevo.datosBanco.saldo = 0;

    return nuevo;
}

void verificarDatoPersonal(cadena datoPersonal){
    
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

void generarReporteCliente(Cliente cliente)
{

    Movimientos reg;
    FILE *f, *txt;

    txt = fopen(RUTA_REPORTES_CLIENTE, "w");

    if (txt != NULL)
    {

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
    }
    else
    {
        puts("No se pudo abrir el archivo");
    }
}

void imprimirMovimiento(FILE *txt, Cliente cliente, Movimientos reg)
{
    // Escribe solo los movimientos de este cliente.
    if (strcmp(reg.alias, cliente.datosBanco.alias) == 0)
    {
        fprintf(txt, "Cliente: %s %s\nDestino: %s\nMes: %d\nMonto: $%.2f\n\n",
                cliente.datosPersonales.nombre,
                cliente.datosPersonales.apellido,
                reg.aliasDestino,
                reg.mes,
                reg.monto);
    }
}

void generarReporteSistema(Cliente listUser[], int cantUsuarios)
{
    FILE *f = fopen(RUTA_REPORTES_CLIENTE, "w");

    if (f != NULL)
    {
        ordenarVectorDeCadenasAscendente(listUser, cantUsuarios);

        fprintf(f, "========== REPORTE DE SISTEMA - BANCO ABRAXAS ==========\n\n");
        fprintf(f, "Total de clientes activos en el sistema: %d\n\n", cantUsuarios);

        for (int i = 0; i < cantUsuarios; i++)
        {
            fprintf(f, "%d. %s %s | DNI: %s | Alias: %s\n", i + 1, listUser[i].datosPersonales.nombre,
                    listUser[i].datosPersonales.apellido,
                    listUser[i].datosPersonales.DNI, listUser[i].datosBanco.alias);
            fprintf(f, "-> Saldo: $%.2f | Tipo: %s\n", listUser[i].datosBanco.saldo,
                    listUser[i].datosBanco.esPyme  ? "Pyme" : "Cliente normal");
        }
        fclose(f);
        printf("\n[SISTEMA] Reporte TXT generado exitosamente en '%s'.\n", RUTA_REPORTES_CLIENTE);
    }
    else
    {
        printf("\n[ERROR] No se pudo crear el archivo de auditoria.\n");
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
                listUser[posUsuario].datosBanco.esPyme = esClientePyme();
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
        leerCadenaEntre(LONG_MIN_NOM, LONG_MAX_NOM, texto, nomDato);
        cadenaAPascal(nomDato);
        if (confirmaUsuario("Estas seguro?"))
        {
            strcpy(usuario[posUsuario].datosPersonales.nombre, nomDato);
        }
        break;

    case OPC_APELLIDO:
        leerCadenaEntre(LONG_MIN_APE, LONG_MAX_APE, texto, nomDato);
        cadenaAPascal(nomDato);
        if (confirmaUsuario("Estas seguro?"))
        {
            strcpy(usuario[posUsuario].datosPersonales.apellido, nomDato);
        }
        break;

    case OPC_DNI:
        leerCadenaEntre(LONG_MIN_DNI, LONG_MAX_DNI, texto, nomDato);
        verificarDatoExistente(nomDato, "DNI", usuario, cantidad, obtenerDNI, verificarDNI, LONG_MIN_DNI, LONG_MAX_DNI);
        if (confirmaUsuario("Estas seguro?"))
        {
            strcpy(usuario[posUsuario].datosPersonales.DNI, nomDato);
        }
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
            printf("\nCantidad maxima de intentos superada. Baja cancelada.");
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

    if (confirmaUsuario("Estas seguro de eliminar este usuario?"))
    {
        if (registrarBaja(&listUser[posUsuario]))
        {

            eliminarUsuario(listUser, posUsuario, cantUsuarios);
            printf("Usuario eliminado con exito.\n");
        }
        else
        {
            puts("No se pudo registrar la baja.");
        }
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

void listaBajas()
{
    Cliente baja;
    int cont = 0;
    FILE *f = fopen(RUTA_BAJAS, "rb");
    if (f != NULL)
    {

        while (fread(&baja, sizeof(Cliente), 1, f) == 1)
        {
            cont++;
            printf("%d. CLIENTE: %s %s\n", cont, baja.datosPersonales.nombre, baja.datosPersonales.apellido);
            printf("DNI %s\n\n", baja.datosPersonales.DNI);
        }
        fclose(f);
    }
    else
    {
        puts("\nNo hay bajas resgistradas");
    }
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

bool verificarCadena(cadena unaCadena, boolean esUnAlias)
{
    bool valido = (unaCadena[0] != '\0'); // el unaCadena vacío no es válido
    int posicion = 0;


    while (valido && unaCadena[posicion] != '\0')
    {
        valido = isalpha((unsigned char)unaCadena[posicion]);

        posicion++;
    }
    if (valido && esUnAlias)
    {
        while (valido && unaCadena[posicion] != '\0')
        {
            valido = isdigit((unsigned char)unaCadena[posicion]); 
            bool signos = verificaCaracter(unaCadena[posicion], '-', '.');
            valido = valido || signos; //verificar
            
            posicion++;
        }
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

// dejalo asi, no saques el fflush
void limpiarPantalla(void)
{
    fflush(stdin);
    system("pause");
    system("cls");
}