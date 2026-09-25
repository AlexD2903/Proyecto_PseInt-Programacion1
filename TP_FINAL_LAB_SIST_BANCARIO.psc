Algoritmo TP_FINAL_LAB_SISTEMA_BANCARIO
	// definir vectores de login y cantidades
	Definir vecUsuarios Como Caracter
	Definir vecClaves Como Caracter
	Definir vecRoles Como Entero
	Definir CANT_MAX_USUARIOS Como Entero
	Definir cant_Usuarios Como Entero
	Definir i Como Entero
	Definir posicionUsuario Como Entero
	Definir vecNombres Como Caracter
	Definir vecApellidos Como Caracter
	Definir vecDNI Como Caracter
	Definir vecLegajo Como Entero
	Definir vecSaldo Como Real
	Definir vecNumCliente Como Entero
	//
	Definir usuarioIngresado Como Caracter
	Definir claveIngresada Como Caracter
	//
	Definir GERENTE_PRINCIPAL Como Entero
	Definir AGENTE Como Entero
	Definir CLIENTE Como Entero
	Definir intentos Como Entero
	Definir MAX_INTENTOS Como Entero
	// variables menu acceso
	Definir accesoConcedido Como Logico
	Definir opcion, opcion1 Como Entero
	cant_Usuarios<- 3
	// variables constantes
	GERENTE_PRINCIPAL <- 1
	AGENTE<- 2
	CLIENTE<- 3
	MAX_INTENTOS <- 3
	CANT_MAX_USUARIOS<- 5
	intentos <- 0
	accesoConcedido <- Falso
	//vectore usuarios
	Dimension vecUsuarios[CANT_MAX_USUARIOS]
	Dimension vecClaves[CANT_MAX_USUARIOS]
	Dimension vecNombres[CANT_MAX_USUARIOS]
	Dimension vecApellidos[CANT_MAX_USUARIOS]
	Dimension vecDNI[CANT_MAX_USUARIOS]
	Dimension vecLegajo[CANT_MAX_USUARIOS]
	Dimension vecSaldo[CANT_MAX_USUARIOS]
	Dimension vecNumCliente[CANT_MAX_USUARIOS]
	Dimension vecRoles[CANT_MAX_USUARIOS]
	// Datos del administrador
	vecUsuarios[0] <- "admin"
	vecClaves[0] <- "1234"
	vecRoles[0] <- GERENTE_PRINCIPAL
	//Datos clientes por defecto
	vecUsuarios[1] <- "juams89"
	vecClaves[1] <- "Juams89"
	vecNombres[1]<- "Juan Ubaldo"
	vecApellidos[1] <- "Britez"
	vecDNI[1]<- "34554543"
	vecNumCliente[1]<- 1
	vecSaldo[1]<- 1258863
	vecRoles[1] <- CLIENTE
	//===============================
	vecUsuarios[2] <- "Icamacho88"
	vecClaves[2] <- "Ivan1988"
	vecNombres[2]<- "Ivan" 
	vecApellidos[2]<- "Camacho Padilla" 
	vecDNI[2]<- "33654897"
	vecNumCliente[2]<- 0001
	vecSaldo[2]<- 2568369.50
	vecRoles[2] <- CLIENTE
	
	// ==============================
	// INICIO DE SESIÓN
	// ==============================
	
	mostrarLogo()
	
	accesoConcedido <- login(vecUsuarios, vecClaves, cant_Usuarios, MAX_INTENTOS, posicionUsuario)
	
	// ==============================
	// MENÚ DEL GERENTE
	// ==============================
	Limpiar Pantalla
	Si accesoConcedido  Entonces
		Segun vecRoles[posicionUsuario] Hacer
			
			GERENTE_PRINCIPAL:
				Repetir
					Escribir ""
					Escribir "========== GERENTE PRINCIPAL =========="
					Escribir "                              usuario: ", vecUsuarios[posicionUsuario]
					Escribir "1. Gestionar personal"
					Escribir "2. Gestionar agentes"
					Escribir "3. Gestionar Clientes"
					Escribir "4. Consultas y Reportes"
					Escribir "5. Gestionar solicitudes"
					Escribir "6. Configuración del banco"
					Escribir "7. Cerrar sesión"
					Escribir "Seleccione una opción: " Sin Saltar
					Leer opcion
					Segun opcion Hacer
						1:
							Limpiar Pantalla
							Repetir
								Escribir "1. Alta"
								Escribir "2. Baja"
								Escribir "3. Modificar"
								Escribir "4. Consultar"
								Escribir "5. Solicitudes"
								Escribir "6. Volver"
								leer opcion1
								
								Escribir "========== Gestión de personal =========="
								
								Segun opcion1 Hacer
									1: Limpiar Pantalla
										AltaPersonal(vecUsuarios, vecClaves, vecRoles, cant_Usuarios, CANT_MAX_USUARIOS, GERENTE_PRINCIPAL, AGENTE)
										
									2: Limpiar Pantalla
										BajaPersonal(vecUsuarios, vecClaves, vecRoles, cant_Usuarios)
										
									3:
										Escribir "FALTA COMPLETAR"
									4:
										Escribir "FALTA COMPLETAR"
									5:
										Escribir "FALTA COMPLETAR"
									6:
										//vuelve al menu anterior
								FinSegun
								Limpiar Pantalla
							Hasta Que  opcion1 = 6
						2:
							Limpiar Pantalla
							Repetir
								Escribir "========== Gestión de agentes =========="
								Escribir "1. Alta"
								Escribir "2. Baja"
								Escribir "3. Modificar"
								Escribir "4. Consultar"
								Escribir "5. Volver"
								Leer opcion1
								Segun opcion1 Hacer
									1:
										Escribir "FALTA COMPLETAR"
										
									2:
										Escribir "FALTA COMPLETAR"
										
									3:
										Escribir "FALTA COMPLETAR"
										
									4:
										Escribir "FALTA COMPLETAR"
										
									5:
										// Volver al menú anterior
										
									De Otro Modo:
										Escribir "Opción inválida"
								FinSegun
								
								Limpiar Pantalla
								
							Hasta Que opcion1 = 5
							
						3:
							Limpiar Pantalla
							Repetir
								Escribir "========== Gestión de clientes =========="
								Escribir "1. Consultar cliente"
								Escribir "2. Consultar todos los clientes"
								Escribir "3. Buscar cliente por DNI"
								Escribir "4. Buscar cliente por número de cuenta"
								Escribir "5. Consultar cuentas"
								Escribir "6. Consultar estado de cliente"
								Escribir "7. Volver"
								Leer opcion1
								
								Segun opcion1 Hacer
									1:
										Escribir "FALTA COMPLETAR"
										
									2:
										Escribir "FALTA COMPLETAR"
										
									3:
										Escribir "FALTA COMPLETAR"
										
									4:
										Escribir "FALTA COMPLETAR"
										
									5:
										Escribir "FALTA COMPLETAR"
										
									6:
										Escribir "FALTA COMPLETAR"
										
									7:
										// Volver al menú anterior
										
									De Otro Modo:
										Escribir "Opción inválida"
								FinSegun
								
								Limpiar Pantalla
								
							Hasta Que opcion1 = 7
						4:
							Limpiar Pantalla
							Repetir
								Escribir "========== Consultas y reportes =========="
								Escribir "1. Cantidad de clientes"
								Escribir "2. Cantidad de agentes"
								Escribir "3. Cantidad de cuentas"
								Escribir "4. Resumen de operaciones"
								Escribir "5. Total de depósitos"
								Escribir "6. Total de retiros"
								Escribir "7. Total de transferencias"
								Escribir "8. Operaciones por agente"
								Escribir "9. Operaciones por período"
								Escribir "10. Volver"
								Leer opcion1
								
								Segun opcion1 Hacer
									1:
										Escribir "FALTA COMPLETAR"
										
									2:
										Escribir "FALTA COMPLETAR"
										
									3:
										Escribir "FALTA COMPLETAR"
										
									4:
										Escribir "FALTA COMPLETAR"
										
									5:
										Escribir "FALTA COMPLETAR"
										
									6:
										Escribir "FALTA COMPLETAR"
										
									7:
										Escribir "FALTA COMPLETAR"
										
									8:
										Escribir "FALTA COMPLETAR"
										
									9:
										Escribir "FALTA COMPLETAR"
										
									10:
										// Volver al menú anterior
										
									De Otro Modo:
										Escribir "Opción inválida"
								FinSegun
								
								Limpiar Pantalla
								
							Hasta Que opcion1 = 10
						5:
							Limpiar Pantalla
							Repetir
								Escribir "========== Gestión de solicitudes =========="
								Escribir "1. Ver solicitudes pendientes"
								Escribir "2. Consultar solicitud"
								Escribir "3. Historial de solicitudes"
								Escribir "4. Volver"
								Leer opcion1 
								
								Segun opcion1 Hacer
									1:
										Escribir "FALTA COMPLETAR"
										
									2:
										Escribir "FALTA COMPLETAR"
										
									3:
										Escribir "FALTA COMPLETAR"
										
									4:
										// Volver al menú anterior
										
									De Otro Modo:
										Escribir "Opción inválida"
								FinSegun
								Limpiar Pantalla
							Hasta Que opcion1 = 4
						6:
								Limpiar Pantalla
								Repetir
									Escribir "========== Configuración del banco =========="
									Escribir "1. Consultar datos del banco"
									Escribir "2. Configurar límites operativos"
									Escribir "3. Gestionar tipos de cuenta"
									Escribir "4. Consultar tasas y comisiones"
									Escribir "5. Configurar parámetros de seguridad"
									Escribir "6. Volver"
									Leer opcion1
									
									Segun opcion1 Hacer
										1:
											Escribir "FALTA COMPLETAR"
											
										2:
											Escribir "FALTA COMPLETAR"
											
										3:
											Escribir "FALTA COMPLETAR"
											
										4:
											Escribir "FALTA COMPLETAR"
											
										5:
											Escribir "FALTA COMPLETAR"
											
										6:
											// Volver al menú anterior
											
										De Otro Modo:
											Escribir "Opción inválida"
									FinSegun
									
									Limpiar Pantalla
									
								Hasta Que opcion1 = 6
							
						7:
							Escribir "Cerrando sesión" Sin Saltar
							
							Para i <- 1 Hasta 3 Hacer
								Escribir "." Sin Saltar
								Esperar 1 Segundo
							FinPara
							
							Limpiar Pantalla
							
						De Otro Modo:
							Escribir "Opción inválida"
							
					FinSegun
					
				Hasta Que opcion = 7
			AGENTE:
				
			CLIENTE:
				Repetir
					
					Escribir ""
					
					Escribir "================ HOME BANKING ================"
					Escribir "   BIENVENIDO(A): ", vecNombres[posicionUsuario], " ", vecApellidos[posicionUsuario]
					Escribir "----------------------------------------------"
					Escribir "INFORMACION DE LA CUENTA"
					Escribir "----------------------------------------------"
					mostrarNumCliente(vecNumCliente, posicionUsuario)
					mostrarSaldo(vecSaldo, posicionUsuario)
					Escribir "=============================================="
					Escribir "1. Mi cuenta"
					Escribir "2. Operaciones bancarias"
					Escribir "3. Transferencias"
					Escribir "4. Pagos"
					Escribir "5. Solicitudes"
					Escribir "6. Entretenimiento"
					Escribir "7. Mi perfil"
					Escribir "8. Cerrar sesión"
					Escribir ""
					Escribir "Seleccione una opción: " Sin Saltar
					Leer opcion
					
					Segun opcion Hacer
						
						1:
							Limpiar Pantalla
							repetir
								Escribir "FALTA COMPLETAR"
								Escribir "1. Volver"
								Escribir ""
								Escribir "Seleccione una opción: " Sin Saltar
								Leer opcion1
								Segun opcion1 Hacer
									1:
										Limpiar Pantalla
										// Volver al menú anterior
									De Otro Modo:
										Escribir "Opción inválida"
								FinSegun
							Hasta Que opcion1= 1 
						2:
							Limpiar Pantalla
							repetir
								Escribir "FALTA COMPLETAR"
								Escribir "1. Volver"
								Escribir ""
								Escribir "Seleccione una opción: " Sin Saltar
								Leer opcion1
								Segun opcion1 Hacer
									1:
										Limpiar Pantalla
										// Volver al menú anterior
									De Otro Modo:
										Escribir "Opción inválida"
								FinSegun
							Hasta Que opcion1= 1
						3:
							Limpiar Pantalla
							Escribir "FALTA COMPLETAR"
							
						4:
							Limpiar Pantalla
							Escribir "FALTA COMPLETAR"
							
						5:
							Limpiar Pantalla
							Escribir "FALTA COMPLETAR"
							
						6:
							Limpiar Pantalla
							Escribir "FALTA COMPLETAR"
							
						7:
							Limpiar Pantalla
							Escribir "FALTA COMPLETAR"
							
						8:
							Limpiar Pantalla
							Escribir "Cerrando sesión" Sin Saltar
							
							Para i <- 1 Hasta 3 Hacer
								Escribir "." Sin Saltar
								Esperar 1 Segundo
							FinPara
							
							Limpiar Pantalla
							
						De Otro Modo:
							Escribir "Opción inválida"
							
					FinSegun
					
				Hasta Que opcion = 8
		FinSegun
	SiNo
		
		Escribir "Acceso denegado."
		Escribir "Se agotaron los intentos permitidos."
		Escribir "Cerrando sesión" Sin Saltar
		
		Para i <- 1 Hasta 3 Hacer
			Escribir "." Sin Saltar
			Esperar 1 Segundo
		FinPara
		
		Limpiar Pantalla
		
	FinSi
	
