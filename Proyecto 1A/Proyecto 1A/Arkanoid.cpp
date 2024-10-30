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


	//Obetenemos la informacion del monitor respectiva y se almacena en una variable llamada monitor
	ALLEGRO_MONITOR_INFO monitor;

	//Aqui mediante 0 obtenemos la informacion del primer monitor o unico de la computadora y se pasa la variable como parametro por referencia para ser modificada
	al_get_monitor_info(0, &monitor);

	//Constante que calcula ;la resolucion real del monitor desde el punto inicial hasta el final
	const int RX = monitor.x2 - monitor.x1;
	const int RY = monitor.y2 - monitor.y1;

	//Tambien inicializamos la posicion del mouse en la ventana, aqui se almacena la posicion
	int mousex = 0;
	int mousey = 0;

	//Creacion de la pantalla o ventana desplegable mediante un puntero pantalla que almacena la resolucion dada anteriormente
	ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
	al_set_display_flag(pantalla, ALLEGRO_FULLSCREEN, true); 

	// Obtener dimensiones de la pantalla
	int ResX = al_get_display_width(pantalla);
	int ResY = al_get_display_height(pantalla);

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
	//Se puede clasificar como un dibujo que se realiza en la consola
	ALLEGRO_BITMAP* Fondo = al_load_bitmap("Imagenes/fondo_main.jpg");
	ALLEGRO_BITMAP* Logo = al_load_bitmap("Imagenes/arkanoid.png");


	// Verificamos que las imágenes se cargaron correctamente
	if (!Fondo) {
		al_show_native_message_box(NULL, "Error", "Error", "No se pudo cargar el fondo", NULL, NULL);
		return;
	}

	if (!Logo) {
		al_show_native_message_box(NULL, "Error", "Error", "No se pudo cargar el logo", NULL, NULL);
		return;
	}

	//Establecemos las fonts para una impresion del texto
	ALLEGRO_FONT* font = al_load_ttf_font("Video-Font.ttf", 40, NULL);
	ALLEGRO_FONT* font2 = al_load_ttf_font("Video-Font.ttf", 25, NULL);

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

	int X = al_get_display_width(pantalla);
	int Y = al_get_display_height(pantalla);

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
			

		}

		// Texto en pantalla y dibujo de los elementos
		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			if (eventos.timer.source == timer) {


				// Limpiar la pantalla
				al_clear_to_color(al_map_rgb(0, 0, 0));


				// Escalamos el logo para ubicarlo en una parte de la pantalla especifica
				float scale = 1.3; 
				int ancho = al_get_bitmap_width(Logo) * scale;
				int alto = al_get_bitmap_height(Logo) * scale;
				int logoX = (ResX - ancho) / 2;
				int logoY = ResY / 15;

				al_draw_bitmap(Fondo, 0, 0, 0);

				al_draw_scaled_bitmap(Logo, 0, 0, al_get_bitmap_width(Logo), al_get_bitmap_height(Logo), logoX, logoY, ancho, alto, 0);

				//Textos y su posicion exacta para el menu principal
				al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (RY * (250.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "JUGAR");
				al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (RY * (325.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "REGLAS");
				al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (RY * (390.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "RESULTADOS");
				al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (RY * (470.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "SALIR");
				al_draw_text(font2, al_map_rgb(255, 255, 255), 50, RY - al_get_font_line_height(font2) - 50, ALLEGRO_ALIGN_LEFT, "by Santiago and Christopher");
				
			}
		}

		

		//Si se posiciona el mouse en las coordenadas donde indica la opción jugar
		if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * 255.0 / 720.0) && mousey <= (RY * 290.0 / 720.0))) {

			//Esto lo que hace es cambiar el color de jugar en color amarillo
			al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (RY * (250.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "JUGAR");

			//En caso de que se realice un evento del mouse

			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					

					
				}
			}
		}

		//Si se posiciona el mouse en las coordenadas donde indica la opción reglas
		if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * 330.0 / 720.0) && mousey <= (RY * 375.0 / 720.0))) {

			//Esto lo que hace es cambiar el color de jugar en color amarillo
			al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (RY * (325.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "REGLAS");

			//En caso de que se realice un evento del mouse

			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {



				}
			}
		}

		//Si se posiciona el mouse en las coordenadas donde indica la opción de resultados
		if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * 395.0 / 720.0) && mousey <= (RY * 450.0 / 720.0))) {

			//Esto lo que hace es cambiar el color de jugar en color amarillo
			al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (RY * (390.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "RESULTADOS");

			//En caso de que se realice un evento del mouse

			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {



				}
			}
		}

		//Si se posiciona el mouse en las coordenadas donde indica la opción de salir
		if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * 475.0 / 720.0) && mousey <= (RY * 530.0 / 720.0))) {

			//Esto lo que hace es cambiar el color de jugar en color amarillo
			al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (RY * (470.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");

			//En caso de que se realice un evento del mouse

			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {

					creacion = false;

				}
			}
		}

		//Aqui hay un condicional aparte del salir normal en donde si se presiona "esc" de primeras se sale del programa
		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			//Si se presiona la tecla escape se sale del juego
			switch (eventos.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				creacion = false;
			}
		}

		al_flip_display();

	}

	// Liberar recursos esto con el fin de mantener la memoria
	al_destroy_bitmap(Fondo);
	al_destroy_bitmap(Logo);
	al_destroy_font(font);
	al_destroy_font(font2);
	al_destroy_timer(timer);
	al_destroy_timer(timer2);
	al_destroy_event_queue(cola_eventos);
	al_destroy_display(pantalla);

}


