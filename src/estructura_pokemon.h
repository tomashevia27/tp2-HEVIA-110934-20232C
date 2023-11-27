#ifndef ESTRUCTURA_POKEMON_H_
#define ESTRUCTURA_POKEMON_H_

#include "pokemon.h"
#include "tipo.h"
#include "ataque.h"

#define CANT_ATAQUES 3

struct pokemon {
	char nombre[20];
	enum TIPO tipo_pokemon;
	struct ataque ataques[CANT_ATAQUES];
	bool ataque_fue_usado[CANT_ATAQUES];
};

struct info_pokemon {
	pokemon_t **pokemones;
	int cantidad_pokemones;
};

#endif // ESTRUCTURA_POKEMON_H_