FinAlgoritmo
// Funcion Para altas de personal, gestiona usuario, contraseña y rol. 
Funcion AltaPersonal(vecUsuarios, vecClaves, vecRoles, cant_Usuarios Por Referencia, CANT_MAX_USUARIOS, GERENTE_PRINCIPAL, AGENTE)
	
    Definir usuarioNuevo Como Caracter
    Definir claveNueva Como Caracter
    Definir rolNuevo Como Entero
	Definir usuarioExiste Como Logico
	
    Si cant_Usuarios < CANT_MAX_USUARIOS Entonces
		
        Escribir "========== ALTA DE PERSONAL =========="
		repetir 
			usuarioNuevo<- validarUsuario()
			usuarioExiste<- buscarUsuario(usuarioNuevo, vecUsuarios, cant_Usuarios)
			Si UsuarioExiste Entonces
				Escribir "El usuario ya existe elija otro"
			FinSi
		Mientras Que usuarioExiste
		
		claveNueva <- validarContrasena
		
		rolNuevo <- seleccionar_Rol(GERENTE_PRINCIPAL,AGENTE)
       
        // Asignamos nuevos datos a cada vector
		vecUsuarios[cant_Usuarios] <- usuarioNuevo
		vecClaves[cant_Usuarios] <- claveNueva
		vecRoles[cant_Usuarios] <- rolNuevo
		
		// suma una posicion asi el para lo puede recorrer. 
        cant_Usuarios <- cant_Usuarios + 1
		
        Escribir ""
        Escribir "Usuario registrado correctamente."
		Escribir ""
        Escribir "Usuario: ", usuarioNuevo
		Escribir "Contraseña: ",claveNueva
		
    SiNo
		
        Escribir "No se puede registrar el usuario."
        Escribir "Se alcanzó el límite máximo."
		
    FinSi
	
