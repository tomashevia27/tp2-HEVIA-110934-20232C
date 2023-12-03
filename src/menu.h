#ifndef MENU_H_
#define MENU_H_
#include <stdbool.h>
#include <stdlib.h>
#include "hash.h"

typedef struct menu menu_t;

typedef enum { MENU_OK, MENU_INEXISTENTE, MENU_ERROR } MENU_RESULTADO;

menu_t *menu_crear();

void menu_agregar_comando(menu_t *m, char *comando, char *descripcion,
			  bool (*f)(void *));

bool menu_ejecutar_comando(menu_t *m, char *comando, void *contexto);

void menu_destruir(menu_t *m);

#endif // MENU_H_