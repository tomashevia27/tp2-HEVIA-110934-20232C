#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

abb_t *abb_crear(abb_comparador comparador)
{
	if (comparador == NULL) {
		return NULL;
	}
	abb_t *abb = calloc(1, sizeof(abb_t));
	if (!abb)
		return NULL;
	abb->comparador = comparador;
	return abb;
}

nodo_abb_t *insertar_recursivamente(nodo_abb_t *raiz, void *elemento,
				    int (*comparador)(void *, void *))
{
	if (!raiz) {
		nodo_abb_t *raiz = calloc(1, sizeof(abb_t));
		if (!raiz) {
			return NULL;
		}
		raiz->elemento = elemento;
		return raiz;
	}

	int comparacion = comparador(elemento, raiz->elemento);

	if (comparacion <= 0) {
		raiz->izquierda = insertar_recursivamente(raiz->izquierda,
							  elemento, comparador);
	}

	if (comparacion > 0) {
		raiz->derecha = insertar_recursivamente(raiz->derecha, elemento,
							comparador);
	}

	return raiz;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}
	arbol->nodo_raiz = insertar_recursivamente(arbol->nodo_raiz, elemento,
						   arbol->comparador);
	if (arbol->nodo_raiz != NULL) {
		(arbol->tamanio)++;
	}

	return arbol;
}

nodo_abb_t *extraer_predecesor_inorden(nodo_abb_t *raiz,
				       void **elemento_extraido)
{
	if (raiz->derecha == NULL) {
		*elemento_extraido = raiz->elemento;
		nodo_abb_t *subarbol_izquierdo = raiz->izquierda;
		free(raiz);
		return subarbol_izquierdo;
	}

	raiz->derecha =
		extraer_predecesor_inorden(raiz->derecha, elemento_extraido);

	return raiz;
}

nodo_abb_t *eliminar_recursivo(nodo_abb_t *raiz, void *elemento,
			       int (*comparador)(void *, void *),
			       void **elemento_extraido)
{
	if (!raiz) {
		return NULL;
	}

	int comparacion = comparador(raiz->elemento, elemento);

	if (comparacion == 0) {
		nodo_abb_t *izquierda = raiz->izquierda;
		nodo_abb_t *derecha = raiz->derecha;
		*elemento_extraido = raiz->elemento;

		if (raiz->izquierda != NULL && raiz->derecha != NULL) {
			void *elemento_derecho = NULL;
			raiz->izquierda = extraer_predecesor_inorden(
				raiz->izquierda, &elemento_derecho);

			raiz->elemento = elemento_derecho;
			return raiz;
		}

		free(raiz);
		if (izquierda == NULL) {
			return derecha;
		}
		return izquierda;
	}

	if (comparacion > 0) {
		raiz->izquierda = eliminar_recursivo(raiz->izquierda, elemento,
						     comparador,
						     elemento_extraido);
	}
	if (comparacion < 0) {
		raiz->derecha = eliminar_recursivo(
			raiz->derecha, elemento, comparador, elemento_extraido);
	}

	return raiz;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	if (abb_vacio(arbol))
		return NULL;

	void *elemento_extraido = NULL;
	arbol->nodo_raiz = eliminar_recursivo(arbol->nodo_raiz, elemento,
					      arbol->comparador,
					      &elemento_extraido);
	(arbol->tamanio)--;
	return elemento_extraido;
}

void *busqueda_recursiva(nodo_abb_t *raiz, void *elemento,
			 int (*comparador)(void *, void *))
{
	if (!raiz)
		return NULL;

	int comparacion = comparador(elemento, raiz->elemento);

	if (comparacion < 0) {
		return busqueda_recursiva(raiz->izquierda, elemento,
					  comparador);
	}

	if (comparacion > 0) {
		return busqueda_recursiva(raiz->derecha, elemento, comparador);
	}

	return raiz->elemento;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	return busqueda_recursiva(arbol->nodo_raiz, elemento,
				  arbol->comparador);
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol)
		return true;
	if (arbol->nodo_raiz == NULL && arbol->tamanio == 0) {
		return true;
	}
	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return 0;
	return arbol->tamanio;
}

void abb_destruir_raices(nodo_abb_t *raiz, void (*destructor)(void *))
{
	if (raiz == NULL)
		return;
	abb_destruir_raices(raiz->izquierda, destructor);
	abb_destruir_raices(raiz->derecha, destructor);
	if (destructor) {
		destructor(raiz->elemento);
	}
	free(raiz);
}

void abb_destruir(abb_t *arbol)
{
	if (arbol == NULL)
		return;
	abb_destruir_raices(arbol->nodo_raiz, NULL);
	free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (arbol == NULL)
		return;
	abb_destruir_raices(arbol->nodo_raiz, destructor);
	free(arbol);
}