FinFuncion
Funcion nueva_Contrasena <- validarContrasena
	Definir nueva_Contrasena, caracter Como Caracter
	Definir i Como Entero
	Definir cantNumeros Como Entero
	Definir cantMinusculas, cantMayusculas Como Entero
	
	Repetir
		
		Escribir "Ingrese una contrasena:"
		Leer nueva_Contrasena
		
		// Inicializamos todos los contadores
		cantNumeros <- 0
		cantMinusculas <- 0
		cantMayusculas <- 0
		
		// Recorremos la contrasena caracter por caracter
		Para i <- 0 Hasta Longitud(nueva_Contrasena) - 1 Hacer
			
			caracter <- Subcadena(nueva_Contrasena, i, i)
			
			// ¿Es una letra minuscula?
			Si esMinuscula(caracter) Entonces
				cantMinusculas <- cantMinusculas + 1
			FinSi
			
			// ¿Es una letra mayuscula?
			Si esMayuscula(caracter) Entonces
				cantMayusculas <- cantMayusculas + 1
			FinSi
			
			// ¿Es un numero?
			Si esNumero(caracter) Entonces
				cantNumeros <- cantNumeros + 1
			FinSi
			
		FinPara
		
		Escribir "-------------------------"
		
		Si cantMayusculas < 1 Entonces
			Escribir "La nueva clave debe contener al menos una letra mayuscula"
		FinSi
		
		Si cantMinusculas < 1 Entonces
			Escribir "La nueva clave debe contener al menos una letra minuscula"
		FinSi
		
		Si cantNumeros < 1 Entonces
			Escribir "La nueva clave debe contener al menos un numero"
		FinSi
		
		Si Longitud(nueva_Contrasena) <> 8 Entonces
			Escribir "La nueva clave debe contener 8 caracteres"
		FinSi
		
