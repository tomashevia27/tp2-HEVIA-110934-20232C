<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de Tomás Hevia - 110934 - thevia@fi.uba.ar

- Para compilar:

```bash
make pruebas_chanutron
```
```bash
make main
```

- Para ejecutar:

```bash
./pruebas_chanutron
```
```bash
./main
```

- Para ejecutar con valgrind:
```bash
make valgrind-chanutron
```
```bash
make valgrind-main
```
---
##  Funcionamiento y Respuestas a las preguntas teóricas

#### juego.c
en juego.c decidi utilizar estas estructuras:
```c
typedef struct jugador{
	lista_t* pokemones;
	abb_t* ataques_disponibles;
	unsigned int puntaje;
}jugador_t;

struct juego{
	informacion_pokemon_t* info_pokemon;
	lista_t* lista_pokemones;
	jugador_t* jugador_1;
	jugador_t* jugador_2;
	int rondas_jugadas;
};
```
en juego uso `informacion_pokemon_t` para guardar los datos del archivo (mediante lo implementado en el tp1).
luego uso `lista_pokemones` para guardar los pokemones y poder acceder a ellos.
y luego los `jugador_1` y `jugador_2` guardan la informacion de los jugadores.

en la estructura jugador decidi usar una lista para guardar los 3 pokemones que le pertenecen al jugador (podria tambien implementarse con un vector yo creo) y uso el abb para guardar los ataques, esto lo decidi para que, cuando se usa un ataque en alguna jugada, poder borrarlo del abb (esto en una lista no podria hacerlo) y asi validar que no se repitan los ataques usados (lo detallo mejor en la explicacion de juego_jugar_turno)

- paso a explicar las funciones:

`juego_crear`: basicamente reserva la memoria para juego_t, si falla algo devuelve NULL. usa funciones de otros tdas como `lista_crear`o `abb_crear`

`juego_cargar_pokemon`: recibe un juego y un archivo. utiliza la funcion del tp1 `pokemon_cargar_archivo` para guardar la informacion del archivo en juego->info_pokemon, tambien valida que la cantidad de pokemones sea ,mayor o igual que 4. devuelve el estado del juego (ERROR_GENERAL, POKEMON_INSUFICIENTES o TODO_OK). 
la complejidad es O(n) (recorre todos los pokemones para guardarlos en info_pokemon)

`juego_listar_pokemon`: insertar en `juego->lista_pokemones` los pokemones de `juego->info_pokemon` y devuelve `juego->lista_pokemones` (en caso de que ya esten listados, devuelve directamente la lista)
complejidad: O(n)

