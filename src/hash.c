#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "hash_estructura_privada.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MINIMA 3

size_t funcion_hash(const char *clave)
{
	size_t numero = 5381;
	int entero;
	while ((entero = *clave++)) {
		numero = (((numero << 5) + numero) + (size_t)entero);
	}
	return numero;
}

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));

	if (!hash)
		return NULL;

	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;

	hash->capacidad = capacidad;
	hash->tabla_hash = calloc(capacidad, sizeof(par_t));
	if (!(hash->tabla_hash)) {
		hash_destruir(hash);
		return NULL;
	}

	return hash;
}

hash_t *hash_insertar_par(hash_t *hash, par_t *par)
{
	size_t posicion_hash = (funcion_hash(par->clave) % hash->capacidad);
	par->siguiente = hash->tabla_hash[posicion_hash];
	hash->tabla_hash[posicion_hash] = par;
	hash->cantidad++;
	return hash;
}

hash_t *rehashear(hash_t *hash)
{
	hash_t *nuevo_hash = hash_crear((hash->capacidad) * 2);
	if (!nuevo_hash)
		return NULL;

	for (int posicion = 0; posicion < hash->capacidad; posicion++) {
		par_t *actual = hash->tabla_hash[posicion];
		while (actual) {
			par_t *siguiente = actual->siguiente;
			actual->siguiente = NULL;
			nuevo_hash = hash_insertar_par(nuevo_hash, actual);
			actual = siguiente;
		}
		hash->tabla_hash[posicion] = NULL;
	}

	hash_t aux = *hash;
	*hash = *nuevo_hash;
	*nuevo_hash = aux;
	hash_destruir(nuevo_hash);

	return hash;
}

par_t *crear_par_nuevo(const char *clave, void *elemento)
{
	par_t *nuevo_par = calloc(1, sizeof(par_t));
	if (!nuevo_par) {
		return NULL;
	}

	nuevo_par->clave = malloc(strlen(clave) + 1);
	if (!(nuevo_par->clave)) {
		free(nuevo_par);
		return NULL;
	}
	strcpy(nuevo_par->clave, clave);
	nuevo_par->valor = elemento;

	return nuevo_par;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	float factor_carga = (float)(hash->cantidad) / (float)(hash->capacidad);

	if (factor_carga > FACTOR_CARGA_MAXIMO)
		hash = rehashear(hash);

	size_t posicion_hash = funcion_hash(clave) % hash->capacidad;

	par_t *nodo_actual = hash->tabla_hash[posicion_hash];
	while (nodo_actual) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			if (anterior)
				*anterior = nodo_actual->valor;

			nodo_actual->valor = elemento;
			return hash;
		}
		nodo_actual = nodo_actual->siguiente;
	}
	if (anterior != NULL) {
		*anterior = NULL;
	}

	par_t *nuevo_par = crear_par_nuevo(clave, elemento);
	if (!nuevo_par) {
		return NULL;
	}

	nuevo_par->siguiente = hash->tabla_hash[posicion_hash];
	hash->tabla_hash[posicion_hash] = nuevo_par;
	hash->cantidad++;
	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	void *elemento_eliminado = NULL;
	size_t posicion_hash = funcion_hash(clave) % hash->capacidad;
	par_t *par_actual = hash->tabla_hash[posicion_hash];
	par_t *par_anterior = NULL;

	while (par_actual != NULL) {
		if (strcmp(par_actual->clave, clave) == 0) {
			if (par_anterior) {
				par_anterior->siguiente = par_actual->siguiente;
			} else {
				hash->tabla_hash[posicion_hash] =
					par_actual->siguiente;
			}
			elemento_eliminado = par_actual->valor;

			free(par_actual->clave);
			free(par_actual);

			hash->cantidad--;
			return elemento_eliminado;
		}
		par_anterior = par_actual;
		par_actual = par_actual->siguiente;
	}

	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t posicion_hash = funcion_hash(clave) % hash->capacidad;
	par_t *par_actual = hash->tabla_hash[posicion_hash];
	while (par_actual) {
		if (strcmp(par_actual->clave, clave) == 0) {
			return par_actual->valor;
		}
		par_actual = par_actual->siguiente;
	}

	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;

	size_t posicion_hash = funcion_hash(clave) % hash->capacidad;
	par_t *par_actual = hash->tabla_hash[posicion_hash];
	while (par_actual) {
		if (strcmp(par_actual->clave, clave) == 0) {
			return true;
		}
		par_actual = par_actual->siguiente;
	}

	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	for (int i = 0; i < hash->capacidad; i++) {
		par_t *actual = hash->tabla_hash[i];
		while (actual != NULL) {
			par_t *par_siguiente = actual->siguiente;
			if (destructor)
				destructor(actual->valor);
			free(actual->clave);
			free(actual);
			actual = par_siguiente;
		}
		hash->tabla_hash[i] = NULL;
	}
	free(hash->tabla_hash);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;

	bool sigue_iterando = true;
	for (int posicion = 0; posicion < hash->capacidad && sigue_iterando;
	     posicion++) {
		par_t *actual = hash->tabla_hash[posicion];
		while (actual && sigue_iterando) {
			if (f(actual->clave, actual->valor, aux) == false) {
				sigue_iterando = false;
			}
			n++;
			actual = actual->siguiente;
		}
	}
	return n;
}
