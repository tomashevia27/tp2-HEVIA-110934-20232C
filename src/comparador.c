#include "comparador.h"
#include <string.h>
#include "pokemon.h"

int comparar_pokemones(void *p1, void *nombre)
{
	pokemon_t *pokemon = p1;
	return strcmp(pokemon_nombre(pokemon), (char *)nombre);
}