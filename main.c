#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include "src/menu.h"
#include "src/funciones_main.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	limpiar_consola();
	juego_t *j = juego_crear();

	menu_t *menu = menu_crear();
	menu_agregar_comando(menu, "c", "Cargar un archivo",
			     cargar_archivo_pokemones);
	menu_agregar_comando(menu, "l", "Listar pokemones", listar_pokemones);
	menu_agregar_comando(menu, "s", "Seleccionar pokemones",
			     seleccionar_pokemones);
	menu_agregar_comando(menu, "j", "Arranca a jugar", jugar_rondas);
	menu_agregar_comando(menu, "q", "Sale del juego", salir);
	menu_agregar_comando(menu, "help", "Muestra ayuda", mostrar_ayuda);

	struct estado_juego estado = { .juego = j,
				       .adversario = NULL,
				       .pokemones_disponibles = NULL,
				       .continuar = true,
				       .se_eligieron_pokemones = false,
				       .archivo_cargado = false,
				       .rondas_jugadas = 0,
				       .menu = menu };

	printf("Bienvenido al juego de batallas de pokemon\n\nEstos son los comandos para jugar:\n");
	mostrar_ayuda(&estado);
	printf("\nIngrese comandos a continuacion o escriba 'help' para obtener ayuda.\n\n");

	while (!juego_finalizado(j) && estado.continuar) {
		printf("TP2> ");

		char linea[200];
		char *leido;
		leido = fgets(linea, 200, stdin);
		if (leido == NULL)
			continue;
		linea[strlen(linea) - 1] = 0;

		if (!menu_ejecutar_comando(menu, linea, &estado))
			printf("Ese comando no existe. Escriba 'help' para obtener ayuda\n");
	}

	if (estado.adversario != NULL)
		adversario_destruir(estado.adversario);
	if (estado.pokemones_disponibles != NULL)
		lista_destruir(estado.pokemones_disponibles);
	juego_destruir(j);
	menu_destruir(menu);

	return 0;
}
