#ifndef FUNCIONES_MAIN_H_
#define FUNCIONES_MAIN_H_

#include "ataque.h"
#include "juego.h"
#include "lista.h"
#include "menu.h"
#include "adversario.h"
#include <stdbool.h>

struct estado_juego {
	juego_t *juego;
	adversario_t *adversario;
	lista_t *pokemones_disponibles;
	bool continuar;
	bool se_eligieron_pokemones;
	bool archivo_cargado;
	int rondas_jugadas;
	menu_t *menu;
};

void limpiar_consola();

bool cargar_archivo_pokemones(void *e);

bool listar_pokemones(void *e);

bool seleccionar_pokemones(void *e);

bool jugar_rondas(void *e);

bool mostrar_ayuda(void *e);

bool salir(void *e);

#endif // FUNCIONES_MAIN_H_