#include "utils_vAmpliada.h"
#include <windows.h>

#define OPC_MIN 1
#define OPC_MAX 5
#define CANT_USERS 4


/////////////////// FUNCIONES DEL CLIENTE ///////////////////

void menuVistaCliente();
void menuOpcCliente(int* saldo);
void consultarSaldo(int* saldoConsultado);

/////////////////// FUNCIONES DEL ADMINISTRADOR ///////////////////

void menuVistaAdmin();
void menuOpcAdmin(char vecUser[],int vecClaves[],int* cantUsuarios);
void menuGestionarPersonal(char vecUser[],int vecClaves[],int* cantUsuarios);
void vistaGestionPersonal();
void enEspera();

void altaPersonal(char vecUser[],int vecClaves[],int* cantUsuarios);
char validarUsuario();
int validarPass();

int main(){
    
    char listaUser[CANT_USERS] = {0};
    int vecClaves[CANT_USERS] = {0};
    int cantUsuarios = 0;
    
    menuVistaAdmin();
    menuOpcAdmin(listaUser, vecClaves, &cantUsuarios);
    
    return 0;
}

void menuOpcAdmin(char vecUser[],int vecClaves[],int* cantUsuarios){
     
    switch ( leerEnteroEntre(OPC_MIN, OPC_MAX,"Elija una opcion del menu: ") ){
    case 1:
        vistaGestionPersonal();
        menuGestionarPersonal(vecUser,vecClaves,cantUsuarios);
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
        // temporizador de 3 seg
        Sleep(3000);
        break;
    }
}


void menuGestionarPersonal(char vecUser[],int vecClaves[],int* cantUsuarios){
     
    switch ( leerEnteroEntre(OPC_MIN, OPC_MAX,"Elija una opcion del menu: ") ){
    case 1:
        altaPersonal(vecUser,vecClaves,cantUsuarios);
        
        break;
    case 2:
        
        break;
    case 3:
        
        break;
    case 4:
        
        break;
    case 5:
        system("cls");
        menuVistaAdmin();
        menuOpcAdmin(vecUser,vecClaves,cantUsuarios);
        break;
    }
}

// --------------------ALTA PERSONAL
void altaPersonal(char vecUser[],int vecClaves[],int* cantUsuarios){
    char usuNuevo;
    int claveNueva;
    bool usuExistente = false;
    
    if(*cantUsuarios < CANT_USERS){

        printf("========== ALTA DE PERSONAL ==========\n");
        
        
        do
        {
            usuNuevo = validarUsuario();
            usuExistente = buscador(usuNuevo,vecUser,cantUsuarios);
            if (usuExistente)
            {
                printf("El usuario ya existe elija otro.");


            }
        } while (usuExistente);
        
        claveNueva = validarPass();

        vecUser[*cantUsuarios+1] = usuNuevo;
        vecClaves[*cantUsuarios+1] = claveNueva;
        (*cantUsuarios)++;

        printf("\nUsuario registrado correctamente.");
        printf("\nUsuario: %c", usuNuevo);
        printf("\nClave: %d",claveNueva);
    }else{
        printf("No se puede registrar el usuario.");
        printf("No hay  mas espacio");
        printf("Se alcanzo el limite maximo.");
       
    }
    

}

int validarPass(){
    return leerEnteroEntre(1000,9999,"Ingresar clave nueva: ");
}
char validarUsuario(){
    return leerCaracter("Ingresar Usu: ");
}
void enEspera(){
    printf("Queda pendiente");
}

void vistaGestionPersonal(){
    //Vista de usuario
    printf("GESTION PERSONAL\n\n");
    printf("1. Alta\n");
    printf("2. Baja\n");
    printf("3. Modificar\n");
    printf("4. Consultar\n");
    printf("5. Volver\n\n");
}
void menuVistaAdmin(){
    //Vista de usuario
    printf("VISTA ADMINISTRADOR\n\n");
    printf("1. Gestionar personal\n");
    printf("2. Gestionar agentes\n");
    printf("3. Consultar clientes\n");
    printf("4. Consultar balance\n");
    printf("5. Cerrar sesion\n\n");
}

void menuVistaCliente(){
    //Vista de usuario
    printf("HOME BANKING\n\n");
    printf("1. Consultar Saldo\n");
    printf("2. Ralizar deposito\n");
    printf("3. Realizar retiro\n");
    printf("4. Transferir\n"); //Vamos viendo
    printf("5. Solicitar prestamos\n");
    printf("6. Ver movimientos\n"); //vamos viendo
    printf("7. CBU\n"); //no olvidarse de agregar Alias
    printf("8. Ver beneficios\n");
}
void menuOpcCliente(int* saldo){
    
    switch ( leerEnteroEntre(OPC_MIN, OPC_MAX,"Elija una opcion del menu: ") ){
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

void consultarSaldo(int* saldoConsultado){
    printf("El saldo actual es %d", *saldoConsultado );
}


