#include "juego.h"
#include "lista.h"
#include <stdbool.h>
#include "pokemon.h"
#include "adversario.h"
#include "ataque.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "comparador.h"
#include "abb.h"

#define CANT_ATAQUES 3

typedef struct jugador {
	lista_t *pokemones;
	abb_t *ataques_disponibles;
	unsigned int puntaje;
} jugador_t;

struct juego {
	informacion_pokemon_t *info_pokemon;
	lista_t *lista_pokemones;
	jugador_t *jugador_1;
	jugador_t *jugador_2;
	int rondas_jugadas;
};

int funcion_comparadora_abb(void *p1, void *p2)
{
	pokemon_t *pokemon1 = p1;
	pokemon_t *pokemon2 = p2;
	return strcmp(pokemon_nombre(pokemon1), pokemon_nombre(pokemon2));
}

juego_t *juego_crear()
{
	juego_t *juego = calloc(1, sizeof(juego_t));
	if (!juego)
		return NULL;

	juego->jugador_1 = calloc(1, sizeof(jugador_t));
	if (!juego->jugador_1) {
		free(juego);
		return NULL;
	}
	juego->jugador_1->pokemones = lista_crear();
	if (!juego->jugador_1->pokemones) {
		free(juego->jugador_1);
		free(juego);
	}

	juego->jugador_2 = calloc(1, sizeof(jugador_t));
	if (!juego->jugador_2) {
		juego_destruir(juego);
		return NULL;
	}

	juego->jugador_2->pokemones = lista_crear();
	if (!juego->jugador_2->pokemones)
		juego_destruir(juego);

	abb_comparador comparador = (abb_comparador)funcion_comparadora_abb;
	juego->jugador_1->ataques_disponibles = abb_crear(comparador);
	juego->jugador_2->ataques_disponibles = abb_crear(comparador);
	if (!juego->jugador_1->ataques_disponibles ||
	    !juego->jugador_2->ataques_disponibles)
		juego_destruir(juego);

	return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if (!juego || !archivo)
		return ERROR_GENERAL;

	juego->info_pokemon = pokemon_cargar_archivo(archivo);
	if (!juego->info_pokemon) {
		return ERROR_GENERAL;
	}
	if (pokemon_cantidad(juego->info_pokemon) < 4) {
		pokemon_destruir_todo(juego->info_pokemon);
		juego->info_pokemon = NULL;
		return POKEMON_INSUFICIENTES;
	}
	return TODO_OK;
}

void insertar_pokemones_en_lista(pokemon_t *pokemon, void *l)
{
	lista_t *lista = l;
	if (lista != NULL)
		lista = lista_insertar(lista, pokemon);
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if (!juego)
		return NULL;
	if (!juego->info_pokemon)
		return NULL;

	if (lista_vacia(juego->lista_pokemones)) {
		juego->lista_pokemones = lista_crear();
		if (!juego->lista_pokemones)
			return NULL;

		con_cada_pokemon(juego->info_pokemon,
				 insertar_pokemones_en_lista,
				 juego->lista_pokemones);
		if (!juego->lista_pokemones)
			return NULL;
	}

	return juego->lista_pokemones;
}

void listar_ataques(const struct ataque *ataque, void *ataques_disponibles)
{
	abb_t *abb_ataques = ataques_disponibles;
	abb_ataques = abb_insertar(abb_ataques, (struct ataque *)ataque);
}

JUEGO_ESTADO asignar_pokemones_jugador(jugador_t *jugador,
				       jugador_t *adversario,
				       lista_t *lista_pokemones,
				       char *nombres[3])
{
	pokemon_t *pokemones_encontrados[3];
	for (int i = 0; i < 3; i++) {
		pokemones_encontrados[i] = (pokemon_t *)lista_buscar_elemento(
			lista_pokemones, comparar_pokemones, nombres[i]);
		if (!pokemones_encontrados[i])
			return POKEMON_INEXISTENTE;
	}

	if (con_cada_ataque(pokemones_encontrados[0], listar_ataques,
			    jugador->ataques_disponibles) != 3 ||
	    con_cada_ataque(pokemones_encontrados[1], listar_ataques,
			    jugador->ataques_disponibles) != 3 ||
	    con_cada_ataque(pokemones_encontrados[2], listar_ataques,
			    adversario->ataques_disponibles) != 3)
		return ERROR_GENERAL;

	jugador->pokemones =
		lista_insertar(jugador->pokemones, pokemones_encontrados[0]);
	jugador->pokemones =
		lista_insertar(jugador->pokemones, pokemones_encontrados[1]);
	adversario->pokemones =
		lista_insertar(adversario->pokemones, pokemones_encontrados[2]);
	if (!jugador->pokemones || !adversario->pokemones)
		return ERROR_GENERAL;

	return TODO_OK;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if (!juego) {
		return ERROR_GENERAL;
	}

	if (strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 ||
	    strcmp(nombre2, nombre3) == 0)
		return POKEMON_REPETIDO;

	char *nombres[3] = { (char *)nombre1, (char *)nombre2,
			     (char *)nombre3 };
	if (jugador == JUGADOR1)
		return asignar_pokemones_jugador(juego->jugador_1,
						 juego->jugador_2,
						 juego->lista_pokemones,
						 nombres);
	else
		return asignar_pokemones_jugador(juego->jugador_2,
						 juego->jugador_1,
						 juego->lista_pokemones,
						 nombres);
}