//		Escribir "Mayusculas: ", cantMayusculas
//		Escribir "Minusculas: ", cantMinusculas
//		Escribir "Numeros: ", cantNumeros
		
	Mientras Que NO (Longitud(nueva_Contrasena) = 8 Y cantMayusculas >= 1 Y cantMinusculas >= 1 Y cantNumeros >= 1)
	
	Escribir "-------------------------"
	Escribir "Contrasena valida."	
FinFuncion
Funcion existe <- buscarUsuario(usuarioNuevo, vecUsuarios, cantUsuarios)
	Definir existe Como Logico
	Definir i Como Entero
	existe <- falso
	Para i<- 0 Hasta cantUsuarios -1 Hacer
		Si usuarioNuevo = vecUsuarios[i] Entonces
			existe <- Verdadero
		FinSi
	FinPara
FinFuncion
funcion nuevoUsuario <- validarUsuario
	Definir nuevoUsuario Como Caracter
	Definir i Como Entero
	Definir cantLetras, cantNumeros Como Entero
	Definir cantMinusculas, cantMayusculas Como Entero
	Definir cantEspacio Como Entero
	Definir caracter Como Caracter
	Repetir
		
		Escribir "Ingrese nuevo Usuario:"
		Leer nuevoUsuario
		
		// Inicializamos todos los contadores
		cantLetras <- 0
		cantNumeros <- 0
		cantMinusculas <- 0
		cantMayusculas <- 0
		cantEspacio <- 0
		// Recorremos el usuario caracter por caracter
		Para i <- 0 Hasta Longitud(nuevoUsuario) - 1 Hacer
			
			caracter <- Subcadena(nuevoUsuario, i, i)
			//¿Es una letra minuscula?
			Si esMinuscula(caracter) Entonces
				cantMinusculas <- cantMinusculas + 1
				cantLetras <- cantLetras + 1
			FinSi
			// ¿Es una letra mayuscula?
			Si esMayuscula(caracter) Entonces
				cantMayusculas <- cantMayusculas + 1
				cantLetras <- cantLetras + 1
			FinSi
			
			// ¿Es un numero?
			Si esNumero(caracter) Entonces
				cantNumeros <- cantNumeros + 1
			FinSi
			
			// ¿Tiene espacios?
			Si esEspacio(caracter)Entonces
				cantEspacio <- cantEspacio +1
			FinSi
		FinPara
		
		Escribir "-------------------------"
		Si cantLetras < 1 Entonces
			Escribir "El nombre de Usuario debe contener al menos 1 letra"
		FinSi
		Si cantMayusculas > 2 Entonces
			Escribir "El nombre de Usuario no puede tener mas de 2 letra mayuscula"
		FinSi
		SI cantNumeros < 1 Entonces
			Escribir "El nombre de usuario debe contener al menos 1 numero"
		FinSi
		Si cantNumeros > 4 Entonces
			Escribir "El nombre de usuario no puede tener mas de 4 numeros"
		FinSi
		
		Si cantEspacio >=1 Entonces
			Escribir "El nombre de usuario no debe contener espacios"
		FinSi
		Si Longitud(nuevoUsuario) < 4  o Longitud(nuevoUsuario) > 20 Entonces
			Escribir "El nombre de usuario debe contener minimo 4 caracteres y maximo 20"
		FinSi
