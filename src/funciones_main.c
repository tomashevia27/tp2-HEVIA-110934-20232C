#include "funciones_main.h"
#include "pokemon.h"
#include "ataque.h"
#include "juego.h"
#include "lista.h"
#include "adversario.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>

void limpiar_consola()
{
	printf("\033c");
}

void imprimir_grafica_ganador()
{
	printf("\n");
	printf("*     ,o888888o.             .8.          b.             8          .8.             d888888o.   8888888 8888888888 8 88888888888888\n");
	printf("*    8888     `88.          .888.         888o.          8         .888.          .`8888:' `88.       8 8888       8 8888\n");
	printf("* ,8 8888       `8.        :88888.        Y88888o.       8        :88888.         8.`8888.   Y8       8 8888       8 8888\n");
	printf("* 88 8888                 . `88888.       .`Y888888o.    8       . `88888.        `8.`8888.           8 8888       8 8888\n");
	printf("* 88 8888                .8. `88888.      8o. `Y888888o. 8      .8. `88888.        `8.`8888.          8 8888       8 888888888888\n");
	printf("* 88 8888               .8`8. `88888.     8`Y8o. `Y88888o8     .8`8. `88888.        `8.`8888.         8 8888       8 8888\n");
	printf("* 88 8888   8888888    .8' `8. `88888.    8   `Y8o. `Y8888    .8' `8. `88888.        `8.`8888.        8 8888       8 8888\n");
	printf("* `8 8888       .8'   .8'   `8. `88888.   8      `Y8o. `Y8   .8'   `8. `88888.   8b   `8.`8888.       8 8888       8 8888\n");
	printf("*    8888     ,88'   .888888888. `88888.  8         `Y8o.`  .888888888. `88888.  `8b.  ;8.`8888       8 8888       8 8888\n");
	printf("*     `8888888P'    .8'       `8. `88888. 8            `Yo .8'       `8. `88888.  `Y8888P ,88P'       8 8888       8 88888888888888\n");
	printf("\n");
}

void imprimir_grafica_perdedor()
{
	printf("\n");
	printf("* 8 888888888o   8 8888888888   8 888888888o.   8 888888888o.       8 8888    d888888o.   8888888 8888888888 8 88888888888888\n");
	printf("* 8 8888    `88. 8 8888         8 8888    `88.  8 8888    `^888.    8 8888  .`8888:' `88.       8 8888       8 8888\n");
	printf("* 8 8888     `88 8 8888         8 8888     `88  8 8888        `88.  8 8888  8.`8888.   Y8       8 8888       8 8888\n");
	printf("* 8 8888     ,88 8 8888         8 8888     ,88  8 8888         `88  8 8888  `8.`8888.           8 8888       8 8888\n");
	printf("* 8 8888.   ,88' 8 888888888888 8 8888.   ,88'  8 8888          88  8 8888   `8.`8888.          8 8888       8 888888888888\n");
	printf("* 8 888888888P'  8 8888         8 888888888P'   8 8888          88  8 8888    `8.`8888.         8 8888       8 8888\n");
	printf("* 8 8888         8 8888         8 8888`8b       8 8888         ,88  8 8888     `8.`8888.        8 8888       8 8888\n");
	printf("* 8 8888         8 8888         8 8888 `8b.     8 8888        ,88'  8 8888 8b   `8.`8888.       8 8888       8 8888\n");
	printf("* 8 8888         8 8888         8 8888   `8b.   8 8888    ,o88P'    8 8888 `8b.  ;8.`8888       8 8888       8 8888\n");
	printf("* 8 8888         8 888888888888 8 8888     `88. 8 888888888P'       8 8888  `Y8888P ,88P'       8 8888       8 88888888888888\n");
	printf("\n");
}

