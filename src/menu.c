#include "menu.h"
#include "hash.h"
#include <stdlib.h>

#include <stdio.h>

struct menu {
	hash_t *comandos;
};

struct informacion_comando {
	char *descripcion;
	bool (*funcion)(void *);
};

menu_t *menu_crear()
{
	menu_t *m = calloc(1, sizeof(menu_t));
	m->comandos = hash_crear(3);
	return m;
}

void menu_agregar_comando(menu_t *m, char *comando, char *descripcion,
			  bool (*f)(void *))
{
	struct informacion_comando *i =
		malloc(sizeof(struct informacion_comando));
	i->descripcion = descripcion;
	i->funcion = f;
	hash_insertar(m->comandos, comando, i, NULL);
}

bool menu_ejecutar_comando(menu_t *m, char *comando, void *contexto)
{
	struct informacion_comando *i = hash_obtener(m->comandos, comando);
	if (!i)
		return false;
	return (i->funcion(contexto));
}

void destruir_informacion_comando(void *i)
{
	struct informacion_comando *info_comando = i;
	free(info_comando);
}

void menu_destruir(menu_t *m)
{
	hash_destruir_todo(m->comandos, destruir_informacion_comando);
	free(m);
}