void con_cada_inorden(nodo_abb_t *raiz, bool (*funcion)(void *, void *),
		      void *aux, size_t *veces_aplicada,
		      bool *seguir_recorriendo)
{
	if (raiz) {
		if ((*seguir_recorriendo) == true)
			con_cada_inorden(raiz->izquierda, funcion, aux,
					 veces_aplicada, seguir_recorriendo);

		if ((*seguir_recorriendo) == true) {
			(*veces_aplicada)++;
			if (!funcion(raiz->elemento, aux)) {
				*seguir_recorriendo = false;
			}
		}

		if ((*seguir_recorriendo) == true)
			con_cada_inorden(raiz->derecha, funcion, aux,
					 veces_aplicada, seguir_recorriendo);
	}
}

void con_cada_preorden(nodo_abb_t *raiz, bool (*funcion)(void *, void *),
		       void *aux, size_t *veces_aplicada,
		       bool *seguir_recorriendo)
{
	if (raiz) {
		if ((*seguir_recorriendo) == true) {
			(*veces_aplicada)++;
			if (!funcion(raiz->elemento, aux)) {
				*seguir_recorriendo = false;
			}
		}

		if ((*seguir_recorriendo) == true)
			con_cada_preorden(raiz->izquierda, funcion, aux,
					  veces_aplicada, seguir_recorriendo);

		if ((*seguir_recorriendo) == true)
			con_cada_preorden(raiz->derecha, funcion, aux,
					  veces_aplicada, seguir_recorriendo);
	}
}

void con_cada_postorden(nodo_abb_t *raiz, bool (*funcion)(void *, void *),
			void *aux, size_t *veces_aplicada,
			bool *seguir_recorriendo)
{
	if (raiz) {
		if ((*seguir_recorriendo) == true)
			con_cada_postorden(raiz->izquierda, funcion, aux,
					   veces_aplicada, seguir_recorriendo);

		if ((*seguir_recorriendo) == true)
			con_cada_postorden(raiz->derecha, funcion, aux,
					   veces_aplicada, seguir_recorriendo);

		if ((*seguir_recorriendo) == true) {
			(*veces_aplicada)++;
			if (!funcion(raiz->elemento, aux)) {
				*seguir_recorriendo = false;
			}
		}
	}
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion) {
		return 0;
	}

	size_t veces_aplicada = 0;
	bool seguir_recorriendo = true;
	if (recorrido == INORDEN) {
		con_cada_inorden(arbol->nodo_raiz, funcion, aux,
				 &veces_aplicada, &seguir_recorriendo);
	} else if (recorrido == PREORDEN) {
		con_cada_preorden(arbol->nodo_raiz, funcion, aux,
				  &veces_aplicada, &seguir_recorriendo);
	} else if (recorrido == POSTORDEN) {
		con_cada_postorden(arbol->nodo_raiz, funcion, aux,
				   &veces_aplicada, &seguir_recorriendo);
	}

	return veces_aplicada;
}

void agregar_elemento_a_array(void *elemento, void **array,
			      size_t tamanio_array,
			      size_t *elementos_almacenados)
{
	if (*elementos_almacenados < tamanio_array) {
		array[*elementos_almacenados] = elemento;
		(*elementos_almacenados)++;
	}
}

void recorrer_recursivo(nodo_abb_t *raiz, abb_recorrido recorrido, void **array,
			size_t tamanio_array, size_t *elementos_almacenados)
{
	if (recorrido == INORDEN) {
		if (raiz->izquierda == NULL) {
			agregar_elemento_a_array(raiz->elemento, array,
						 tamanio_array,
						 elementos_almacenados);

		} else {
			recorrer_recursivo(raiz->izquierda, recorrido, array,
					   tamanio_array,
					   elementos_almacenados);
			agregar_elemento_a_array(raiz->elemento, array,
						 tamanio_array,
						 elementos_almacenados);
		}
		if (raiz->derecha != NULL) {
			recorrer_recursivo(raiz->derecha, recorrido, array,
					   tamanio_array,
					   elementos_almacenados);
		}
	}

	if (recorrido == PREORDEN) {
		agregar_elemento_a_array(raiz->elemento, array, tamanio_array,
					 elementos_almacenados);
		if (raiz->izquierda != NULL) {
			recorrer_recursivo(raiz->izquierda, recorrido, array,
					   tamanio_array,
					   elementos_almacenados);
		}
		if (raiz->derecha != NULL) {
			recorrer_recursivo(raiz->derecha, recorrido, array,
					   tamanio_array,
					   elementos_almacenados);
		}
	}

	if (recorrido == POSTORDEN) {
		if (raiz->izquierda != NULL) {
			recorrer_recursivo(raiz->izquierda, recorrido, array,
					   tamanio_array,
					   elementos_almacenados);
		}
		if (raiz->derecha != NULL) {
			recorrer_recursivo(raiz->derecha, recorrido, array,
					   tamanio_array,
					   elementos_almacenados);
		}
		agregar_elemento_a_array(raiz->elemento, array, tamanio_array,
					 elementos_almacenados);
	}
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol || !array || abb_vacio(arbol))
		return 0;
	if (recorrido != INORDEN && recorrido != PREORDEN &&
	    recorrido != POSTORDEN)
		return 0;

	size_t elementos_almacenados = 0;
	recorrer_recursivo(arbol->nodo_raiz, recorrido, array, tamanio_array,
			   &elementos_almacenados);

	return elementos_almacenados;
}