RESULTADO_ATAQUE calcular_ataque(enum TIPO tipo_ataque,
				 enum TIPO tipo_pokemon_rival)
{
	if ((tipo_ataque == FUEGO && tipo_pokemon_rival == PLANTA) ||
	    (tipo_ataque == PLANTA && tipo_pokemon_rival == ROCA) ||
	    (tipo_ataque == ROCA && tipo_pokemon_rival == ELECTRICO) ||
	    (tipo_ataque == ELECTRICO && tipo_pokemon_rival == AGUA) ||
	    (tipo_ataque == AGUA && tipo_pokemon_rival == FUEGO))
		return ATAQUE_EFECTIVO;
	if ((tipo_ataque == FUEGO && tipo_pokemon_rival == AGUA) ||
	    (tipo_ataque == PLANTA && tipo_pokemon_rival == FUEGO) ||
	    (tipo_ataque == ROCA && tipo_pokemon_rival == PLANTA) ||
	    (tipo_ataque == ELECTRICO && tipo_pokemon_rival == ROCA) ||
	    (tipo_ataque == AGUA && tipo_pokemon_rival == ELECTRICO))
		return ATAQUE_INEFECTIVO;
	return ATAQUE_REGULAR;
}

unsigned int calcular_puntaje(RESULTADO_ATAQUE resultado, unsigned int poder)
{
	if (resultado == ATAQUE_EFECTIVO)
		return (poder * 3);
	if (resultado == ATAQUE_INEFECTIVO)
		return (poder / 2 + poder % 2);
	return poder;
}

int buscar_ataque(void *ataque_usado, void *ataque_jugador)
{
	return strcmp(((struct ataque *)ataque_usado)->nombre,
		      ((struct ataque *)ataque_jugador)->nombre);
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;
	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;

	if (!juego)
		return resultado;

	pokemon_t *pokemon_jugador1 = NULL;
	pokemon_jugador1 = lista_buscar_elemento(juego->lista_pokemones,
						 comparar_pokemones,
						 jugada_jugador1.pokemon);
	if (!pokemon_jugador1)
		return resultado;

	pokemon_t *pokemon_jugador2 = NULL;
	pokemon_jugador2 = lista_buscar_elemento(juego->lista_pokemones,
						 comparar_pokemones,
						 jugada_jugador2.pokemon);
	if (!pokemon_jugador2)
		return resultado;

	struct ataque *ataque_jugador1 = (struct ataque *)pokemon_buscar_ataque(
		pokemon_jugador1, jugada_jugador1.ataque);
	struct ataque *ataque_jugador2 = (struct ataque *)pokemon_buscar_ataque(
		pokemon_jugador2, jugada_jugador2.ataque);
	if (!ataque_jugador1 || !ataque_jugador2)
		return resultado;

	if (abb_buscar(juego->jugador_1->ataques_disponibles,
		       ataque_jugador1) != NULL) {
		resultado.jugador1 = calcular_ataque(
			ataque_jugador1->tipo, pokemon_tipo(pokemon_jugador2));
		juego->jugador_1->puntaje += calcular_puntaje(
			resultado.jugador1, ataque_jugador1->poder);
		ataque_jugador1 = abb_quitar(
			juego->jugador_1->ataques_disponibles, ataque_jugador1);
	}
	if (abb_buscar(juego->jugador_2->ataques_disponibles,
		       ataque_jugador2) != NULL) {
		resultado.jugador2 = calcular_ataque(
			ataque_jugador2->tipo, pokemon_tipo(pokemon_jugador1));
		juego->jugador_2->puntaje += calcular_puntaje(
			resultado.jugador2, ataque_jugador2->poder);
		ataque_jugador1 = abb_quitar(
			juego->jugador_2->ataques_disponibles, ataque_jugador2);
	}

	if (resultado.jugador1 != ATAQUE_ERROR &&
	    resultado.jugador2 != ATAQUE_ERROR)
		juego->rondas_jugadas++;
	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if (!juego)
		return 0;

	if (jugador == JUGADOR1)
		return (int)juego->jugador_1->puntaje;

	return (int)juego->jugador_2->puntaje;
}

bool juego_finalizado(juego_t *juego)
{
	if (!juego)
		return true;
	return (juego->rondas_jugadas >= 9);
}

void juego_destruir(juego_t *juego)
{
	lista_destruir(juego->jugador_1->pokemones);
	abb_destruir(juego->jugador_1->ataques_disponibles);
	free(juego->jugador_1);
	lista_destruir(juego->jugador_2->pokemones);
	abb_destruir(juego->jugador_2->ataques_disponibles);
	free(juego->jugador_2);
	lista_destruir(juego->lista_pokemones);
	pokemon_destruir_todo(juego->info_pokemon);
	free(juego);
}