//      Escribir "Espacios: ", cantEspacios
//		Escribir "Letras: ", cantLetras
//		Escribir "Mayusculas: ", cantMayusculas
//		Escribir "Minusculas: ", cantMinusculas		
	Mientras Que NO ((Longitud(nuevoUsuario) >= 4  y Longitud(nuevoUsuario) <= 20 )y cantLetras >= 1  Y cantMayusculas <= 2 Y cantNumeros >= 1 y cantNumeros <= 4 y cantEspacio = 0)
FinFuncion
Funcion condicion <- esMinuscula(conjunto)
	Definir condicion Como Logico
	condicion<- Falso
	// ¿Es una letra minuscula?
	Si conjunto >= "a" Y conjunto <= "z" Entonces
		condicion<- Verdadero
	FinSi
FinFuncion
Funcion condicion<- esMayuscula(conjunto)	
	Definir condicion Como Logico
	condicion<- Falso
	// ¿Es una letra mayuscula?
	Si conjunto >= "A" Y conjunto <= "Z" Entonces
		condicion<- Verdadero
	FinSi
FinFuncion
Funcion condicion<- esNUmero(conjunto)
	Definir condicion Como Logico
	condicion<- Falso
	// ¿Es un Numero?
	Si conjunto >= "0" Y conjunto <= "9" Entonces
		condicion<- Verdadero
	FinSi