`juego_seleccionar_pokemon`: recibe un juego_t, el jugador (jugador1 o jugador2) y 3 nombres de pokemones.
primero chequea que los nombres no se repitan, en caso de que si devuelve POKEMON_REPETIDO.
luego, asigna los pokemones en el jugador correspondiente mediante `asignar_pokemones_jugador`, en esta funcion se encarga de buscar los pokemones en `juego->lista_pokemones`, si no los encuentra devuelve POKEMON_REPETIDO. si los encuentra, guarda los ataques en `juego->ataques_disponibles` (usando `con_cada_ataque` y `abb_insertar`), al final inserta los primeros dos pokemones en la lista del jugador que los buscaba, y el ultimo en la lista del otro jugador. si falla la asignacion devuelve ERROR_GENERAL y sino devuelve TODO_OK.
la complejidad es O(n), ya que `lista_buscar_elemento` es O(n), al igual que `con_cada_ataque. mientras que insertar en lista o abb son O(1), por lo tanto lo que realiza son 3 O(n) + O(n) + 9 O(1) + 3 O(1), esto termina haciendo O(n).

`juego_jugar_turno`: recibe el juego y dos `jugada_t`.
primero busca los pokemones (por su nombre) de las jugadas en la lista de pokemonesr, si no los encuentra devuelve el resultado_t con las jugadas en ataque_error. 
luego, busca los ataques (tambien por su nombre) de las jugadas en el pokemon encontrado (con la funcion `pokemon_buscar_ataque`).
si no los encuentra devuelve error.
despues se usa `abb_buscar` para buscar esos ataques en los abb `juego->jugador_1->ataques_disponibles` y `juego->jugador_2->ataques_disponibles`, si los encuentra, calcula el resultado de las jugadas y el puntaje con las funciones `calcular_ataque` que devuelve un RESULTADO_ATAQUE y `calcular_puntaje` que devuelve el puntaje segun el valor de la funcion anterior y ese puntaje se lo suma al puntaje que tiene el jugador que realizo esa jugada. por ultimo quita el ataque usado de `ataques_disponibles` asi se evita que al llamar devuelta a la funcion se pueda usar un ataque ya usado.
al final aumenta el contador con la cantidad de rondas jugadas (en caso de que los resultados no den error) y devuelve el resultado de las jugadas.
la complejidad de esta funcion es O(log n).
porque usa 2 veces `lista_buscar_elemento` que como maximo es O(3), al igual que `pokemon_buscar_ataque` que tambien la usa 2 veces y es O(3), despues usa 2 veces `abb_buscar` que es O(log n)  y `abb_quitar` que tambien es O(log n)

`juego_obtener_puntaje`: devuelve el puntaje del jugador correspondiente. es O(1)

`juego_destruir`: libera toda la memoria usada por el juego, usando los destructores de listas y abb. es O(n) 

#### adversario.c

decidi usar esta estructura:
```c
struct adversario {
	lista_t *pokemones_disponibles;
	lista_t *pokemones_adversario;
	lista_t *pokemones_jugador;
	jugada_t jugadas_posibles[9];
	bool se_uso_jugada[9];
	int posicion_ultima_jugada_usada;
	abb_t *ataques_disponibles_jugador;
};
```

basicamente la idea es la misma que lo anterior, usar las listas para almacenar los pokemones (pokemones_disponibles es la misma lista que juego->listas_pokemones).
despues los vectores de jugadas_posibles y se_uso_jugada son para guardar las 9 jugadas posibles y saber si se usaron o no (los booleanos). podria haberse implementado con abb de jugadas como lo hice en juego.c con los ataques tambien yo creo, pero opte por esta implementacion.

`adversario_crear`: crea el adversario_t, reservando la memoria necesaria.
guarda en adversario->pokemones_disponibles la lista que recibe por parametro.
usa `lista_crear` para crear las listas de pokemones_adversario y pokemones_jugador.

`adversario_seleccionar_pokemon`: selecciona 3 pokemones random de la lista `adversario->pokemones_disponibles`.
usa `rand()` para obtener posiciones random y buscar los pokemones. luego guarda los nombres de los pokemones elegidos en los char** recibidos por parametro.
por ultimo inserta los dos primeros pokemones en `adversario->pokemones_adversario` y el ultimo en `adversario->pokemones_jugador`
la complejidad es O(n), ya que busca tres pokemones en la lista (3 O(n)) y luego insertar es O(1). por lo tanto termina siendo 3 O(n) + 3 O(1) = O(n).

`adversario_pokemon_seleccionado`: recibe los nombres de los pokemones seleccionados por el jugador, los busca en `adversario->pokemones_disponibles` y los inserta en las listas correspondientes.
la complejidad es O(n) (mismo caso que la funcion anterior).

`adversario_proxima_jugada`: si aun no se asignaron las 9 jugadas posibles en `adversario->jugadas_posibles`, se le asignan.
luego se genera un numero random, se verifica que la jugada en la posicion del numero random no se haya utilizado (si se utilizo se recorre hasta el numero que no se haya usado) y luego se devuelve una jugada que no se haya usado aun (siempre a menos que se la llame mas de 9 veces a la funcion, que en nuestro juego no pasa).
complejidad O(n), ya que puede llegar a recorrer todas las jugadas.

`adversario_informar_jugada`:use esta funcion para controlar que la jugada del jugador haya sido valida, en caso de no serlo, toma la ultima jugada del adversario y la marca como no usada. esto lo hago para que durante las 9 rondas el adversario tenga jugadas disponibles para jugar.

`adversario_destruir`: destruye al adversario. complejidad O(n)


#### main.c y funciones_main.c
aca decidi usar el menu_t que nos mostro lucas. usando un hash para los comandos y un informacion_comando donde guardo la descripcion del comando y la funcion que realiza.
las funciones son para crear el menu, para agregar comandos al menu, para ejecutar los comandos y para destruir el menu y los comandos.

en main.c decidi usar 4 comandos claves:
    - `cargar_archivo`
    - `seleccionar_pokemones`
    - `jugar`
    - `salir `   

despues use `listar_pokemones` y `mostrar_ayuda` pero son mas opcionales si se quiere

luego uso un while para el bucle del juego, donde se pide un comando a ejecutar y lo ejecuta, esto hasta que se finalice el juego o se decida usar el comando salir (que corta el juego poniendo estado.continuar en false)

todos los comandos claramente usan las funciones explicadas anteriormente, por lo tanto solo las voy a nombrar.

paso a explicar los comandos:

`cargar_archivo`: se inicia con 'c'. se encarga de pedir la direccion de un archivo y cargarlo en el juego mediante `juego_cargar_pokemon`, si no lo puede cargar se debe volver a ejecutar el comando. si los pokemon no son sufucientes lo mismo, en ambos casos se le avisa al usuario con un mensaje por pantalla.
una vez cargado el archivo correctamente, se listan los pokemones y se crea el adversario. esta funcion es O(n) ya que recorre los todos los pokemones al listar.

`seleccionar_pokemones`: primero chequea que haya un archivo cargado y de que no se hayan elegido ya los pokemones, si algo de esto ocurre, se avisa al usuario y se sale de la funcion.
si corresponde continuar la funcion, se piden 3 nombres de pokemones. no se pueden repetir estos nombres, va a pedir 3 nombres hasta que el usuario ingrese 3 nombre distintos y de pokemones que existan en el juego.
luego de pedirle los pokemones al usuario, se le piden al adversario (mediante funciones de adversario.h)
se asignan los pokemones elegidos por ambos (con juego_seleccionar_pokemon)
los pokemones que le corresponden al jugador, son insertados en una lista del estado, para poder imprimirlos a la hora de jugar.
la complejidad es O(n), ya que usa juego_seleccionar_pokemon que es O(n) y luego las de adversario tambien son O(n)


`jugar`: se encarga de hacer jugar las 9 rondas al usuario. le pide que ingrese una jugada, genera una jugada del adversario y juego un turno, si la jugada ingresada no es valida, la ronda no cuenta y se vuelve a pedir una jugada, asi hasta jugar las 9 rondas.
luego compara los puntajes obtenidos por los jugadores y imprime el resultado por pantalla. la complejidad es O(n) ya que recorre la lista (O(n)) varias veces para mostrar los pokemones, 

`salir`: corta el juego haciendo que estado->continuar sea false, por lo tanto al salir de esta funcion se termina el bucle while. es O(1).

`listar_pokemones`: se encarga de mostrar todos los pokemones por pantalla. es O(n)

`mostrar_ayuda`: imprime por pantalla los comandos.





