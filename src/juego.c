#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include "pokemon.h"
#include "ataque.h"
#include "adversario.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CANT_ATAQUES 3

struct pokemon {
	char nombre[20];
	enum TIPO tipo_pokemon;
	struct ataque ataques[CANT_ATAQUES];
};

struct info_pokemon {
	pokemon_t **pokemones;
	int cantidad_pokemones;
};

typedef struct jugador{
	pokemon_t* pokemones[3];
	unsigned int puntaje;
	bool pokemones_usados[3];
}jugador_t;

struct juego{
	informacion_pokemon_t* info_pokemon;
	lista_t* lista_pokemones;
	jugador_t* jugador_1;
	jugador_t* jugador_2;
	bool juego_finalizado;

};

juego_t *juego_crear()
{
	juego_t* juego = calloc(1, sizeof(juego_t));
	if(!juego)
		return NULL;

	juego->jugador_1 = calloc(1,sizeof(jugador_t));
	if(!juego->jugador_1){
		free(juego);
		return NULL;
	}

	juego->jugador_2 = calloc(1,sizeof(jugador_t));
	if(!juego->jugador_2){
		free(juego->jugador_1);
		free(juego);
		return NULL;
	}

	for (int i = 0; i < 3; i++){
		juego->jugador_1->pokemones_usados[i] = false;
		juego->jugador_2->pokemones_usados[i] = false;
	}

	return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if(!juego || !archivo)
		return ERROR_GENERAL;

	juego->info_pokemon = pokemon_cargar_archivo(archivo);
	if(!juego->info_pokemon){
		return ERROR_GENERAL;
	}
	if(juego->info_pokemon->cantidad_pokemones < 4){
		pokemon_destruir_todo(juego->info_pokemon);
		juego->info_pokemon = NULL;
		return POKEMON_INSUFICIENTES;
	}
	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if(!juego)
		return NULL;
	if(!juego->info_pokemon)
		return NULL;
	
	juego->lista_pokemones = lista_crear();
	if(!juego->lista_pokemones)
		return NULL;
	
	for (int i = 0; i < juego->info_pokemon->cantidad_pokemones; i++){
		juego->lista_pokemones = lista_insertar(juego->lista_pokemones, juego->info_pokemon->pokemones[i]);
		if(!juego->lista_pokemones){
			juego_destruir(juego);
			return NULL;
		}
	}
	return juego->lista_pokemones;
}

int comparar_pokemones(void* p1, void* nombre){
	return strcmp(((pokemon_t*)p1)->nombre,(char*)nombre);
}

JUEGO_ESTADO asignar_pokemones_jugador(jugador_t* jugador, lista_t* lista_pokemones, char* nombres[3]){
	pokemon_t* pokemon_encontrado = NULL;
	for (int i = 0; i < 3; i++){
		pokemon_encontrado = (pokemon_t*)lista_buscar_elemento(lista_pokemones, comparar_pokemones, nombres[i]);
		if(!pokemon_encontrado){
			for (int i = 0; i < 3; i++){
				jugador->pokemones[i] = NULL;
			}
			return POKEMON_INEXISTENTE;
		} else{
			jugador->pokemones[i] = pokemon_encontrado;
		}
	}
	return TODO_OK;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if(!juego){
		return ERROR_GENERAL;
	}

	if(strcmp(nombre1,nombre2)==0 || strcmp(nombre1,nombre3)==0 || strcmp(nombre2,nombre3)==0)
		return POKEMON_REPETIDO;

	char* nombres[3] = {(char*)nombre1, (char*)nombre2, (char*)nombre3};
	if(jugador == JUGADOR1){
		return asignar_pokemones_jugador(juego->jugador_1, juego->lista_pokemones, nombres);
	} else {
		return asignar_pokemones_jugador(juego->jugador_2, juego->lista_pokemones, nombres);
	}
}

RESULTADO_ATAQUE calcular_ataque(enum TIPO tipo_ataque, enum TIPO tipo_pokemon_rival){
	if((tipo_ataque == FUEGO && tipo_pokemon_rival == PLANTA) || (tipo_ataque == PLANTA && tipo_pokemon_rival == ROCA) || (tipo_ataque == ROCA && tipo_pokemon_rival == ELECTRICO) || (tipo_ataque == ELECTRICO && tipo_pokemon_rival == AGUA) || (tipo_ataque == AGUA && tipo_pokemon_rival == FUEGO)){
		return ATAQUE_EFECTIVO;
	} else 	if((tipo_ataque == FUEGO && tipo_pokemon_rival == AGUA) || (tipo_ataque == PLANTA && tipo_pokemon_rival == FUEGO) || (tipo_ataque == ROCA && tipo_pokemon_rival == PLANTA) || (tipo_ataque == ELECTRICO && tipo_pokemon_rival == ROCA) || (tipo_ataque == AGUA && tipo_pokemon_rival == ELECTRICO)){
		return ATAQUE_INEFECTIVO;
	}
	return ATAQUE_REGULAR;
}

