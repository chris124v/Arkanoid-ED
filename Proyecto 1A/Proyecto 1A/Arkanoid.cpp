// Proyecto 1A.cpp 
// Christopher Daniel Vargas Villalta, Carnet: 2024108443
// Santiago Espinoza Rendon, Carnet: 2024156530


//Librerias de allegro para la realizacion del proyecto
#include <stdio.h>
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>


using namespace std;

#define FPS 60.0
#define FPS1 60.0

void main()
{
	//Este condicional if funciona en caso de que se de un error al crear la ventana emergente
	if (!al_init()) {
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar la Practica de Allegro", NULL, NULL);
		return;
	}

	//Aqui inicializamos 
	al_init_font_addon();//Permite obtener las fuentes de texto
	al_init_ttf_addon();//Funciona para extender la cantidad de fuentes de texto
	al_init_image_addon();//Aqui se inicializa la capacidad de agregar imagenes
	al_init_primitives_addon(); //Se posibilita la creacion de figuras geometricas
	al_install_keyboard(); //Se inicializa la funcionalidad del teclado
	al_install_mouse();//Se inicializa el mouse y sus funciones

	//Establecemos direcciones y inicialamos el teclado como un evento
	//Variables del Jugador
	float x = 10, y = 430, speed = 5; //Aqui se establece la posicion a la esquina inferior izquierda 
	bool activo = false; //Esto es para definir la existencia o utilizacion del timer
	enum Direccion { DOWN, LEFT, RIGHT, UP }; //Direcciones posibles
	ALLEGRO_KEYBOARD_STATE teclado; //Inicializacion del teclado
	int dir = DOWN; //Variable para el movimiento por pixeles

	//variables enemigo
	bool caminar = true; //Para reflejar que el enemigo siempre esta en movimiento
	int dirE = RIGHT; //Direccion general
	int eneX = 10, speedE = 6; //Movimiento por pixeles 

	//Obetenemos la informacion del monitor respectiva y se almacena en una variable llamada monitor
	ALLEGRO_MONITOR_INFO monitor;

	//Aqui mediante 0 obtenemos la informacion del primer monitor o unico de la computadora y se pasa la variable como parametro por referencia para ser modificada
	al_get_monitor_info(0, &monitor);

	//Constante que calcula ;la resolucion real del monitor desde el punto inicial hasta el final
	const int RX = monitor.x2 - monitor.x1;
	const int RY = monitor.y2 - monitor.y1;

	//Aqui se establecen las variables para la creacion de una ventana de 100 pixeles de ancho y 800 de alto
	int ResX = 1200;
	int ResY = 675;

	//Tambien inicializamos la posicion del mouse en la ventana, aqui se almacena la posicion
	int mousex = 0;
	int mousey = 0;

	//Creacion de la pantalla o ventana desplegable mediante un puntero pantalla que almacena la resolucion dada anteriormente
	ALLEGRO_DISPLAY* pantalla = al_create_display(ResX, ResY);

	//Se establece la pantalla en una parte del monitor, esto lo centra tomando un tercio de la pantalla
	al_set_window_position(pantalla, RX / 3 - ResX / 3, RY / 3 - ResY / 3);

	//Titulo de la ventana
	al_set_window_title(pantalla, "Practica Allegro, Santiago y Christopher");

	//Esto se podria tomar como una excepcion en caso de que la pantalla no pueda ser creada por alguna razon.
	if (!pantalla) {
		al_show_native_message_box(NULL, "Error", "Error", "No se pudo crear la ventana", NULL, NULL);
		return;
	}

	//Aqui creamos una cola de eventos, recurso necesario para realizar los displays, timers y eventos de teclado y mouse para que sean identificados.
	ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();

	//Creacion de los timers, estos corresponde a un recurso de allegro utilizado para el control de la velocidad de los movimientos
	//En este caso creamos dos timers uno para el movimiento de la primera imagen y otro con el movimiento de la segunda imagen.
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER* timer2 = al_create_timer(1.0 / FPS1);

	//En este punto cargaremos las imagenes y fonts (sprites) para el simulador, esto se realiza mediante un bitmap que 
	//Se puede clasificar como un dibujo que se realiza en la consola.
	ALLEGRO_BITMAP* Enemigo = al_load_bitmap("Bill.png");
	ALLEGRO_BITMAP* Jugador = al_load_bitmap("dipper.png");
	ALLEGRO_BITMAP* Fondo = al_load_bitmap("shibuya.jpg");

	// Verificamos que las imágenes se cargaron correctamente
	if (!Fondo) {
		al_show_native_message_box(NULL, "Error", "Error", "No se pudo cargar el fondo", NULL, NULL);
		return;
	}

	//Establecemos las fonts para una impresion del texto
	ALLEGRO_FONT* font = al_load_ttf_font("Bangers-Regular.ttf", 40, NULL);
	ALLEGRO_FONT* font2 = al_load_ttf_font("Bangers-Regular.ttf", 25, NULL);

	//Aqui se establece el color de la panatlla cuando inicia el juego
	al_clear_to_color(al_map_rgb(0, 0, 0));

	//Aqui mediante la operacion al register event logramos anadir a la cola de eventos, el timer, la pantalla, el teclado y las diversas acciones que se realizaran
	//El fin de esto en la cola de eventos recae en estar listo para ejecutar las acciones cuando sean invocados
	al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
	al_register_event_source(cola_eventos, al_get_timer_event_source(timer2));
	al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
	al_register_event_source(cola_eventos, al_get_keyboard_event_source());
	al_register_event_source(cola_eventos, al_get_mouse_event_source());

	//Esta variable nos permetira inicializar el ciclo de while en true para que mientras la varianle no cambie 
	//El ciclo se ejecute
	bool creacion = true;

	//Estos dos timers nos permitiran el movimiento de los dos personajes principales
	al_start_timer(timer);
	al_start_timer(timer2);

	//Aqui definimos el ciclo while que mientra la variable sea true se sigue ejecutando
	while (creacion) {

		//Aqui mediante un allegro event creamos una variable eventos que registra las acciones para despues
		//Realizarla en la cola de eventos
		ALLEGRO_EVENT eventos;

		//Aqui en el wait for event estos se guardaran en el parametro por referencia eventos para cuando sea ejecutado
		al_wait_for_event(cola_eventos, &eventos);

		//Esteblecemos el evento de teclado
		al_get_keyboard_state(&teclado);

		//Esto se registra en la cola de eventos para cerrar el programa y acabar el ciclo while
		if (eventos.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			creacion = false; //Al pasarlo a falso se cierra
		}

		//Evento para la inicializacion del mouse
		if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES) {
			mousex = eventos.mouse.x;
			mousey = eventos.mouse.y;
		}

		//Movimiento del jugador
		//Aqui se establece en la cola de eventos el timer dado a que el movimiento mediante intervalos se da por el mismo
		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			activo = true; //Retomamos la variable activo para establecer que el timer se esta utilizando

			//Aqui mediante operaciones del propio allegro presionando la tecla Down en especifico se define la direccion a donde se mueve el jugador que es hacia abajo
			if (al_key_down(&teclado, ALLEGRO_KEY_DOWN)) {
				y += speed; //La posicion en y se modificara 5 pixeles hacia abajo
				dir = DOWN;
			}

			//Caso de subir
			else if (al_key_down(&teclado, ALLEGRO_KEY_UP)) {
				y -= speed;//La posicion en y se modificara 5 pixeles hacia arriba
				dir = UP;
			}

			//Caso de la derecha
			else if (al_key_down(&teclado, ALLEGRO_KEY_RIGHT)) {
				x += speed; //5 pixeles a la derecha
				dir = RIGHT;
			}

			//Caso de la izquierda
			else if (al_key_down(&teclado, ALLEGRO_KEY_LEFT)) {
				x -= speed; //5 pixeles a la izquierda
				dir = LEFT;
			}

			//Movimiento enemigo mediante el segundo timer

			if (eventos.timer.source == timer2) {

				//Si la direccion que toma es la derecha
				if (dirE == RIGHT) {
					\

						// Verifica si el enemigo está dentro de los límites de la pantalla
						if (eneX + al_get_bitmap_width(Enemigo) < al_get_display_width(pantalla)) {
							eneX += speedE; //Va hacia la derecha segun la cantidad de pixeles
						}
						else {
							dirE = LEFT; // Cambia la dirección a izquierda
						}
				}

				//Caso de que la direccion este a la izquierda
				else if (dirE == LEFT) {
					// Verifica si el enemigo está dentro de los límites de la pantalla
					if (eneX > 0) {
						eneX -= speedE;
					}
					else {
						dirE = RIGHT; // Cambia la dirección a derecha
					}
				}
			}
		}

		// Texto en pantalla y dibujo de los elementos
		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			if (eventos.timer.source == timer) {

				// Limpiar la pantalla
				al_clear_to_color(al_map_rgb(0, 0, 0));

				// Calcular posiciones para centrar el texto
				int centro = ResX / 2; // Centro de la pantalla en X
				int texto1 = ResY / 8;   // Posición Y para el primer texto
				int texto2 = ResY / 4;   // Posición Y para el segundo texto

				// Dibuja el fondo en todo el display
				al_draw_bitmap(Fondo, 0, 0, 0);

				// Dibujar texto en blanco y centrado
				al_draw_text(font, al_map_rgb(255, 255, 255), centro, texto1, ALLEGRO_ALIGN_CENTER, "Practica Allegro, Santiago y Christopher");
				al_draw_text(font2, al_map_rgb(255, 255, 255), centro, texto2, ALLEGRO_ALIGN_CENTER, "Moviendo al jugador");

				// Dibuja el jugador
				al_draw_bitmap(Jugador, x, y, 0);

				// Dibuja el enemigo
				al_draw_bitmap(Enemigo, eneX, 10, 0);

				// Actualizar la pantalla
				al_flip_display();
			}
		}
	}

	// Liberar recursos esto con el fin de mantener la memoria
	al_destroy_bitmap(Jugador);
	al_destroy_bitmap(Enemigo);
	al_destroy_bitmap(Fondo);
	al_destroy_font(font);
	al_destroy_font(font2);
	al_destroy_timer(timer);
	al_destroy_timer(timer2);
	al_destroy_event_queue(cola_eventos);
	al_destroy_display(pantalla);



}