FinFuncion
Funcion condicion<- esEspacio(conjunto)
	Definir condicion Como Logico
	condicion<- Falso
	//¿Tiene espacio?
	Si conjunto = " " Entonces
		condicion<- Verdadero
	FinSi
FinFuncion
Funcion asignar_Rol <- seleccionar_Rol(rol1,rol2)
	Definir asignar_Rol Como Entero
	Definir opcionRol Como Entero
	Repetir
		// seleccionamos el rol del NUEVO PERSONAL 
		Escribir ""
		Escribir "Seleccione el rol:"
		Escribir "1. Gerente Principal"
		Escribir "2. Agente"
		Leer opcionRol
		
		Segun opcionRol Hacer
			
			1:
				asignar_Rol <- rol1
				
			2:
				asignar_Rol <- rol2
				
			De Otro Modo:
				Escribir "Opción inválida."
				
		FinSegun
		
	Hasta Que opcionRol = 1 O opcionRol = 2
FinFuncion
Funcion posicion <-buscarPosicionUsuario(usuarioBuscado, vecUsuarios, cant_Usuarios)
	Definir posicion Como Entero
	Definir i Como Entero
	posicion<- -1
	Para i<- 0 Hasta cant_Usuarios-1 Con Paso 1 Hacer
		Si usuarioBuscado = vecUsuarios[i] Entonces
			posicion <- i
		FinSi
	FinPara
FinFuncion
Funcion reordenarUsuarios(vecUsuarios,vecClaves, vecRoles, cant_Usuarios Por Referencia, posicion)
	Definir i Como Entero
	Para i<- posicion Hasta cant_Usuarios -2 Hacer
		vecUsuarios[i]<- vecUsuarios[i+1]
		vecClaves[i]<- vecClaves[i+1]
		vecRoles[i]<- vecRoles[i+1]
	FinPara
	cant_Usuarios <- cant_Usuarios -1