void imprimir_grafica_empate()
{
	printf("\n");
	printf("* 8 8888888888            ,8.       ,8.          8 888888888o            .8.          8888888 8888888888 8 88888888888888");
	printf("* 8 8888                 ,888.     ,888.         8 8888    `88.         .888.               8 8888       8 8888");
	printf("* 8 8888                .`8888.   .`8888.        8 8888     `88        :88888.              8 8888       8 8888");
	printf("* 8 8888               ,8.`8888. ,8.`8888.       8 8888     ,88       . `88888.             8 8888       8 8888");
	printf("* 8 888888888888      ,8'8.`8888,8^8.`8888.      8 8888.   ,88'      .8. `88888.            8 8888       8 888888888888");
	printf("* 8 8888             ,8' `8.`8888' `8.`8888.     8 888888888P'      .8`8. `88888.           8 8888       8 8888");
	printf("* 8 8888            ,8'   `8.`88'   `8.`8888.    8 8888            .8' `8. `88888.          8 8888       8 8888");
	printf("* 8 8888           ,8'     `8.`'     `8.`8888.   8 8888           .8'   `8. `88888.         8 8888       8 8888");
	printf("* 8 8888          ,8'       `8        `8.`8888.  8 8888          .888888888. `88888.        8 8888       8 8888");
	printf("* 8 888888888888 ,8'         `         `8.`8888. 8 8888         .8'       `8. `88888.       8 8888       8 88888888888888");
	printf("\n");
}

void leer_linea(char *linea)
{
	char *leido;
	leido = fgets(linea, 200, stdin);
	if (leido == NULL)
		return;
	linea[strlen(linea) - 1] = 0;
}

char *mostrar_tipo(enum TIPO c)
{
	if (c == NORMAL) {
		return "Normal";
	} else if (c == FUEGO) {
		return "Fuego";
	} else if (c == AGUA) {
		return "Agua";
	} else if (c == PLANTA) {
		return "Planta";
	} else if (c == ELECTRICO) {
		return "Electrico";
	} else {
		return "Roca";
	}
}

void mostrar_ataque(const struct ataque *ataque, void *v)
{
	if (!v) {
		printf("| %s; Tipo %s; Poder:%i \n", ataque->nombre,
		       mostrar_tipo(ataque->tipo), ataque->poder);
	}
}

bool mostrar_pokemon(void *p, void *c)
{
	printf("%s (tipo %s)\n", pokemon_nombre(p),
	       mostrar_tipo(pokemon_tipo(p)));
	con_cada_ataque((pokemon_t *)p, mostrar_ataque, NULL);
	printf("\n");
	return true;
}

bool cargar_archivo_pokemones(void *e)
{
	struct estado_juego *estado = e;
	if (estado->archivo_cargado) {
		printf("El juego ya contiene un archivo cargado\nVuelva a iniciar el juego para jugar con otro archivo\n");
		return true;
	}

	printf("Ingresar ubicacion del archivo: ");
	char linea[200];
	char *leido;
	leido = fgets(linea, 200, stdin);
	if (leido == NULL)
		return false;
	linea[strlen(linea) - 1] = 0;

	JUEGO_ESTADO estado_juego = juego_cargar_pokemon(estado->juego, linea);
	if (estado_juego == ERROR_GENERAL) {
		printf("Ocurrio un error al cargar el archivo, pruebe con otro archivo\n");
		return true;
	}

	if (estado_juego == POKEMON_INSUFICIENTES) {
		printf("La cantidad de pokemones en el archivo es insuficiente para poder jugar.\nVuelva a intentarlo con un archivo que contenga mas de 4 pokemones\n");
	} else {
		lista_t *lista_pokemones = juego_listar_pokemon(estado->juego);
		if (!lista_pokemones) {
			printf("Ocurrio un error en el desarrollo del juego, por favor reinicie el juego y vuelva a intentarlo\n");
			estado->continuar = false;
		} else {
			estado->adversario = adversario_crear(lista_pokemones);
			if (!estado->adversario) {
				printf("Ocurrio un error en el desarrollo del juego, por favor reinicie el juego y vuelva a intentarlo\n");
				estado->continuar = false;
			}
		}

		if (estado->continuar)
			printf("El archivo se cargo con exito!\n\n");

		estado->archivo_cargado = true;
	}
	return true;
}

bool listar_pokemones(void *e)
{
	struct estado_juego *estado = e;
	limpiar_consola();
	printf("Pokemones disponibles para jugar:\n\n");
	lista_con_cada_elemento(juego_listar_pokemon(estado->juego),
				mostrar_pokemon, NULL);
	if (!estado->archivo_cargado) {
		printf("No hay pokemones para listar. Usa 'c' para cargar un archivo de pokemones\n");
	}
	return true;
}

