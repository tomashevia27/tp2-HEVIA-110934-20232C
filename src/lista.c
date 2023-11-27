#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

struct nodo {
	void *elemento;
	struct nodo *siguiente;
};

struct lista {
	struct nodo *nodo_inicio;
	struct nodo *nodo_final;
	size_t cantidad_nodos;
};

struct lista_iterador {
	struct nodo *nodo_actual;
	lista_t *lista;
};

lista_t *lista_crear()
{
	lista_t *lista = calloc(1, sizeof(lista_t));
	if (!lista) {
		return NULL;
	}
	return lista;
}

bool lista_vacia(lista_t *lista)
{
	return (!lista || !lista->nodo_inicio);
}

struct nodo *obtener_nodo_en_posicion(lista_t *lista, size_t posicion)
{
	struct nodo *nodo_en_posicion = lista->nodo_inicio;
	for (size_t posicion_nodo = 0;
	     nodo_en_posicion->siguiente != NULL && posicion_nodo < posicion;
	     posicion_nodo++) {
		nodo_en_posicion = nodo_en_posicion->siguiente;
	}
	return nodo_en_posicion;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista) {
		return NULL;
	}

	struct nodo *nodo_a_insertar = calloc(1, sizeof(struct nodo));
	if (nodo_a_insertar == NULL) {
		return NULL;
	}
	nodo_a_insertar->elemento = elemento;

	if (lista_vacia(lista)) {
		lista->nodo_inicio = nodo_a_insertar;
	} else {
		lista->nodo_final->siguiente = nodo_a_insertar;
	}

	lista->nodo_final = nodo_a_insertar;
	(lista->cantidad_nodos)++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista) {
		return NULL;
	}

	if (posicion >= lista->cantidad_nodos) {
		lista = lista_insertar(lista, elemento);
		if (!lista) {
			return NULL;
		}
		return lista;
	}

	struct nodo *nuevo_nodo = calloc(1, sizeof(struct nodo));
	if (!nuevo_nodo) {
		return NULL;
	}
	nuevo_nodo->elemento = elemento;

	if (posicion == 0) {
		nuevo_nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo_nodo;
		(lista->cantidad_nodos)++;
		return lista;
	}

	struct nodo *nodo_posicion_anterior =
		obtener_nodo_en_posicion(lista, posicion - 1);
	nuevo_nodo->siguiente = nodo_posicion_anterior->siguiente;
	nodo_posicion_anterior->siguiente = nuevo_nodo;
	(lista->cantidad_nodos)++;

	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	if (lista->nodo_inicio == lista->nodo_final) {
		struct nodo *nodo_a_eliminar = lista->nodo_inicio;
		void *elemento_eliminado = nodo_a_eliminar->elemento;
		lista->nodo_inicio = NULL;
		lista->nodo_final = NULL;
		lista->cantidad_nodos = 0;
		free(nodo_a_eliminar);
		return elemento_eliminado;
	}

	struct nodo *anteultimo_nodo = lista->nodo_inicio;
	while (anteultimo_nodo->siguiente != lista->nodo_final) {
		anteultimo_nodo = anteultimo_nodo->siguiente;
	}

	struct nodo *nodo_a_eliminar = anteultimo_nodo->siguiente;
	anteultimo_nodo->siguiente = NULL;
	lista->nodo_final = anteultimo_nodo;

	void *elemento_eliminado = nodo_a_eliminar->elemento;
	(lista->cantidad_nodos)--;
	free(nodo_a_eliminar);

	return elemento_eliminado;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (lista_vacia(lista)) {
		return NULL;
	}

	void *elemento_retirado;

	if (posicion > lista->cantidad_nodos) {
		elemento_retirado = lista_quitar(lista);
		if (!elemento_retirado) {
			return NULL;
		}
		return elemento_retirado;
	}

	struct nodo *nodo_a_eliminar;
	if (posicion == 0) {
		nodo_a_eliminar = lista->nodo_inicio;
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
	} else {
		struct nodo *nodo_anterior =
			obtener_nodo_en_posicion(lista, posicion - 1);
		nodo_a_eliminar = nodo_anterior->siguiente;
		nodo_anterior->siguiente = nodo_a_eliminar->siguiente;
	}

	elemento_retirado = nodo_a_eliminar->elemento;
	(lista->cantidad_nodos)--;
	free(nodo_a_eliminar);
	return elemento_retirado;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista_vacia(lista)) {
		return NULL;
	}

	if (posicion >= lista->cantidad_nodos) {
		return NULL;
	}

	struct nodo *nodo_en_posicion =
		obtener_nodo_en_posicion(lista, posicion);

	return nodo_en_posicion->elemento;
}

void *lista_primero(lista_t *lista)
{
	if (lista_vacia(lista)) {
		return NULL;
	}
	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (lista_vacia(lista)) {
		return NULL;
	}
	return lista->nodo_final->elemento;
}

size_t lista_tamanio(lista_t *lista)
{
	if (lista_vacia(lista)) {
		return 0;
	}
	return lista->cantidad_nodos;
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador) {
		return NULL;
	}

	if (lista_vacia(lista)) {
		iterador->nodo_actual = NULL;
	} else {
		iterador->nodo_actual = lista->nodo_inicio;
	}
	iterador->lista = lista;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador) {
		return false;
	}

	return iterador->nodo_actual != NULL;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador) {
		return false;
	}
	if (!iterador->nodo_actual) {
		return false;
	}

	if (iterador->nodo_actual->siguiente) {
		iterador->nodo_actual = iterador->nodo_actual->siguiente;
		return true;
	} else {
		iterador->nodo_actual = NULL;
		return false;
	}
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->nodo_actual) {
		return NULL;
	}
	return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (iterador) {
		iterador->nodo_actual = NULL;
		free(iterador);
	}
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || !comparador || lista_vacia(lista)) {
		return NULL;
	}

	struct nodo *nodo_actual = lista->nodo_inicio;
	void *elemento_encontrado = NULL;

	while (nodo_actual && !elemento_encontrado) {
		if (comparador(nodo_actual->elemento, contexto) == 0) {
			elemento_encontrado = nodo_actual->elemento;
		}
		nodo_actual = nodo_actual->siguiente;
	}
	return elemento_encontrado;
}

void lista_destruir(lista_t *lista)
{
	if (lista) {
		if (!lista_vacia(lista)) {
			struct nodo *nodo_actual = lista->nodo_inicio;
			struct nodo *nodo_siguiente;
			while (nodo_actual->siguiente != NULL) {
				nodo_siguiente = nodo_actual->siguiente;
				free(nodo_actual);
				nodo_actual = nodo_siguiente;
			}
			free(nodo_actual);
			nodo_siguiente = NULL;
		}
		free(lista);
	}
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (funcion && lista) {
		lista_iterador_t *iterador = lista_iterador_crear(lista);
		while (lista_iterador_tiene_siguiente(iterador)) {
			funcion(iterador->nodo_actual->elemento);
			lista_iterador_avanzar(iterador);
		}
		lista_iterador_destruir(iterador);
	}
	if (lista) {
		lista_destruir(lista);
	}
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion || lista_vacia(lista)) {
		return 0;
	}

	struct nodo *nodo_actual = lista->nodo_inicio;
	size_t nodos_recorridos = 1;
	while (funcion(nodo_actual->elemento, contexto) &&
	       nodo_actual->siguiente != NULL) {
		nodo_actual = nodo_actual->siguiente;
		nodos_recorridos++;
	}

	return nodos_recorridos;
}