#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>
#include <stdbool.h>

#define CANT_ATAQUES 3
#define CANT_CARACTERES 255

struct pokemon {
	char nombre[20];
	enum TIPO tipo_pokemon;
	struct ataque ataques[CANT_ATAQUES];
};

struct info_pokemon {
	pokemon_t **pokemones;
	int cantidad_pokemones;
};

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip != NULL) {
		for (int i = 0; i < ip->cantidad_pokemones; i++) {
			free(ip->pokemones[i]);
		}
		free(ip->pokemones);
		free(ip);
	}
}

enum TIPO char_a_enum_tipo(char caracter_tipo)
{
	enum TIPO tipo_enum = -1;
	if (caracter_tipo == 'N') {
		tipo_enum = NORMAL;
	} else if (caracter_tipo == 'F') {
		tipo_enum = FUEGO;
	} else if (caracter_tipo == 'A') {
		tipo_enum = AGUA;
	} else if (caracter_tipo == 'P') {
		tipo_enum = PLANTA;
	} else if (caracter_tipo == 'E') {
		tipo_enum = ELECTRICO;
	} else if (caracter_tipo == 'R') {
		tipo_enum = ROCA;
	}
	return tipo_enum;
}

pokemon_t *leer_pokemon(FILE *archivo)
{
	pokemon_t *pokemon_leido = malloc(sizeof(pokemon_t));
	char caracter_tipo;

	if (fscanf(archivo, "%[^;];%c\n", pokemon_leido->nombre,
		   &caracter_tipo) != 2) {
		free(pokemon_leido);
		return NULL;
	}
	pokemon_leido->tipo_pokemon = char_a_enum_tipo(caracter_tipo);
	if (pokemon_leido->tipo_pokemon == -1) {
		free(pokemon_leido);
		return NULL;
	}

	bool error_asignacion = false;
	for (int i = 0; i < CANT_ATAQUES && !error_asignacion; i++) {
		if (fscanf(archivo, "%[^;];%c;%u\n",
			   pokemon_leido->ataques[i].nombre, &caracter_tipo,
			   &(pokemon_leido->ataques[i].poder)) != 3) {
			error_asignacion = true;
		}
		pokemon_leido->ataques[i].tipo =
			char_a_enum_tipo(caracter_tipo);
		if (pokemon_leido->ataques[i].tipo == -1) {
			error_asignacion = true;
		}
	}

	if (error_asignacion) {
		free(pokemon_leido);
		return NULL;
	}

	return pokemon_leido;
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if (path == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(path, "r");
	if (archivo == NULL) {
		return NULL;
	}

	informacion_pokemon_t *info_pokemon =
		calloc(1, sizeof(informacion_pokemon_t));
	if (info_pokemon == NULL) {
		fclose(archivo);
		return NULL;
	}

	pokemon_t *pokemon_leido;
	bool fallo_realloc = false;

	while ((pokemon_leido = leer_pokemon(archivo)) != NULL &&
	       !fallo_realloc) {
		(info_pokemon->cantidad_pokemones)++;
		pokemon_t **puntero_aux = realloc(
			info_pokemon->pokemones,
			(long unsigned int)(info_pokemon->cantidad_pokemones) *
				sizeof(pokemon_t));
		if (puntero_aux == NULL) {
			free(pokemon_leido);
			fclose(archivo);
			fallo_realloc = true;
		} else {
			info_pokemon->pokemones = puntero_aux;
			info_pokemon
				->pokemones[(info_pokemon->cantidad_pokemones) -
					    1] = pokemon_leido;
		}
	}

	fclose(archivo);

	if (info_pokemon->cantidad_pokemones == 0) {
		free(info_pokemon);
		return NULL;
	}

	return info_pokemon;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (ip == NULL || nombre == NULL) {
		return NULL;
	}

	int indice_pokemon_encontrado = -1;
	for (int i = 0;
	     i < ip->cantidad_pokemones && indice_pokemon_encontrado == -1;
	     i++) {
		if (strcmp(ip->pokemones[i]->nombre, nombre) == 0) {
			indice_pokemon_encontrado = i;
		}
	}

	return (indice_pokemon_encontrado != -1) ?
		       ip->pokemones[indice_pokemon_encontrado] :
		       NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip == NULL) {
		return 0;
	}

	return ip->cantidad_pokemones;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NULL;
	}

	return pokemon->nombre;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NORMAL;
	}

	return pokemon->tipo_pokemon;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	int indice_ataque_encontrado = -1;
	for (int i = 0; i < CANT_ATAQUES && indice_ataque_encontrado == -1;
	     i++) {
		if (strcmp(pokemon->ataques[i].nombre, nombre) == 0) {
			indice_ataque_encontrado = i;
		}
	}
	return (indice_ataque_encontrado != -1) ?
		       &(pokemon->ataques[indice_ataque_encontrado]) :
		       NULL;
}

void ordenar_pokemones(informacion_pokemon_t *ip)
{
	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		for (int j = 0; j < ip->cantidad_pokemones - i - 1; j++) {
			if (strcmp(ip->pokemones[j]->nombre,
				   ip->pokemones[j + 1]->nombre) > 0) {
				pokemon_t aux = *(ip->pokemones[j]);
				*(ip->pokemones[j]) = *(ip->pokemones[j + 1]);
				*(ip->pokemones[j + 1]) = aux;
			}
		}
	}
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if (ip == NULL || f == NULL) {
		return 0;
	}

	ordenar_pokemones(ip);

	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		f(ip->pokemones[i], aux);
	}

	return ip->cantidad_pokemones;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	if (pokemon == NULL || f == NULL) {
		return 0;
	}

	int i = 0;
	while (i < CANT_ATAQUES) {
		f(&(pokemon->ataques[i]), aux);
		i++;
	}

	return i;
}