int buscar_pokemon(void *pokemon, void *nombre)
{
	return strcmp(pokemon_nombre((pokemon_t *)pokemon), (char *)nombre);
}

bool seleccionar_pokemones(void *e)
{
	struct estado_juego *estado = e;

	if (!estado->archivo_cargado) {
		printf("Antes de seleccionar los pokemones, debe cargar el archivo para poder jugar. Pulse 'c' para hacerlo\n");
		return true;
	}

	if (estado->se_eligieron_pokemones) {
		printf("Ya se han elegido los pokemones para jugar, ahora debe realizar una jugada. Pulse 'j' para hacerlo\n");
		return true;
	}

	printf("Ahora vas a elegir los pokemones para jugar.\nRecorda que los primeros 2 pokemones que elijas los vas a utilizar vos y el restante ira para el adversario.\n\n");
	listar_pokemones(estado);
	printf("\n\n");
	char nombres[3][200];
	bool se_asignaron_los_pokemones = false;
	while (!se_asignaron_los_pokemones) {
		for (int i = 0; i < 3; i++) {
			printf("Ingrese el nombre del pokemon: \n");
			char *leido;
			leido = fgets(nombres[i], 200, stdin);
			if (leido == NULL)
				return false;
			nombres[i][strlen(nombres[i]) - 1] = 0;
		}

		JUEGO_ESTADO resultado = juego_seleccionar_pokemon(
			estado->juego, JUGADOR1, nombres[0], nombres[1],
			nombres[2]);

		if (resultado == ERROR_GENERAL) {
			printf("Ocurrio un error al seleccionar los pokemones, por favor reinicie el juego y vuelva a comenzar");
			return false;
		}

		char *eleccionAdversario1 = " ";
		char *eleccionAdversario2 = " ";
		char *eleccionAdversario3 = " ";
		if (resultado == TODO_OK) {
			adversario_pokemon_seleccionado(estado->adversario,
							nombres[0], nombres[1],
							nombres[2]);
			adversario_seleccionar_pokemon(estado->adversario,
						       &eleccionAdversario1,
						       &eleccionAdversario2,
						       &eleccionAdversario3);
			juego_seleccionar_pokemon(estado->juego, JUGADOR2,
						  eleccionAdversario1,
						  eleccionAdversario2,
						  eleccionAdversario3);
			se_asignaron_los_pokemones = true;
		} else {
			printf("Por favor ingrese 3 pokemones que se encuentren en la lista y sean diferentes\n\n");
			listar_pokemones(estado);
			printf("\n");
		}

		if (se_asignaron_los_pokemones) {
			lista_t *lista_pokemones =
				juego_listar_pokemon(estado->juego);
			estado->pokemones_disponibles = lista_crear();
			lista_insertar(estado->pokemones_disponibles,
				       lista_buscar_elemento(lista_pokemones,
							     buscar_pokemon,
							     nombres[0]));
			lista_insertar(estado->pokemones_disponibles,
				       lista_buscar_elemento(lista_pokemones,
							     buscar_pokemon,
							     nombres[1]));
			lista_insertar(estado->pokemones_disponibles,
				       lista_buscar_elemento(
					       lista_pokemones, buscar_pokemon,
					       eleccionAdversario3));

			limpiar_consola();
			printf("Tus pokemones son: %s, %s y %s\n", nombres[0],
			       nombres[1], eleccionAdversario3);
			printf("Los pokemones para el adversario son: %s, %s y %s\n",
			       eleccionAdversario1, eleccionAdversario2,
			       nombres[2]);
		}
	}
	estado->se_eligieron_pokemones = true;
	return true;
}