unsigned int calcular_puntaje(RESULTADO_ATAQUE resultado, unsigned int poder){
	printf("entro a calcular_ataque\n");
	if(resultado == ATAQUE_EFECTIVO)
		return (poder*3);
	if(resultado == ATAQUE_INEFECTIVO)
		return (poder/2);
	return poder;
}

pokemon_t* buscar_pokemon_jugador(char* nombre_pokemon, jugador_t* jugador){
	for (int i = 0; i < 3; i++){
		if(strcmp(nombre_pokemon, jugador->pokemones[i]->nombre) == 0){
			if(jugador->pokemones_usados[i] == false){
				printf("entro al if del false, el pokemon es:%s\n", nombre_pokemon);
				jugador->pokemones_usados[i] = true;
				return jugador->pokemones[i];
			}
			return NULL;
		}
	}
	return NULL;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{

	resultado_jugada_t resultado;
	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;

	if(!juego)
		return resultado;

	pokemon_t* pokemon_jugador1 = NULL;
	pokemon_jugador1 = buscar_pokemon_jugador(jugada_jugador1.pokemon, juego->jugador_1);
	if(!pokemon_jugador1)
		return resultado;
	struct ataque* ataque_jugador1 = (struct ataque*)pokemon_buscar_ataque(pokemon_jugador1, jugada_jugador1.ataque);
	if(!ataque_jugador1)
		return resultado;

	printf("el pokemon del J1 es: %s, ataque: %s\n", pokemon_jugador1->nombre, ataque_jugador1->nombre);

	pokemon_t* pokemon_jugador2 = NULL;
	pokemon_jugador2 = buscar_pokemon_jugador(jugada_jugador2.pokemon, juego->jugador_2);
	if(!pokemon_jugador2)
		return resultado;
	struct ataque* ataque_jugador2 = (struct ataque*)pokemon_buscar_ataque(pokemon_jugador2, jugada_jugador2.ataque);
	if(!ataque_jugador2)
		return resultado;
	
	printf("el pokemon del J2 es: %s, ataque: %s\n", pokemon_jugador2->nombre, ataque_jugador2->nombre);

	
	resultado.jugador1 = calcular_ataque(ataque_jugador1->tipo, pokemon_jugador2->tipo_pokemon);
	printf("puntaje jugador 1 antes de calcular:%i\n", juego->jugador_1->puntaje);
	juego->jugador_1->puntaje += calcular_puntaje(resultado.jugador1, ataque_jugador1->poder);
	printf("puntaje jugador 1 despues de calcular:%i\n", juego->jugador_1->puntaje);
	resultado.jugador2 = calcular_ataque(ataque_jugador2->tipo, pokemon_jugador1->tipo_pokemon);
	printf("puntaje jugador 2 antes de calcular:%i\n", juego->jugador_2->puntaje);
	juego->jugador_2->puntaje += calcular_puntaje(resultado.jugador2, ataque_jugador2->poder);
	printf("puntaje jugador 2 despues de calcular:%i\n", juego->jugador_2->puntaje);
	// if(resultado.jugador1 == ATAQUE_EFECTIVO){
	// 	juego->jugador_1->puntaje += (int)(ataque_jugador1->poder)*3; 
	// } else if(resultado.jugador1 == ATAQUE_INEFECTIVO){
	// 	juego->jugador_1->puntaje += (int)(ataque_jugador1->poder)/2;
	// } else{
	// 	juego->jugador_1->puntaje += (int)(ataque_jugador1->poder);
	// }
	
	// if(resultado.jugador2 == ATAQUE_EFECTIVO){
	// 	juego->jugador_2->puntaje += (int)(ataque_jugador2->poder)*3; 
	// } else if(resultado.jugador2 == ATAQUE_INEFECTIVO){
	// 	juego->jugador_2->puntaje += (int)(ataque_jugador2->poder)/2;
	// } else{
	// 	juego->jugador_2->puntaje += (int)(ataque_jugador2->poder);
	// }

	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if(!juego){
		return 0;
	}

	if(jugador == JUGADOR1)
		return (int)juego->jugador_1->puntaje;
	
	return (int)juego->jugador_2->puntaje;
}

bool juego_finalizado(juego_t *juego)
{
	return juego->juego_finalizado;
}

void juego_destruir(juego_t *juego)
{
}