FinFuncion
Funcion BajaPersonal(vecUsuarios, vecClaves, vecRoles, cant_Usuarios Por Referencia)
	Definir usuarioBuscado Como Caracter
	Definir usuarioExiste Como Logico
	Definir posicion Como Entero
	Definir confirmarEliminacion Como Logico
	Escribir "========== BAJA DE PERSONAL =========="
	repetir 
		Escribir "Ingrese nombre del Usuario que desea dar de baja: "
		Leer usuarioBuscado
		usuarioExiste<- buscarUsuario(usuarioBuscado, vecUsuarios, cant_Usuarios)
		Si no UsuarioExiste Entonces
			Escribir "El usuario no existe, reingrese usuario"
		FinSi
	Mientras Que no usuarioExiste
	Escribir "--------------------"
	Escribir "Usuario encontrado"
	posicion<- buscarPosicionUsuario(usuarioBuscado, vecUsuarios, cant_Usuarios)
	confirmarEliminacion<- deseaContinuar("Desea eliminar este usuario [S/N]: ",usuarioBuscado)
	Si confirmarEliminacion Entonces
		reordenarUsuarios(vecUsuarios,vecClaves, vecRoles, cant_Usuarios, posicion)
		Escribir "Usuario Eliminado Correctamente."
	SiNo
		Escribir "Eliminacion cancelada"
	FinSi
	
FinFuncion
Funcion confirmacion<- deseaContinuar(mensaje, dato)
	Definir seleccion Como Caracter
	Definir confirmacion Como Logico
	Repetir
		Escribir mensaje, dato
		leer seleccion
		seleccion <-  Mayusculas(seleccion)
		Si (seleccion <> 'S' y seleccion <> 'N') Entonces
			Escribir "ERROR: Opcion invalida! [S/N]"
		FinSi
	Mientras Que seleccion <> "S" Y seleccion <> "N"
	
	Si seleccion = "S" Entonces
		confirmacion <- Verdadero
	sino 
		confirmacion<- falso
	FinSi
FinFuncion
Funcion mostrarLogo
	Escribir "+====================================================================================+"
	Escribir "||                     /$$$$$$ /$$   /$$  /$$$$$$  /$$$$$$$  /$$$$$$$$              ||"
	Escribir "||                    |_  $$_/| $$$ | $$ /$$__  $$| $$__  $$|__  $$__/              ||"
	Escribir "||                      | $$  | $$$$| $$| $$  \__/| $$  \ $$   | $$                 ||"
	Escribir "||                      | $$  | $$ $$ $$|  $$$$$$ | $$$$$$$/   | $$                 ||"
	Escribir "||                      | $$  | $$  $$$$ \____  $$| $$____/    | $$                 ||"
	Escribir "||                      | $$  | $$\  $$$ /$$  \ $$| $$         | $$                 ||"
	Escribir "||                     /$$$$$$| $$ \  $$|  $$$$$$/| $$         | $$                 ||"
	Escribir "||                    |______/|__/  \__/ \______/ |__/         |__/                 ||"
	Escribir "||==================================================================================||"
	Escribir "||                                 SISTEMA BANCARIO                                 ||"
	Escribir "+====================================================================================+"	