jugada_t pedir_jugada(struct estado_juego *estado)
{
	jugada_t jugada;
	strcpy(jugada.pokemon, " ");
	strcpy(jugada.ataque, " ");
	pokemon_t *pokemon_elegido = NULL;
	struct ataque *ataque_elegido = NULL;
	while (!pokemon_elegido || !ataque_elegido) {
		printf("¿Que pokemon y ataque queres utilizar para la siguiente jugada?\n");
		printf("Pokemon: ");
		char *leido;
		leido = fgets(jugada.pokemon, 20, stdin);
		if (leido == NULL)
			return jugada;
		jugada.pokemon[strlen(jugada.pokemon) - 1] = 0;

		printf("Ataque: ");
		leido = fgets(jugada.ataque, 20, stdin);
		if (leido == NULL)
			return jugada;
		jugada.ataque[strlen(jugada.ataque) - 1] = 0;

		pokemon_elegido =
			lista_buscar_elemento(estado->pokemones_disponibles,
					      buscar_pokemon, jugada.pokemon);
		if (pokemon_elegido != NULL)
			ataque_elegido = (struct ataque *)pokemon_buscar_ataque(
				pokemon_elegido, jugada.ataque);

		if (!pokemon_elegido || !ataque_elegido)
			printf("por favor selecciona un pokemon y ataque validos\n\n");
	}

	return jugada;
}

bool jugar_rondas(void *e)
{
	struct estado_juego *estado = e;
	resultado_jugada_t resultado_jugada;

	limpiar_consola();

	if (!estado->se_eligieron_pokemones) {
		printf("Antes de realizar jugadas, tenes que elegir los pokemones. Pulse 's' para hacerlo\n");
		return false;
	}

	while (!juego_finalizado(estado->juego)) {
		lista_con_cada_elemento(estado->pokemones_disponibles,
					mostrar_pokemon, NULL);

		if (estado->rondas_jugadas > 0)
			printf("EN LA RONDA ANTERIOR SUMASTE %i PUNTOS y EL ADVERSARIO SUMO %i PUNTOS\n\n",
			       resultado_jugada.jugador1,
			       resultado_jugada.jugador2);

		printf("TU PUNTAJE: %i     -     PUNTAJE ADVERSARIO: %i\nRONDAS JUGADAS: %i (son 9 rondas en total)\n\n",
		       juego_obtener_puntaje(estado->juego, JUGADOR1),
		       juego_obtener_puntaje(estado->juego, JUGADOR2),
		       estado->rondas_jugadas);

		jugada_t jugada_jugador = pedir_jugada(estado);
		jugada_t jugada_adversario =
			adversario_proxima_jugada(estado->adversario);
		resultado_jugada = juego_jugar_turno(
			estado->juego, jugada_jugador, jugada_adversario);
		limpiar_consola();
		adversario_informar_jugada(estado->adversario, jugada_jugador);
		if (resultado_jugada.jugador1 == ATAQUE_ERROR) {
			printf("\nNo se pudo realizar la jugada anterior, recorda que no podes usar ataques que ya han sido utilizados y de escribir correctamente los nombres del pokemon y el ataque\n\n\n\n");
		} else {
			estado->rondas_jugadas++;
		}
	}

	limpiar_consola();
	if (juego_finalizado(estado->juego)) {
		if (juego_obtener_puntaje(estado->juego, JUGADOR1) >
		    juego_obtener_puntaje(estado->juego, JUGADOR2)) {
			imprimir_grafica_ganador();
		} else if (juego_obtener_puntaje(estado->juego, JUGADOR1) <
			   juego_obtener_puntaje(estado->juego, JUGADOR2)) {
			imprimir_grafica_perdedor();
		} else {
			imprimir_grafica_empate();
		}
		printf("TU PUNTAJE: %i     -     PUNTAJE ADVERSARIO: %i\n\n",
		       juego_obtener_puntaje(estado->juego, JUGADOR1),
		       juego_obtener_puntaje(estado->juego, JUGADOR2));
		printf("\n\n\nGRACIAS POR HABER JUGADO EL JUEGO! volve a jugarlo cuando quieras!\n\n\n");
	}
	return true;
}

bool salir(void *e)
{
	if (!e)
		return true;
	struct estado_juego *estado = e;
	estado->continuar = false;
	return true;
}

bool mostrar_ayuda(void *e)
{
	printf("\n");
	printf("COMANDO	 |  DESCRIPCION\n");
	printf("---------|------------------------\n");
	printf("  'c' 	 |  Cargar un archivo\n");
	printf("  'l' 	 |  Listar pokemones\n");
	printf("  's' 	 |  Seleccionar pokemones\n");
	printf("  'j' 	 |  Arranca a jugar\n");
	printf("  'q' 	 |  Sale del juego\n");
	return true;
}