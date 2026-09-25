Algoritmo TP_FINAL_LAB_SISTEMA_BANCARIO
	// definir vectores de login y cantidades
	Definir vecUsuarios Como Caracter
	Definir vecClaves Como Caracter
	Definir vecRoles Como Entero
	Definir CANT_MAX_USUARIOS Como Entero
	Definir cant_Usuarios Como Entero
	Definir i Como Entero
	Definir posicionUsuario Como Entero
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
	Definir opcion Como Entero
	cant_Usuarios<- 1
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
	Dimension vecRoles[CANT_MAX_USUARIOS]
	// Datos del administrador
	vecUsuarios[0] <- "admin"
	vecClaves[0] <- "1234"
	vecRoles[0] <- GERENTE_PRINCIPAL
	
	
	// ==============================
	// INICIO DE SESIÓN
	// ==============================
	
	Escribir "========== BANCO =========="
	
	Repetir 
		
		Escribir "Usuario:"
		Leer usuarioIngresado
		
		Escribir "Contraseña:"
		Leer claveIngresada
		posicionUsuario<- -1
		Para i<- 0 Hasta cant_Usuarios -1 Hacer
			
			Si usuarioIngresado = vecUsuarios[i] Entonces
				posicionUsuario<- i
			FinSi
		FinPara
		// ¿Encontramos el usuario?
		Si posicionUsuario <> -1 Entonces
			
			// Comprobar su contraseña
			Si claveIngresada = vecClaves[posicionUsuario] Entonces
				
				Escribir "Inicio de sesión correcto"
				
				accesoConcedido <- Verdadero
				
			SiNo
				
				Escribir "Contraseña incorrecta"
				intentos <- intentos + 1
				
			FinSi
			
		SiNo
			
			Escribir "Usuario inexistente"
			intentos <- intentos + 1
			
		FinSi
		// Condicion: Repetir mientras NO se haya concedido el acceso Y mientras queden intentos disponibles.
	Mientras Que No accesoConcedido Y intentos < MAX_INTENTOS
	
	
	// ==============================
	// MENÚ DEL GERENTE
	// ==============================
	
	Si accesoConcedido  Entonces
		Segun vecRoles[posicionUsuario] Hacer
			
			GERENTE_PRINCIPAL:
				Repetir
					
					Escribir ""
					Escribir "========== GERENTE PRINCIPAL =========="
					Escribir "1. Gestionar personal"
					Escribir "2. Gestionar agentes"
					Escribir "3. Consultar clientes"
					Escribir "4. Consultar balance"
					Escribir "5. Cerrar sesión"
					Escribir "Seleccione una opción:"
					Leer opcion
					Segun opcion Hacer
						1:
							
							Escribir "========== Gestión de personal =========="
							Escribir "1. Alta"
							Escribir "2. Baja"
							Escribir "3. Modificar"
							Escribir "4. Consultar"
							Escribir "5. Solicitudes"
							Escribir "6. Volver"
							leer opcion
							Segun opcion Hacer
								1:AltaPersonal(vecUsuarios, vecClaves, vecRoles, cant_Usuarios, CANT_MAX_USUARIOS, GERENTE_PRINCIPAL, AGENTE)
									
								2:BajaPersonal(vecUsuarios, vecClaves, vecRoles, cant_Usuarios, CANT_MAX_USUARIOS)
									
								3:
									Escribir "FALTA COMPLETAR"
								4:
									Escribir "FALTA COMPLETAR"
								5:
									Escribir "FALTA COMPLETAR"
								6:
									Escribir "FALTA COMPLETAR"
							FinSegun
							
						2:
							Escribir "Gestión de agentes"
							
						3:
							Escribir "Consulta de clientes"
							
						4:
							Escribir "Consulta de balance"
							
						5:
							Escribir "Cerrando sesión..."
							
						De Otro Modo:
							Escribir "Opción inválida"
							
					FinSegun
					
				Hasta Que opcion = 5
			AGENTE:
				Repetir
					
					Escribir ""
					Escribir "========== AGENTE =========="
					Escribir "1. Gestionar personal"
					Escribir "2. Gestionar agentes"
					Escribir "3. Consultar clientes"
					Escribir "4. Consultar balance"
					Escribir "5. Cerrar sesión"
					Escribir "Seleccione una opción:"
					Leer opcion
					
					Segun opcion Hacer
						
						1:
							Escribir "Gestión de personal"
							
						2:
							Escribir "Gestión de agentes"
							
						3:
							Escribir "Consulta de clientes"
							
						4:
							Escribir "Consulta de balance"
							
						5:
							Escribir "Cerrando sesión..."
							
						De Otro Modo:
							Escribir "Opción inválida"
							
					FinSegun
					
				Hasta Que opcion = 5
			CLIENTE:
				Repetir
					
					Escribir ""
					Escribir "========== CLIENTE =========="
					Escribir "1. Gestionar personal"
					Escribir "2. Gestionar agentes"
					Escribir "3. Consultar clientes"
					Escribir "4. Consultar balance"
					Escribir "5. Cerrar sesión"
					Escribir "Seleccione una opción:"
					Leer opcion
					
					Segun opcion Hacer
						
						1:
							Escribir "Gestión de personal"
							
						2:
							Escribir "Gestión de agentes"
							
						3:
							Escribir "Consulta de clientes"
							
						4:
							Escribir "Consulta de balance"
							
						5:
							Escribir "Cerrando sesión..."
							
						De Otro Modo:
							Escribir "Opción inválida"
							
					FinSegun
					
				Hasta Que opcion = 5
		FinSegun
	SiNo
		
		Escribir "Acceso denegado."
		Escribir "Se agotaron los intentos permitidos."
		
	FinSi
	
FinAlgoritmo
// Funcion Para altas de personal, gestiona usuario, contraseña y rol. 
Funcion AltaPersonal(vecUsuarios, vecClaves, vecRoles, cant_Usuarios Por Referencia, CANT_MAX_USUARIOS, GERENTE_PRINCIPAL, AGENTE)
	
    Definir usuarioNuevo Como Caracter
    Definir claveNueva Como Caracter
    Definir rolNuevo Como Entero
    Definir opcionRol Como Entero
	Definir usuarioExiste Como Logico
	
    Si cant_Usuarios < CANT_MAX_USUARIOS Entonces
		
        Escribir "========== ALTA DE PERSONAL =========="
		repetir 
			usuarioNuevo<- validarUsuario
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
		Si Longitud(nuevoUsuario) < 4  o Longitud(nuevoUsuario) >= 20 Entonces
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
Funcion BajaPersonal(vecUsuarios, vecClaves, vecRoles, cant_Usuarios Por Referencia, CANT_MAX_USUARIOS)
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
	