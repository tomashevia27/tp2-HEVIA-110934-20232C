#include "juego.h"
#include "lista.h"
#include <stdbool.h>
#include "pokemon.h"
#include "adversario.h"
#include "estructura_pokemon.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CANT_ATAQUES 3

typedef struct jugador{
	lista_t* pokemones;
	unsigned int puntaje;
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
	juego->jugador_1->pokemones = lista_crear();
	if(!juego->jugador_1->pokemones){
		free(juego);
	}

	juego->jugador_2 = calloc(1,sizeof(jugador_t));
	if(!juego->jugador_2){
		lista_destruir(juego->jugador_1->pokemones);
		free(juego->jugador_1);
		free(juego);
		return NULL;
	}

	juego->jugador_2->pokemones = lista_crear();
	if(!juego->jugador_2->pokemones){
		lista_destruir(juego->jugador_1->pokemones);
		free(juego->jugador_1);
		free(juego->jugador_2);				// CAMBIAR TODOS LOS FREE POR DESTRUIR_JUEGO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		free(juego);
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

bool imprimir_pokes(void* p, void* aux){
	pokemon_t* pokemon = (pokemon_t*)p;
	printf("pokemon: %s, ataques: %s, %s, %s\n", pokemon->nombre, pokemon->ataques[0].nombre, pokemon->ataques[1].nombre, pokemon->ataques[2].nombre);
	return true;
}


JUEGO_ESTADO asignar_pokemones_jugador(jugador_t* jugador, lista_t* lista_pokemones, char* nombres[3]){
	pokemon_t* pokemon_encontrado = NULL;
	for (int i = 0; i < 3; i++){
		pokemon_encontrado = (pokemon_t*)lista_buscar_elemento(lista_pokemones, comparar_pokemones, nombres[i]);
		if(!pokemon_encontrado)
			return POKEMON_INEXISTENTE;
	
		pokemon_t* pokemon_copia_jugador = malloc(sizeof(pokemon_t));
		if(!pokemon_copia_jugador)
			return POKEMON_INEXISTENTE;

		*pokemon_copia_jugador = *pokemon_encontrado;
		jugador->pokemones = lista_insertar(jugador->pokemones, pokemon_copia_jugador);
	}

	printf("NUEVA LISTA\n");
	int j = (int)lista_con_cada_elemento(jugador->pokemones, imprimir_pokes, NULL);
	
	if(j>=0)
		return TODO_OK;
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
	if(resultado == ATAQUE_EFECTIVO)
		return (poder*3);
	if(resultado == ATAQUE_INEFECTIVO)
		return (poder/2 + poder%2);
	return poder;
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
	pokemon_jugador1 = lista_buscar_elemento(juego->lista_pokemones, comparar_pokemones, jugada_jugador1.pokemon);
	if(!pokemon_jugador1)
		return resultado;

	int posicion_ataque_j1 = -1;
	for (int i = 0; i < 3; i++){
		if(strcmp(pokemon_jugador1->ataques[i].nombre, jugada_jugador1.ataque) == 0 && !pokemon_jugador1->ataque_fue_usado[i])
			posicion_ataque_j1 = i;
	}
	if(posicion_ataque_j1 == -1)
		return resultado;

	pokemon_t* pokemon_jugador2 = NULL;
	pokemon_jugador2 = lista_buscar_elemento(juego->lista_pokemones, comparar_pokemones, jugada_jugador2.pokemon);
	if(!pokemon_jugador2)
		return resultado;

	int posicion_ataque_j2 = -1;
	for (int i = 0; i < 3; i++){
		if(strcmp(pokemon_jugador2->ataques[i].nombre, jugada_jugador2.ataque) == 0 && !pokemon_jugador2->ataque_fue_usado[i])
			posicion_ataque_j2 = i;
	}
	if(posicion_ataque_j2 == -1)
		return resultado;

	resultado.jugador1 = calcular_ataque(pokemon_jugador1->ataques[posicion_ataque_j1].tipo, pokemon_jugador2->tipo_pokemon);
	juego->jugador_1->puntaje += calcular_puntaje(resultado.jugador1, pokemon_jugador1->ataques[posicion_ataque_j1].poder);
	resultado.jugador2 = calcular_ataque(pokemon_jugador2->ataques[posicion_ataque_j2].tipo, pokemon_jugador1->tipo_pokemon);
	juego->jugador_2->puntaje += calcular_puntaje(resultado.jugador2, pokemon_jugador2->ataques[posicion_ataque_j2].poder);

	pokemon_jugador1->ataque_fue_usado[posicion_ataque_j1] = true;
	pokemon_jugador2->ataque_fue_usado[posicion_ataque_j2] = true;
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
	lista_destruir(juego->jugador_1->pokemones);
	free(juego->jugador_1);
	lista_destruir(juego->jugador_2->pokemones);
	free(juego->jugador_2);
	lista_destruir(juego->lista_pokemones);
	pokemon_destruir_todo(juego->info_pokemon);
	free(juego);
}
