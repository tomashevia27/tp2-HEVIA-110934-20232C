#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "estructura_pokemon.h"
#include "pokemon.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "comparador.h"

struct adversario {
	lista_t *pokemones_disponibles;
	lista_t *pokemones_adversario;
	lista_t *pokemones_jugador;
	jugada_t jugadas_posibles[9];
	bool se_uso_jugada[9];
};

adversario_t *adversario_crear(lista_t *pokemon)
{
	if (!pokemon)
		return NULL;

	adversario_t *adversario = calloc(1, sizeof(adversario_t));
	if (!adversario)
		return NULL;

	adversario->pokemones_disponibles = pokemon;
	adversario->pokemones_adversario = lista_crear();
	if (!adversario->pokemones_adversario) {
		free(adversario);
		return NULL;
	}

	adversario->pokemones_jugador = lista_crear();
	if (!adversario->pokemones_adversario) {
		free(adversario->pokemones_adversario);
		free(adversario);
		return NULL;
	}

	for (int i = 0; i < 9; i++) {
		adversario->se_uso_jugada[i] = false;
		strcpy(adversario->jugadas_posibles[i].pokemon, "\0");
		strcpy(adversario->jugadas_posibles[i].ataque, "\0");
	}
	return adversario;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	srand((unsigned int)time(NULL));
	int posicion_1 = rand() % 3;
	int posicion_2 = rand() % 3;
	while (posicion_2 == posicion_1)
		posicion_2 = rand() % 3;
	int posicion_3 = rand() % 3;
	while (posicion_3 == posicion_1 || posicion_3 == posicion_2)
		posicion_3 = rand() % 3;

	pokemon_t *pokemon_1 = lista_elemento_en_posicion(
		adversario->pokemones_disponibles, (size_t)posicion_1);
	pokemon_t *pokemon_2 = lista_elemento_en_posicion(
		adversario->pokemones_disponibles, (size_t)posicion_2);
	pokemon_t *pokemon_3 = lista_elemento_en_posicion(
		adversario->pokemones_disponibles, (size_t)posicion_3);
	if (!pokemon_1 || !pokemon_2 || !pokemon_3)
		return false;

	*nombre1 = (char *)pokemon_nombre(pokemon_1);
	*nombre2 = (char *)pokemon_nombre(pokemon_2);
	*nombre3 = (char *)pokemon_nombre(pokemon_3);

	adversario->pokemones_adversario =
		lista_insertar(adversario->pokemones_adversario, pokemon_1);
	adversario->pokemones_adversario =
		lista_insertar(adversario->pokemones_adversario, pokemon_2);
	adversario->pokemones_jugador =
		lista_insertar(adversario->pokemones_jugador, pokemon_2);

	return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	pokemon_t *pokemon_1 = lista_buscar_elemento(
		adversario->pokemones_disponibles, comparar_pokemones, nombre1);
	pokemon_t *pokemon_2 = lista_buscar_elemento(
		adversario->pokemones_disponibles, comparar_pokemones, nombre2);
	pokemon_t *pokemon_3 = lista_buscar_elemento(
		adversario->pokemones_disponibles, comparar_pokemones, nombre3);
	if (!pokemon_1 || !pokemon_2 || !pokemon_3)
		return false;

	adversario->pokemones_jugador =
		lista_insertar(adversario->pokemones_jugador, pokemon_1);
	adversario->pokemones_jugador =
		lista_insertar(adversario->pokemones_jugador, pokemon_2);
	adversario->pokemones_adversario =
		lista_insertar(adversario->pokemones_adversario, pokemon_3);

	return true;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	if (strcmp(adversario->jugadas_posibles[0].pokemon, "\0") == 0) {
		int n = 0;
		for (size_t i = 0; i < 3; i++) {
			pokemon_t *pokemon_adversario =
				lista_elemento_en_posicion(
					adversario->pokemones_adversario, i);
			for (int j = 0; j < 3; j++) {
				strcpy(adversario->jugadas_posibles[n].pokemon,
				       pokemon_nombre(pokemon_adversario));
				strcpy(adversario->jugadas_posibles[n].ataque,
				       pokemon_adversario->ataques[j].nombre);
				n++;
			}
		}
	}

	srand((unsigned int)time(NULL));
	int numero_aleatorio = rand() % 9;

	while (adversario->se_uso_jugada[numero_aleatorio]) {
		numero_aleatorio++;
		if (numero_aleatorio == 9)
			numero_aleatorio = 0;
	}

	adversario->se_uso_jugada[numero_aleatorio] = true;

	return adversario->jugadas_posibles[numero_aleatorio];
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
}

void adversario_destruir(adversario_t *adversario)
{
	lista_destruir(adversario->pokemones_adversario);
	lista_destruir(adversario->pokemones_jugador);
	free(adversario);
}