FinFuncion
Funcion accesoConcedido <- login(vecUsuarios, vecClaves, cant_Usuarios, MAX_INTENTOS, posicionUsuario Por Referencia)
    
    Definir usuarioIngresado Como Caracter
    Definir claveIngresada Como Caracter
    Definir intentos Como Entero
    Definir i Como Entero
    Definir accesoConcedido Como Logico
	
    intentos <- 0
    accesoConcedido <- Falso
    posicionUsuario <- -1
    
    Repetir
        
        Escribir ""
        Escribir "+================================= INICIO DE SESION =================================+"
        Escribir ""
        Escribir "                                   Usuario: " Sin Saltar
        Leer usuarioIngresado
        Escribir "                                   Contraseña: " Sin Saltar
        Leer claveIngresada
        Escribir ""
        Escribir "+====================================================================================+"
        Escribir ""
        posicionUsuario <- -1
        
        Para i <- 0 Hasta cant_Usuarios - 1 Hacer
            Si usuarioIngresado = vecUsuarios[i] Entonces
                posicionUsuario <- i
            FinSi
        FinPara
        
        Si posicionUsuario <> -1 Entonces
            Si claveIngresada = vecClaves[posicionUsuario] Entonces
                Escribir ""
                Escribir "Inicio de sesion correcto."
                accesoConcedido <- Verdadero
            SiNo
                Escribir ""
                Escribir "Contraseña incorrecta."
                intentos <- intentos + 1
                Esperar 2 segundos 
				Limpiar Pantalla
            FinSi
            
        SiNo
            
            Escribir ""
            Escribir "Usuario inexistente."
            intentos <- intentos + 1
            Esperar 2 segundos 
			Limpiar Pantalla
        FinSi
        
//        Si NO accesoConcedido Y intentos < MAX_INTENTOS Entonces
//            Escribir "Intentos restantes: ", MAX_INTENTOS - intentos
//        FinSi
        
    Mientras Que NO accesoConcedido Y intentos < MAX_INTENTOS
	Esperar 2 Segundos
FinFuncion
Funcion mostrarNumCliente(vecNumCliente, posicion)
	Definir numero Como Entero
	
	numero <- vecNumCliente[posicion]
	
	Si numero < 10 Entonces
		Escribir "Número de cuenta:                      00000", numero
	SiNo
		Si numero < 100 Entonces
			Escribir "Número de cuenta:                      0000", numero
		SiNo
			Si numero < 1000 Entonces
				Escribir "Número de cuenta:                      000", numero
			SiNo
				Si numero < 10000 Entonces
					Escribir "Número de cuenta:                      00", numero
				SiNo
					Escribir "Número de cuenta:                       ", numero
				FinSi
			FinSi
		FinSi
	FinSi
FinFuncion
Funcion mostrarSaldo(vecSaldo, posicion)
	Definir saldo Como Real
	Definir parteEntera Como Entero
	Definir centavos Como Entero
	Definir millones Como Entero
	Definir miles Como Entero
	Definir resto Como Entero
	
	saldo <- vecSaldo[posicion]
	parteEntera <- Trunc(saldo)
	centavos <- Trunc((saldo - parteEntera) * 100)
	
	millones <- Trunc(parteEntera / 1000000)
	miles <- Trunc((parteEntera MOD 1000000) / 1000)
	resto <- parteEntera MOD 1000
	Escribir "Saldo disponible:              " Sin Saltar
	Si millones > 0 Entonces
		Escribir Sin Saltar "$ ", millones, "."
		
		Si miles < 100 Entonces
			Si miles < 10 Entonces
				Escribir Sin Saltar "00", miles, "."
			SiNo
				Escribir Sin Saltar "0", miles, "."
			FinSi
		SiNo
			Escribir Sin Saltar miles, "."
		FinSi
		
		Si resto < 100 Entonces
			Si resto < 10 Entonces
				Escribir Sin Saltar "00", resto
			SiNo
				Escribir Sin Saltar "0", resto
			FinSi
		SiNo
			Escribir Sin Saltar resto
		FinSi
		
	SiNo
		Si parteEntera >= 1000 Entonces
			Escribir Sin Saltar "$ ", miles, "."
			
			Si resto < 100 Entonces
				Si resto < 10 Entonces
					Escribir Sin Saltar "00", resto
				SiNo
					Escribir Sin Saltar "0", resto
				FinSi
			SiNo
				Escribir Sin Saltar resto
			FinSi
		SiNo
			Escribir Sin Saltar "$ ", parteEntera
		FinSi
	FinSi
	
	Si centavos < 10 Entonces
		Escribir ",0", centavos
	SiNo
		Escribir ",", centavos
	FinSi
FinFuncion