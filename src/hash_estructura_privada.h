#ifndef __HASH_ESTRUCTURA_H__
#define __HASH_ESTRUCTURA_H__

#include <stdio.h>

typedef struct par {
	void *valor;
	char *clave;
	struct par *siguiente;
} par_t;

struct hash {
	par_t **tabla_hash;
	size_t capacidad;
	size_t cantidad;
};

#endif /* __HASH_ESTRUCTURA_H__ */