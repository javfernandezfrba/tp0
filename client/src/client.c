#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");
	

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	// Loggeamos el valor de config
	log_info(logger, "Lei la IP %s y puerto %s", ip, puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	valor = config_get_string_value(config, "CLAVE");

	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log", "TP0", true, LOG_LEVEL_INFO);
	
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	
	nuevo_config = config_create("/home/utnso/tp0/client/tp0.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	while(1){
		leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

		if (!strncmp(leido, "exit", 4)) {
            free(leido);
            break;
        }
		
		if(leido){
			log_info(logger, leido);
		}

		if(!leido){
			free(leido);
			break;
		}


		printf("%s\n", leido);
		free(leido);
	}
	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();


	// Leemos y esta vez agregamos las lineas al paquete

	while(1){
		leido = readline("> ");

		if (!strncmp(leido, "exit", 4)) {
            free(leido);
            break;
        }
		else if(leido){
			agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		}

		printf("%s\n", leido);
		free(leido);
	}

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	if(logger != NULL){
		log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config);
	}

	if(conexion){
		liberar_conexion(conexion);
	}
}
